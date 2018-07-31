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
#ifndef __MTK_MINOR_PLANETS_H__
#define __MTK_MINOR_PLANETS_H__
#include  <stdio.h>
#include  "mtk_draw_struct.h"
#include  "mtk_space_curve.h"
#include  "mtk_orbital_element.h"



const float DwarfPlanetNameSizeDef	= 2.6f;
const float SmallObjectNameSizeDef	= 2.4f;





//--  Object Type  --
enum {
	SMALLOBJ_TYPE_UNDEF,
	SMALLOBJ_TYPE_ATIRA,
	SMALLOBJ_TYPE_ATEN,
	SMALLOBJ_TYPE_APOLLO,
	SMALLOBJ_TYPE_AMOR,
	SMALLOBJ_TYPE_OBJ_SMALL_Q,
	SMALLOBJ_TYPE_HUNGARIA,
	SMALLOBJ_TYPE_PHOCAEA,
	SMALLOBJ_TYPE_HILDA,
	SMALLOBJ_TYPE_JUPITER_TROJAN,
	SMALLOBJ_TYPE_CENTAUR,
	SMALLOBJ_TYPE_SDO,
	SMALLOBJ_TYPE_TNO
};




//-----------------
//  Small objects
//-----------------
struct  small_object
{
	std::string  key;		// Key

	int  type;				// �^�C�v
	int  no;				// ���f���ԍ�
	int  obj_idx;			// �I�u�W�F�N�g�̃C���f�b�N�X
	int  phys_param_idx;
	int  ring_idx;
	bool bSelected;			// �I������Ă��邩
	bool bHasLabel;			// ���x��������
	bool bDwarfPlanet;		// Dwarf planet ��
	orbital_elements  oe;	// �O���v�f

	int ori_idx;			// orientation

	vector3f	eR, eU;		// �r���{�[�h�̕ӂ̌���
};




//---  Load / Init  ---
int			load_small_objects(FILE* fp);


//---  Object
int	get_small_object_index(const std::string& key);


//---  Position  ---
vector3d	get_small_object_position(int idx);
//---  Scale  ---
double		get_scale_small_object(int idx);
//---  Label  ---
bool		is_small_object_label_visible(int idx, bool bTgt);

//--
double		get_landing_radius_small_object(int idx);

//---  Textures  ---
void	load_minor_planet_textures();



//---  Draw  ---
bool		prepare_draw_small_object(const mtk_draw_struct& ds, int idx, double* pRb, double* pRh);
float		get_minor_planet_point_alpha(const mtk_draw_struct& ds, int idx);

void		draw_a_small_object(const mtk_draw_struct& ds, int idx, const vector3d& rpos, const double& scl);
void		draw_a_small_object_point(const mtk_draw_struct& ds, int idx, const vector3d& rpos, float alpha);



int			get_small_objects_type_num();
int			get_small_objects_num();
small_object&	get_small_object(int idx);

void		update_small_objects_position(const double& t);
int			get_small_object_type(int idx);
bool		is_small_object_selected(int idx);
void		get_small_object_size(int idx, double& Rx, double& Ry, double& Rz);
void		setup_drawing_small_object(int idx);

void		register_small_objects_orbit_curves();
void		make_small_objects_orbit_curve(space_curve& sc, int div_N);
float		get_small_object_label_alpha(const mtk_draw_struct& ds, int idx);
float		get_small_object_orbit_alpha(const mtk_draw_struct& ds, int idx);
double		calc_visible_radius_small_object(int idx);

//-- Text size --
float	get_dwarf_planet_name_size();
float	get_small_object_name_size();
void	set_dwarf_planet_name_size(float sz);
void	set_small_object_name_size(float sz);


//--  Hit-test  --
bool	hit_test_small_object(const mtk_draw_struct& ds, int idx, const vector3d& rpos, const vector3d& nv);


//--  Info  --
void	make_object_info_small_object(const mtk_draw_struct& ds, int idx, std::vector<std::string>& text);





#endif
