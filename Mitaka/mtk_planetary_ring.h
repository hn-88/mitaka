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
#ifndef __MTK_PLANETARY_RING_H__
#define __MTK_PLANETARY_RING_H__
#include  "../common/math/vector3.h"
#include  "../common/graphics/gl_common.h"
#include  "../common/graphics/color.h"
#include  "../common/JulianDate.h"
#include  "../common/astro/astrometry.h"
#include  "mtk_draw_struct.h"
#include  <string>


const int particle_ring_RN   =  30;
const int particle_ring_PHIN = 180;




class  particle_ring
{
private:
	float		R1, R2;
	vector3f	pos[particle_ring_RN][particle_ring_PHIN+1];
	float		s[particle_ring_RN];
	float		shadow_fac[particle_ring_RN][particle_ring_PHIN + 1];
	bool		b_shadow_boundary[particle_ring_RN][particle_ring_PHIN + 1];
	int			last_mode;
	bool		bLastSideLight;
	double		JD;
	float		alp;	// �f���ɗ��Ƃ��e�p�̓����x

	//---  Textures
	std::string tex_file_name;
	GLuint tex_idx;


public:
	particle_ring(float _R1, float _R2, float _alp, const std::string& tex_fn);
	float	get_R1() const	{ return R1; }
	float	get_R2() const	{ return R2; }
	float	get_alpha() const { return alp; }

	void	calc_shadow_factor(const mtk_draw_struct& ds, const vector3d& r0, const double& R, const double& f, const vector3d& ex, const vector3d& ey, const vector3d& ez, const vector3d& _lv);

	// Textures
	GLuint		load_texture(int shrink_rate);
	GLuint		get_texture_index() const	{ return tex_idx; }


	void	draw_triangles(const vector2f* pt, const float *u, const float *f, const int *pt_idx, const int N, bool bShadow) const;
	void	draw_boundary_mesh(int i, int j) const;

	void	draw(const mtk_draw_struct& ds, const vector3d& r0, const double& R, const vector3d& ex, const vector3d& ey, const vector3d& lv)  const;
	void	draw_3Dchart() const;
};




void	draw_ring_shaded_sphere(const vector3d& r0, const double& R, const double& f, const vector3d& ex, const vector3d& ey, const vector3d& ez, const vector3d &_lv, const particle_ring& pr, const GLuint texPlanet, const RGBf& mat_col);





int		add_particle_ring(float R1, float R2, float alp, const std::string& tex_fn);
particle_ring&	get_particle_ring(int idx);

void	load_ring_textures();




// for init
struct ring_info {
	double  Rr1, Rr2;	// �ւ̓����a�A�O���a�i�ԓ����a��P�ʂƂ��āj
	float	ring_alp;	// �f���ɗ��Ƃ��e�p�̓����x
	const char* tex_fn;	// �����O�̃e�N�X�`���̃t�@�C����
};


#endif
