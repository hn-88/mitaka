/**************************************************************************************************

Copyright (c) 2014-2015   Tsunehiko Kato  and  4D2U Project, National Astronomical Observatory of Japan

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
#include  <Windows.h>
#include  <tchar.h>
#include  <stdio.h>
#include  <atlbase.h>
#include  <dshow.h>
#include  "mtk_movie.h"
#include  "mtk_window_mode.h"
#include  "mtk_data_file.h"
#include  "../common/text/string_util.h"
#include  "../common/text/key_value_file.h"
#include  "mtk_strings.h"




#define SAFE_RELEASE(x) { if (x) x->Release(); x = NULL; }


//****
extern bool	bNetwork;
extern int disp_w, disp_h;
extern OSVERSIONINFO osvi;




// 2014/12/26�G�N�T�ǉ� -- �������� --
void
ForceForegroundWindow(HWND hwnd, int x0, int y0, int w, int h)
{
	// ���ɂ���āA(1)�܂���(2)�̂ǂ��炩�������͂��B
	// (1)(2)�������Ȃ��ꍇ�A�ň�(3)�ł��t�H�A�O���E���h�ɂȂ�B

	// (1) ��������E�B���h�E�T�C�Y��ς��邱�ƂŃr�f�I���t�H�A�O���E���h�ɂ����Ă���
	// SWP_NOOWNERZORDER ���w�肵�Ȃ���(2)�̏����������Ȃ��Ȃ�
	SetWindowPos(hwnd, HWND_TOP, x0, y0, w, h + 1, SWP_NOOWNERZORDER | SWP_SHOWWINDOW);
	SetWindowPos(hwnd, HWND_TOP, x0, y0, w, h, SWP_NOOWNERZORDER | SWP_SHOWWINDOW);

	// (2) �����I�Ƀt�H�A�O���E���h�ɏ��������Ă��鏈��
	UINT foreThread = GetWindowThreadProcessId(GetForegroundWindow(), 0);
	UINT appThread = GetCurrentThreadId();
	if (foreThread != appThread)
	{
		AttachThreadInput(foreThread, appThread, true);
		BringWindowToTop(hwnd);
		ShowWindow(hwnd, SW_SHOW);
		AttachThreadInput(foreThread, appThread, false);
	}
	else
	{
		BringWindowToTop(hwnd);
		ShowWindow(hwnd, SW_SHOW);
	}

	// (3) �ŏ������čő剻
	//ShowWindow(hwnd, SW_MINIMIZE);
	//ShowWindow(hwnd, SW_MAXIMIZE);
}
// 2014/12/24�G�N�T�C�� -- �����܂� --





mtk_movie::mtk_movie() :
	m_pGB(NULL),
	m_pMC(NULL),
	m_pEv(NULL),
	m_pMS(NULL),
	m_pVideo(NULL),
	m_state(STATE_NO_MOVIE),
	m_end_pos(0)
{

}


mtk_movie::~mtk_movie()
{
	CleanUpGraph();
}



void
mtk_movie::CleanUpGraph()
{
	if (m_pMC) {
		m_pMC->Stop();
	}

	SAFE_RELEASE(m_pMS);
	SAFE_RELEASE(m_pEv);
	SAFE_RELEASE(m_pMC);

	delete m_pVideo;
	m_pVideo = NULL;

	SAFE_RELEASE(m_pGB);

	m_state = STATE_NO_MOVIE;
	m_end_pos = 0;
}



HRESULT
mtk_movie::Open(const char *fn, HWND hwnd, int x0, int y0, int w, int h, UINT u_WM_NOTIFY, int iRenderer)
{
	HRESULT hr;
	WCHAR wFile[MAX_PATH];



	//--  Clear
	CleanUpGraph();


	//--  Init Graph
	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&m_pGB);
	if (FAILED(hr)) {
		goto done;
	}


	//--  Create Interfaces
	hr = m_pGB->QueryInterface(IID_IMediaControl, (void**)&m_pMC);
	if (FAILED(hr)) {
		goto done;
	}

	hr = m_pGB->QueryInterface(IID_IMediaEventEx, (void**)&m_pEv);
	if (FAILED(hr)) {
		goto done;
	}
	hr = m_pEv->SetNotifyWindow((OAHWND)hwnd, u_WM_NOTIFY, NULL);
	if (FAILED(hr)) {
		goto done;
	}

	hr = m_pGB->QueryInterface(IID_IMediaSeeking, (void**)&m_pMS);
	if (FAILED(hr)) {
		goto done;
	}



	//--  Create Video Renderer
	switch (iRenderer) {
	case RENDERER_WINDOWED_VMR7:
		m_pVideo = new (mtk_video_window_mode);
		break;
	case RENDERER_WINDOWLESS_VMR7:
		m_pVideo = new (mtk_video_windowless_VMR7);
		break;
	case RENDERER_WINDOWLESS_VMR9:
		m_pVideo = new (mtk_video_windowless_VMR9);
		break;
	case RENDERER_EVR:
		m_pVideo = new (mtk_video_EVR);
		break;
	}

	if (m_pVideo == NULL) {
		hr = E_OUTOFMEMORY;
		goto done;
	}



	
	
	hr = m_pVideo->Init(hwnd, m_pGB);
	if (FAILED(hr)) {
		goto done;
	}
	
	


	//--  File Open
	wcscpy(wFile, CA2W(fn));
	hr = m_pGB->RenderFile(wFile, NULL);
	if (FAILED(hr)) {
		goto done;
	}




	//--  State
	m_state = STATE_STOPPED;
	hr = m_pMS->GetStopPosition(&m_end_pos);


	//m_pMC->Pause();


	//--  Set View Window (Fit to Window)
	long lw, lh;
	m_pVideo->GetVideoSize(&lw, &lh) ;

	float rt_wn = float(h) / float(w);
	float rt_mv = float(lh) / float(lw);
	if (rt_mv > rt_wn) {
		// �E�B���h�E�������[�r�[�̕����c��
		m_w = int(h / rt_mv);
		m_h = h;
	}
	else {
		// ���[�r�[�����E�B���h�E�̕����c��
		m_w = w;
		m_h = int(w*rt_mv);
	}
	
	int dx, dy;
	dx = (w - m_w) / 2;
	dy = (h - m_h) / 2;
	m_x = x0 + dx;
	m_y = y0 + dy;

	bool bFullScreen = (!bNetwork && is_fullscreen_mode()) || (bNetwork && (w==disp_w) && (h==disp_h));


	hr = m_pVideo->SetViewWindow(hwnd, m_x, m_y, m_w, m_h, bFullScreen);


	m_pMC->Pause();



	bool bWindows7 = (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 1);
	

	if (bFullScreen) {
		do {
			Sleep(10);
			ForceForegroundWindow(hwnd, x0, y0, w, h); // 2014/12/26�G�N�T�C��
		} while (GetForegroundWindow() != hwnd);
		SetCursorPos(0, h); // 2014/12/26�G�N�T�ǉ�
	}

done:
	if (FAILED(hr)) {
		CleanUpGraph();
	}
	return hr;
}



void
mtk_movie::Play()
{
	if (m_state != STATE_PAUSED && m_state != STATE_STOPPED) {
		return;
	}

	HRESULT hr = m_pMC->Run();
	if (SUCCEEDED(hr)) {
		m_state = STATE_PLAYING;
	}
}



void
mtk_movie::Pause()
{
	if (m_state != STATE_PLAYING) {
		return;
	}

	HRESULT hr = m_pMC->Pause();
	if (SUCCEEDED(hr)) {
		m_state = STATE_PAUSED;
	}
}



void
mtk_movie::Home()
{
	SetPosition(0);
}



void
mtk_movie::End()
{
	SetPosition(m_end_pos);
}



void
mtk_movie::Close()
{
	CleanUpGraph();
}



LONGLONG
mtk_movie::GetPosition() const
{
	if (m_state == STATE_NO_MOVIE) {
		return 0;
	}

	LONGLONG pos;
	HRESULT hr = m_pMS->GetCurrentPosition(&pos);
	if (FAILED(hr)) {
		return 0;
	}
	return pos;
}


LONGLONG
mtk_movie::GetEndPosition() const
{
	if (m_state == STATE_NO_MOVIE) {
		return 0;
	}

	return m_end_pos;
}



void
mtk_movie::SetPosition(LONGLONG pos)
{
	if (m_state == STATE_NO_MOVIE) {
		return;
	}

	if (m_state != STATE_PAUSED) {
		HRESULT hr = m_pMC->Pause();
		if (FAILED(hr)) {
			return;
		}
		m_state = STATE_PAUSED;
	}

	m_pMS->SetPositions(&pos, AM_SEEKING_AbsolutePositioning | AM_SEEKING_SeekToKeyFrame, NULL, AM_SEEKING_NoPositioning);
}


void
mtk_movie::GetWindowPosition(int &x, int &y, int &w, int &h) const
{
	x = m_x;
	y = m_y;
	w = m_w;
	h = m_h;
}


int
mtk_movie::GetState() const
{
	return  m_state;
}



HRESULT
mtk_movie::OnPaint(HWND hwnd, HDC hdc)
{
	if (m_pVideo) {
		return m_pVideo->OnPaint(hwnd, hdc);
	}
	else {
		return S_OK;
	}
}



HRESULT
mtk_movie::OnDisplayChange()
{
	if (m_pVideo) {
		return m_pVideo->OnDisplayChange();
	}
	else {
		return S_OK;
	}
}



bool
mtk_movie::GetEvent(long& evCode, long& prm1, long& prm2)
{
	if (m_pEv == NULL)  return false;

	if (!SUCCEEDED(m_pEv->GetEvent(&evCode, &prm1, &prm2, 0)))	return false;
	if (!SUCCEEDED(m_pEv->FreeEventParams(evCode, prm1, prm2)))	return false;

	return true;
}





//---------------
//  Media  File
//---------------
bool
load_mif_file(const char* fn, const std::string& lang_id, media_file_info& mfi)
{
	FILE* fp = fopen_utf8(fn, "rt");
	if (fp == NULL)  return false;


	mfi.all_file_path.clear();


	std::vector<key_value> vKeyValue;
	if (!load_key_value_file(fp, lang_id, vKeyValue)) {
		fclose(fp);
		return false;
	}


	for (int i = 0; i < vKeyValue.size(); i++) {
		const key_value& kv = vKeyValue[i];


		if (stricmp(kv.key, "MEDIA_TYPE") == 0) {
			if (stricmp(kv.value, "Image") == 0)		{ mfi.type = MEDIATYPE_IMAGE; }
			if (stricmp(kv.value, "PopupImage") == 0)	{ mfi.type = MEDIATYPE_POPUP_IMAGE; }
			if (stricmp(kv.value, "Movie") == 0)		{ mfi.type = MEDIATYPE_MOVIE; }
			if (stricmp(kv.value, "Audio") == 0)		{ mfi.type = MEDIATYPE_AUDIO; }
		}
		else if (stricmp(kv.key, "FILE_PATH") == 0) {
			mfi.all_file_path.push_back(kv.value);
		}
		else if (stricmp(kv.key, "MENU_TITLE") == 0) {
			mfi.menu_title = kv.value;
		}
		else if (stricmp(kv.key, "DISP_TITLE") == 0) {
			mfi.disp_title = kv.value;
		}
		else {
			continue;
		}
	}


	int file_num = mfi.all_file_path.size();
	if (file_num > 0) {
		mfi.file_path = mfi.all_file_path[file_num - 1];
	}


	fclose(fp);
	return true;
}


bool
MediaFiles::open_info_file(const directory& dir, const char* fn, const std::string& lang_id)
{
	const int BUF_N = 1024;
	static char  buf[BUF_N];


	FILE* fp = dir.fopen_utf8(fn, "rt");
	if (fp == NULL)  return false;
	clear();

	//-- �e���f�B�A�t�@�C���̏����擾
	while (true) {
		char *p;
		p = fgets(buf, BUF_N, fp);
		if (p == NULL) break;

		std::string fn = trim(remove_cpp_comment(buf), " \t\n");
		if (fn == "") {
			continue;
		}


		media_file_info  mfi;

		if (!load_mif_file(dir.get_path(fn.c_str()), lang_id, mfi)) {
			continue;
		}
		add_media_file(mfi);
	}

	fclose(fp);
	return  true;
}


void
MediaFiles::add_media_file(const media_file_info& _mfi)
{
	media_file_info mfi = _mfi;

	int idx = size();
	mfi.menu_title_key = list_prefix + "_MENU_TITLE_KEY_" + to_string(idx);
	mfi.disp_title_key = list_prefix + "_DISP_TITLE_KEY_" + to_string(idx);

	set_string(mfi.menu_title_key, mfi.menu_title);


	if (mfi.disp_title != "") {
		set_string(mfi.disp_title_key, mfi.disp_title);
	}
	else {
		set_string(mfi.disp_title_key, mfi.menu_title);
	}


	push_back(mfi);
}
