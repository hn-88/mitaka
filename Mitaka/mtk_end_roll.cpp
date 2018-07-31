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
#include  <gl/GL.h>
#include  <stdio.h>
#include  <string>
#include  "mtk_directory.h"
#include  "mtk_text.h"



using namespace std;



// �e�L�X�g
static mtk_text  end_credits;


//
static float width    = 100;
static float DY       = 0.0;
static float duration = 10.0;
static float angle    = 0.0;

static bool bLoadedEndCredits = false;



void
load_end_credits(const char* fn)
{
	FILE* fp;

	end_credits.clear();
	bLoadedEndCredits = false;

	directory dir = get_directory(DIR_LOCALE);
	if ((fp = dir.fopen_utf8(fn, "rt")) == NULL)  return;


	string buf;
	int ch;
	while ((ch = getc(fp)) != EOF) {
		buf.push_back(char(ch));
	}
	buf.push_back('\0');
	fclose(fp);


	bLoadedEndCredits = end_credits.set_text(buf.c_str());

	if (!bLoadedEndCredits) {
		end_credits.set_text("Failed!");
	}

	bLoadedEndCredits = true;
}




void
draw_end_credits(float frac)
{
	if (!bLoadedEndCredits)  return;


	mtk_text_block_size  bsz;
	bsz = end_credits.get_total_size();


	float scl = width / bsz.w;
	float total_h = bsz.hu - bsz.hd;
	float dy = total_h * frac;
	float y = dy - total_h + DY / scl;

	glPushMatrix();
		glScalef(scl, scl, scl);
		glRotatef(-angle, 1, 0, 0);
		glTranslatef(-0.5*bsz.w, y, 0);
		glColor3f(1, 1, 1);
		end_credits.draw();
	glPopMatrix();
}




//-----------
//  Set/Get
//-----------
void
set_credit_roll_width(float w)
{
	width = w;
}

float
get_credit_roll_width()
{
	return width;
}

void
set_credit_roll_dy(float dy)
{
	DY = dy;
}

float
get_credit_roll_dy()
{
	return DY;
}

void
set_credit_roll_duration(float dur)
{
	duration = dur;
}

float
get_credit_roll_duration()
{
	return duration;
}

void
set_credit_roll_angle(float ang)
{
	angle = ang;
}

float
get_credit_roll_angle()
{
	return angle;
}


