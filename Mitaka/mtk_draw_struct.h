/**************************************************************************************************

Copyright (c) 2007-2016   Tsunehiko Kato  and  4D2U Project, National Astronomical Observatory of Japan

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
#ifndef __MTK_DRAW_STRUCT_H__
#define __MTK_DRAW_STRUCT_H__
#include  <windows.h>
#include  "../common/math/vector3.h"
#include  "mtk_text.h"
#include  "mtk_screen.h"
#include  "mtk_camera.h"




//-------------------
//  �`��p���̍\����
//-------------------
class mtk_draw_struct
{
public:
	int			tyMode;			// ���[�h
	int			tySubMode;



	// �������W���王�_���W�ւ̕ϊ��i�X�P�[�����O���܂ށj
	matrix3d	M_view, M_view_inv;
	vector3d	r_view;

	// ���[���h���W�n�ł̎����䕽��
	vector3d nv_near[6], nv_far[6];
	double D_near[6], D_far[6];


	double		JD;					// �����i�����E�X���j  //***

	double		scale;				// �X�P�[��(pc)
	double		scale_factor;		// �X�P�[�����q

	float		plnt_R, plnt_angle, plnt_d;
	
	mtk_camera	camera_base0;		// �x�[�X�J�������i�f���ł̎����ړ����܂܂Ȃ����́B�w�b�h���C�g�p�j
	mtk_camera	camera_base;		// �x�[�X�J�������
	mtk_camera	camera;				// �`��Ɏg�p����J����
	double		cnorm;				// �J�����̌��_����̋���

	vector3d	tgt_pos;			// �^�[�Q�b�g�̈ʒu
	int			tgt_ID;

	int			vp_x1, vp_x2, vp_y1, vp_y2, vp_w, vp_h;		// �r���[�|�[�g
	float		screen_aspect_ratio;

	frustum		frs_near, frs_far;	// Frustum

	vector3f	eye_offset;

	float		rad_per_pixel;		// 1�s�N�Z��������̊p�x�i���W�A���P�ʁj
	float		ltf;				// �����̑��ΓI�T�C�Y *** �i���j
	float		line_width_fac;		// ���̑����Ɋ|������q

	bool		bRotateBillboard;	// �r���{�[�h���J�����Ɍ����ĉ�]�����邩
	vector3f	eRf, eUf;			// ��]���Ȃ��ꍇ�̃r���{�[�h�̌���

private:
	bool		bNear;
	void	calc_frustum_planes_on_world(const frustum& frs, vector3d *nv, double *D) const;

public:
	mtk_draw_struct()	{};

	void	set(int mode, int submode, const mtk_camera& _camera_base, const double& _JD, float scrn_aspct_ratio, int _vp_x1, int _vp_x2, int _vp_y1, int _vp_y2, const float font_size, int target_ID, const vector3d& target_pos, vector3f dr, bool bRotBB, float line_w);
	void	set_red_per_pixel();

	vector3d	calc_pos_on_viewframe(const vector3d& pos) const	{ return  M_view * pos + r_view; }
	vector2f	calc_pos_on_screen(const vector3d& pos) const;

	void	set_near_region()		{ bNear = true; }
	void	set_far_region()		{ bNear = false; }
	frustum	get_frustum() const 	{ return  ((bNear) ? frs_near : frs_far); }

	void	recalc_frustum_planes_on_world();		//***** �h�[���}�X�^�[�p�̎b��[�u *****
	void	get_frustum_planes_on_world(const vector3d* &nv, const double* &D) const;

	bool	is_near_region() const	{ return bNear; }
};


#endif
