/**************************************************************************************************

Copyright (c) 2012-2015   Tsunehiko Kato

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
#include <windows.h>
#include "../common/graphics/gl_image.h"
#include "../common/graphics/gl_win.h"
#include  "mtk_draw_struct.h"
#include  "mtk_camera_setting.h"
#include  "mtk_stereo.h"
#include  "mtk_mode.h"
#include  "mtk_objects.h"
#include  "mtk_planetarium.h"





//--  Externs
void  draw_scene_all(const mtk_draw_struct& ds, bool bClear);
mtk_draw_struct calc_draw_struct(const mtk_camera& camera_base, float aspc_rt, int _vp_x1, int _vp_x2, int _vp_y1, int _vp_y2, vector3f dr, bool bRotBB, float line_w);
void set_latest_draw_struct(const mtk_draw_struct& ds, int idx);


//--  Objects
static float eye_dist = 5.0;




//----------------
//  Eye distance
//----------------
float
get_stereo_eye_distance()
{
	return eye_dist;
}

void
set_stereo_eye_distance(float d)
{
	eye_dist = d;
}



//============
//  �A�i�O���t
//============

//-------------------------------
//  �O���[�X�P�[���ւ̕ϊ��W��
//-------------------------------

//---   NTSC (�W���� 1000 �{�j  ---
const int cR = 299;
const int cG = 587;
const int cB = 114;



static image img, imgAnaglyph;


void
make_anaglyph_left(int w, int h)
{
	img.alloc(w,h,3);
	imgAnaglyph.alloc(w,h,3);

	// Read Pixels
	GLubyte* p0 = (GLubyte*)img.get_pointer();
	glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, p0);


	GLubyte* p  = img.get_pointer();
	GLubyte* pa = imgAnaglyph.get_pointer();
	const int imax = w * h;
	for (int i=0; i<imax; i++) {
		int lv = (cR*p[0] + cG*p[1] + cB*p[2])/1000;
		GLubyte val = GLubyte(lv);
		pa[0] = val;		// R-channel
		p  += 3;
		pa += 3;
	}
}


void
make_anaglyph_right(int w, int h)
{
	GLubyte* p0 = (GLubyte*)img.get_pointer();
	glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, p0);


	GLubyte* p  = img.get_pointer();
	GLubyte* pa = imgAnaglyph.get_pointer();
	const int imax = w * h;
	for (int i=0; i<imax; i++) {
		int lv = (cR*p[0] + cG*p[1] + cB*p[2])/1000;
		GLubyte val = GLubyte(lv);
		
		pa[1] = val;
		pa[2] = val;
		p  += 3;
		pa += 3;
	}


	// Draw Pixels
	gl_enter_2D_mode();
		glRasterPos2i(0,h-1);
		glPixelZoom(1,1);
		glDrawPixels(w, h, GL_RGB, GL_UNSIGNED_BYTE, (GLubyte*)imgAnaglyph.get_pointer());
	gl_leave_2D_mode();
}



void
draw_stereo_anaglyph(const mtk_camera camera_base, int w, int h)
{
	float aspc_rt = float(h) / float(w);
	mtk_draw_struct  ds[2];

	float eye_x = get_eye_offset_x();

	vector3f dr = 0.5f*eye_dist*vector3f(1,0,0);

	ds[0] = calc_draw_struct(camera_base, aspc_rt, 0, w, 0, h, -dr, false, 1.0);
	ds[1] = calc_draw_struct(camera_base, aspc_rt, 0, w, 0, h, dr, false, 1.0);
	set_latest_draw_struct(ds[0], 0);


	if (ds[0].tyMode == MODE_3D) {
		prepare_draw_objects(ds, 2);
	}
	else {
		prepare_draw_planetarium(ds, 2);
	}



	// draw L
	draw_scene_all(ds[0], true);
	make_anaglyph_left(w, h);

	// draw R
	draw_scene_all(ds[1], true);
	make_anaglyph_right(w, h);


	set_eye_offset_x(eye_x);
}


void
draw_stereo_color_anaglyph(const mtk_camera camera_base, int w, int h)
{
	float aspc_rt = float(h) / float(w);
	mtk_draw_struct  ds[2];


	float eye_x = get_eye_offset_x();

	vector3f dr = 0.5f*eye_dist*vector3f(1, 0, 0);

	ds[0] = calc_draw_struct(camera_base, aspc_rt, 0, w, 0, h, -dr, false, 1.0);
	ds[1] = calc_draw_struct(camera_base, aspc_rt, 0, w, 0, h, dr, false, 1.0);
	set_latest_draw_struct(ds[0], 0);


	if (ds[0].tyMode == MODE_3D) {
		prepare_draw_objects(ds, 2);
	}
	else {
		prepare_draw_planetarium(ds, 2);
	}


	// draw L
	glColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_FALSE);
	draw_scene_all(ds[0], true);

	// draw R
	glColorMask(GL_FALSE, GL_TRUE, GL_TRUE, GL_FALSE);
	draw_scene_all(ds[1], true);


	set_eye_offset_x(eye_x);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE);
}






//============
//  ��������
//============

//--- Side by Side �i���ڗp�摜�������j
void
draw_stereo_side_by_side(const mtk_camera camera_base, int w, int h)
{
	float aspc_rt = float(h) / float(w);
	mtk_draw_struct  ds[2];

	vector3f dr = 0.5f*eye_dist*vector3f(1, 0, 0);


	ds[0] = calc_draw_struct(camera_base, aspc_rt, 0, w / 2, 0, h, -dr, false, 1.0);
	ds[1] = calc_draw_struct(camera_base, aspc_rt, w / 2, w, 0, h, dr, false, 1.0);
	set_latest_draw_struct(ds[0], 0);
	set_latest_draw_struct(ds[1], 1);


	if (ds[0].tyMode == MODE_3D) {
		prepare_draw_objects(ds, 2);
	}
	else {
		prepare_draw_planetarium(ds, 2);
	}


	// ����
	draw_scene_all(ds[0], true);

	// �E��
	draw_scene_all(ds[1], false);

	glViewport(0,0, w, h);
}



//--- Above Below �i���ڗp�摜����j
void
draw_stereo_above_below(const mtk_camera camera_base, int w, int h)
{
	float aspc_rt = float(h) / float(w);
	mtk_draw_struct  ds[2];
	
	vector3f dr = 0.5f*eye_dist*vector3f(1, 0, 0);

	//***  glviewport�͍��������_  ***
	ds[0] = calc_draw_struct(camera_base, aspc_rt, 0, w, h / 2, h, -dr, false, 1.0);
	ds[1] = calc_draw_struct(camera_base, aspc_rt, 0, w, 0, h / 2, dr, false, 1.0);
	set_latest_draw_struct(ds[0], 0);
	set_latest_draw_struct(ds[1], 1);


	if (ds[0].tyMode == MODE_3D) {
		prepare_draw_objects(ds, 2);
	}
	else {
		prepare_draw_planetarium(ds, 2);
	}


	// ����
	draw_scene_all(ds[0], true);

	// �E��
	draw_scene_all(ds[1], false);

	glViewport(0,0, w,h);
}
