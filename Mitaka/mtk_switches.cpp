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
#include  "mtk_switches.h"


//
extern bool EarthCityLightsIni;


//-----------------------------------------
//  �^�C�}�[:  joystick, �A�j���[�V�����Ŏg�p
//-----------------------------------------
static int	TIMER_INTERVAL = 1;		// �^�C�}���荞�݂̊Ԋu (in msec)

void set_timer_interval(int itvl) {
	if (itvl <= 0)		{ itvl = 1; }
	if (itvl > 10000)	{ itvl = 10000; }
	TIMER_INTERVAL = itvl;
}

int get_timer_interval()  { return TIMER_INTERVAL; }



//-------
//  �ݒ�
//-------
int TexShrinkRate = 1;	// �ǂݍ��ݎ��̏k����

static bool bVSYNC = true;		// ��������
void	set_VSYNC(bool bSync) { bVSYNC = bSync; }
bool	get_VSYNC()  { return bVSYNC;  }


//---------
//  ���[�h
//---------
static char tyLandingMode = LANDING_MODE_HORIZONTAL;
char	get_landing_mode()			{ return tyLandingMode; }
void	set_landing_mode(char mode)	{ tyLandingMode = mode; }


//----------------
//  �\���p�X�C�b�`
//----------------
static  char DispSwitch[DISPSW_NUM];

//---  Init
void
init_disp_switches()
{
	char sw = DISPM_DISP | DISPM_NAME | DISPM_ORBIT | DISPM_CLOUD;
	if (EarthCityLightsIni)  sw |= DISPM_CITY_LIGHTS;
	DispSwitch[ DISPSW_PLANET ]			= sw;
	DispSwitch[ DISPSW_SATELLITE ]		= DISPM_DISP | DISPM_NAME | DISPM_ORBIT | DISPM_SELECT;
	DispSwitch[ DISPSW_MINOR_PLANET ]	= DISPM_DISP | DISPM_NAME | DISPM_ORBIT | DISPM_SELECT;
	DispSwitch[ DISPSW_TNO ]			= DISPM_DISP | DISPM_NAME | DISPM_ORBIT | DISPM_SELECT;
}

//---  Set / Get
void
set_disp_switch(char idx, char val)
{
	DispSwitch[idx] = val;
}

char
get_disp_switch(char idx)
{
	return DispSwitch[idx];
}

bool
test_disp_switch(char idx, char mask)
{
	char sw = DispSwitch[idx];
	return  (sw & mask) != 0;
}

//--  ���H
char  swEclipse				= ECLIPSE_DISP;
//--  �P��
char  swStar				= STAR_DISP | STAR_NAME | STAR_NAME_SELECT | STAR_ORBIT;
//--  ����
char  swConstellation		= 0;
//--  ���
char  swGalaxy				= GLXY_OUR_GALAXY | GLXY_SGR_A_ST | GLXY_GLOB_CLSTR | GLXY_NEARBY_GALAXY | GLXY_DISTANT_GALAXY | GLXY_QSO;
//-- ��C
char AtmosphereMode = ATMOSPHERE_MULTIPLE;
//-- �X�P�[����
char swScaleLine			= SCALE_CIRCLE;

bool  bSpacecraft			= true;			// �T���@
bool  bSurfacePointer		= true;			// �n�\�̃|�C���^
bool  bOortCloud			= true;			// �I�[���g�̉_
bool  bCMB					= true;			// CMB
bool  bGalCenterBG			= true;			// ��͌n���S�̔w�i
bool  bVirgoCluster			= false;		// ���Ƃߍ���͒c�̃}�[�N	
bool  bNGC					= false;		// NGC�V��
bool  bCelestialGrid		= false;		// �Ԍo�E�Ԉ܂̐�
bool  bEcliptic				= false;		// ����
bool  bHorizonCoord			= false;		// �n�����W
bool  bGalacticGrid			= false;		// ��o�E��܂̐�
bool  bDate					= true;			// ������\��
bool  bSideLight			= false;		// ������̏Ɩ�
bool  bSubtitles			= false;		// �������[�h
bool  bAppTitle				= false;		// �A�v���P�[�V�����̃^�C�g��
bool  bAbout				= false;		// ���
bool  bDebug				= false;		// �f�o�b�O���̕\��
bool  bViewInfo				= false;		// ���_���
bool  bSingleEye			= false;		// �Жځi�E�ځj�p�f���̂�
bool  bPathInfo				= false;		// �p�X���̕\��
bool  bKeyHelp				= false;		// �L�[�{�[�h�w���v��\��
bool  bFPS					= false;		// fps �̕\��


//----------------
//  �_����̋�����
//----------------
static float mag_factor;

void
set_mag_factor(float fac)
{
	mag_factor = fac;
}

float
get_mag_factor()
{
	return mag_factor;
}


//--------------
//  ���[�r�[�쐬
//--------------
char make_movie_type = MV_AVI;
char mv_filename[256];
char mv_basefilename[256];
