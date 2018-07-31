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
#include  <windows.h>
#include  "mtk_label.h"
#include  "mtk_mode.h"
#include  <vector>
#include  <string>
#include  "mtk_gl_common.h"
#include  "mtk_draw_struct.h"
#include  "mtk_font.h"
#include  "mtk_colors.h"
#include  "mtk_objects.h"
#include  "mtk_object_position.h"
#include  "mtk_planet_position.h"
#include  "mtk_solar_system.h"
#include  "mtk_sun.h"
#include  "mtk_planets.h"
#include  "mtk_minor_planets.h"
#include  "mtk_fixed_object.h"
#include  "mtk_stars_2.h"
#include  "mtk_switches.h"
#include  "mtk_planetarium.h"
#include  "mtk_strings.h"




struct  mtk_label {
	std::string		string_key;	// key for string

	int   ID;			// ���x���� ID
	int	  obj_ID;		// �e�I�u�W�F�N�g�̃C���f�b�N�X
	int   color_no;
	float alp;			// �����x
	float size;			// �����T�C�Y
	float R;			// ��V�̂���̃��x���̕\���ʒu�̔��a
	float offset_x;		// �I�t�Z�b�g
	float offset_y;
	float thc;			// �t�F�[�h�̊�ɂȂ鎋���a(rad) 
};
static  std::vector<mtk_label>  vLabels;





//------------
//  Register
//------------
int
add_label(const std::string& str_key, int obj_idx, int color_no, float alpha, float size, float R, float thc, bool bTargetable)
{

	mtk_label  lbl;
	lbl.obj_ID = obj_idx;
	lbl.color_no = color_no;
	lbl.alp = alpha;
	lbl.size = size;
	lbl.R = R;
	lbl.offset_x = 0.707;
	lbl.offset_y = 0.707;
	lbl.thc = thc;
	lbl.string_key = str_key;

	int label_idx;
	label_idx = vLabels.size();

	mtk_object  obj(OBJ_TYPE_LABEL, label_idx, 0.0f, bTargetable);

	std::string  label_key = "LABEL_" + str_key;
	int ID = register_object(obj, label_key.c_str());
	lbl.ID = ID;
	vLabels.push_back(lbl);


	return  ID;
}






vector3d
get_label_position(int idx)
{
	const mtk_label&  lbl = vLabels[idx];
	return  get_object_positon( get_object(lbl.obj_ID) );
}

float
get_label_letter_size(int idx)
{
	return  vLabels[idx].size;
}






//*****  ��肠��  unicode string *****
int
get_label_letter_count(int idx)
{
	return  strlen(get_string(vLabels[idx].string_key).c_str());
}

//*************************************





const std::string&
get_label_string_key(int idx)
{
	return  vLabels[idx].string_key;
}

const std::string&
get_label_text(int idx)
{
	return  get_string(vLabels[idx].string_key);
}


int
get_label_parent_object_index(int idx)
{
	return  vLabels[idx].obj_ID;
}


//------------
//  Hit-test
//------------
bool
hit_test_label(const mtk_draw_struct& ds, int idx, const vector3d& pos, const vector3d& cpos, const vector3d& nv)
{
	const mtk_label& label = vLabels[idx];
	const mtk_object& obj = get_object( label.obj_ID );



	//---  �J�������W�n�̎�
	vector3d  eF, eU, eR;
	ds.camera.get_axes(eF, eU, eR);


	//---  ���x�����W�n�̎�
	vector3d  rpos, eF2, eU2, eR2;
	double R2, R;
	rpos = pos - cpos;
	R2 = norm2(rpos);
	if (R2 < 1e-100)  return  false;
	
	R = sqrt(R2);
	eF2 = rpos / R;
	eR2 = normalize( cross_product(eF2, eU) );
	eU2 = cross_product( eR2, eF2 );


	//---  ���x�����W��̌�_�̈ʒu
	double dnm, t;
	dnm = dot_product(nv, eF2);
	if (fabs(dnm) < 1e-100)  return  false;
	t = dot_product(rpos, eF2) / dnm;

	if (t < 0.0)  return  false;

	vector3d  rxy;
	double fac;
	float  x, y;
	
	fac = R / get_screen_z() * get_screen_w() / 180 * label.size * ds.ltf;
	
	rxy = (nv*t - rpos) / fac;



	const int type = obj.get_type();
	/***** �g�厞�� hit-test �̂��߁A�ȉ����K�v *****/
	/*
	float fac2 = 1.0f;
	if (ds.tyMode == MODE_3D) {
		if (type == OBJ_TYPE_PLANET) {
			fac2 = get_zoom_rate().zmhPlanet;
		}
		if (type == OBJ_TYPE_SATELLITE && obj.get_index() == get_satellite_index("SAT_E1")) {
			fac2 = get_zoom_rate().zmhPlanet;
		}
		if (type == OBJ_TYPE_SUN) {
			fac2 = get_zoom_rate().zmhSun;
		}
	}
	*/



	float RR0;
	switch ( type ) {
		case OBJ_TYPE_STAR:				RR0 = 1.2*obj.get_R();	break;
		case OBJ_TYPE_SMALL_OBJECT:		RR0 = 1.4*obj.get_R();	break;
		default:						RR0 = label.R;
		//default:						RR0 = fac2*label.R;
	}
	

	float  RR = RR0 / fac;

	x = dot_product( rxy, eR2 ) - RR * label.offset_x;
	y = dot_product( rxy, eU2 ) - RR * label.offset_y;


	

	//**************
	if (type == OBJ_TYPE_SPACECRAFT) {
		y += 1.0;
	}





	//---  ������̃T�C�Y
	float w, h;
	if (type == OBJ_TYPE_STAR) {
		w = get_star_label_size(obj.get_index());
		h = 1;
	}
	else if (type == OBJ_TYPE_SUN) {
		w = get_sun_label_size(ds);
		h = 1;
	}
	else {
		get_text_size( get_string(label.string_key.c_str()), w, h );
	}



	//---  ����
	if ( x>0.0f  &&  x<w  && y>0.0f  &&  y<h )  return true;

	return  false;
}










//--------
//  Draw
//--------
bool
prepare_draw_label(const mtk_draw_struct& ds, int idx, double* pRb, double* pRh, float* pAlpha)
{
	const mtk_label& label = vLabels[idx];
	const mtk_object& obj = get_object( label.obj_ID );



	if ( !is_object_label_visible(ds, obj) )  return false;



	//--  �����x
	*pAlpha = label.alp * get_label_alpha( ds, obj );
	if (*pAlpha < 0.02f)  return false;



	//**********   ��    *******
	double L = norm( get_label_position(idx) - ds.camera_base.get_pos() );
	double  fac = ds.ltf * L / get_screen_z() * get_screen_w() / 180.0;


		// �����T�C�Y in pc
	if (obj.get_type() == OBJ_TYPE_STAR) {
		//--- ���̏ꍇ
		float sz = get_star_label_size( obj.get_index() );
		if (sz < 0.01)  return false;
		sz *= fac * get_label_letter_size(idx);
		*pRb = *pRh = sz;
	} else if (obj.get_type() == OBJ_TYPE_SUN ) {
		float sz = get_sun_label_size(ds);
		sz *= fac * get_label_letter_size(idx);
		*pRb = *pRh = sz;
	} else {
		float sz = fac * get_label_letter_size(idx);
		int len = get_label_letter_count(idx); 
		*pRb = *pRh = sz*len;
	}



	return  true;
}



void
draw_a_label(const mtk_draw_struct& ds, int idx, const vector3d& rpos, const double& scl, float alpha, float dz)
{
	static bool first = true;

	const mtk_label& label = vLabels[idx];
	int obj_ID = label.obj_ID;
	const mtk_object& obj0 = get_object(obj_ID);

	//***
	if ( obj0.get_type() == OBJ_TYPE_FIXED_OBJECT) {
		if ( get_fixed_object_type( obj0.get_index() ) == OBJ_TYPE_OUR_GALAXY ) {
			obj_ID = get_object_ID("OUR_GALAXY");
		}
	}
	
	const mtk_object& obj = get_object(obj_ID);


	static int idx_Moon;
	if (first) {
		first = false;
		idx_Moon = get_object_ID("SAT_E_MOON");
	}



	//************************
	//  �ȉ������ׂăR�[���o�b�N�ɂ���
	//***********************
	const int type = obj.get_type();
	float fac = 1.0f;
	if (ds.tyMode == MODE_3D) {
		if (type == OBJ_TYPE_PLANET) {
			fac = get_zoom_rate().zmhPlanet;
		}
		if (type == OBJ_TYPE_SATELLITE && obj.get_index()==idx_Moon) {
			fac = get_zoom_rate().zmhPlanet;
		}
		if (type == OBJ_TYPE_SUN) {
			fac = get_zoom_rate().zmhSun;
		}
	} else {
		if (type == OBJ_TYPE_PLANET) {
			fac = get_zoom_rate().zmpPlanet;
		}
		if (type == OBJ_TYPE_SATELLITE && obj.get_index() == idx_Moon) {
			fac = get_zoom_rate().zmpSunMoon;
		}
		if (type == OBJ_TYPE_SUN) {
			fac = get_zoom_rate().zmpSunMoon;
		}

		fac *= ds.plnt_R;	//***
	}



	float R;
	switch (type) {
		case OBJ_TYPE_STAR:				R = 1.2*scl*obj.get_R();	break;
		case OBJ_TYPE_S_STAR:			R = 0.6*scl*obj.get_R();	break;
		case OBJ_TYPE_SMALL_OBJECT:		R = 1.4*scl*obj.get_R();	break;
		default:
			R = fac*scl*label.R;
			break;
	}

	float dx, dy;
	dx = dy = 0;
	if (label.R == 0.0) {
		dx = dy = 0.1;	// �������炷
	}



	//**************
	if (type == OBJ_TYPE_SPACECRAFT) {
		dy -= 1.0;
	}



	int sel_ID = get_selected_object_ID();
	RGBf  col;
	if (sel_ID != obj_ID && sel_ID != label.ID) {
		col = get_color( label.color_no );
	} else {
		col.set(0, 1.0, 0.1);
	}




	//--  �`��
	glPushMatrix();
		glTranslatef(rpos.x, rpos.y, rpos.z);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);

		prepare_3Dtext_1(R, 45, label.size*ds.ltf, ds.scale_factor, false);

		glTranslatef(dx, dy, dz);

		switch (type) {
			case OBJ_TYPE_SUN:
				draw_sun_label(ds, col, alpha);
				break;
			case OBJ_TYPE_STAR:
				{
					glColor4f(col.r, col.g, col.b, alpha);
					draw_a_star_label(ds, obj.get_index());
				}
				break;
			default:
				{
					glColor4f(col.r, col.g, col.b, alpha);
					draw_text( get_string(label.string_key) );
				}
				break;
		}

	glPopMatrix();
}


