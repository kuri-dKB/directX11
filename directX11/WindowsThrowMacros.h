//========================================================================
// WindowsThrowMacros.h
// �}�N��
//
// �X�V���F2020/08/04
// �I�� �B��
//========================================================================
#pragma once

// �G���[��O�̃w���p�[
#define CHWND_EXCEPT(hr) CWindow::CHrException(__LINE__,__FILE__,(hr))
#define CHWND_LAST_EXCEPT() CWindow::CHrException(__LINE__,__FILE__,GetLastError())
#define CHWND_NOGFX_EXCEPT() CWindow::NoGfxException(__LINE__,__FILE__)