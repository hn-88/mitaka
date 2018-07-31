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
#include  "mtk_fixed_object.h"
#include  "mtk_object.h"
#include  "mtk_calc_alpha.h"
#include  "mtk_switches.h"
#include  "mtk_our_galaxy.h"
#include  "../common/astro/astro_const.h"
#include  <vector>


struct fixed_object
{
	vector3d  pos;
	int  obj_type;
};

static  std::vector<fixed_object>	vFixedObjects;




//--------
//  Init
//--------
int
add_fixed_object(const vector3d& pos, int obj_type)
{
	fixed_object  fo;
	fo.pos = pos;
	fo.obj_type = obj_type;


	vFixedObjects.push_back( fo );
	return  vFixedObjects.size() - 1;
}


//------------
//  Position
//------------
vector3d
get_fixed_object_position(int idx)
{
	return  vFixedObjects[idx].pos;
}



//---------
//  Label
//---------
bool
is_fixed_object_label_visible(const mtk_draw_struct& ds, int idx)
{
	switch  ( vFixedObjects[idx].obj_type ) {
		case OBJ_TYPE_SMALL_OBJECT:
			return  (test_disp_switch(DISPSW_MINOR_PLANET, DISPM_DISP) && test_disp_switch(DISPSW_MINOR_PLANET, DISPM_NAME));
		case OBJ_TYPE_OUR_GALAXY:
			return  is_our_galaxy_label_visible(ds);
		default:	break;
	}

	return  true;
}


float
get_fixed_object_label_alpha(const mtk_draw_struct& ds, int idx)
{
	float alp = 1.0f;
	switch  ( vFixedObjects[idx].obj_type ) {
		case OBJ_TYPE_SMALL_OBJECT:
			alp =  calc_alpha_by_disapear_scale_inside( ds.scale, 2.3*_1AU_pc );
			break;
		case OBJ_TYPE_OUR_GALAXY:
			return  get_our_galaxy_label_alpha(ds);
		default:
			break;
	}

	const vector3d& pos  = vFixedObjects[idx].pos;
	const vector3d  rpos = pos - ds.camera.get_pos();
	float  Rv  = pos.norm();
	float  Dth = 2.0 * Rv / rpos.norm();

	return  alp * calc_alpha_by_apparent_diameter( Dth );
}



//--------
//  Type
//--------
int
get_fixed_object_type(int idx)
{
	return  vFixedObjects[idx].obj_type;
}



