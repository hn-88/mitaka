/**************************************************************************************************

Copyright (c) 2007-2015   Tsunehiko Kato  and  4D2U Project, National Astronomical Observatory of Japan

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
#include  "mtk_network_mode.h"
#include  "../common/graphics/gl_win.h"
#include  "../common/frame_rate.h"
#include  "mitaka.h"
#include  "mtk_mode.h"
#include  "mtk_switches.h"
#include  "mtk_dialog_box.h"
#include  "mtk_window_mode.h"
#include  "mtk_menu_bar.h"
#include  "mtk_time.h"
#include  "mtk_camera.h"
#include  "mtk_camera_setting.h"
#include  "mtk_directory.h"
#include  "mtk_tcp.h"
#include  "mtk_movie.h"
#include  "mtk_languages.h"
#include  "mtk_location.h"
#include  "resource.h"



//-----------
//  Externs
//-----------
extern HINSTANCE	g_hInst;
extern HWND			g_hWnd;
extern HCURSOR		hCursorArrow;
extern COpenGL		ogl;
extern int			win_w, win_h, win_w0, win_h0;
extern int			win_x0, win_y0;
extern int			gl_x0, gl_y0, gl_w, gl_h;
extern fps_counter  FrmCounter, FrmCounter2;
extern bool			bMotion;
extern char			szAppName[];
extern bool			bDrawLowRes, bEnableDrawLowRes;
extern int			win_mgn_w1, win_mgn_h1;
extern int			win_mgn_w2, win_mgn_h2;
extern MediaFiles	MVF, IMGF, AUDF;
extern float  landing_lon, landing_lat;


extern	mtk_movie  dsmv2;


void	init();
void	init_menu();
void	OnCommandSignal();
void	ClearSignal();
bool	GetSignal();
void	WaitForSignal();
void	OnBaseCameraOri(quaterniond ori);
void	draw(const mtk_camera camera_base);
void	OnCreate(HWND hWnd);
void	OnDestroy();
void	OnActivate(WPARAM wParam);
void	OnDirectShowEvent();
int		OnKeyDown(HWND hWnd, WPARAM wParam);
void	OnTakeoff();
void	OnLanding();
void	OnChangeTime(const double& JD);
void	process_zoom(const float& _zm);
void	output_elapsed_time(const fps_counter& fps_draw, const fps_counter& fps_others);

mtk_camera	update_time_and_global_camera();

void	open_media_info_files(const std::string& lang_id);




//--  Network
bool	bNetwork = false;	// �ʐM�����邩�ǂ���
bool	bControler = false; // �R���g���[���[��
//--  Draw
bool bDraw				= false;	// �`����s�t���O
bool bDrawing			= false;	// �`����s���̃t���O



//========
//  Draw
//========
void
Draw_NetworkMode(const mtk_camera& camera_base)
{
	draw(camera_base);
}

void
OnDraw_NetworkMode(HDC hDC)
{
	static bool first = true;

	if (bMovieMode) {
		dsmv2.OnPaint(g_hWnd, hDC);
		return;
	}



	if (first) {
		FrmCounter2.begin_frame();
		first = false;
	}

	FrmCounter2.end_frame();


	bDrawing = true;
	FrmCounter.begin_frame();

	mtk_camera  camera_base;
	camera_base = update_time_and_global_camera();	// �`��̑O�Ɉ�x�Ă�

	ogl.MakeCurrent(hDC);
		Draw_NetworkMode(camera_base);
		BOOL res;
		res = SwapBuffers(hDC);
	ogl.MakeCurrent(NULL);
	FrmCounter.end_frame();
	bDrawing =false;

	if (bDebug) {
		output_elapsed_time(FrmCounter, FrmCounter2);
	}

	FrmCounter2.begin_frame();
}

void
OnCommandDraw_LowRes()
{
	if (bControler)  { send_command(COMMAND_DRAW_LOWRES); }
	bDraw = true;
	bDrawLowRes = true;
}

void
OnCommandDraw_HighRes()
{
	if (bControler)  { send_command(COMMAND_DRAW_HIGHRES); }
	bDraw = true;
	bDrawLowRes = false;
}

void
OnCommandDraw()
{
	if (bControler || !bNetwork) {
		if (bEnableDrawLowRes && bMotion)	{ OnCommandDraw_LowRes(); }
		else			{ OnCommandDraw_HighRes(); }
	}

	//***********
	if (!bNetwork) {
		InvalidateRect(g_hWnd, NULL, false);
	}
}


//--------
//  Size
//--------
// �E�B���h�E�T�C�Y��ς���
void
Reshape_NetworkMode(int w, int h)
{
	if (w==0 || h==0)  return;
	set_trackball_area(win_w0, win_h0);
	glViewport(gl_x0, gl_y0, gl_w, gl_h);

}

void
OnSize_NetworkMode(HWND hWnd)
{
	/*
	if (bMovieMode) {
		//dsmv2.OnSize(g_hWnd, hDC);
		return;
	}
	*/

	int  w, h;
	RECT  rt;
	GetClientRect(hWnd, &rt);
	w = rt.right  - rt.left;
	h = rt.bottom - rt.top;

	SetScreenButtonSize(w, h);

	//
	HDC  hDC = GetDC(hWnd);
	ogl.MakeCurrent(hDC);
		Reshape_NetworkMode(w, h);
	ogl.MakeCurrent(NULL);
	ReleaseDC(hWnd, hDC);
}


//====================
//  Window Procedure
//====================
LRESULT CALLBACK
WndProc_NetworkMode(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	PAINTSTRUCT ps;
	
	switch (msg) {
	case WM_CREATE:
		OnCreate(hWnd);
		return 0;
	case WM_COMMAND:
		if (process_menu_command(hWnd, LOWORD(wParam)))  {
			OnCommandDraw();
			return 0;
		}
		break;
	case WM_INITMENUPOPUP:
		process_menu_checks(wParam);
		break;
	case WM_PAINT:

		hDC = BeginPaint(hWnd, &ps);
			//--- OnDraw(hDC); ---
			//dsmv2.OnPaint(hWnd, hDC);
		EndPaint(hWnd, &ps);

		if (bMovieMode) {
			dsmv2.OnPaint(hWnd, hDC);
			return 0;
		}

		bDraw = true;
		OnCommandDraw();
		return 0;
	case WM_SIZE:
		OnSize_NetworkMode(hWnd);
		return 0;
	case WM_ACTIVATE:
		OnActivate(wParam);
		return 0;
	case WM_DISPLAYCHANGE:
		dsmv2.OnDisplayChange();
		break;
	case WM_LBUTTONDOWN:
		OnLB( LOWORD(lParam), HIWORD(lParam), MOUSE_DOWN, wParam );
		return 0;
	case WM_LBUTTONUP:
		OnLB( LOWORD(lParam), HIWORD(lParam), MOUSE_UP, wParam );
		return 0;
	case WM_RBUTTONDOWN:
		OnRB( LOWORD(lParam), HIWORD(lParam), MOUSE_DOWN, wParam );
		return 0;
	case WM_RBUTTONUP:
		OnRB( LOWORD(lParam), HIWORD(lParam), MOUSE_UP, wParam );
		return 0;
	case WM_MOUSEMOVE:
		OnMove( LOWORD(lParam), HIWORD(lParam), wParam );
		return 0;
	case WM_MBUTTONUP:
		// �����E����
		switch (get_mode()) {
			case MODE_PLANETARIUM:	OnTakeoff();	break;
			case MODE_3D:			OnLanding();	break;
			default:								return 0;
		}
		OnCommandDraw();
		return 0;
	case WM_MOUSEWHEEL:
		OnMouseWheel( (short)HIWORD(wParam) );
		return 0;
	case WM_LBUTTONDBLCLK:
		OnLBDBLCLK(LOWORD(lParam), HIWORD(lParam));
		return 0;
	case WM_KEYDOWN:
		if (OnKeyDown(hWnd, wParam)==0)  return 0;
		break;
	case WM_SETCURSOR:
		POINT pos;
		GetCursorPos(&pos);
		ScreenToClient(hWnd, &pos);
		if (pos.x >= win_mgn_w1 && pos.x < win_mgn_w1 + win_w
			&& pos.y >= win_mgn_h1 && pos.y < win_mgn_h1 + win_h) {
			SetCursor(NULL);
			return 0;
		}
		break;	// �f�t�H���g�̃J�[�\������
	case WM_ERASEBKGND:
		return 0;
	case WM_GRAPHNOTIFY:
		OnDirectShowEvent();
		return 0;
	case WM_DESTROY:
		OnDestroy();
		PostQuitMessage(0);
		return 0;
	}	
	return  DefWindowProc(hWnd , msg , wParam , lParam);
}


//--------
//  Init
//--------
void
init_network_mode()
{
	//--  ������
	init();

	//--  TCP �̏�����
	SetSplashText("WATING_FOR_NETWORK_CONNECTION");
	init_TCP(bControler);


	//--  ���f�B�A�E����W�Ȃǂ̏��𓯊�������  --//
	ClearSignal();
	if (bControler) {

		//--- Media info file �i���j���[�����O�ɏ������j---
		const std::string& lang_id = get_current_language_id();
		open_media_info_files(lang_id);

		send_media_info(MVF, MEDIATYPE_MOVIE);
		send_media_info(IMGF, MEDIATYPE_IMAGE);
		send_media_info(AUDF, MEDIATYPE_AUDIO);

		//---
		load_initial_location("location.dat", lang_id);
		send_location_info( get_initial_location() );


		// ����W�n
		{
			set_base_camera( quaterniond(1,0,0,0) );
			FILE* fp = fopen("base.cfg", "rt");
			if (fp != NULL) {
				load_base_camera_setting(fp);
				fclose(fp);
			}

			OnBaseCameraOri( get_base_camera_ori() );
		}

		OnCommandSignal();
	} else {
		WaitForSignal();
	}

	//--- location ---
	const location& loc = get_initial_location();
	landing_lon = loc.lon;
	landing_lat = loc.lat;


	//--- setup menu ---
	init_menu();

	//--  ������ݒ肵�Ȃ���
	if (bControler) {
		OnChangeTime( get_time_JD() );
	}
}


//=============
//  Main Loop
//=============
int
Main_NetworkMode( HINSTANCE hInstance, int iCmdShow )
{
	//-----------------------
	//  �E�B���h�E�N���X�̓o�^
	//-----------------------
	g_hInst = hInstance;

	WNDCLASS	wndclass;
	wndclass.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wndclass.lpfnWndProc	= WndProc_NetworkMode;
	wndclass.cbClsExtra		= 0;
	wndclass.cbWndExtra		= 0;
	wndclass.hInstance		= hInstance;
	wndclass.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MITAKA_ICON) );
	wndclass.hCursor		= hCursorArrow;
	wndclass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszMenuName	= NULL;
	wndclass.lpszClassName	= szAppName;
	if (!RegisterClass(&wndclass)) {
		MessageBox(NULL, TEXT("This program requires WindowsNT!"), szAppName, MB_ICONERROR);
		return 0;
	}

	//--- �X�v���b�V���E�B���h�E�N�� ---
	CreateSplashWindow();

	//-- init data and view
	init_network_mode();

	//--- window size/pos ---
	DWORD  dwStyle;
	dwStyle = WS_VISIBLE|WS_POPUP;

	//--- Create window ---
	HWND hWnd;
	hWnd = CreateWindowEx(
						WS_EX_TOPMOST,
						szAppName,
						szAppName,							// window caption
						dwStyle,							// window style
						win_x0, win_y0,						// window position
						win_w0, win_h0,						// window size
						NULL,								// parent window handle
						NULL,								// window menu handle
						hInstance,							// program instance handle
						NULL);								// creation parameters

	if (hWnd == NULL) {
		MessageBox(NULL, TEXT("�E�B���h�E���쐬�ł��܂���ł���"), szAppName, MB_ICONERROR);
		return 0;
	}

	//------------------
	//  �E�B���h�E�̕\��
	//------------------
	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);
	SetForegroundWindow(hWnd);
	SetCursorPos(0, 0);

	//--- �X�v���b�V���E�E�B���h�E������ ---
	DestroySplashWindow();

	//---  �X�N���[���Z�[�o�[���I�t ---
	SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, FALSE, NULL, SPIF_SENDCHANGE);

	//-------------
	//  main loop
	//-------------
	MSG msg;
	while (true) {

		if (PeekMessage(&msg, NULL, 0,0, PM_REMOVE))  {
			if (msg.message == WM_QUIT)  break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else if (bDraw) {
			HDC  hDC = GetDC(hWnd);
			bDraw = false;
			OnDraw_NetworkMode(hDC);
			ReleaseDC(hWnd, hDC);
		} /*else if (bRecordMovie) {
			HDC  hDC = GetDC(hWnd);
			OnDraw_NetworkMode(hDC);
			ReleaseDC(hWnd, hDC);
		}*/ else {
			Sleep(1);
		}
	}

	return 0;
}
