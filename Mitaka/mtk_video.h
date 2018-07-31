/**************************************************************************************************

Copyright (c) 2014   Tsunehiko Kato  and  4D2U Project, National Astronomical Observatory of Japan

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

�ȉ��ɒ�߂�����ɏ]���A�{�\�t�g�E�F�A����ъ֘A�����̃t�@�C���i�ȉ��u�\�t�g�E�F�A�v�j�̕������擾��
�邷�ׂĂ̐l�ɑ΂��A�\�t�g�E�F�A�𖳐����Ɉ������Ƃ𖳏��ŋ����܂��B����ɂ́A�\�t�g�E�F�A�̕�����
�g�p�A���ʁA�ύX�A�����A�f�ځA�Еz�A�T�u���C�Z���X�A�����/�܂��͔̔����錠���A����у\�t�g�E�F�A��
�񋟂��鑊��ɓ������Ƃ������錠�����������Ɋ܂܂�܂��B

��L�̒��쌠�\������і{�����\�����A�\�t�g�E�F�A�̂��ׂĂ̕����܂��͏d�v�ȕ����ɋL�ڂ�����̂Ƃ���
���B

�\�t�g�E�F�A�́u����̂܂܁v�ŁA�����ł��邩�Öقł��邩���킸�A����̕ۏ؂��Ȃ��񋟂���܂��B����
�ł����ۏ؂Ƃ́A���i���A����̖ړI�ւ̓K�����A����ь�����N�Q�ɂ��Ă̕ۏ؂��܂݂܂����A����Ɍ���
�������̂ł͂���܂���B ��҂܂��͒��쌠�҂́A�_��s�ׁA�s�@�s�ׁA�܂��͂���ȊO�ł��낤�ƁA�\�t
�g�E�F�A�ɋN���܂��͊֘A���A���邢�̓\�t�g�E�F�A�̎g�p�܂��͂��̑��̈����ɂ���Đ������؂̐����A��
�Q�A���̑��̋`���ɂ��ĉ���̐ӔC������Ȃ����̂Ƃ��܂��B
***************************************************************************************************/
#ifndef __MTK_VIDEO_H__
#define __MTK_VIDEO_H__
#include  <Windows.h>
#include  <dshow.h>
#include  <d3d9.h>
#include  <vmr9.h>
#include  <evr.h>



enum {
	RENDERER_WINDOWED_VMR7,
	RENDERER_WINDOWLESS_VMR7,
	RENDERER_WINDOWLESS_VMR9,
	RENDERER_EVR
};





//------------------
//  Abstract class
//------------------

class  mtk_video
{
public:
	virtual ~mtk_video() {};

	virtual HRESULT Init(HWND hwnd, IGraphBuilder *pGraph) = 0;
	virtual HRESULT SetViewWindow(HWND hwnd, int x, int y, int w, int h, bool bFullScreen) = 0;

	virtual	HRESULT SetWindowPosition(int x, int y, int w, int h) = 0;
	virtual HRESULT GetVideoSize(long *pw, long *ph) = 0;

	virtual HRESULT OnPaint(HWND hwnd, HDC hDC) = 0;
	virtual HRESULT OnDisplayChange() = 0;
	virtual HRESULT OnSize(const LPRECT pRect) = 0;
};



//----------------------
//  Window mode (VMR7)
//----------------------

class  mtk_video_window_mode : public mtk_video
{
	IVideoWindow*   m_pVW;
	IBasicVideo*	m_pBV;

public:
	mtk_video_window_mode();
	~mtk_video_window_mode();

	HRESULT Init(HWND hwnd, IGraphBuilder *pGraph);
	HRESULT SetViewWindow(HWND hwnd, int x, int y, int w, int h, bool bFullScreen);

	HRESULT	SetWindowPosition(int x, int y, int w, int h);
	HRESULT GetVideoSize(long *pw, long *ph);

	HRESULT OnPaint(HWND hwnd, HDC hDC);
	HRESULT OnDisplayChange();
	HRESULT OnSize(const LPRECT pRect);
};



//-------------------
//  Windowless VMR7
//-------------------

class  mtk_video_windowless_VMR7 : public mtk_video
{
	IVMRWindowlessControl *m_pWC;

public:
	mtk_video_windowless_VMR7();
	~mtk_video_windowless_VMR7();

	HRESULT Init(HWND hwnd, IGraphBuilder *pGraph);
	HRESULT SetViewWindow(HWND hwnd, int x, int y, int w, int h, bool bFullScreen);

	HRESULT	SetWindowPosition(int x, int y, int w, int h);
	HRESULT GetVideoSize(long *pw, long *ph);

	HRESULT OnPaint(HWND hwnd, HDC hDC);
	HRESULT OnDisplayChange();
	HRESULT OnSize(const LPRECT pRect);
};



//-------------------
//  Windowless VMR9
//-------------------

class  mtk_video_windowless_VMR9 : public mtk_video
{
	IVMRWindowlessControl9 *m_pWC;

public:
	mtk_video_windowless_VMR9();
	~mtk_video_windowless_VMR9();

	HRESULT Init(HWND hwnd, IGraphBuilder *pGraph);
	HRESULT SetViewWindow(HWND hwnd, int x, int y, int w, int h, bool bFullScreen);

	HRESULT	SetWindowPosition(int x, int y, int w, int h);
	HRESULT GetVideoSize(long *pw, long *ph);

	HRESULT OnPaint(HWND hwnd, HDC hDC);
	HRESULT OnDisplayChange();
	HRESULT OnSize(const LPRECT pRect);
};



//-------
//  EVR
//-------

class  mtk_video_EVR : public mtk_video
{
	IBaseFilter *m_pEVR;
	IMFVideoDisplayControl *m_pVD;

public:
	mtk_video_EVR();
	~mtk_video_EVR();

	HRESULT Init(HWND hwnd, IGraphBuilder *pGraph);
	HRESULT SetViewWindow(HWND hwnd, int x, int y, int w, int h, bool bFullScreen);

	HRESULT	SetWindowPosition(int x, int y, int w, int h);
	HRESULT GetVideoSize(long *pw, long *ph);

	HRESULT OnPaint(HWND hwnd, HDC hDC);
	HRESULT OnDisplayChange();
	HRESULT OnSize(const LPRECT pRect);
};



#endif
