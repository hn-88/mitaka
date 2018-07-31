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
#include  <windows.h>
#include  "../common/graphics/gl_common.h"
#include  "../common/graphics/gl_win.h"
#include  "mtk_text.h"
#include  "mtk_font.h"
#include  "mtk_gl_common.h"
#include  "mtk_image.h"
#include  "mtk_title_page.h"
#include  "mtk_end_roll.h"
#include  "mtk_strings.h"
#include  "mtk_directory.h"



//--  Caution
static mtk_text txtCaution;
static bool bLoadedCautionText = false;

//--  Title Image
static mtk_image TitleImage;
static bool bExistTitleImage = false;



//-----------
//  Externs
//-----------
extern char  tyProgram;
extern int	win_w, win_h;

/***/
float	get_animation_frac();
extern bool	bControler;
void	OnCommandSignal();
void	ClearSignal();
bool	GetSignal();
void	WaitForSignal();
void	goto_center_screen2();



//-------------
//  Variables
//-------------
static char  nTitlePage			= TITLEPAGE_CREDITS;	// �^�C�g�����[�h�̃y�[�W
static float TitleWidthRatio	= 1.0;
float	TitleDuration = 1.0;
float	TitleZPosIni  = 0.0;





//----------------
//  Caution text
//----------------
void
load_caution_text(const char* fn)
{
	FILE* fp;


	txtCaution.clear();
	bLoadedCautionText = false;

	directory dir = get_directory(DIR_LOCALE);
	if ((fp = dir.fopen_utf8(fn, "rt")) == NULL)  return;


	string buf;
	int ch;
	while ((ch = getc(fp)) != EOF) {
		buf.push_back(char(ch));
	}
	buf.push_back('\0');
	fclose(fp);

	bLoadedCautionText = txtCaution.set_text(buf.c_str());
}




//---------------
//  Title Image
//---------------
void
load_title_image(const char* fn)
{
	bExistTitleImage = false;
	
	directory dir = get_directory(DIR_LOCALE);
	
	TitleImage.set_image(dir.get_path(fn));
	bExistTitleImage = true;
}




//----------------
//  Title Page
//----------------
void
draw_image_for_title_page()
{
	float scl = 0.5;

	// �摜��\��
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		TitleImage.draw(win_w*TitleWidthRatio, win_h);
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}


//--- Caution ---
void
draw_title_caution()
{
	if (!bLoadedCautionText)  return;

	mtk_text_block_size  bsz;
	bsz = txtCaution.get_total_size();

	float scl = 0.8 * get_screen_w() / bsz.w;
	float total_h = bsz.hu - bsz.hd;

	glPushMatrix();
	glScalef(scl, scl, scl);
	glTranslatef(-0.5*bsz.w, -0.5*total_h, 0);
	glColor3f(1, 1, 1);
	txtCaution.draw();
	glPopMatrix();
}


void
draw_title_page_attention()
{
	draw_title_caution();

	// �^�C�g����ǂݍ���ł���
	if (!TitleImage.is_loaded()) {
		TitleImage.load();
	}

	return;
}



//--- �^�C�g�� ---
void
draw_title_page_title()
{
	// �A�j���[�V�����̏���
	float t = get_animation_frac();
	float rt = get_title_zpos_ini();
	float z  = rt*(1.0-t);
	glTranslatef(0,0,z);

	
	if (bExistTitleImage) {
		glColor4f(1, 1, 1, t);
		draw_image_for_title_page();
	} else {
		float sz = 0.08;
		glScalef(sz,sz,sz);
		glColor4f(0.5,0.7,1,t);

		draw_text_centering(get_string("THEATER_TITLE"));
	}
}


//--- �e�[�} ---
void
draw_title_page_theme()
{
	float t = get_animation_frac();
	float sz = 0.06;
	glScalef(sz,sz,sz);
	glTranslatef(0,0.5,0);
	glColor4f(1,1,1,t);

	std::string key;

	key = "PROGRAM_TITLE_" + std::to_string(tyProgram);
	draw_text_line(get_string(key), TEXT_CENTER, true);
	sz = 0.8;
	glScalef(sz,sz,sz);
	glTranslatef(0,-0.5,0);

	key = "PROGRAM_SUB_TITLE_" + std::to_string(tyProgram);
	draw_text_line(get_string(key), TEXT_CENTER, true);
}


//--- �N���W�b�g ---
void
draw_title_page_credits()
{
	// �X�N���[���̏���
	float t = get_animation_frac();
	draw_end_credits(t);
	return;
}


void
draw_title_page()
{
	const float screen_w = get_screen_w();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glPushMatrix();
		goto_center_screen2();

		//*****
		if (nTitlePage == TITLEPAGE_CREDITS) {
			draw_title_page_credits();
		} else if (nTitlePage == TITLEPAGE_ATTENTION) {
			draw_title_page_attention();
		} else if (nTitlePage == TITLEPAGE_TITLE) {
			const float scl = 0.5 * screen_w;
			glScalef(scl, scl, scl);
			draw_title_page_title();
		} else {
			// �X�N���[���̕����P�ɃX�P�[�����O����
			const float scl = screen_w * TitleWidthRatio;
			glScalef(scl, scl, scl);

			switch (nTitlePage) {
				case TITLEPAGE_THEME:		draw_title_page_theme();		break;
				default:	break;
			}
		}
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
}



//-------------
//  set / get
//-------------
void
set_title_page_no(int idx)
{
	nTitlePage = idx;
}

int
get_title_page_no()
{
	return nTitlePage;
}

void
set_title_width_ratio(float twr)
{
	TitleWidthRatio = twr;
}

float
get_title_width_ratio()
{
	return  TitleWidthRatio;
}


//  Duraton
void
set_title_duration(float dur)
{
	TitleDuration = dur;
}

float
get_title_duration()
{
	return  TitleDuration;
}

//  ZPos
void
set_title_zpos_ini(float z)
{
	TitleZPosIni = z;
}

float
get_title_zpos_ini()
{
	return  TitleZPosIni;
}
