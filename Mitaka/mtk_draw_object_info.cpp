/**************************************************************************************************

Copyright (c) 2007-2012   Tsunehiko Kato

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
#include  "mtk_draw_object_info.h"
#include  <stdio.h>
#include  "../common/graphics/gl_win.h"
#include  "mtk_mode.h"
#include  "mtk_objects.h"
#include  "mtk_target.h"
#include  "mtk_font.h"
#include  "mtk_stereo.h"


//*****
extern int			win_w, win_h;


static bool  bDispObjectInfo = true;
static float objinfo_size = 1.0;



bool
is_object_info_enable()
{
	return bDispObjectInfo;
}


void
enable_object_info(bool flg)
{
	bDispObjectInfo = flg;
}

void
set_object_info_size(float sz)
{
	objinfo_size = sz;
}



void
draw_selected_object_info_sub(const mtk_draw_struct& ds, float x0, float y0, int mx, int my, float fx, float fy, std::vector<std::string>&  text)
{
	const int csN = 10;
	static bool bFirst = true;
	static float cs[csN], sn[csN];
	if (bFirst) {
		bFirst = false;

		for (int i=0; i<csN; i++) {
			float th = 0.5*M_PI * float(i)/(csN-1);
			cs[i] = cos(th);
			sn[i] = sin(th);
		}
	}
	

	float w, h;
	w = ds.vp_w;
	h = ds.vp_h;
	

	float a = min(ds.vp_w, ds.vp_h);
	float fac = (a < 200) ? a/200 : 1.0;

	fac *= objinfo_size;

	const float sub_size = 0.7f * fac;
	const float text_size = 21.0f * fac;
	const float spc1 = 0.4f;
	const float spc2 = 0.2f;
	const float line_width = 1.0 * fac;

	const float szw = text_size * fx;
	const float szh = text_size * fy;


	float dx1, dy1;
	dx1 = 0.6 * szw;
	dy1 = 0.6 * szh;

	float mgn_x, mgn_y;
	mgn_x = 0.3 * szw;
	mgn_y = 0.3 * szh;

	//---  Calc size  ---
	float wt, ht;

	wt = 0.0f;
	for (int i=0; i<text.size(); i++) {
		float w, h;
		get_text_size(text[i].c_str(), w, h);
		if (i >= 1)  w *= sub_size;
		if (wt < w)  wt = w;
	}


	ht = 1.0f + spc1 + sub_size * ((text.size() - 1) + (text.size() - 2)*spc2);



	wt *= szw;
	ht *= szh;

	//---
	float x1, y1, x2, y2, wm, hm;
	x1 = float(mx) - mgn_x + dx1;
	y1 = float(my) - mgn_y + dy1;
	x2 = x1 + wt + mgn_x;
	y2 = y1 + ht + mgn_y;
	wm = wt + 2*mgn_x;
	hm = ht + 2*mgn_y;


	//-- Adjust position
	float mgn_xx = 0.7*mgn_x;
	float mgn_yy = 0.7*mgn_y;
	if (x2+mgn_x+mgn_xx > x0+w) {
		x2 = x0+w - mgn_xx - mgn_x;
		x1 = x2 - wt - mgn_x;
	}
	if (y2+mgn_y+mgn_yy > y0+h) {
		y2 = y0+h - mgn_yy - mgn_y;
		y1 = y2 - ht - mgn_y;
	}


	//---  Draw  ---
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);

	gl_enter_2D_mode();
		glTranslatef(x1, y1, 0);

		glColor4f(0, 0.15, 0.23, 0.9);
		glBegin(GL_QUADS);
			//-- upper
			glVertex2f(mgn_x,    0);
			glVertex2f(mgn_x+wt, 0);
			glVertex2f(mgn_x+wt, mgn_y);
			glVertex2f(mgn_x,    mgn_y);
			//-- main body
			glVertex2f( 0,  mgn_y +szh);
			glVertex2f(wm,  mgn_y +szh);
			glVertex2f(wm,  ht+mgn_y);
			glVertex2f( 0,  ht+mgn_y);
			//-- lower
			glVertex2f(mgn_x,    ht+mgn_y);
			glVertex2f(mgn_x+wt, ht+mgn_y);
			glVertex2f(mgn_x+wt, hm);
			glVertex2f(mgn_x,    hm);
		glEnd();

		//-- Corner UL
		float Rx = mgn_x;
		float Ry = mgn_y;
		float xc, yc;
		xc = mgn_x;
		yc = mgn_y;
		glBegin(GL_TRIANGLE_FAN);
		glVertex3f(xc, yc, 0);
		for (int i=0; i<csN; i++) {
			float x = -Rx*cs[i] + xc;
			float y = -Ry*sn[i] + yc;
			glVertex3f(x, y, 0);
		}
		glEnd();

		//-- Corner UR
		xc = mgn_x + wt;
		yc = mgn_y;
		glBegin(GL_TRIANGLE_FAN);
		glVertex3f(xc, yc, 0);
		for (int i=0; i<csN; i++) {
			float x =  Rx*cs[i] + xc;
			float y = -Ry*sn[i] + yc;
			glVertex3f(x, y, 0);
		}
		glEnd();

		//-- Corner BR
		xc = mgn_x;
		yc = mgn_y + ht;
		glBegin(GL_TRIANGLE_FAN);
		glVertex3f(xc, yc, 0);
		for (int i=0; i<csN; i++) {
			float x = -Rx*cs[i] + xc;
			float y =  Ry*sn[i] + yc;
			glVertex3f(x, y, 0);
		}
		glEnd();

		//-- Corner BR
		xc = mgn_x + wt;
		yc = mgn_y + ht;
		glBegin(GL_TRIANGLE_FAN);
		glVertex3f(xc, yc, 0);
		for (int i=0; i<csN; i++) {
			float x =  Rx*cs[i] + xc;
			float y =  Ry*sn[i] + yc;
			glVertex3f(x, y, 0);
		}
		glEnd();


		//--  Hi-light
		glBegin(GL_QUADS);
			glColor4f(0, 0.4, 0.7, 1.0);
			glVertex2f( 0,  mgn_y);
			glVertex2f(wm,  mgn_y);
			glColor4f(0, 0.25, 0.4, 0.95);
			glVertex2f(wm,  mgn_y +szh);
			glVertex2f( 0,  mgn_y +szh);
		glEnd();



		//--  Line
		glColor4f(0, 0.4, 0.7, 1.0);
		glLineWidth( line_width );
		glBegin(GL_LINES);
			glVertex3f(mgn_x, 0, 0);
			glVertex3f(wt+mgn_x, 0, 0);

			glVertex3f(mgn_x, hm, 0);
			glVertex3f(wt+mgn_x, hm, 0);

			glVertex3f(0, mgn_y, 0);
			glVertex3f(0, mgn_y+ht, 0);

			glVertex3f(wm, mgn_y, 0);
			glVertex3f(wm, mgn_y+ht, 0);
		glEnd();


		//-- Corner UL
		xc = mgn_x;
		yc = mgn_y;
		glBegin(GL_LINE_STRIP);
		for (int i=0; i<csN; i++) {
			float x = -Rx*cs[i] + xc;
			float y = -Ry*sn[i] + yc;
			glVertex3f(x, y, 0);
		}
		glEnd();

		//-- Corner UR
		xc = mgn_x + wt;
		yc = mgn_y;
		glBegin(GL_LINE_STRIP);
		for (int i=0; i<csN; i++) {
			float x =  Rx*cs[i] + xc;
			float y = -Ry*sn[i] + yc;
			glVertex3f(x, y, 0);
		}
		glEnd();

		//-- Corner DL
		xc = mgn_x;
		yc = mgn_y + ht;
		glBegin(GL_LINE_STRIP);
		for (int i=0; i<csN; i++) {
			float x = -Rx*cs[i] + xc;
			float y =  Ry*sn[i] + yc;
			glVertex3f(x, y, 0);
		}
		glEnd();

		//-- Corner UR
		xc = mgn_x + wt;
		yc = mgn_y + ht;
		glBegin(GL_LINE_STRIP);
		for (int i=0; i<csN; i++) {
			float x =  Rx*cs[i] + xc;
			float y =  Ry*sn[i] + yc;
			glVertex3f(x, y, 0);
		}
		glEnd();



		glTranslatef(mgn_x, mgn_y, 0);
		glScalef(szw, -szh, szw);
		glTranslatef(0,-1,0);
		glTranslatef(0,0.1,0);


		//---  Name  ---
		glColor4f(1, 1, 1, 1);
		glPushMatrix();
			draw_text( text[0] );
		glPopMatrix();


		glScalef(sub_size, sub_size, sub_size);
		glTranslatef(0,-1-spc1,0);
		if (text.size() >= 1) {
			//---  Text  ---
			for (int i=1; i<text.size(); i++) {
				glPushMatrix();
					draw_text( text[i] );
				glPopMatrix();
				if (i < text.size()-1)  { glTranslatef(0,-1-spc2,0); }
			}
		}
	gl_leave_2D_mode();

	glEnable(GL_DEPTH_TEST);
}



void
draw_selected_object_info(const mtk_draw_struct& ds)
{
	if (!bDispObjectInfo) return;


	//***** Planetarium mode �́A�܂��Ή����Ă��Ȃ�
	if (ds.tyMode != MODE_3D)  return;
	//*****
	if ( !is_there_selected_object() )  return;



	//---  Make text  ---
	const mtk_object&  obj = get_selected_object();
	std::vector<std::string>  text;
	obj.make_object_info(ds, text);
	if (text.size() <= 0.0)  return;



	//---  Hit position  ---
	int mx, my;
	get_object_hit_pos(&mx, &my);
	my = win_h - my;

	switch (get_display_mode()) {
		case DISPMODE_SIDE_BY_SIDE:
			if (mx > win_w / 2) { mx -= win_w / 2; }
			draw_selected_object_info_sub(ds, 0,0,       mx, my,         0.5, 1.0, text);
			draw_selected_object_info_sub(ds, win_w/2,0, win_w/2+mx, my, 0.5, 1.0, text);
			break;
		case DISPMODE_TOP_AND_BOTTOM:
			if (my > win_h / 2) { my -= win_h / 2; }
			draw_selected_object_info_sub(ds, 0,0,       mx, my/2,         1.0, 0.5, text);
			draw_selected_object_info_sub(ds, 0,win_h/2, mx, win_h/2+my/2, 1.0, 0.5, text);
			break;
		default:
			draw_selected_object_info_sub(ds, 0,0, mx, my, 1.0, 1.0, text);
			break;
	}
}
