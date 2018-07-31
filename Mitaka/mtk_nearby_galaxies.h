/**************************************************************************************************

Copyright (c) 2007   Tsunehiko Kato

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
#ifndef __MTK_NEARBY_GALAXIES_H__
#define __MTK_NEARBY_GALAXIES_H__
#include  "mtk_draw_struct.h"
#include  "../common/win_file.h"
#include  "../common/astro/astrometry.h"
#include  <vector>
#include  <string>





//--  Nearby galaxy
struct nearby_galaxy2
{
	std::string  key;			// key

	char		cat_type;		// 'N':NGC, 'T':Tully, 'U':UGC, 'P':PGC, '*':Undefined
	int			cat_no;			//

	RA_DEC		eqPos;			// Right ascension/Declination (in degree)
	float		distance;		// Distance (in pc)
	char		dtype;			// Draw type
	char		mtype;			// Morphological type
	bool		bBar;
	int			tex_idx;		// texture index for image

	float		D;				// diameter (in arcmin)
	float		d_D;			// Axial ratio of minor to major diameter
	char		group;			// Group

	float		R;				// ���a (in pc)
	vector3f	xyz;			// �������W

	vector3f	eR, eU;			// �r���{�[�h�̕ӂ̌���

	int			obj_idx;
};




//---  Load / Init  ---
bool	load_nearby_galaxies(const directory& dir);
void	load_galaxies_textures();
void	init_galaxy_display_list();


//---
nearby_galaxy2&	get_nearby_galaxy(int idx);

//---  Position  ---
vector3d	get_nearby_galaxy_position(int idx);
//---  Scale  ---
double	get_scale_nearby_galaxy(int idx);
//---  Label  ---
bool	is_nearby_galaxy_label_visible(const mtk_draw_struct& ds, int idx, bool bTgt);
float	get_nearby_galaxy_label_alpha(const mtk_draw_struct& ds, int idx);


//
double	get_landing_radius_nearby_galaxy(int idx);


//---  Draw
bool	prepare_draw_nearby_galaxy(const mtk_draw_struct& ds, int idx, double* pRb, double* pRh, float* pAlp);
void	draw_a_nearby_galaxy(const mtk_draw_struct& ds, int idx, const vector3d& rpos, const double& scl);
void	draw_a_nearby_galaxy_as_point(const mtk_draw_struct& ds, int idx, const vector3d& rpos, float alpha);


//---  Info
void	make_object_info_nearby_galaxy(const mtk_draw_struct& ds, int idx, std::vector<std::string>& text);


//---  Number  ---
int		get_number_of_nearby_galaxies();



#endif
