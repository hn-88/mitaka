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
#include  "mtk_window_mode.h"
#include <mmsystem.h>
#include  "../common/graphics/gl_win.h"
#include  "../common/astro/astro_const.h"
#include  "../common/interface/trackball.h"
#include  "../common/frame_rate.h"
#include  "../common/win_dlg.h"
#include  "mitaka.h"
#include  "mtk_mode.h"
#include  "mtk_switches.h"
#include  "mtk_ini_file.h"
#include  "mtk_stereo.h"
#include  "mtk_dialog_box.h"
#include  "mtk_menu_bar.h"
#include  "mtk_screen.h"
#include  "mtk_target.h"
#include  "mtk_movement.h"
#include  "mtk_planets.h"
#include  "mtk_planet_orientation.h"
#include  "mtk_label.h"
#include  "mtk_planetarium.h"
#include  "mtk_on_screen.h"
#include  "mtk_screen_button.h"
#include  "mtk_camera_setting.h"
#include  "mtk_directory.h"
#include  "mtk_movie.h"
#include  "mtk_pbuffer.h"
#include  "mtk_objects.h"
#include  "mtk_fixed_object.h"
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
extern int			gl_w, gl_h;
extern fps_counter  FrmCounter, FrmCounter2;
extern bool			bMotion;
extern mtk_camera	g_camera;
extern char			szAppName[];
extern mtk_movie  dsmv2;
extern MediaFiles	MVF, IMGF, AUDF;
extern bool bIniEnablePBuffer;
extern float  landing_lon, landing_lat;


void	init();
void	init_menu();
void	draw(const mtk_camera camera_base);
void	OnCreate(HWND hWnd);
void	OnDestroy();
void	OnActivate(WPARAM wParam);
void	OnDirectShowEvent();
int		OnKeyDown(HWND hWnd, WPARAM wParam);
void	OnTakeoff();
void	OnLanding();
void	OnChangeTargetControler(int tgt_ID);
void	OnPlanetariumAngle(float val);
void	OnPopup(int pidx);
void	OnSetZoom(double val);
void	OnUpdateViewCtrl();
void	OnUpdateViewCtrl2();
void	OnBaseCameraOri(quaterniond ori);
void	process_zoom(const float& _zm);
float	get_move_speed();

void	open_media_info_files(const std::string& lang_id);



mtk_draw_struct get_latest_draw_struct(int idx);
mtk_camera	update_time_and_global_camera();



enum {
	MOUSE_AREA_NONE,
	MOUSE_AREA_LEFT,
	MOUSE_AREA_RIGHT,
	MOUSE_AREA_TOP,
	MOUSE_AREA_BOTTOM
};


//--  Window
static bool bMaximized		= false;	// �ő剻���Ă��邩�ǂ���
static bool bFullScreen		= false;	// �t���X�N���[�����ǂ���
static bool bResizingWindow	= false;	// �E�B���h�E�����T�C�Y�����ǂ���
//--  mouse
bool	bMouseZoomReverse = false;	// �Y�[������𔽓]����
float   mouse_hide_timeout = 3.0f;	// �}�E�X�J�[�\���������܂ł̎��ԁi0�ȉ��ŏ����Ȃ��j
//--  trackball
static trackball  tb;
//--- ���ԑ���{�^���E�Y�[���{�^�� ---
mtk_screen_button_minus	sbTimeMinus, sbZoomMinus;
mtk_screen_button_plus	sbTimePlus,  sbZoomPlus;
const int TimeBtnSize = 24;
const int ZoomBtnSize = 24;
//
static int mouse_area = MOUSE_AREA_NONE;	// �}�E�X�̃h���b�O���J�n�����G���A


//-------
//  Get
//-------
bool
is_fullscreen_mode()
{
	return  bFullScreen;
}



//-------------
//  Trackball
//-------------
void
set_trackball_area(int w, int h)
{
	tb.set_area(w, h);
}

void
set_trackball_point1(float x, float y)
{
	tb.set_point1(x, y);
}

quaternionf
set_trackball_point2(float x, float y)
{
	return tb.set_point2(x, y);
}



//--------------
//  �{�^���̕`��
//--------------
void
draw_screen_buttons_sub(int x0, int y0, int w, int h)
{
	gl_enter_2D_mode_vwpt(x0,y0, w,h);
		//--  ���ԃ{�^��
		if (sbTimeMinus.GetState() != SCRBTN_UNPUSH || sbTimePlus.GetState() != SCRBTN_UNPUSH) { 
			sbTimeMinus.Draw();
			sbTimePlus.Draw();
		}
		//--  �Y�[���{�^��
		if (sbZoomMinus.GetState() != SCRBTN_UNPUSH || sbZoomPlus.GetState() != SCRBTN_UNPUSH) { 
			sbZoomMinus.Draw();
			sbZoomPlus.Draw();
		}
	gl_leave_2D_mode();
}

void
draw_screen_buttons()
{
	glDisable(GL_TEXTURE_2D);

	int xh = win_w/2;
	int yh = win_h/2;

	switch(get_display_mode()) {
		case DISPMODE_SIDE_BY_SIDE:
			draw_screen_buttons_sub(0, 0, xh,win_h);
			draw_screen_buttons_sub(xh,0, xh,win_h);
			break;
		case DISPMODE_TOP_AND_BOTTOM:
			draw_screen_buttons_sub(0, 0,  win_w,yh);
			draw_screen_buttons_sub(0, yh, win_w,yh);
			break;
		default:
			draw_screen_buttons_sub(0,0,win_w, win_h);
			break;
	}
}



//--------
//  Draw
//--------
void
Draw_WindowMode(const mtk_camera &camera)
{
	//--  �V�[���̕`��
	draw(camera);
	//--  �{�^���̕`��
	draw_screen_buttons();
	//--  �T�C�Y�ύX��
	if (bResizingWindow) {
		draw_window_size();
	}
}


void
output_elapsed_time(const fps_counter& fps_draw, const fps_counter& fps_others)
{
	static bool first = true;
	FILE *fp;
	float t1, t2, t_tot, fps;


	t1 = fps_draw.get_elapsed_time();
	t2 = fps_others.get_elapsed_time();
	t_tot = t1 + t2;
	fps = 1000.0 / t_tot;

	if (first) {
		fp = fopen("elap_time.dat", "wt");
		first = false;
	}
	else {
		fp = fopen("elap_time.dat", "at");
	}
	fprintf(fp, "%e  %e  %e  %e\n", t1, t2, t_tot, fps);
	fclose(fp);
}


void
OnDraw_WindowMode(HDC hDC)
{
	static bool first = true;
	static int count = 0;

	count++;
	{
		char str[256];
		sprintf(str, "Draw(%d)\n", count);
		OutputDebugString(str);
	}


	if (bMovieMode) {
		dsmv2.OnPaint(g_hWnd, hDC);
		return;
	}


	if (first) {
		FrmCounter2.begin_frame();
		first = false;
	}

	FrmCounter2.end_frame();


	FrmCounter.begin_frame();


	mtk_camera camera_base;
	camera_base = update_time_and_global_camera();	// �`��̑O�Ɉ�x�Ă�

	ogl.MakeCurrent(hDC);
		Draw_WindowMode(camera_base);
		BOOL res;
		res = SwapBuffers(hDC);
	ogl.MakeCurrent(NULL);
	FrmCounter.end_frame();



	//******
	if (bDebug) {
		output_elapsed_time(FrmCounter, FrmCounter2);
	}


	FrmCounter2.begin_frame();
}


//---------
//  �T�C�Y
//---------
// �E�B���h�E�T�C�Y��ς���
void
Reshape_WindowMode(int w, int h)
{
	if (w==0 || h==0)  return;
	win_w = w;
	win_h = h;
	gl_w = win_w;
	gl_h = win_h;
	tb.set_area(w, h);
	glViewport(0, 0, gl_w, gl_h);
}


void
SetScreenButtonSize(int w, int h)
{
	if (w==0 || h==0)  return;
	//-- �{�^���̈�̐ݒ�
	sbTimeMinus.SetRect(w - 2*TimeBtnSize, 0, TimeBtnSize);
	sbTimePlus.SetRect( w - TimeBtnSize, 0, TimeBtnSize);
	sbZoomMinus.SetRect(w - ZoomBtnSize, h-2*ZoomBtnSize, ZoomBtnSize);
	sbZoomPlus.SetRect( w - ZoomBtnSize, h-ZoomBtnSize,   ZoomBtnSize);
}


void
OnSize_WindowMode(HWND hWnd)
{
	int  w, h;
	RECT  rt;
	GetClientRect(hWnd, &rt);
	w = rt.right  - rt.left;
	h = rt.bottom - rt.top;
	SetScreenButtonSize(w, h);

	HDC  hDC = GetDC(hWnd);
	ogl.MakeCurrent(hDC);
		Reshape_WindowMode(w, h);
	ogl.MakeCurrent(NULL);
	ReleaseDC(hWnd, hDC);
}


//----------------------------
//  �E�B���h�E�̍ő剻�ƃ��X�g�A
//----------------------------
static bool bReMaximize = false;
static RECT win_rect;

//---  �ő剻���̏���
void
OnWindowMaximized(HWND hWnd)
{
	bMaximized = true;
}

//---  ���ɖ߂������̏���
void
OnWindowRestored(HWND hWnd)
{
	if (bFullScreen) {
		int w, h;
		w = win_rect.right-win_rect.left;
		h = win_rect.bottom-win_rect.top;
		if (w <= 0) w = 640;
		if (h <= 0) h = 480;

		// �E�B���h�E�̈ʒu�ƃT�C�Y�𕜌�����
		SetWindowPos(	hWnd, HWND_NOTOPMOST,
						win_rect.left, win_rect.top, w, h,
						NULL);
		bFullScreen = false;
	}
	bMaximized = false;
}

//---  �t���X�N���[�����̏���
void
OnEnterFullScreenMode(HWND hWnd)
{
	EnableMenuBar(hWnd, false);								// ���j���[���폜

	if (bMaximized) {
		SetWindowLong(hWnd, GWL_STYLE, WS_VISIBLE|WS_POPUP|WS_MAXIMIZE);	// �^�C�g���o�[�����폜
		ShowWindow(hWnd, SW_RESTORE);	// ��U�A�ő剻��߂�
		bReMaximize = true;
	} else {
		SetWindowLong(hWnd, GWL_STYLE, WS_VISIBLE|WS_POPUP);	// �^�C�g���o�[�����폜
		bReMaximize = false;
	}

	bFullScreen = true;
	GetWindowRect(hWnd, &win_rect);
	ShowWindow(hWnd, SW_MAXIMIZE);					// �ő剻
}

//---  �t���X�N���[�����畜�A���鎞�̏���
void
OnLeaveFullScreen(HWND hWnd)
{
	ShowWindow(hWnd, SW_RESTORE);								// �ő剻����̉�
	SetWindowLong(hWnd, GWL_STYLE, WS_VISIBLE|WS_OVERLAPPEDWINDOW);
	if (bReMaximize) {
		ShowWindow(hWnd, SW_MAXIMIZE);							// �Ăэő剻
	}
	EnableMenuBar(hWnd, true);
}

//
void
OnFullScreen(bool bFlg)
{
	if (bFlg)	{ OnEnterFullScreenMode(g_hWnd); }
	else		{ OnLeaveFullScreen(g_hWnd); }
}



//----------------------------
//  �������[�h�ł� x,y ���W�̏C��
//----------------------------
void
set_mouse_area(int x, int y)
{
	switch (get_display_mode()) {
		case DISPMODE_SIDE_BY_SIDE:
			x *= 2;
			if (x > win_w) { mouse_area = MOUSE_AREA_RIGHT; }
			else		   { mouse_area = MOUSE_AREA_LEFT; }
			break;
		case DISPMODE_TOP_AND_BOTTOM:
			y *= 2;
			if (y > win_h) { mouse_area = MOUSE_AREA_BOTTOM; }
			else		   { mouse_area = MOUSE_AREA_TOP; }
			break;
		default:
			mouse_area = MOUSE_AREA_NONE;
			break;
	}
}

void
correct_mouse_xy(int &x, int &y)
{
	switch (mouse_area) {
		case MOUSE_AREA_LEFT:
			x = x*2;
			break;
		case MOUSE_AREA_RIGHT:
			x = x*2 - win_w;
			break;
		case MOUSE_AREA_TOP:
			y = y * 2;
			break;
		case MOUSE_AREA_BOTTOM:
			y = y*2 - win_h;
			break;
		case MOUSE_AREA_NONE:
		default:
			break;
	}
}



//------------
//  �}�E�X����
//------------
bool
isInsideRect(int x, int y, RECT& rc)
{
	return  (x>=rc.left && x<=rc.right && y>=rc.top && y<=rc.bottom);
}

void
process_mouse_rotation_plnt(const float x, const float y, const float lon0, const float lat0,
							float& _dlon, float& _dlat)
{
	const float fac = 0.001 * min(win_w, win_h) * get_plnt_angle()/45.0;
	float dlon = 0.05 * fac * x;
	float dlat = 0.05 * fac * y;

	float lon = lon0 + dlon;
	if (lon < 0)		lon += 360;
	if (lon >= 360)		lon -= 360;
	set_plnt_long(lon);

	float lat = lat0 + dlat;
	if (lat < 0)		lat = 0;
	if (lat > 90)		lat = 90;
	set_plnt_lati(lat);

	_dlon = dlon;
	_dlat = dlat;
}


float get_rotation_factor();

// Time
bool
process_time_button_LB_down(short x, short y)
{
	if (sbTimeMinus.SetPushState(x,y) || sbTimePlus.SetPushState(x,y)) {
		return true;
	} else {
		return false;
	}
}

bool
process_time_button_LB_up()
{
	if (sbTimeMinus.GetState() != SCRBTN_PUSH && sbTimePlus.GetState() != SCRBTN_PUSH) {
		return false;
	}
	sbTimeMinus.SetState( SCRBTN_UNPUSH );
	sbTimePlus.SetState(  SCRBTN_UNPUSH );
	return true;
}

// Zoom
bool
process_zoom_button_LB_down(short x, short y)
{
	if (sbZoomMinus.SetPushState(x,y) || sbZoomPlus.SetPushState(x,y)) {
		return true;
	} else {
		return false;
	}
}

bool
process_zoom_button_LB_up()
{
	if (sbZoomMinus.GetState() != SCRBTN_PUSH && sbZoomPlus.GetState() != SCRBTN_PUSH) {
		return false;
	}
	sbZoomMinus.SetState( SCRBTN_UNPUSH );
	sbZoomPlus.SetState(  SCRBTN_UNPUSH );
	return true;
}

//--------------
//  ���N���b�N��
//--------------
quaternionf
OnLB_get_rotation_quaternion(int x, int y)
{
	quaternionf orif = tb.set_point2(x,y);
	float norm = orif.v.norm();
	if (norm < 1e-10)	return  quaternionf(1,0,0,0);	// ��]���Ȃ�
	// ��]�p�𒲐�
	if (get_mode() == MODE_3D) {
		if (orif.w < 0.0f)			orif = -orif;
		double th0;
		if (orif.w >= 1.0f)			th0 = 0.0f;
		else						th0 = acos(orif.w);

		const double th = th0 * get_rotation_factor();

		orif.w = cos(-th);
		orif.v = float(sin(-th)) * orif.v.normalized();
	}
	return  orif;
}

void
OnLB(int x, int y, mouse_state stat, WPARAM wParam)
{
	static bool fBlocking = false;
	static quaterniond  ori0;
	static float lon0, lat0;
	static int x1, y1;
	static bool bTimeButton = false;
	static bool bZoomButton = false;
	static DWORD last_time;
	static quaternionf  last_orif;
	static vector3f  Omg;
	static float last_dlon, last_dlat;
	static float vlon, vlat;

	const mtk_object& tgt = get_target();

	if (stat==MOUSE_DOWN) {
		set_mouse_area(x,y);
	}
	correct_mouse_xy(x, y);


	bool bFlg = (get_mode() == MODE_3D || tgt.get_type() != OBJ_TYPE_PLANET);
	switch (stat) {
	case MOUSE_DOWN:
		SetCapture(g_hWnd);
		bTimeButton = false;
		bZoomButton = false;
		if (process_time_button_LB_down(x,y)) {
			bTimeButton = true;
		} else if (process_zoom_button_LB_down(x,y)) {
			bZoomButton = true;
		} else {
			if (bFlg)	{ ori0 = g_camera.get_ori();  tb.set_point1(x,y); }
			else {
				x1=x; y1=y;
				lon0 = get_plnt_long(); lat0 = get_plnt_lati();
				last_dlon = 0;
				last_dlat = 0;
				vlon = vlat = 0;
			}
			last_orif.set(1,0,0,0);
		}
		last_time = timeGetTime();
		fBlocking = true;
		break;
	case MOUSE_UP:
		if (fBlocking) {
			if (bTimeButton) { process_time_button_LB_up(); }
			if (bZoomButton) { process_zoom_button_LB_up(); }
			if (!bTimeButton && !bZoomButton) {
				if (bFlg) {
					if (timeGetTime() - last_time > 10)  { Omg.set(0,0,0); }
					SetInputAngularVelocity3D(Omg);	//-- ��]���x��ݒ�
				} else {
					if (timeGetTime() - last_time > 10)  { vlon = vlat = 0; }
					SetInputLonLatSpeedPlanetarium(vlon, vlat);
				}
			}
			ReleaseCapture();
			fBlocking = false;
		}
		break;
	case MOUSE_MOVE:
		if (fBlocking && !bTimeButton && !bZoomButton) {
			float dt = (timeGetTime() - last_time) / 1000.0;
			if (dt < 1.0/1000)  dt = 1.0/1000;
			if (bFlg)	{ 
				quaternionf  orif = OnLB_get_rotation_quaternion(x, y);
				g_camera.set_ori(ori0 * quaterniond(orif));
				//-- �O�t���[������̕ω��������]���x�����߂�
				quaternionf  dori = last_orif.inv() * orif;
				float ang = acos(dori.w);
				Omg = (ang/dt) * dori.v.normalized();
				last_orif = orif;
			} else {
				float dlon, dlat;
				process_mouse_rotation_plnt(x-x1, y-y1, lon0, lat0, dlon, dlat);
				vlon = (dlon - last_dlon) / dt;
				vlat = (dlat - last_dlat) / dt;
				last_dlon = dlon;
				last_dlat = dlat;
			}
			bMotion = true;
			OnUpdateViewCtrl();
			OnUpdateViewCtrl2();
			last_time = timeGetTime();
			InvalidateRect(g_hWnd, NULL, false);
		}
		break;
	}
}


int		check_popup();

void
process_mouse_zoom(const float zm, const float scale0, const float angle0)
{
	bMotion = true;

	const mtk_object&  tgt = get_target();
	bool bFlg = (get_mode() == MODE_3D || tgt.get_type() != OBJ_TYPE_PLANET);

	if (bFlg) {
		float zma = 1.0 + fabs(zm);
		if (zm > 0)		{ OnSetZoom(scale0/zma); }
		else			{ OnSetZoom(scale0*zma); }
		int pidx = check_popup();
		if (pidx >= 0) {
			OnPopup(pidx);
		}
	} else {
		float a = 1.0 + fabs(zm);
		float ang;
		if (zm > 0)		{ ang = angle0/a; }
		else			{ ang = angle0*a; }

		if (ang >= plnt_angle_min && ang <= plnt_angle_max) {
			OnPlanetariumAngle(ang);
		}
	}
}



bool
process_time_button_RB_down(short x, short y)
{
	if (sbTimeMinus.isInside(x, y) || sbTimePlus.isInside(x, y)) {
		return true;
	}
	return false;
}


bool
process_zoom_button_RB_down(short x, short y)
{
	if (sbZoomMinus.isInside(x, y) || sbZoomPlus.isInside(x, y)) {
		return true;
	}
	return false;
}

//--------------
//  �E�N���b�N��
//--------------
void
OnRB(int x, int y, mouse_state stat, WPARAM wParam)
{
	static int   Rx=0, Ry=0;
	static bool fBlocking = false;
	static bool bTimeButton = false;
	static float scale0;
	static float angle0;
	//--
	static DWORD last_time;
	static float last_zm;
	static float v_zoom;


	if (stat==MOUSE_DOWN) {
		set_mouse_area(x,y);
	}
	correct_mouse_xy(x, y);


	switch (stat) {
		case MOUSE_DOWN:
			if (process_time_button_RB_down(x,y)) {
				popup_time_menu(g_hWnd, x,y);
			} else if (process_zoom_button_RB_down(x,y)) {
				popup_scale_menu(g_hWnd, x,y);
			} else {
				scale0 = get_current_scale();
				angle0 = get_plnt_angle();
				Rx = x;
				Ry = y;
				SetCapture(g_hWnd);
				last_zm = 0;
				last_time = timeGetTime();
				v_zoom = 0;
				fBlocking = true;
			}
			break;
		case MOUSE_UP:
			if (fBlocking) {
				ReleaseCapture();
				fBlocking = false;
				OnUpdateViewCtrl();
				OnUpdateViewCtrl2();

				if (timeGetTime() - last_time > 10) {
					v_zoom = 0;
				}
				if (get_mode() == MODE_3D) {
					SetInputZoomSpeed3D(v_zoom);
				} else {
					SetInputZoomSpeedPlanetarium(v_zoom);
				}

				InvalidateRect(g_hWnd, NULL, false);
			}
			break;
		case MOUSE_MOVE:
			if (fBlocking) {
				float zm = 0.001 * float(y - Ry);
				if (bMouseZoomReverse)  { zm = -zm; }
				process_mouse_zoom(zm, scale0, angle0);
				float dzm = zm - last_zm;
				DWORD t  = timeGetTime();
				float dt = (t - last_time) / 1000.0f;
				if (dt < 1.0/1000)  dt = 1.0/1000;
				v_zoom = dzm/dt;
				last_zm = zm;
				last_time = t;
				OnUpdateViewCtrl();
				OnUpdateViewCtrl2();
				InvalidateRect(g_hWnd, NULL, false);
			}
			break;
	}
}


//-------------
//  �}�E�X�ړ���
//-------------
void
OnMove(int x, int y, WPARAM wParam)
{
	static int x_prev = -1;
	static int y_prev = -1;

	// to avoid fake move message
	if (x == x_prev && y == y_prev) {
		return;
	}
	x_prev = x;
	y_prev = y;


	SetMouseCursorCounter(mouse_hide_timeout);

	if      ( wParam & MK_LBUTTON )	OnLB( x, y, MOUSE_MOVE, wParam );
	else if ( wParam & MK_RBUTTON )	OnRB( x, y, MOUSE_MOVE, wParam );
	else {
		bool bFlg = false;

		
		int _x, _y;
		_x = x;
		_y = y;
		set_mouse_area(_x,_y);
		correct_mouse_xy(_x,_y);
		if (sbTimeMinus.SetOverlapState(_x, _y))  bFlg = true;
		if (sbTimePlus.SetOverlapState( _x, _y))  bFlg = true;
		if (sbZoomMinus.SetOverlapState(_x, _y))  bFlg = true;
		if(sbZoomPlus.SetOverlapState( _x, _y))  bFlg = true;
		

		//--- Hit test ---
		int disp_mode = get_display_mode();

		if (get_mode() == MODE_3D && disp_mode != DISPMODE_DOME_MASTER) {
			int idx = 0;
			if (mouse_area == MOUSE_AREA_RIGHT || mouse_area == MOUSE_AREA_BOTTOM) {
				idx = 1;
			}
			const mtk_draw_struct ds = get_latest_draw_struct(idx);

			int gl_x, gl_y;
			gl_x = x;
			gl_y = win_h - y;
			if (hit_test_objects(ds, gl_x, gl_y)) {
				bFlg = true;
			}
		}


		if (bFlg) {
			InvalidateRect(g_hWnd, NULL, false);
		}
	}
}


//-------------------------------------
//  ��ʏ�̍��W�ƁA�f���\�ʂ̌�_�����߂�
//  x, y: �X�N���[�����W
//-------------------------------------
bool
get_planet_surface_point(const mtk_draw_struct& ds, int x, int y, const double& R, float& lon, float& lat)
{
	const mtk_object& tgt = get_target();
	const int tgt_idx = tgt.get_index();

	// �J�����̕����x�N�g���𓾂�
	vector3d  eF, eU, eR;
	ds.camera.get_axes(eF, eU, eR);
	// x,y �����̕����x�N�g��
	const frustum frs = ds.get_frustum();
	const float x1 = frs.get_x1();
	const float x2 = frs.get_x2();
	const float y1 = frs.get_y1();
	const float y2 = frs.get_y2();
	const float z1 = frs.get_z1();
	const float scl = (x2 - x1)/gl_w;
	const float xx = scl*float(x) + x1;
	const float yy = -scl*float(y) + y2;
	const vector3d n0 = normalize(vector3d(xx, yy, -z1));	// ���_���W�n�ł̕����x�N�g��
	// �������W�n�ł̃N���b�N�����x�N�g��
	const vector3d n = n0.x*eR + n0.y*eU - n0.z*eF;
	// �f���Ƃ̌�_�����߂�
	const vector3d nv = normalize(tgt.get_pos() - ds.camera.get_pos());
	const vector3d r0 = tgt.get_pos() - ds.camera.get_pos();
	const double b = -dot_product(n, r0);
	const double D = b*b - r0.norm2() + R*R;
	// ��_�������Ȃ��ꍇ
	if (D <= 0.0)  return false;
	// �������W�n�ł̌�_�̈ʒu�����߂�
	const double t = -b - sqrt(D);
	const vector3d rc = n*t - r0;
	const vector3d nv_ec = normalize(rc);
	// ��_�̈ܓx�E�o�x�����߂�
	vector2d lon_lat;
	lon_lat = get_planet_lon_lat_ec(tgt_idx, nv_ec);
	lon = lon_lat.x;
	lat = lon_lat.y;
	return  true;
}




//----------------
//  �_�u���N���b�N
//----------------
void
OnLBDBLCLK(int x, int y)
{
	const mtk_draw_struct ds = get_latest_draw_struct(0);
	const mtk_object&  tgt = get_target();
	const int mode = get_mode();


	set_mouse_area(x,y);
	correct_mouse_xy(x,y);


	//-- �X�N���[���E�{�^����ł̃_�u���N���b�N�͖�������
	if (sbTimeMinus.isInside(x, y) || sbTimePlus.isInside(x, y) ||
		sbZoomMinus.isInside(x, y) || sbZoomPlus.isInside(x, y) ) {
		return;
	}

	if (mode == MODE_3D) {

		if (is_there_selected_object()) {
			int ID = get_selected_object_ID();

			const mtk_object&  obj = get_object(ID);

			if ( ID != get_target_ID() ) {
				OnChangeTargetControler( ID );
				return;
			}
		}


		if (tgt.get_type() == OBJ_TYPE_PLANET) {
			const int idx = tgt.get_index();

			// �^�[�Q�b�g�̔��a
			const float Re = get_planet_Re(mode, idx) * _1km_pc;
			// �ڋ߂��Ă��邩�̔���
			
			const double L  = norm( tgt.get_pos() - ds.camera.get_pos() );				// �^�[�Q�b�g�܂ł̋���
			
			
			const float thR = tan(2.0f /180.0 *M_PI);								// �����ł��鎞�̋��̎����a
			if (Re/L < thR)  return;
			// �N���b�N�����ʒu�̓��o�E�k�܂��擾
			float lon, lat;
			bool res = get_planet_surface_point(ds, x, y, Re, lon, lat);
			if (res) {
				g_camera.set_ori( get_ori_from_planetarium_dirc(idx, lon, lat, 180.0) );


				InvalidateRect(g_hWnd, NULL, false);
			}
		}
	}
}

//--------------
//  MouseWheel
//--------------
void
OnMouseWheel(short Delta)
{
	//--  Zoom-in/-out
	float v = 2 * 10 * 0.7 * get_move_speed();
	if (Delta < 0) v=-v;
	if (get_mode() == MODE_3D) {
		v /= GetInputZoomFriction3D();
		SetInputZoomSpeed3D(v);
	} else {
		v *= 0.7;
		v /= GetInputZoomFrictionPlanetarium();
		SetInputZoomSpeedPlanetarium(v);
	}
	InvalidateRect(g_hWnd, NULL, false);
}

//====================
//  Window Procedure
//====================
LRESULT CALLBACK
WndProc_WindowMode(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	PAINTSTRUCT ps;

	switch (msg) {
	case WM_CREATE:
		OnCreate(hWnd);
		return 0;
	case WM_COMMAND:
		if (process_menu_command(hWnd, LOWORD(wParam)))  {
			InvalidateRect(hWnd, NULL, false);
			return 0;
		}
		break;
	case WM_INITMENUPOPUP:
		process_menu_checks(wParam);
		break;
	case WM_SYSKEYDOWN:
		switch (wParam) {
			case VK_RETURN:		// Alt + Enter
				OnFullScreen(!bFullScreen);
				return 0;
			default:
				break;
		}
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
			OnDraw_WindowMode(hDC);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_SIZE:
		switch (wParam) {
			case SIZE_MAXIMIZED:
				OnWindowMaximized(hWnd);
				break;
			case SIZE_RESTORED:
				OnWindowRestored(hWnd);
				break;
			default:
				break;
		}
		OnSize_WindowMode(hWnd);
		return 0;
	//--- ���T�C�Y
	case WM_SIZING:
		bResizingWindow = true;
		return 0;
	case WM_EXITSIZEMOVE:
		if (bResizingWindow) {
			bResizingWindow = false;
			InvalidateRect(hWnd, NULL, false);
		}
		return 0;
	case WM_ACTIVATE:
		OnActivate(wParam);
		return 0;
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
		InvalidateRect(hWnd, NULL, false);
		return 0;
	case WM_MOUSEWHEEL:
		OnMouseWheel( (short)HIWORD(wParam) );
		return 0;
	case WM_LBUTTONDBLCLK:
		OnLBDBLCLK(LOWORD(lParam), HIWORD(lParam));
		return 0;
	case WM_NCMOUSEMOVE:
		SetMouseCursorCounter(-1);
		return 0;
	case WM_KEYDOWN:
		if (OnKeyDown(hWnd, wParam)==0)  return 0;
		break;
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


//=============
//  Main Loop
//=============
int
Main_WindowMode( HINSTANCE hInstance, int iCmdShow )
{
	//-----------------------
	//  �E�B���h�E�N���X�̓o�^
	//-----------------------
	g_hInst = hInstance;
	WNDCLASS	wndclass;
	wndclass.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wndclass.lpfnWndProc	= WndProc_WindowMode;
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
	init();

	//--- Media info file �i���j���[�����O�ɏ������j---
	const std::string& lang_id = get_current_language_id();
	open_media_info_files(lang_id);

	//
	load_initial_location("location.dat", lang_id);
	const location& loc = get_initial_location();
	landing_lon = loc.lon;
	landing_lat = loc.lat;
	

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

	//--- setup screen menu ---
	init_menu();

	//--- window size/pos ---
	DWORD  dwStyle;
	dwStyle = WS_OVERLAPPEDWINDOW|WS_VISIBLE;

	//--- Create window ---
	HWND hWnd;
	hWnd = CreateWindow(szAppName,
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
	//  ���j���[�̏�����
	//------------------
	create_menu_bar();
	set_menu_bar(hWnd);

	//------------------
	//  �E�B���h�E�̕\��
	//------------------
	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);
	SetForegroundWindow(hWnd);


	//--- �X�v���b�V���E�E�B���h�E������ ---
	DestroySplashWindow();
	ShowWindow(hWnd, iCmdShow);


	//--- PBuffer
	if (bIniEnablePBuffer) {
		init_pbuffer(hWnd);
	}


	//--- �ő剻 ---
	if (bFullScreenInit) {
		OnFullScreen(true);
	}


	//--------------
	//  ���C�����[�v
	//--------------
	MSG msg;
	while (true) {
		
		ProcessMouseCursor();

		if (PeekMessage(&msg, NULL, 0,0, PM_REMOVE))  {
			if (msg.message == WM_QUIT)  break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} /*else if (bRecordMovie) {
			HDC  hDC = GetDC(hWnd);
			OnDraw_WindowMode(hDC);
			ReleaseDC(hWnd, hDC);
		}*/ else {
			Sleep(1);
		}
	}
	return 0;
}
