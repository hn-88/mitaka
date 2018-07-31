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
#ifndef __MTK_SWITCHES_H__
#define __MTK_SWITCHES_H__



// �^�C�}�[
void set_timer_interval(int itvl);
int get_timer_interval();



//-------
//  �ݒ�
//-------
extern int TexShrinkRate;

void	set_VSYNC(bool bSync);
bool	get_VSYNC();


//---------
//  ���[�h
//---------
enum {
	LANDING_MODE_HORIZONTAL,
	LANDING_MODE_SURFACE_VIEW
};
char	get_landing_mode();
void	set_landing_mode(char mode);

//--------------
//  �\���X�C�b�`
//--------------
//--- ���H�̕\��
const char ECLIPSE_DISP		= 0x01;		// ���H�̉e�̕\��
const char ECLIPSE_BORDER	= 0x02;		// �{�e�E���e�̋��E
//--- �P��
const char STAR_DISP		= 0x01;		// ����\�����邩
const char STAR_NAME		= 0x02;		// ���̌ŗL��
const char STAR_NAME_SELECT	= 0x04;		// �I���������̌ŗL���݂̂�\��
const char STAR_BAYER		= 0x08;		// ���̃o�C�G����
const char STAR_FRAMSTEED	= 0x10;		// ���̃t�����X�`�[�h�ԍ�
const char STAR_HIP			= 0x20;		// ����HIP�ԍ�
const char STAR_ORBIT		= 0x40;		// ���̋O��
//--- ����
const char CNST_NAME		= 0x01;		// ������
const char CNST_LINE		= 0x02;		// ������
const char CNST_ILLUST		= 0x04;		// �����G
const char CNST_BND			= 0x08;		// �����̋��E��
//--- ���
const char GLXY_OUR_GALAXY		= 0x01;	// ��͌n
const char GLXY_SGR_A_ST		= 0x02;	// ���č�A*
const char GLXY_GLOB_CLSTR		= 0x04;	// ���󐯒c
const char GLXY_NEARBY_GALAXY	= 0x08;	// �ߖT���
const char GLXY_DISTANT_GALAXY	= 0x10;	// �������
const char GLXY_QSO				= 0x20;	// QSO
const char GLXY_VERA			= 0x40;	// VERA�V��
//--- �X�P�[����
const char SCALE_CIRCLE		= 0x01;		// �X�P�[���~
const char SCALE_SQUARE		= 0x02;		// �X�P�[���g



//-------------
//  Time Step
//-------------
enum {
	TIMESTEP_10SEC,
	TIMESTEP_MIN,
	TIMESTEP_10MIN,
	TIMESTEP_HOUR,
	TIMESTEP_4HOUR,
	TIMESTEP_DAY,
	TIMESTEP_WEEK,
	TIMESTEP_MONTH,
	TIMESTEP_YEAR,
	TIMESTEP_10YEAR,
	TIMESTEP_100YEAR
};
extern	char  tyTimeStep;



//---------
//  Scale
//---------
enum {
	SCALE_10MKM,
	SCALE_1AU,
	SCALE_10AU,
	SCALE_100AU,
	SCALE_1000AU,
	SCALE_10000AU,
	SCALE_1LY,
	SCALE_10LY,
	SCALE_100LY,
	SCALE_1KLY,
	SCALE_10KLY,
	SCALE_100KLY,
	SCALE_1MLY,
	SCALE_10MLY,
	SCALE_100MLY,
	SCALE_1GLY,
	SCALE_10GLY
};



//-----------------
//  ���̃}�[�N���[�h
//-----------------
enum {
	STARMARK_NONE,
	STARMARK_DOUBLE,
	STARMARK_EXOPLANETS
};
extern	char tyStarMark;



//----------------
//  �V�̐�E�V����
//----------------
enum {
	MILKYWAY_NONE,
	MILKYWAY_VISIBLE,
	MILKYWAY_VISIBLE2,
	MILKYWAY_IR,
	MILKYWAY_CMB
};
extern	char  tyMilkyWay;		// �V�̐�̎��



//--------------
//  ��C�̕\���@
//--------------
enum {
	ATMOSPHERE_NONE,
	ATMOSPHERE_SINGLE,
	ATMOSPHERE_MULTIPLE
};
extern char AtmosphereMode;



//================
//  �\���p�X�C�b�`
//================
const int DISPSW_NUM = 4;

//--  Index
enum {
	DISPSW_PLANET,			// �f��
	DISPSW_SATELLITE,		// �q��
	DISPSW_MINOR_PLANET,	// ���f��
	DISPSW_TNO				// ���z�n�O���V��
};

//--  Mask
const char DISPM_DISP			= 0x01;		// �\��
const char DISPM_NAME			= 0x02;		// ���O
const char DISPM_ORBIT			= 0x04;		// �O��
const char DISPM_SELECT			= 0x08;		// �I��
const char DISPM_AXIS			= 0x10;		// ���]��
const char DISPM_CLOUD			= 0x20;		// �_
const char DISPM_CITY_LIGHTS	= 0x40;		// ��i

void	init_disp_switches();
void	set_disp_switch(char idx, char val);
char	get_disp_switch(char idx);
bool	test_disp_switch(char idx, char mask);


extern	char  swEclipse;			// ���H
extern	char  swStar;				// �P��
extern	char  swConstellation;		// ����
extern	char  swGalaxy;				// ���
extern	char  swScaleLine;			// �X�P�[����

extern  bool  bSpacecraft;			// �T���@
extern	bool  bSurfacePointer;		// �n�\�̃|�C���^
extern	bool  bOortCloud;			// �I�[���g�̉_
extern	bool  bCMB;					// CMB		
extern  bool  bGalCenterBG;			// ��͌n���S�̔w�i
extern	bool  bVirgoCluster;		// ���Ƃߍ���͒c�̃}�[�N	
extern	bool  bNGC;					// NGC�V��
extern	bool  bCelestialGrid;		// �Ԍo�E�Ԉ܂̐�
extern	bool  bEcliptic;			// ����
extern	bool  bHorizonCoord;		// �n�����W
extern	bool  bGalacticGrid;		// ��o�E��܂̐�
extern	bool  bDate;				// ������\��
extern	bool  bSideLight;			// ������̏Ɩ�
extern	bool  bSubtitles;			// �������[�h
extern	bool  bAppTitle;			// �A�v���P�[�V�����̃^�C�g��
extern	bool  bAbout;				// ���
extern	bool  bDebug;				// �f�o�b�O���̕\��
extern	bool  bViewInfo;			// ���_���
extern	bool  bSingleEye;			// �Жځi�E�ځj�p�f���̂�
extern	bool  bPathInfo;			// �p�X���̕\��
extern  bool  bKeyHelp;				// �L�[�{�[�h�w���v��\��
extern	bool  bFPS;					// fps �̕\��

//------------------------
//  �쐬���郀�[�r�[�̃^�C�v
//------------------------
enum {
	MV_AVI,
	MV_TARGA,
	MV_PNG,
	MV_TARGA6,
	MV_PNG6,
	MV_CUBIC
};

extern char make_movie_type;
extern char mv_filename[256];
extern char mv_basefilename[256];

//-------
//  ���
//-------
extern	bool  bMovieMode;			// ���[�r�[���J���Ă��邩
extern	bool  bMoviePlaying;		// ���[�r�[�̍Đ�����
extern	bool  bSettingModeDisp;		// �ݒ胂�[�h�̕\��


//  �_����̋�����
void	set_mag_factor(float fac);
float	get_mag_factor();


#endif
