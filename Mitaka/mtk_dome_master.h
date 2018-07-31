/**************************************************************************************************

Copyright (c) 2015   Tsunehiko Kato  and  4D2U Project, National Astronomical Observatory of Japan

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
#ifndef __MTK_DOME_MASTER_H__
#define __MTK_DOME_MASTER_H__
#include  "../common/graphics/gl_framebuffer.h"
#include  "../common/graphics/gl_win.h"
#include  "../common/graphics/gl_image.h"
#include  "mtk_camera.h"




class gl_dome_master
{
	int N, _Nx[5], _Ny[5], _idx0[5];
	float *vtxbuf, *uvbuf;
	unsigned int *idxbuf;
	float circle_R;

public:
	gl_dome_master();
	~gl_dome_master();

	void	init(int _N);
	void	generate_map(const vector3f& eye_offset, const vector3f& proj_offset, float proj_elev_angle, float proj_azimuth, float proj_slue, float proj_view_angle);
	void	Draw(int face) const;
};



//---------------
//  Dome master
//---------------
void	set_dome_master_buffer_size(int sz);
void	set_dome_master_capture_size(int sz);
void	set_dome_master_mesh_N(int N);
void	set_dome_master_dome_R(float R);
void	set_dome_master_elev_angle(float angle);
float	get_dome_master_elev_angle();
void	set_dome_master_elev_angle_planetarium(float angle);
float	get_dome_master_elev_angle_planetarium();
void	set_dome_master_label_mag(float fac);
float	get_dome_master_label_mag();
void	set_dome_master_line_width_fac(float fac);
float	get_dome_master_line_width_fac();
// projector settings
void	set_dome_master_proj_offset(const vector3f& proj_offset);
void	set_dome_master_proj_elev_angle(float angle);
void	set_dome_master_proj_azimuth(float angle);
void	set_dome_master_proj_slue(float angle);
void	set_dome_master_proj_view_angle(float angle);
// projector settings with intersection point on dome
void	set_dome_master_intersection_mode(bool bFlg);
void	set_dome_master_intersection_elev_angle(float angle);
void	set_dome_master_intersection_azimuth(float angle);
// rotation angle for stereo scopic mode
void	set_dome_master_stereo_rot_angle(float angle);

void	load_dome_master_distortion_map();

void	draw_dome_master(const mtk_camera camera_base, int w, int h);



// Capture
void	capture_dome_master(const mtk_camera camera_base, image& img);




#endif
