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
#include  "mtk_dialog_box.h"
#include  <stdio.h>
#include  <process.h>
#include  <commctrl.h>
#include  "../common/interface/joystick.h"
#include  "../common/JulianDate.h"
#include  "../common/cpu.h"
#include  "mtk_command.h"
#include  "mtk_switches.h"
#include  "mtk_text.h"
#include  "mtk_joystick.h"
#include  "mtk_cpu_info.h"
#include  "mtk_directory.h"
#include  "mtk_pbuffer.h"
#include  "mtk_save_image.h"
#include  "mtk_time.h"
#include  "../common/graphics/dib.h"
#include  "../common/win_dlg.h"
#include  "../common/win_ctrl.h"
#include  <gl/gl.h>
#include  "../common/text/unicode_string.h"
#include  "mtk_strings.h"
#include  "mtk_languages.h"
#include  "resource.h"





//-- externs
extern HINSTANCE	g_hInst;
extern HWND			g_hWnd;
extern char szTitle[256];
extern char	szAppName[];
extern const GLubyte *szGLVersion;
extern mtk_camera g_camera;
extern float DT;


/*
const wchar_t* splash_image_fn = L"mitaka_splash.png";
const int splash_w = 512;
const int splash_h = 384;
const COLORREF  ver_col = RGB(255, 255, 255);
const int ver_x = 250;
const int ver_y = 9;
const COLORREF  loading_col = RGB(153, 153, 153);
const int loading_x = 13;
const int loading_y = 362;
*/
const wchar_t szCopyright_E[] = L"Copyright (C) 2003-2016   4-Dimensional Digital Universe Project, NAOJ, Japan";
const wchar_t szCopyright_J[] = L"Copyright (C) 2003-2016   �����V����S�����f�W�^���F���v���W�F�N�g";
const wchar_t szDeveloper_E[] = L"Developer:  Tsunehiko Kato (4D2U, NAOJ)";
const wchar_t szDeveloper_J[] = L"�J���ҁF �����P�F �i�����V���� 4D2U�j";
const wchar_t szAuthor2_E[] = L"Visual Design:  Sorahiko Nukatani (NAOJ)";
const wchar_t szAuthor2_J[] = L"���o�f�U�C��:  �z�J���F�@�i�����V����j";





//=======================
//  Set Time Dialog Box
//=======================
HWND hWndSetTime = NULL;



LRESULT CALLBACK
SetTimeDialogBox(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	#define BOLDDAY(ds,iDay) if(iDay>0 && iDay<32) (ds)|=(0x00000001<<(iDay-1))
	#define lpnmDS ((NMDAYSTATE *)lParam)
	#define MAX_MONTHS 12


	static HWND	hStaticDate, hStaticYear, hStaticTime;
	static HWND	hStaticHour, hStaticMinute, hStaticSecond;
	static HWND	hBtnTonight, hBtnPresentTime;
	static HWND	hBtnOK, hBtnApply, hBtnCancel;
	static HWND	hCalendar;
	static SYSTEMTIME st;
	static ctrlEdit edYear, edHour, edMin, edSec;
	static ctrlSpin	spn, spn_year;
	static int hour, minute, sec;
	static int SpnMin, SpnMax;


	MONTHDAYSTATE mds[MAX_MONTHS];
	INT i, iMax;
	LPNMHDR hdr = (LPNMHDR)lParam;

	WORD  code, cmd_id;


	switch( message )
	{
		case WM_INITDIALOG:
			hStaticDate = GetDlgItem(hDlg, IDC_STATIC_TIME_DATE);
			hStaticYear = GetDlgItem(hDlg, IDC_STATIC_TIME_YEAR);
			hStaticTime = GetDlgItem(hDlg, IDC_STATIC_TIME_TIME);
			hStaticHour = GetDlgItem(hDlg, IDC_STATIC_TIME_HOUR);
			hStaticMinute = GetDlgItem(hDlg, IDC_STATIC_TIME_MINUTE);
			hStaticSecond = GetDlgItem(hDlg, IDC_STATIC_TIME_SECOND);
			hBtnTonight = GetDlgItem(hDlg, IDC_BUTTON_TONIGHT);
			hBtnPresentTime = GetDlgItem(hDlg, IDC_BUTTON_PRESENT_TIME);
			hBtnOK = GetDlgItem(hDlg, IDOK);
			hBtnApply = GetDlgItem(hDlg, IDC_BUTTON_APPLY);
			hBtnCancel = GetDlgItem(hDlg, IDCANCEL);
			hCalendar = GetDlgItem(hDlg, IDC_MONTHCALENDAR1);

			//--  Title
			::SetWindowTextW(hDlg, (LPCWSTR)get_wstring("DLG_TIME_TITLE").c_str());

			//--  Static Text
			::SendMessageW(hStaticDate, WM_SETTEXT, 0, (LPARAM)get_wstring("DLG_TIME_DATE").c_str());
			::SendMessageW(hStaticYear, WM_SETTEXT, 0, (LPARAM)get_wstring("DLG_TIME_YEAR").c_str());
			::SendMessageW(hStaticTime, WM_SETTEXT, 0, (LPARAM)get_wstring("DLG_TIME_TIME").c_str());
			::SendMessageW(hStaticHour, WM_SETTEXT, 0, (LPARAM)get_wstring("DLG_TIME_HOUR").c_str());
			::SendMessageW(hStaticMinute, WM_SETTEXT, 0, (LPARAM)get_wstring("DLG_TIME_MINUTE").c_str());
			::SendMessageW(hStaticSecond, WM_SETTEXT, 0, (LPARAM)get_wstring("DLG_TIME_SECOND").c_str());
			::SendMessageW(hBtnTonight, WM_SETTEXT, 0, (LPARAM)get_wstring("DLG_TIME_TONIGHT").c_str());
			::SendMessageW(hBtnPresentTime, WM_SETTEXT, 0, (LPARAM)get_wstring("DLG_TIME_PRESENT_TIME").c_str());
			::SendMessageW(hBtnOK, WM_SETTEXT, 0, (LPARAM)get_wstring("DLG_TIME_OK").c_str());
			::SendMessageW(hBtnApply, WM_SETTEXT, 0, (LPARAM)get_wstring("DLG_TIME_APPLY").c_str());
			::SendMessageW(hBtnCancel, WM_SETTEXT, 0, (LPARAM)get_wstring("DLG_TIME_CANCEL").c_str());


			//-- �J�����_�[�R���g���[���̐ݒ�
			MonthCal_SetColor(hCalendar, MCSC_BACKGROUND, RGB(175,175,175));
			MonthCal_SetColor(hCalendar, MCSC_MONTHBK,    RGB(248,245,225));
			{
				UTC utc = JD_to_UTC(get_time_JD() + DT/24.0);
				st.wYear			= utc.year;
				st.wMonth			= utc.month;
				st.wDay				= utc.day;
				st.wHour			= utc.hour;
				st.wMinute			= 0;
				st.wSecond			= 0;
				st.wMilliseconds	= 0;
				hour	= utc.hour;
				minute	= utc.min;
				sec		= int(utc.sec);
				MonthCal_SetCurSel(hCalendar, &st);
			}
			//-- �N�ݒ�G�f�B�b�g�{�b�N�X
			edYear.Setup(hDlg, IDC_EDIT_YEAR, "%d", st.wYear);
			// �X�s���{�^��
			spn_year.Setup(hDlg, IDC_SPIN_YEAR);
			spn_year.SetBuddy(edYear.GetHandle());
			spn_year.SetRange(0, 3000);
			//-- ���E���E�b�ݒ�G�f�B�b�g�{�b�N�X
			edHour.Setup(hDlg, IDC_EDIT_HOUR,	"%d", hour);
			edMin.Setup(hDlg, IDC_EDIT_MIN,	"%d", minute);
			edSec.Setup(hDlg, IDC_EDIT_SEC,	"%d", sec);
			// �X�s���{�^��
			spn.Setup(hDlg, IDC_SPIN_TIME);
			spn.SetBuddy(edHour.GetHandle());
			SpnMin = 0;
			SpnMax = 23;
			spn.SetRange(SpnMin-1, SpnMax+1);

			return TRUE;
		case WM_COMMAND:
			code = HIWORD(wParam);
			cmd_id = LOWORD(wParam);
			switch (cmd_id)  {
				case IDOK:
					OnChangeTime( -DT/24.0 + get_JD(st.wYear, st.wMonth, st.wDay, hour, minute, sec + 0.5));
					InvalidateRect(g_hWnd, NULL, false);
					SendMessage(hDlg, WM_CLOSE, 0, 0);
					return TRUE;
				case IDCANCEL:
					SendMessage(hDlg, WM_CLOSE, 0, 0);
					return TRUE;

				case IDC_BUTTON_PRESENT_TIME:
					{
						// �J�����_�[�R���g���[��
						UTC utc = get_present_LocalTime(DT);
						st.wYear			= utc.year;
						st.wMonth			= utc.month;
						st.wDay				= utc.day;
						st.wHour			= 0;
						st.wMinute			= 0;
						st.wSecond			= 0;
						st.wMilliseconds	= 0;
						hour	= utc.hour;
						minute	= utc.min;
						sec		= int(utc.sec);
						MonthCal_SetCurSel(hCalendar, &st);
						// �N�E���E���E�b�ݒ�G�f�B�b�g�{�b�N�X
						edYear.Setup(hDlg, IDC_EDIT_YEAR,	"%d", st.wYear);
						edHour.Setup(hDlg, IDC_EDIT_HOUR,	"%d", hour);
						edMin.Setup(hDlg, IDC_EDIT_MIN,		"%d", minute);
						edSec.Setup(hDlg, IDC_EDIT_SEC,		"%d", sec);
					}
					break;

				case IDC_BUTTON_TONIGHT:
					{
						// �J�����_�[�R���g���[��
						hour	= 20;
						minute	= 0;
						sec		= 0;
						// �N�E���E���E�b�ݒ�G�f�B�b�g�{�b�N�X
						edHour.Setup(hDlg, IDC_EDIT_HOUR,	"%d", hour);
						edMin.Setup(hDlg, IDC_EDIT_MIN,	"%d", minute);
						edSec.Setup(hDlg, IDC_EDIT_SEC,	"%d", sec);
					}
					break;

				case IDC_BUTTON_APPLY:
					OnChangeTime( -DT/24.0 + get_JD(st.wYear, st.wMonth, st.wDay, hour, minute, sec + 0.5));
					InvalidateRect(g_hWnd, NULL, false);
					break;

				case IDC_EDIT_YEAR:
					switch (code) {
						case EN_UPDATE:
							{
								int itmp;
								edYear.GetText_int(&itmp);
								if (itmp != st.wYear) {
									if (itmp>=0 && itmp<3000)	{ st.wYear = itmp; MonthCal_SetCurSel(hCalendar, &st); }
									else						{ edYear.SetTextf("%d", st.wYear); }
								}
							}
							break;
						default:
							break;
					}
					break;

				case IDC_EDIT_HOUR:
					switch (code) {
						case EN_UPDATE:
							{
								int itmp;
								edHour.GetText_int(&itmp);
								if (itmp>=0 && itmp<24)		{ hour = itmp; }
								else						{ edHour.SetTextf("%d", hour); }
							}
							break;
						case EN_SETFOCUS:
							spn.SetBuddy(edHour.GetHandle());
							SpnMin = 0;
							SpnMax = 23;
							spn.SetRange(SpnMin-1, SpnMax+1);
							break;
						default:
							break;
					}
					break;
				case IDC_EDIT_MIN:
					switch (code) {
						case EN_UPDATE:
							{
								int itmp;
								edMin.GetText_int(&itmp);
								if (itmp>=0 && itmp<60)		{ minute = itmp; }
								else						{ edMin.SetTextf("%d", minute); }
							}
							break;
						case EN_SETFOCUS:
							spn.SetBuddy(edMin.GetHandle());
							SpnMin = 0;
							SpnMax = 59;
							spn.SetRange(SpnMin-1, SpnMax+1);
							break;
						default:
							break;
					}
					break;
				case IDC_EDIT_SEC:
					switch (code) {
						case EN_UPDATE:
							{
								int itmp;
								edSec.GetText_int(&itmp);
								if (itmp>=0 && itmp<60)		{ sec = itmp; }
								else						{ edSec.SetTextf("%d", sec); }
							}
							break;
						case EN_SETFOCUS:
							spn.SetBuddy(edSec.GetHandle());
							SpnMin = 0;
							SpnMax = 59;
							spn.SetRange(SpnMin-1, SpnMax+1);
							break;
						default:
							break;
					}
					break;
			}
			break;

		
		case WM_NOTIFY:
			if (hdr->idFrom == IDC_MONTHCALENDAR1) {
				switch (hdr->code) {
					// �J�����_�[
					case MCN_GETDAYSTATE:
						iMax=lpnmDS->cDayState;
						for(i=0;i<iMax;i++){
							mds[i] = (MONTHDAYSTATE)0;
							BOLDDAY(mds[i],15);
						}
						lpnmDS->prgDayState = mds;
						break;
					// �J�����_�[�I��
					case MCN_SELCHANGE:
						{
							int year0 = st.wYear;
							MonthCal_GetCurSel(hCalendar, &st);
							if (year0 != st.wYear) {
								edYear.Setup(hDlg, IDC_EDIT_YEAR, "%d", st.wYear);
							}
						}
						break;
				}
			} else if (hdr->idFrom == IDC_SPIN_TIME) {
				// �����̃X�s���{�^��
				switch (hdr->code) {
					case UDN_DELTAPOS:
						{
							// ���[�v����
							NMUPDOWN* pud;
							pud = LPNMUPDOWN(lParam);
							if (pud->iPos <= SpnMin && pud->iDelta<0)	{
								spn.SetPos(SpnMax+1);
							} else if (pud->iPos >= SpnMax && pud->iDelta>0) {
								spn.SetPos(SpnMin-1);
							}
						}
						break;
				}
			}
			break;
		case WM_CLOSE:
			DestroyWindow(hDlg);
			hWndSetTime = NULL;
			break;
	}
    return FALSE;
}




void __cdecl
SetTimeDialog_thread(void* pParam)
{
	LPCTSTR  lpDlgTemp = MAKEINTRESOURCE(IDD_DIALOG_TIME);
	hWndSetTime = CreateDialog(g_hInst, lpDlgTemp, g_hWnd, (DLGPROC)SetTimeDialogBox);

	ShowWindow(hWndSetTime, SW_SHOW);

	//--- ���b�Z�[�W�E���[�v
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		if (hWndSetTime == 0 || !IsDialogMessage(hWndSetTime, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	_endthread();
}


void
CreateSetTimeDialog()
{
	if (hWndSetTime == NULL) {
		_beginthread(SetTimeDialog_thread, 0, 0);
	} else {
		SetForegroundWindow(hWndSetTime);
	}
}





//==========================
//�@�@Save Image Dialog Box
//==========================
LRESULT CALLBACK
SaveImageHighResDialogBox(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND	hStaticWidth, hStaticHeight;
	static HWND	hStaticMaxWidth, hStaticMaxHeight;
	static HWND	hStaticSave, hStaticCancel;
	static ctrlEdit edWidth, edHeight;
	static int width;
	static int height;
	static bool bFirst = true;


	const int wmax = get_pbuffer_width_max();
	const int hmax = get_pbuffer_height_max();

	if (bFirst) {
		width = min(1024, wmax);
		height = min(1024, hmax);
		bFirst = false;
	}


	WORD  code;

	switch (message)
	{
	case WM_INITDIALOG:
	{
		hStaticWidth = GetDlgItem(hDlg, IDC_STATIC_SAVE_IMAGE_WIDTH);
		hStaticHeight = GetDlgItem(hDlg, IDC_STATIC_SAVE_IMAGE_HEIGHT);
		hStaticMaxWidth = GetDlgItem(hDlg, IDC_STATIC_SAVE_IMAGE_MAX_WIDTH);
		hStaticMaxHeight = GetDlgItem(hDlg, IDC_STATIC_SAVE_IMAGE_MAX_HEIGHT);
		hStaticSave = GetDlgItem(hDlg, IDOK);
		hStaticCancel = GetDlgItem(hDlg, IDCANCEL);

		//--  Title
		::SetWindowTextW(hDlg, (LPCWSTR)get_wstring("DLG_IMAGE_TITLE").c_str());

		//--  Width and Height
		::SendMessageW(hStaticWidth, WM_SETTEXT, 0, (LPARAM)get_wstring("DLG_IMAGE_WIDTH").c_str());
		::SendMessageW(hStaticHeight, WM_SETTEXT, 0, (LPARAM)get_wstring("DLG_IMAGE_HEIGHT").c_str());

		//--  Max Width and Height
		wchar_t szMax_w[256], szMax_h[256];
		swprintf(szMax_w, get_wstring("DLG_IMAGE_MAX_FMT").c_str(), wmax);
		swprintf(szMax_h, get_wstring("DLG_IMAGE_MAX_FMT").c_str(), hmax);
		::SendMessageW(hStaticMaxWidth, WM_SETTEXT, 0, (LPARAM)szMax_w);
		::SendMessageW(hStaticMaxHeight, WM_SETTEXT, 0, (LPARAM)szMax_h);

		//--  Save/Cancel Button
		::SendMessageW(hStaticSave, WM_SETTEXT, 0, (LPARAM)get_wstring("DLG_IMAGE_SAVE_BUTTON").c_str());
		::SendMessageW(hStaticCancel, WM_SETTEXT, 0, (LPARAM)get_wstring("DLG_IMAGE_CANCEL_BUTTON").c_str());


		if (width > wmax)  width = wmax;
		if (height > hmax) height = hmax;
		edWidth.Setup(hDlg, IDC_EDIT_SAVE_IMAGE_W, "%d", width);
		edHeight.Setup(hDlg, IDC_EDIT_SAVE_IMAGE_H, "%d", height);
	}

	return TRUE;

	case WM_COMMAND:
		code = HIWORD(wParam);
		switch (LOWORD(wParam))  {
		case IDOK:
		{
			if (width <= 0 || height <= 0) {
				MessageBoxWf(hDlg, "Error", MB_OK, get_string("DLG_IMAGE_SIZE_MIN_CAUTION").c_str());
				break;
			}

			if (width > wmax || height > hmax) {
				MessageBoxWf(hDlg, "Error", MB_OK, get_string("DLG_IMAGE_SIZE_MAX_CAUTION").c_str(), wmax, hmax);
				break;
			}

			if (!save_image_high_resolution_get_filename(width, height)) {
				break;
			}

			EndDialog(hDlg, 0);
			return TRUE;
		}
		case IDCANCEL:
			EndDialog(hDlg, 0);
			return TRUE;

		case IDC_EDIT_SAVE_IMAGE_W:
			switch (code) {
			case EN_UPDATE:		edWidth.GetText_int(&width);		break;
			default:												break;
			}
			break;
		case IDC_EDIT_SAVE_IMAGE_H:
			switch (code) {
			case EN_UPDATE:		edHeight.GetText_int(&height);		break;
			default:												break;
			}
			break;
		default:		break;
		}


		break;
	}
	return FALSE;
}






//====================
//  About Dialog Box
//====================
LRESULT CALLBACK
About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND	hStaticAppName, hStaticCopyright, hStaticCopyright2, hStaticJoystick;
	static HWND	hStaticCPU, hStaticExtentions, hStaticOpenGLVer;
	wchar_t str[512];
	bool bJ;


	switch (message)
	{
	case WM_INITDIALOG:
		bJ = is_current_language_ISO639("ja");

		::SetWindowTextW(hDlg, (LPCWSTR)get_wstring("DLG_ABOUT_TITLE").c_str());

		//--  App Name
		hStaticAppName = GetDlgItem(hDlg, IDC_STATIC_APPNAME);
		::SendMessageW(hStaticAppName, WM_SETTEXT, 0, (LPARAM)utf8_to_wstring(szTitle).c_str());

		//--  Copyright
		hStaticCopyright = GetDlgItem(hDlg, IDC_STATIC_COPYRIGHT);
		if (bJ)		::SendMessageW(hStaticCopyright, WM_SETTEXT, 0, (LPARAM)szCopyright_J);
		else		::SendMessageW(hStaticCopyright, WM_SETTEXT, 0, (LPARAM)szCopyright_E);

		//--  Author
		hStaticCopyright = GetDlgItem(hDlg, IDC_STATIC_AUTHOR);
		if (bJ)		::SendMessageW(hStaticCopyright, WM_SETTEXT, 0, (LPARAM)szDeveloper_J);
		else		::SendMessageW(hStaticCopyright, WM_SETTEXT, 0, (LPARAM)szDeveloper_E);

		//--  Author2
		hStaticCopyright2 = GetDlgItem(hDlg, IDC_STATIC_AUTHOR2);
		if (bJ)		::SendMessageW(hStaticCopyright2, WM_SETTEXT, 0, (LPARAM)szAuthor2_J);
		else		::SendMessageW(hStaticCopyright2, WM_SETTEXT, 0, (LPARAM)szAuthor2_E);

		//--  Joystick
		hStaticJoystick = GetDlgItem(hDlg, IDC_STATIC_JOYSTICK);

		if (is_joystick_available())	{ wsprintfW(str, L"%s", L"A joystick is available."); }
		else							{ wsprintfW(str, L" "); }
		::SendMessageW(hStaticJoystick, WM_SETTEXT, 0, (LPARAM)str);

		//--  CPU
		hStaticCPU = GetDlgItem(hDlg, IDC_STATIC_CPU);
		wsprintfW(str, L"CPU: %d", get_number_of_cpu());
		::SendMessageW(hStaticCPU, WM_SETTEXT, 0, (LPARAM)str);

		//--  Extentions
		hStaticExtentions = GetDlgItem(hDlg, IDC_STATIC_EXTENTIONS);
		wsprintfW(str, L"Extentions:   ");
		if (is_SSE_Enable())		{ wcscat(str, L"SSE  "); }
		if (isPBufferEnable())		{ wcscat(str, L"pBuffer  "); }
		::SendMessageW(hStaticExtentions, WM_SETTEXT, 0, (LPARAM)str);

		//--  OpenGL version
		hStaticOpenGLVer = GetDlgItem(hDlg, IDC_STATIC_OPENGL_VERSION);
		wsprintfW(str, L"OpenGL Version: %s", utf8_to_wstring((char*)szGLVersion).c_str());
		::SendMessageW(hStaticOpenGLVer, WM_SETTEXT, 0, (LPARAM)str);

		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))  {
		case IDOK:
		case IDCANCEL:
			EndDialog(hDlg, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}






//=================
//  Splash Window
//=================
HWND	hSplashWindow = NULL;
dib splash_image;
std::wstring SplashText;
HFONT hSplashFont;
const int splash_text_size = 14;

void
DrawSplashBitmap(HDC hDC)
{
	BitBlt(hDC, 0,0,splash_image.get_width(),splash_image.get_height(), splash_image.getDC(), 0,0, SRCCOPY);
	SetBkMode(hDC, TRANSPARENT);

	//--- ������̕`�� ---
	HFONT hFontOld = (HFONT)SelectObject(hDC, (HGDIOBJ)hSplashFont);

	// �o�[�W����
	//-- Mitaka
	SetTextColor(hDC, RGB(255,255,255));
	TextOut(hDC, 250, 9, szTitle, strlen(szTitle));
	//--- Mitaka++
	//SetTextColor(hDC, RGB(200,200,200));
	//TextOut(hDC, 475, 5, szTitle, strlen(szTitle));

	// �ǂݍ��ݏ�
	//-- Mitaka
	SetTextColor(hDC, RGB(153,153,153));
	TextOutW(hDC, 13, 362, SplashText.c_str(), SplashText.length());
	//-- Mitka++
	//SetTextColor(hDC, RGB(170,170,170));
	//TextOut(hDC, 5, 83, SplashText, strlen(SplashText));
	
	SelectObject(hDC, (HGDIOBJ)hFontOld);
}


LRESULT CALLBACK
SplashWindowProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	HDC hDC;
	PAINTSTRUCT ps;
	const char font_name[] = "Tahoma";
	//const char font_name[] = "Arial Unicode MS";

	switch( message )
	{
		case WM_CREATE:
			{
				directory dir = get_directory(DIR_DATA);
				image img;
				img.load( dir.get_path("mitaka_splash.png") );			// Mitaka
				//img.load( dir.get_path("mitaka_pp_splash.png") );		// Mitaka++
				image_to_dib(img, splash_image);

				hSplashFont = CreateFont(
					splash_text_size,
					0, 0, 0, 0,
					FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
					OUT_DEVICE_PRECIS, 
					CLIP_DEFAULT_PRECIS,
					ANTIALIASED_QUALITY, DEFAULT_PITCH,
					font_name);
			}

			return 0;

		case WM_CLOSE:
			DeleteObject((HGDIOBJ)hSplashFont);
			DestroyWindow(hWnd);
			hSplashWindow = NULL;
			splash_image.free();
			return 0;

		case WM_PAINT:
			hDC = BeginPaint(hWnd, &ps);
			DrawSplashBitmap(hDC);
			EndPaint(hWnd, &ps);
			return 0;

		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}


void __cdecl
Splash_thread(void* pParam)
{
	//
	const int w = 512;
	const int h = 384;
	
	//  �E�B���h�E�N���X�̓o�^
	WNDCLASS	wndclass;
	wndclass.style			= CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc	= SplashWindowProc;
	wndclass.cbClsExtra		= 0;
	wndclass.cbWndExtra		= 0;
	wndclass.hInstance		= g_hInst;
	wndclass.hIcon			= NULL;
	wndclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszMenuName	= NULL;
	wndclass.lpszClassName	= "MitakaSplash";
	if (!RegisterClass(&wndclass)) {
		MessageBox(NULL, TEXT("This program requires WindowsNT!"), szAppName, MB_ICONERROR);
		return;
	}

	//  ��ʂ̒��S���擾����
	int x, y;
	x = GetSystemMetrics(SM_CXSCREEN)/2 - w/2;
	y = GetSystemMetrics(SM_CYSCREEN)/2 - h/2;

	//  �E�B���h�E�̐���
	DWORD dwStyle = WS_VISIBLE|WS_POPUP;

	hSplashWindow = CreateWindowEx(WS_EX_APPWINDOW, //WS_EX_TOPMOST,
								"MitakaSplash",			// window class
								szAppName,				// window caption
								dwStyle,				// window style
								x, y,					// window position
								w, h,					// window size
								NULL,					// parent window handle
								NULL,					// window menu handle
								g_hInst,				// program instance handle
								NULL);					// creation parameters


	//--- �X�v���b�V���E�B���h�E�N�� ---
	ShowWindow(hSplashWindow, SW_SHOW); 


	//--- ���b�Z�[�W�E���[�v
	MSG msg;
	while (GetMessage(&msg, NULL, 0,0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	_endthread();
}



//
void
CreateSplashWindow()
{
	_beginthread(Splash_thread, 0, 0);
}

void
DestroySplashWindow()
{
	if (hSplashWindow) {
		SendMessage(hSplashWindow, WM_CLOSE, 0, 0);
	}
}




void
SetSplashText(const std::string& key)
{
	if (hSplashWindow == NULL)  return;

	SplashText = get_wstring(key);

	InvalidateRect(hSplashWindow, NULL, FALSE);
}

