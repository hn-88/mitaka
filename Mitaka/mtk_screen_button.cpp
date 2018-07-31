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
#include  "mtk_screen_button.h"
#include  "../common/graphics/gl_win.h"



bool
mtk_screen_button::isInside(int x, int y)  const
{
	return  (x>=rc.left && x<=rc.right && y>=rc.top && y<=rc.bottom);
}

bool
mtk_screen_button::SetPushState(int x, int y)
{
	int old_state = GetState();

	if ( isInside(x,y) ) {
		SetState( SCRBTN_PUSH );
	} else {
		SetState( SCRBTN_UNPUSH );
	}

	return (old_state != GetState());
}

bool
mtk_screen_button::SetOverlapState(int x, int y)
{
	int old_state = GetState();

	if ( isInside(x,y) ) {
		SetState( SCRBTN_OVERLAPPED );
	} else {
		SetState( SCRBTN_UNPUSH );
	}

	return (old_state != GetState());
}


void
mtk_screen_button_minus::Draw()  const
{
	float f = 0.5;
	switch (GetState()) {
		case SCRBTN_PUSH:		f = 1.0;	break;
		case SCRBTN_OVERLAPPED:	f = 0.7;	break;
		default:							break;
	}

	const RECT& rc = GetRect();
	const int size = GetSize();
	float xc, yc, d, l;
	xc = rc.left + 0.5*size;
	yc = rc.top  + 0.5*size;
	d  = 0.05 * size;
	l  = 0.30 * size;

	//--- Minus Button ---
	glColor3f(0,f,0);
	glBegin(GL_QUADS);
		glVertex3f(xc-l, yc-d, 0);
		glVertex3f(xc-l, yc+d, 0);
		glVertex3f(xc+l, yc+d, 0);
		glVertex3f(xc+l, yc-d, 0);
	glEnd();
}


void
mtk_screen_button_plus::Draw()  const
{
	float f = 0.5;
	switch (GetState()) {
		case SCRBTN_PUSH:		f = 1.0;	break;
		case SCRBTN_OVERLAPPED:	f = 0.7;	break;
		default:							break;
	}

	const RECT& rc = GetRect();
	const int size = GetSize();
	float xc, yc, d, l;
	xc = rc.left + 0.5*size;
	yc = rc.top  + 0.5*size;
	d  = 0.05 * size;
	l  = 0.30 * size;

	//--- Plus Button ---
	glColor3f(0,f,0);
	glBegin(GL_QUADS);
		glVertex3f(xc-l, yc-d, 0);
		glVertex3f(xc-l, yc+d, 0);
		glVertex3f(xc+l, yc+d, 0);
		glVertex3f(xc+l, yc-d, 0);
		glVertex3f(xc-d, yc-l, 0);
		glVertex3f(xc-d, yc+l, 0);
		glVertex3f(xc+d, yc+l, 0);
		glVertex3f(xc+d, yc-l, 0);
	glEnd();
}
