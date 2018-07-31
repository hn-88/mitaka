/**************************************************************************************************

Copyright (c) 2007   Tsunehiko Kato  and  4D2U Project, National Astronomical Observatory of Japan

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
#include  "../common/graphics/gl_win.h"
#include  "mtk_gl_common.h"
#include  "mtk_font.h"
#include  "mtk_screen.h"

// for Visual Studio
#define snprintf _snprintf



extern int gl_w, gl_h;




//------------------------
//  �E�B���h�E�̃T�C�Y��\��
//------------------------
void
draw_window_size()
{
	int w, h;
	w = gl_w;
	h = gl_h;

	char str[50];
	snprintf(str, 50, "%d x %d", w, h);

	glPushAttrib(GL_ENABLE_BIT);
	
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);



	gl_enter_2D_mode();
	
		//-- �w�i���Â�����
		const float af = 0.7;
		glEnable(GL_BLEND);
		glColor4f(0,0,0,1-af);
		glBegin(GL_TRIANGLE_FAN);
			glVertex2f(0,0);
			glVertex2f(w,0);
			glVertex2f(w,h);
			glVertex2f(0,h);
		glEnd();
		
		//-- �T�C�Y��`��
		glTranslatef(w/2, h/2, 0);
		const float sz = 16;
		glDisable(GL_BLEND);
		glColor3f(0,1,0);
		glScalef(sz, -sz, sz);
		draw_text_centering(str);
	gl_leave_2D_mode();

	glPopAttrib();
}


void
draw_black_layer(const frustum& frs, float af)
{
	float x1, y1, x2, y2, z1;
	x1 = 1.5*frs.get_x1();
	x2 = 1.5*frs.get_x2();
	y1 = 1.5*frs.get_y1();
	y2 = 1.5*frs.get_y2();
	z1 = 1.01*frs.get_z1();

	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	glPushMatrix();
		glLoadIdentity();
		//-- �w�i���Â�����
		glEnable(GL_BLEND);
		glColor4f(0,0,0,1-af);
		glTranslatef(0,0,-z1);
		glBegin(GL_TRIANGLE_FAN);
			glVertex2f(x1, y1);
			glVertex2f(x2, y1);
			glVertex2f(x2, y2);
			glVertex2f(x1, y2);
		glEnd();
	glPopMatrix();

	glPopAttrib();
}
