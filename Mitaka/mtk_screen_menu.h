/**************************************************************************************************

Copyright (c) 2007-2015   Tsunehiko Kato

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
#ifndef  __MTK_SCREEN_MENU_H__
#define  __MTK_SCREEN_MENU_H__
#include  "mtk_menu.h"



const float MenuTextSizeDef		= 0.7;
const float MenuBdWidthDef		= 1.0;
const float MenuCornerRDef		= 0.5;
const float MenuBgAlphaDef		= 0.95;
const float MenuBehindAlphaDef	= 0.4;
const float MenuFadingRateDef	= 3.0;



enum menu_command_ID {
	MENU_COMMAND_CLOSE,
	MENU_COMMAND_EXIT_ALL,
	MENU_COMMAND_SCALE,
	MENU_COMMAND_STAR_BRIGHTEN,
	MENU_COMMAND_STAR_DARKEN,
	MENU_COMMAND_STAR_DEFAULT_BRIGHTNESS,
	MENU_COMMAND_LANDING_INIT,
	MENU_COMMAND_TARGET_MOVE,
	MENU_COMMAND_PRESENT_TIME,
	MENU_COMMAND_RESET_VIEWANGLE,
	MENU_COMMAND_MOVIE_OPEN,
	MENU_COMMAND_IMAGE_OPEN,
	MENU_COMMAND_AUDIO_OPEN,
	MENU_COMMAND_3DCHART,
	MENU_COMMAND_PRESET,
	MENU_COMMAND_PLAY_PATH,
	MENU_COMMAND_SAVE_PATH,
	MENU_COMMAND_LOAD_PATH,
	MENU_COMMAND_CLEAR_PATH,
	MENU_COMMAND_GOTO,
	MENU_COMMAND_DRAW_MILKYWAY,
	MENU_COMMAND_DRAW_MILKYWAY2,
	MENU_COMMAND_SPEED_FASTER,
	MENU_COMMAND_SPEED_SLOWER,
	MENU_COMMAND_SPEED_DEFAULT
};

enum menu_check_ID {
	MENU_CHECK_CNST_NAME,
    MENU_CHECK_CNST_LINE,
    MENU_CHECK_CNST_ILLUST,
	MENU_CHECK_CNST_BND,
	MENU_CHECK_STAR,
	MENU_CHECK_STAR_NAME,
	MENU_CHECK_STAR_NAME_SELECT,
	MENU_CHECK_STAR_BAYER,
	MENU_CHECK_STAR_FRAMSTEED,
	MENU_CHECK_STAR_HIP,
	MENU_CHECK_STAR_ORBIT,
	MENU_CHECK_STAR_MARK,

	MENU_CHECK_PLANETS,
	MENU_CHECK_PLANET_NAME,
	MENU_CHECK_PLANET_ORBIT,
	MENU_CHECK_PLANET_AXIS,
	MENU_CHECK_PLANET_ATMOSPHERE,
	MENU_CHECK_PLANET_CLOUD,
	MENU_CHECK_PLANET_CITY_LIGHTS,
	MENU_CHECK_PLANET_ZOOM,
	MENU_CHECK_TOPO_ZOOM,
	MENU_CHECK_LANDING_MODE,

	MENU_CHECK_SATELLITE_DISP,
	MENU_CHECK_SATELLITE_NAME,
	MENU_CHECK_SATELLITE_ORBIT,
	MENU_CHECK_SATELLITE_SELECT,

	MENU_CHECK_MINOR_PLANET_DISP,
	MENU_CHECK_MINOR_PLANET_NAME,
	MENU_CHECK_MINOR_PLANET_ORBIT,
	MENU_CHECK_MINOR_PLANET_SELECT,

	MENU_CHECK_TNO_DISP,
	MENU_CHECK_TNO_NAME,
	MENU_CHECK_TNO_ORBIT,
	MENU_CHECK_TNO_SELECT,
	MENU_CHECK_TNO_OORT_CLOUD,
	

	MENU_CHECK_ECLIPSE_DISP,
	MENU_CHECK_ECLIPSE_BORDER,
	MENU_CHECK_SPACECRAFT,
	MENU_CHECK_SPACECRAFT_TRAJECTORY,
	MENU_CHECK_MILKYWAY_NONE,
	MENU_CHECK_MILKYWAY,
	MENU_CHECK_MILKYWAY2,
	MENU_CHECK_MILKYWAY_IR,
	MENU_CHECK_OUR_GALAXY,
	MENU_CHECK_SGR_A,
	MENU_CHECK_GAL_CENTER_BG,
	MENU_CHECK_VERA_OBJECTS,
	MENU_CHECK_GLOBULAR_CLUSTER,
	MENU_CHECK_GALAXY_NEAR,
	MENU_CHECK_GALAXY_SDSS,
	MENU_CHECK_GALAXY_QSO,
	MENU_CHECK_GALAXY_VIRGO,
	MENU_CHECK_CLST_GRD,
	MENU_CHECK_ECLIPTIC,
	MENU_CHECK_HORIZON_COORD,
	MENU_CHECK_GLX_GRD,
	MENU_CHECK_DIST_SCALE,
	MENU_CHECK_SCALE_SQUARE,
	MENU_CHECK_CMB,
	MENU_CHECK_NGC,
	MENU_CHECK_DATE,
	MENU_CHECK_TIME,
	MENU_CHECK_REALTIME_MODE,
	MENU_CHECK_TARGET,
	MENU_CHECK_PLANETARIUM_TARGET,
	MENU_CHECK_LANG,
	MENU_CHECK_TEST,
	MENU_CHECK_PROGRAM,
	MENU_CHECK_FONT_SIZE,
	MENU_CHECK_TITLE,
	MENU_CHECK_ABOUT,
	MENU_CHECK_DEBUG,
	MENU_CHECK_VIEW_INFO,
	MENU_CHECK_SINGLE_EYE,
	MENU_CHECK_PATH_INFO,
	MENU_CHECK_ZAKU,
	MENU_CHECK_SUBTITLES,
	MENU_CHECK_SIDE_LIGHT,
	MENU_CHECK_SURFACE_POINTER,
	MENU_CHECK_FULLSCREEN
};



//---  Get / Set  ---
bool	is_menu_visible();
void	set_menu_visible(bool bFlg);
float	get_menu_text_size();
void	set_menu_text_size(float sz);

void	set_menu_pos0(const float angle);
float	get_menu_lon_angle();
float	get_menu_lat_angle();


//---  Init  ---
void	init_root_menu();


//---  Draw  ---
void	draw_menu(menu_selection& ms, float x0, float y0, float z0);





//---  Commands  ---
void	OnEnterMenu();
void	OnLeaveMenu();
void	OnCloseMenu();
void	OnMenuPos(int pos);
void	OnMenuLonAngle(float angle);
void	OnMenuLatAngle(float angle);
void	OnMenuFade(float fac);
void	OnExecMenu();


#endif
