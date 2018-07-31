/**************************************************************************************************

Copyright (c) 2012   Tsunehiko Kato
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
#include  "mitaka.h"
#include  "mtk_VERA.h"
#include  "mtk_objects.h"
#include  "mtk_our_galaxy_rt.h"
#include  "mtk_approx_func.h"
#include  "mtk_mode.h"
#include  "mtk_colors.h"
#include  "mtk_switches.h"
#include  "mtk_calc_alpha.h"
#include  "mtk_label.h"
#include  "mtk_solar_system.h"
#include  "../common/astro/astrometry.h"
#include  "../common/graphics/color.h"
#include  "mtk_strings.h"
#include  "mtk_make_info.h"
#include  <vector>
#include  "mtk_data_file.h"
#include  "mtk_selection.h"
using namespace std;




struct VERA_OBJ
{
	string		key;
	vector3f	xyz;		// �������W
	int			type;
	bool		bSelected;
};


static vector<VERA_OBJ>	vVERA;

const int VERA_DATA_NUM = 4;
const float scl_min = 10;
const float scl_max = 1000;
const float scl_out = 500000;
const float scl_out_label = 100000;
const float size_fac = 0.0035;




//--------
//  Load
//--------
int
read_one_line_VERA(FILE *fp, VERA_OBJ& obj)
{
	key_data  kd;


	kd = read_a_key_data(fp);

	// EOF
	if (kd.key.size() == 0) {
		return -1;
	}

	if (kd.data.size() != VERA_DATA_NUM) {
		// Error
		return 0;
	}


	//-- Key
	obj.key = kd.key;


	//-- Position
	float l, b, d;
	double R, rho;
	vector3f xyz;
	int res;

	res = sscanf(kd.data[0].c_str(), "%e", &l);
	if (res != 1) return 0;
	res = sscanf(kd.data[1].c_str(), "%e", &b);
	if (res != 1) return 0;
	res = sscanf(kd.data[2].c_str(), "%e", &d);
	if (res != 1) return 0;

	R = d * 1000.0;
	rho = R*cos(D2R*b);
	xyz.x = rho*cos(D2R*l);
	xyz.y = rho*sin(D2R*l);
	xyz.z = R*sin(D2R*b);
	obj.xyz = M_G2E * xyz;		// ���S�������W�֕ϊ�



	//-- Type
	const char *ty = kd.data[3].c_str();
	if (stricmp(ty, "SFR") == 0) {
		obj.type = VERA_TYPE_SFR;
	}
	else if (stricmp(ty, "AGB") == 0) {
		obj.type = VERA_TYPE_AGB;
	}
	else {
		obj.type = VERA_TYPE_UNKNOWN;
	}


	//-- Selection
	obj.bSelected = is_selected(obj.key.c_str());



	return 1;
}


void
set_VERA_selection()
{
	for (int i = 0; i < vVERA.size(); i++) {
		VERA_OBJ& obj = vVERA[i];
		obj.bSelected = is_selected(obj.key.c_str());
	}
}




//----------------
//  VERA object
//----------------
bool
load_VERA_objects(const directory& dir)
{
	vVERA.clear();

	FILE* fp = dir.fopen("VERA.dat", "rt");
	if (fp == NULL)  return false;

	int count = 0;
	while (true) {
		VERA_OBJ obj;
		char key[256];
		int res;


		//---  Read one data
		res = read_one_line_VERA(fp, obj);
		if (res == -1) break;		// �t�@�C���I�[
		if (res == 0) continue;		// �����ȍs
		vVERA.push_back(obj);
		count++;


		//---  Register object
		int idx = vVERA.size() - 1;
		int obj_idx = register_object( mtk_object(OBJ_TYPE_VERA_OBJECT, idx, 0.0f, true), obj.key.c_str());

		
		//---  Register label
		add_label(obj.key, obj_idx, MTK_COLOR_NAME_OBJECT, 1.0f, get_object_name_size(), 0.0, 10*D2R, true);
	}
	fclose(fp);

	return true;
}



//----------------
//  Prepare draw
//----------------
bool
prepare_draw_VERA_object(const mtk_draw_struct& ds, int idx, double* pRb, double* pRh)
{
	if ( !(swGalaxy & GLXY_VERA) )  return false;
	if (ds.scale < scl_min)  return false;

	if (calc_alpha_by_disapear_scale_outside(ds.scale, scl_out) < 0.01) return false;


	double R = norm(vVERA[idx].xyz - ds.camera_base.get_pos());
	double RR = R * size_fac;

	*pRb = RR;
	*pRh = RR;

	return  true;
}



//--------
//  Draw
//--------
void
draw_a_VERA_object(const mtk_draw_struct& ds, int idx, const vector3d& rpos, const double& scl, float alpha)
{
	const vector3f vCol[3] = {
		vector3f(0.3, 1.0, 0.5),	// SFR
		vector3f(1.0, 0.5, 0.0),	// AGB
		vector3f(0.3, 0.5, 1.0)		// Unknown
	};

	const VERA_OBJ& obj = vVERA[idx];
	

	//-- alpha
	float af;

	//�^�[�Q�b�g�ɂȂ��Ă��鎞
	const mtk_object& tgt = get_object(ds.tgt_ID);
	if (tgt.get_type() == OBJ_TYPE_VERA_OBJECT &&  tgt.get_index() == idx) {
		af = 1.0f;
	}
	else {

		const double lc = ds.scale;
		if (lc < scl_min) return;

		if (lc < scl_max) {
			af = (lc - scl_min) / (scl_max - scl_min);
		}
		else {
			af = calc_alpha_by_disapear_scale_outside(ds.scale, scl_out);
		}
	}

	af *= alpha;
	if (af < 0.01f)  return;

	
	//-- optical depth
	/*
	RGBf col(1, 0, 0);
	const vector3d camera_pos = ds.camera.get_pos();
	float R = 10;
	float tauR = calc_galactic_optical_depth(camera_pos, obj.xyz, 3);
	if (tauR > 3)  return;

	float _r, _g, _b;
	_r = col.r * exp_d(tauR);
	_g = col.g * exp_d(kpG_kpR*tauR);
	_b = col.b * exp_d(kpB_kpR*tauR);
	*/



	glEnable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);



	const float c = af * 0.8f;
	vector3f col;
	col = c * vCol[obj.type];
	glColor3fv(col.get_pointer());
	

	const float fac = scl;



	//---  �J�������W�n�̎�
	vector3d  eF, eU, eR;
	ds.camera.get_axes(eF, eU, eR);
	const vector3f eUf = vector3f(eU);
	


	glPushMatrix();

		if (true) {
	
			// QUAD
			glBegin(GL_QUADS);
			{
				vector3f  rr = rpos;

				//---  �l�p���W�n�̎�
				vector3f  eF2, eU2, eR2;
				float R2, R;
				R2 = norm2(rr);
				
				
				if (R2 > 1e-100) {
					R = sqrt(R2);
					eF2 = rr / R;
					eR2 = normalize( cross_product(eF2, eUf) );
					eU2 = cross_product( eR2, eF2 );

					float sz;
					sz = size_fac * R;

					vector3f  r;
					r = rr + sz*eR2;
					glVertex3fv( r.get_pointer() );
					r = rr + sz*eU2;
					glVertex3fv( r.get_pointer() );
					r = rr - sz*eR2;
					glVertex3fv( r.get_pointer() );
					r = rr - sz*eU2;
					glVertex3fv( r.get_pointer() );
				}
			}
			glEnd();
		} else {
			glBegin(GL_POINTS);
				glVertex3f(0,0,0);
			glEnd();
		}
	glPopMatrix();
}




//-------
//  Pos
//-------
vector3d
get_VERA_object_position(int idx)
{
	return  vVERA[idx].xyz;
}



//---------
//  Scale
//---------
double
get_scale_VERA_object(int idx)
{
	const VERA_OBJ& obj = vVERA[idx];
	return  100.0;	//****
}



//---------
//  Label
//---------
bool
is_VERA_label_visible(const mtk_draw_struct& ds, int idx, bool bTgt)
{
	if (!(swGalaxy & GLXY_VERA))  return false;
	if (!(swStar & STAR_NAME))  return false;


	//�^�[�Q�b�g�ɂȂ��Ă��鎞
	if (bTgt) return true;
	if ((swStar & STAR_NAME_SELECT) && !vVERA[idx].bSelected)  return  false;

	if (ds.scale < scl_min) return false;
	if (calc_alpha_by_disapear_scale_outside(ds.scale, scl_out_label) < 0.01) return false;

	return  true;
}


float
get_VERA_label_alpha(const mtk_draw_struct& ds, int idx)
{
	//�^�[�Q�b�g�ɂȂ��Ă��鎞
	const mtk_object& tgt = get_object(ds.tgt_ID);
	if (tgt.get_type() == OBJ_TYPE_VERA_OBJECT &&  tgt.get_index() == idx) {
		return 1.0f;
	}

	const double lc = ds.scale;
	if (lc < scl_min) return 0.0;
	if (lc < scl_max) {
		return  (lc - scl_min) / (scl_max - scl_min);
	}


	return calc_alpha_by_disapear_scale_outside(ds.scale, scl_out_label);
}




//--------
//  Info
//--------
void
make_object_info_VERA_object(const mtk_draw_struct& ds, int idx, std::vector<std::string>& text)
{
	const char szType[][50] = {"SFR", "AGB", "UNKNOWN"};
	const VERA_OBJ&  obj = vVERA[idx];
	static char str[256];

	float ly = _1pc_ly * norm(obj.xyz);


	//---  Name  ---
	text.push_back(get_string( vVERA[idx].key.c_str() ));

	//---  Type  ---
	sprintf(str, "INFO_TYPE_VERA_TYPE_%s", szType[obj.type]);
	add_info_line(text, "INFO_TYPE", str);

	//---  Observation  ---
	add_info_line(text, "INFO_OBSERVATION", "INFO_OBS_VERA");
}



