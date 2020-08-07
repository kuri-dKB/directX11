//========================================================================
// GraphicsThrowMacros.h
// �}�N��
//
// �X�V���F2020/08/04
// �I�� �B��
//========================================================================
#pragma once

// �O���t�B�b�N�̗�O�`�F�b�N�E�}�N��(�ꕔdxgi)
#define GFX_EXCEPT_NOINFO(hr) CGraphics::CHrException(__LINE__,__FILE__,(hr))
#define GFX_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw CGraphics::CHrException( __LINE__,__FILE__,hr )


#ifndef NDEBUG
#define GFX_EXCEPT(hr) CGraphics::CHrException( __LINE__,__FILE__,(hr),m_infoManager.GetMessages() )
#define GFX_THROW_INFO(hrcall) m_infoManager.Set(); if( FAILED( hr = (hrcall) ) ) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) CGraphics::CDeviceRemovedException( __LINE__,__FILE__,(hr),m_infoManager.GetMessages() )
#define GFX_THROW_INFO_ONLY(call) m_infoManager.Set(); (call); {auto v = m_infoManager.GetMessages(); if(!v.empty()) {throw CGraphics::CInfoException(__LINE__,__FILE__,v);}}
#else
#define GFX_EXCEPT(hr) CGraphics::CHrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) CGraphics::CDeviceRemovedException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO_ONLY(call) (call)
#endif // !NDEBUG

// infomanager�����[�J���X�R�[�v�ɃC���|�[�g���邽�߂̃}�N��
// this.GetInfoManager(CGraphics& gfx) �Ȃ��Ƃ���
#ifdef NDEBUG
#define INFOMAN(gfx) HRESULT hr;
#else
#define INFOMAN(gfx) HRESULT hr; CDxgiInfoManager& m_infoManager = GetInfoManager((gfx))
#endif
