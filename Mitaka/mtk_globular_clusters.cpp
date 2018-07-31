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
#include  <stdio.h>
#include  "mtk_globular_clusters.h"
#include  "../common/math/random.h"
#include  "../common/graphics/color.h"
#include  "../common/astro/astrometry.h"
#include  "mtk_gl_common.h"
#include  "mtk_font.h"
#include  "mtk_text.h"
#include  "mtk_switches.h"
#include  "mtk_solar_system.h"
#include  "mtk_globular_clusters.h"
#include  "mtk_approx_func.h"
#include  "mtk_our_galaxy_rt.h"
#include  "mtk_colors.h"
#include  "mtk_objects.h"
#include  "mtk_target.h"
#include  "mtk_label.h"
#include  "mtk_GC_model.h"
#include  "mtk_calc_alpha.h"
#include  "mtk_draw_common.h"
#include  "mtk_data_file.h"
#include  "mtk_strings.h"
#include  "mtk_make_info.h"




//-------------------------
//  Globular Cluster data
//-------------------------

struct globular_cluster
{
	std::string  key;		// key
	std::string  cat_name;
	int cat_no;

	RA_DEC	eqPos;			// �Ԍo�E�Ԉ�(deg)
	float	distance;		// ����(pc)
	float	Rc, Rh;			// �R�A���a�Ɣ����ʔ��a(pc)
	float	B_V, V_R;		// color index

	vector3f	xyz;		// �������W
	RGBf		RGB;		// �F
	int			draw_type;
};



static vector<globular_cluster>	vGlbc;	// ���󐯒c�̈ʒu
static vector<vector3f>			GCPT;	// ���󐯒c�̓_
const int GCPT_N = 500;



//-------------
//  Draw type
//-------------
enum {
	GC_DRAW_TYPE_NORMAL,
	GC_DRAW_TYPE_M13
};



//------------------
//  Display scales
//------------------
const float GC_scl_min = 1000;
const float GC_scl_max = 12500;





//---------------
//  Load / Init
//---------------
bool
load_globular_cluster_data(const directory& dir)
{
	FILE* fp;
	fp = dir.fopen("globular_clusters.dat", "rt");
	if (fp==NULL)  return false;


	while (true) {
		key_data kd;
		globular_cluster gc;
		

		//--- read data
		kd = read_a_key_data(fp);
		if (kd.key == "") {
			break;
		}

		if (kd.data.size() != 9) {
			continue;
		}
		
		gc.key = kd.key;
		gc.cat_name = kd.data[0];
		if (gc.cat_name != "*") {
			gc.cat_no = stoi(kd.data[1]);
		}
		else {
			gc.cat_no = 0;
		}
		gc.eqPos.RAdeg = stof(kd.data[2]);
		gc.eqPos.DEdeg = stof(kd.data[3]);
		gc.distance = stof(kd.data[4]);
		gc.Rc = stof(kd.data[5]);
		gc.Rh = stof(kd.data[6]);
		gc.B_V = stof(kd.data[7]);
		gc.V_R = stof(kd.data[8]);


		
		//--- �V���ʏ�̈ʒu�i�P�ʔ��a�j
		vector3f Cxyz = gc.eqPos.get_dirc_vec();

		//--- ���S�������W(pc)
		gc.xyz  = M_R2E * ( gc.distance * Cxyz );

		//--- �F
		RGBf rgb;
		double  T = B_V_to_Tc(gc.B_V);
		rgb = correct_RGB( blackbody_color(T) );
		float RGB_max = max(rgb.r, max(rgb.g, rgb.b));
		gc.RGB = rgb / RGB_max;


		//---  Draw type
		int dtype = GC_DRAW_TYPE_NORMAL;
		if (gc.key == "GC_NGC_6205") {	// M13
			dtype = GC_DRAW_TYPE_M13;
		}
		gc.draw_type = dtype;


		vGlbc.push_back(gc);



		//---  Register object
		int idx = vGlbc.size() - 1;
		int obj_idx = register_object(mtk_object(OBJ_TYPE_GLOBULAR_CLUSTER, idx, 0.0f, true), gc.key);


		//---  Register label
		if (is_defined_key(gc.key)) {
			std::string label_key;
			label_key = "LABEL_" + gc.key;
			add_label(gc.key, obj_idx, MTK_COLOR_NAME_OBJECT, 1.0f, get_object_name_size(), 5 * gc.Rh, 10 * D2R, true);
		}

	}

	return  true;
}


void
init_globular_cluster_points()
{
	for (int i=0; i<GCPT_N; ++i) {
		float phi = rndUni(0,2*pi);
		float mu  = rndUni(-1,1);
		float x,y,z,rho, R;

		R = rndGauss(0, 1);

		z   = R*mu;
		rho = R*sqrt(1-mu*mu);
		x   = rho*cos(phi);
		y   = rho*sin(phi);
		
		GCPT.push_back(vector3f(x,y,z));
	}
}



//-------
//  Pos
//-------
vector3d
get_globular_cluster_position(int idx)
{
	return  vGlbc[idx].xyz;
}



//---------
//  Scale
//---------
double
get_scale_globular_cluster(int idx)
{
	const globular_cluster& gc = vGlbc[idx];
	return  30.0 * gc.Rh;
}




//---------
//  Label
//---------
const double gc_lc1 = 1000;
const double gc_lc2 = 3000;

bool
is_globular_cluster_label_visible(const mtk_draw_struct& ds, int idx, bool bTgt)
{
	if ( !(swGalaxy & GLXY_GLOB_CLSTR && test_disp_switch(DISPSW_PLANET, DISPM_NAME)) )  return false;
	if (bTgt) return true;
	if ( ds.cnorm < gc_lc1 )  return  false;

	return  true;
}


float
get_globular_cluster_label_alpha(const mtk_draw_struct& ds, int idx)
{
	const double lc = ds.scale;
	if (lc < gc_lc2) {
		return  (lc - gc_lc1) / (gc_lc2 - gc_lc1);
	}

	const vector3d   pos = get_globular_cluster_position(idx);
	const vector3d  cpos = ds.camera.get_pos();
	const vector3d  rpos = pos - cpos;
	const vector3d  gpos = pos - get_our_galaxy_position();
	float  Rv  = 0.5f * gpos.norm();
	float  Dth = 2.0f * Rv / rpos.norm();
	return  calc_alpha_by_apparent_diameter( Dth );
}




//------------------
//  Landing radius
//------------------
double
get_landing_radius_globular_cluster(int idx)
{
	return vGlbc[idx].Rh;
}




//----------------
//  Prepare draw
//----------------
bool
prepare_draw_globular_cluster(const mtk_draw_struct& ds, int idx, double* pRb, double* pRh)
{
	if ( !(swGalaxy & GLXY_GLOB_CLSTR) )  return false;

	
	bool bTgt = (get_target().get_type() == OBJ_TYPE_GLOBULAR_CLUSTER && get_target().get_index() == idx);
	if (!bTgt && ds.scale < GC_scl_min)  return false;

	const globular_cluster& gc = vGlbc[idx];
	const float rate  = 5.0;				// Rh �̉��{�����󐯒c�̑傫���ƌ��Ȃ���

	*pRb = rate * gc.Rh;
	*pRh = gc.Rh;

	return  true;
}



//--------
//  Draw
//--------
void
draw_a_globular_cluster(const mtk_draw_struct& ds, int idx, const vector3d& rpos, const double& scl)
{
	const globular_cluster& gc = vGlbc[idx];
	float af;

	bool bTgt = (get_target().get_type() == OBJ_TYPE_GLOBULAR_CLUSTER && get_target().get_index() == idx);

	if (bTgt) {
		af = 1.0f;
	}
	else {
		const double scale = ds.scale;
		af = (scale > GC_scl_max) ? 1 : (scale - GC_scl_min) / (GC_scl_max - GC_scl_min);
		if (af < 0.01f)  return;
	}


	if (gc.draw_type == GC_DRAW_TYPE_M13) {
		draw_GC_model(ds, idx, rpos, scl, af);
		return;
	}


	const vector3d camera_pos = ds.camera.get_pos();


	//-- optical depth
	float tauR = calc_galactic_optical_depth(camera_pos, gc.xyz, 3);
	if (tauR > 3)  return;

	float _r, _g, _b;
	_r = gc.RGB.r * exp_d(tauR);
	_g = gc.RGB.g * exp_d(kpG_kpR*tauR);
	_b = gc.RGB.b * exp_d(kpB_kpR*tauR);



	glEnable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);


	const float c = af * 0.8f;
	glColor3f(c*_r, c*_g, c*_b);

	const float fac = scl * gc.Rh;



	//---  �J�������W�n�̎�
	const float a = 0.0018;



	glPushMatrix();

		if (true) {
			for (int i=0; i<GCPT.size(); i++) {
				vector3f  rr = fac * GCPT[i] + rpos;

				float R2, R;
				R2 = norm2(rr);
				if (R2 < 1e-100)  continue;
				R = sqrt(R2);

				draw_point(ds, a, R, rr, ds.eRf, ds.eUf);
			}
		} else {
			glBegin(GL_POINTS);
				for (int j=0; j<GCPT.size(); j++) {
					vector3f xyz = fac * GCPT[j];
					glVertex3fv(xyz.get_pointer());
				}
			glEnd();
		}
	glPopMatrix();
}



//--------
//  Info
//--------
void
make_object_info_globular_cluster(const mtk_draw_struct& ds, int idx, std::vector<std::string>& text)
{
	const globular_cluster&  gc = vGlbc[idx];

	// name
	if (is_defined_key(gc.key)) {
		text.push_back(get_string(gc.key));
	}

	// catalog name
	if (gc.cat_name != "*") {
		text.push_back(gc.cat_name + " " + to_string(gc.cat_no));
	}

	if (text.size() == 0) {
		text.push_back(get_string("INFO_TYPE_GLOBULAR_CLUSTER"));
	}

	add_info_line(text, "INFO_TYPE", "INFO_TYPE_GLOBULAR_CLUSTER");
}





//------
//  Rh
//------
double
get_globular_cluster_Rh(int idx)
{
	return  vGlbc[idx].Rh;
}
