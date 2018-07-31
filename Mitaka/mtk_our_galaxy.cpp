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
#include  "mtk_our_galaxy.h"
#include  "../common/astro/astrometry.h"
#include  "mtk_objects.h"
#include  "mtk_label.h"
#include  "mtk_solar_system.h"
#include  "mtk_switches.h"
#include  "mtk_colors.h"
#include  "mtk_our_galaxy_rt.h"
#include  "mtk_fixed_object.h"
#include  "mtk_strings.h"
#include  "mtk_make_info.h"





static vector3d  galactic_center_pos;
static vector3d  our_galaxy_label_pos;


const float label_scl_min = 4000;
const float label_scl_max = 50000;




//------------
//  Register
//------------
void
register_object_our_galaxy()
{
	mtk_object  obj;

	//---  Register object
	obj.init(OBJ_TYPE_OUR_GALAXY, 0, 0.0f, true);
	register_object(obj, "OUR_GALAXY");
	
	//---  Init
	galactic_center_pos = M_G2E * vector3d(R_gal, 0, 0);
	our_galaxy_label_pos = M_G2E * vector3d(2*R_gal, R_gal, 0);


	//---  Fixed object for label
	int fixed_obj_idx = add_fixed_object( our_galaxy_label_pos, OBJ_TYPE_OUR_GALAXY );
	obj.init(OBJ_TYPE_FIXED_OBJECT, fixed_obj_idx, 0.0f, false);
	int obj_idx = register_object(obj, "LABEL_OUR_GALAXY");


	//---  Register label
	float thc = 10 * D2R;	//***

	add_label("LABEL_OUR_GALAXY", obj_idx,
		MTK_COLOR_NAME_SPECIAL_OBJECT, 1.0f, get_special_object_name_size(), 0, thc, true);
}


//------------
//  Position
//------------
vector3d
get_our_galaxy_position()
{
	return  galactic_center_pos;
}


//---------
//  Scale
//---------
double
get_scale_our_galaxy()
{
	return  1e5 * _1AU_pc;
}



//---------
//  Label
//---------


bool
is_our_galaxy_label_visible(const mtk_draw_struct& ds)
{
	if ( !(swGalaxy & GLXY_OUR_GALAXY) )  return false;
	if ( !test_disp_switch(DISPSW_PLANET, DISPM_NAME) )  return  false;

	if (ds.scale < label_scl_min)  return  false;

	return  true;
}


float
get_our_galaxy_label_alpha(const mtk_draw_struct& ds)
{
	const float scale = ds.scale;

	float af = (scale > label_scl_max)  ?  1  :  (scale - label_scl_min) / (label_scl_max - label_scl_min);
	const float scl1 = 1e8;
	const float scl2 = 1e9;
	if (scale > scl1) {
		af *= 1 - (scale - scl1)/(scl2 - scl1);
	}
	if (af < 0.01)  return  0.0f;

	return af;
}




//------------------
//  Landing radius
//------------------
double
get_landing_radius_our_galaxy(int idx)
{
	return  0.1 * _1AU_pc;
}



//----------------
//  Prepare draw
//----------------
bool
prepare_draw_our_galaxy(const mtk_draw_struct& ds, double* pRb, double* pRh)
{
	if ( !(swGalaxy & GLXY_OUR_GALAXY) )  return false;


	*pRb = 2.2*R_gal;
	*pRh = R_gal;

	return  true;
}



//--------
//  Draw
//--------
void
draw_our_galaxy(const mtk_draw_struct& ds, int idx, const vector3d& rpos, const double& scl)
{
	draw_our_galaxy_model_rt(ds, rpos);
}




//------------
//  Hit-test
//------------
bool
hit_test_our_galaxy(const mtk_draw_struct& ds, const vector3d& rpos, const vector3d& nv)
{
	const double l_min2 = 2 * R_gal*R_gal;
	double l2 = rpos.norm2();
	if (l2 < l_min2)  return  false;

	// ��͍��W�n�ł̒l
	vector3d  _rpos, _nv;
	_rpos = M_E2G * rpos;
	_nv = M_E2G * nv;

	// �G����
	double f = 0.2;
	_rpos.z /= f;
	_nv.z /= f;
	_nv = normalize(_nv);

	// ��_
	double R, b, c, D, t;
	R = R_gal;
	b = dot_product(_rpos, _nv);
	c = _rpos.norm2() - R*R;
	D = b*b - c;

	if (D <= 0.0)  return false;
	t = -b -sqrt(D);
	if (t < 0.0)  return  false;

	return  true;
}




//--------
//  Info
//--------
void
make_object_info_our_galaxy(const mtk_draw_struct& ds, std::vector<std::string>& text)
{
	//---  Name  ---
	text.push_back(get_string("OUR_GALAXY"));

	//---  Type  ---
	add_info_line(text, "INFO_TYPE", "INFO_TYPE_GALAXY");


	//---  Galaxy type  ---
	std::string str;
	str = get_string("INFO_GALAXY_TYPE_SB") + " (SBbc)";


	add_info_line_str(text, "INFO_GALAXY_TYPE", str.c_str());
}

