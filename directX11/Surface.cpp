//========================================================================
// CSurface.cpp
// 
//
// 更新日：2020/08/10
// 栗城 達也
//========================================================================
#define FULL_WINTARD
#include "Surface.h"
#include <algorithm>
namespace Gdiplus
{
	using std::min;
	using std::max;
}
#include <gdiplus.h>
#include <sstream>

#pragma comment( lib,"gdiplus.lib" )

CSurface::CSurface(unsigned int width, unsigned int height) noexcept
	:
	m_pBuffer(std::make_unique<Color[]>(width * height)),
	m_width(width),
	m_height(height)
{}

CSurface& CSurface::operator=(CSurface&& donor) noexcept
{
	m_width = donor.m_width;
	m_height = donor.m_height;
	m_pBuffer = std::move(donor.m_pBuffer);
	donor.m_pBuffer = nullptr;
	return *this;
}

CSurface::CSurface(CSurface && source) noexcept
	:
	m_pBuffer(std::move(source.m_pBuffer)),
	m_width(source.m_width),
	m_height(source.m_height)
{}

CSurface::~CSurface()
{}

void CSurface::Clear(Color fillValue) noexcept
{
	memset(m_pBuffer.get(), fillValue.dword, m_width * m_height * sizeof(Color));
}

void CSurface::PutPixel(unsigned int x, unsigned int y, Color c) noexcept(!IS_DEBUG)
{
	assert(x >= 0);
	assert(y >= 0);
	assert(x < m_width);
	assert(y < m_height);
	m_pBuffer[y * m_width + x] = c;
}

CSurface::Color CSurface::GetPixel(unsigned int x, unsigned int y) const noexcept(!IS_DEBUG)
{
	assert(x >= 0);
	assert(y >= 0);
	assert(x < m_width);
	assert(y < m_height);
	return m_pBuffer[y * m_width + x];
}

unsigned int CSurface::GetWidth() const noexcept
{
	return m_width;
}

unsigned int CSurface::GetHeight() const noexcept
{
	return m_height;
}

CSurface::Color* CSurface::GetBufferPtr() noexcept
{
	return m_pBuffer.get();
}

const CSurface::Color* CSurface::GetBufferPtr() const noexcept
{
	return m_pBuffer.get();
}

const CSurface::Color* CSurface::GetBufferPtrConst() const noexcept
{
	return m_pBuffer.get();
}

CSurface CSurface::FromFile(const std::string& name)
{
	unsigned int width = 0;
	unsigned int height = 0;
	std::unique_ptr<Color[]> pBuffer;

	{
		// Gdiplus用にワイド文字列に変換
		wchar_t wideName[512];
		mbstowcs_s(nullptr, wideName, name.c_str(), _TRUNCATE);

		Gdiplus::Bitmap bitmap(wideName);
		if (bitmap.GetLastStatus() != Gdiplus::Status::Ok)
		{
			std::stringstream ss;
			ss << "Loading image [" << name << "]: failed to load.";
			throw CException(__LINE__, __FILE__, ss.str());
		}

		width = bitmap.GetWidth();
		height = bitmap.GetHeight();
		pBuffer = std::make_unique<Color[]>(width * height);

		for (unsigned int y = 0; y < height; y++)
		{
			for (unsigned int x = 0; x < width; x++)
			{
				Gdiplus::Color c;
				bitmap.GetPixel(x, y, &c);
				pBuffer[y * width + x] = c.GetValue();
			}
		}
	}

	return CSurface(width, height, std::move(pBuffer));
}

void CSurface::Save(const std::string& filename) const
{
	auto GetEncoderClsid = [&filename](const WCHAR* format, CLSID* pClsid) -> void
	{
		UINT  num = 0;          // 画像エンコーダの数
		UINT  size = 0;         // 画像エンコーダ配列のサイズをバイト単位で指定

		Gdiplus::ImageCodecInfo* pImageCodecInfo = nullptr;

		Gdiplus::GetImageEncodersSize(&num, &size);
		if (size == 0)
		{
			std::stringstream ss;
			ss << "Saving CSurface to [" << filename << "]: failed to get encoder; size == 0.";
			throw CException(__LINE__, __FILE__, ss.str());
		}

		pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
		if (pImageCodecInfo == nullptr)
		{
			std::stringstream ss;
			ss << "Saving CSurface to [" << filename << "]: failed to get encoder; failed to allocate memory.";
			throw CException(__LINE__, __FILE__, ss.str());
		}

		GetImageEncoders(num, size, pImageCodecInfo);

		for (UINT j = 0; j < num; ++j)
		{
			if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
			{
				*pClsid = pImageCodecInfo[j].Clsid;
				free(pImageCodecInfo);
				return;
			}
		}

		free(pImageCodecInfo);
		std::stringstream ss;
		ss << "Saving CSurface to [" << filename <<
			"]: failed to get encoder; failed to find matching encoder.";
		throw CException(__LINE__, __FILE__, ss.str());
	};

	CLSID bmpID;
	GetEncoderClsid(L"image/bmp", &bmpID);


	// Gdiplus用にワイド文字列に変換
	wchar_t wideName[512];
	mbstowcs_s(nullptr, wideName, filename.c_str(), _TRUNCATE);

	Gdiplus::Bitmap bitmap(m_width, m_height, m_width * sizeof(Color), PixelFormat32bppARGB, (BYTE*)m_pBuffer.get());
	if (bitmap.Save(wideName, &bmpID, nullptr) != Gdiplus::Status::Ok)
	{
		std::stringstream ss;
		ss << "Saving CSurface to [" << filename << "]: failed to save.";
		throw CException(__LINE__, __FILE__, ss.str());
	}
}

void CSurface::Copy(const CSurface& src) noexcept(!IS_DEBUG)
{
	assert(m_width == src.m_width);
	assert(m_height == src.m_height);
	memcpy(m_pBuffer.get(), src.m_pBuffer.get(), m_width * m_height * sizeof(Color));
}

CSurface::CSurface(unsigned int width, unsigned int height, std::unique_ptr<Color[]> pBufferParam) noexcept
	:
	m_width(width),
	m_height(height),
	m_pBuffer(std::move(pBufferParam))
{}


// CSurface exception stuff
CSurface::CException::CException(int line, const char* file, std::string note) noexcept
	:
	CChiliException(line, file),
	note(std::move(note))
{}

const char* CSurface::CException::what() const noexcept
{
	std::ostringstream oss;
	oss << CChiliException::what() << std::endl
		<< "[Note] " << GetNote();
	m_whatBuffer = oss.str();
	return m_whatBuffer.c_str();
}

const char* CSurface::CException::GetType() const noexcept
{
	return "Chili Graphics Exception";
}

const std::string& CSurface::CException::GetNote() const noexcept
{
	return note;
}

