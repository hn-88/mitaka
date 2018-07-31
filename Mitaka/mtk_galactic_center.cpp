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
#include  "mtk_galactic_center.h"
#include  "mtk_orbital_element.h"
#include  "mtk_our_galaxy.h"
#include  "mtk_draw_common.h"
#include  "mtk_object.h"
#include  "mtk_objects.h"
#include  "mtk_stars.h"
#include  "mtk_stars_2.h"
#include  "mtk_label.h"
#include  "mtk_target.h"
#include  "mtk_switches.h"
#include  "mtk_mode.h"
#include  "mtk_colors.h"
#include  "mtk_solar_system.h"
#include  "mtk_calc_alpha.h"
#include  "mtk_dialog_box.h"
#include  "mtk_our_galaxy_rt.h"
#include  "mtk_OpenGL.h"
#include  "mtk_strings.h"
#include  "mtk_make_info.h"
#include  "../common/astro/astro_const.h"
#include  "../common/astro/astrometry.h"
#include  <vector>

#include  "mtk_data_file.h"
#include  "mtk_selection.h"




//***
extern GLuint  texStar;
extern fast_pow  pow_for_stars;
extern float  StarImagePowIndex;
//***



static GLuint texGalCenter;
const char *fn_tex_GalCenter = "galactic_center_bg.png";
static double Lv0_ss;





//-----------
//  Texture
//-----------
void
load_galactic_center_texture()
{
	SetSplashText("LOADING_GALAXTIC_CENTER_BG_TEXTURE");
	texGalCenter = LoadTexture(fn_tex_GalCenter, TexShrinkRate, false);
}



//-----------------------
//  ��͌n���S�̔w�i��`��
//-----------------------
void
draw_galactic_center_background(const mtk_draw_struct& ds)
{
	if (!bGalCenterBG)  return;


	float af, Rmax;
	af = 1.0f;
	if (ds.tyMode == MODE_3D) {
		const float dist = norm(get_our_galaxy_position() - ds.camera_base.get_pos());

		const float dist1 = 2000;
		const float dist2 = 3000;
		if (dist > dist1) {
			float t = (dist - dist1) / (dist2 - dist1);
			af = 1 - t;
		}

		if (af <= 0.0) return;
		Rmax = get_Rmax();
	}

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, texGalCenter);


	af *= 0.7;

	glColor4f(1, 1, 1, af);
	glPushMatrix();
	go_to_galactic_coord();
	glScalef(Rmax, Rmax, Rmax);
	glRotatef(180, 0, 0, 1);
	glFrontFace(GL_CW);
	gl_draw_sphere(10, true);
	glFrontFace(GL_CCW);
	glPopMatrix();

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}



struct S_star {
	string key;
	orbital_elements oe;
	char	SpcType;
	bool	bSelected;
	int		TempIdx;
};

static std::vector<S_star>  vS_stars;



static matrix3d M_C2E;
static float L00;



//--------
//  Load
//--------
const int S_STAR_DATA_NUM = 9;

int
load_a_S_star(FILE *fp, S_star& ss)
{
	const double arcsec_to_pc = 1.0 / (60.0*60.0)*M_PI / 180.0 * R_gal;
	key_data kd;


	kd = read_a_key_data(fp);
	if (kd.key.size()==0) {
		return -1;
	}
	if (kd.data.size() != S_STAR_DATA_NUM) {
		// Error
		0;
	}


	// key
	ss.key = kd.key;


	// �O���v�f�̏�����
	double a, e, inc, Omg, omg, tP, T, temp;
	double epoch, M0, n;
	char spc_type;


	a = stod(kd.data[0]);
	e = stod(kd.data[1]);
	inc = stod(kd.data[2]);
	Omg = stod(kd.data[3]);
	omg = stod(kd.data[4]);
	tP = stod(kd.data[5]);
	T = stod(kd.data[6]);
	spc_type = kd.data[7][0];
	temp = stod(kd.data[8]);
		
	a = a * arcsec_to_pc;			// pc�P��
	T = T * 365.25;					// ����P�ʂƂ�������

	epoch = tP * 365.25 + J2000;	// �ߓ_�ʉߎ��������Ƃ���
	M0 = 0.0;
	n = 360.0 / T;					// �x�ŕ\�������ω^��

	ss.oe.set_parameters(epoch, a, e, M0, n, Omg, inc, omg);


	// �I��
	ss.bSelected = is_selected(ss.key.c_str());

	// �X�y�N�g���^
	ss.SpcType = spc_type;

	// ���̐F�i�e�N�X�`���ԍ��j
	if (temp <= 0) {
		switch (ss.SpcType) {
		case 'O':	temp = 34000;	break;
		case 'B':	temp = 20000;	break;
		case 'K':	temp = 4000;	break;
		case 'E':	temp = 15000;	break;	// Early type
		case 'L':	temp = 4400;	break;	// Late type
		default:	temp = 2000;	break;
		}
	}
	ss.TempIdx = get_star_picture().get_TempIdx(temp);

	return 1;
}

bool
load_S_stars(const directory& dir)
{
	FILE *fp;


	// �V���ʍ��W���物�����W�n�ւ̕ϊ��s������߂�
	vector3d X, Y, Z;
	Z = normalize(-vGC);
	X = vector3d(0, 0, 1);	// �V�̖k��
	Y = normalize(cross_product(Z, X));
	X = normalize(cross_product(Y, Z));

	const matrix3d M_C2R(X, Y, Z);
	M_C2E = M_R2E * M_C2R;


	vS_stars.clear();

	fp = dir.fopen("S_stars.dat", "rt");
	if (fp == NULL)  return false;

	while (true) {
		int res;
		S_star ss;

		//---  Read one data
		res = load_a_S_star(fp, ss);
		if (res == -1) break;		// �t�@�C���I�[
		if (res == 0) continue;		// �����ȍs
		vS_stars.push_back(ss);


		//--  �I�u�W�F�N�g��o�^
		int idx = vS_stars.size();
		std::string& key = ss.key;

		int obj_ID;
		obj_ID = register_object(mtk_object(OBJ_TYPE_S_STAR, idx, 0.0f, true), key.c_str());



		//--  ���x����o�^
		//*******
		float R = 0;
		float thc = 8 * D2R;
		//*******
		add_label(key, obj_ID, MTK_COLOR_NAME_OBJECT, 1.0f, get_object_name_size(), R, thc, true);

		vS_stars.push_back(ss);


		//--  �O����o�^
		space_curve sc(mtk_object(OBJ_TYPE_S_STAR, idx, 0.0f, false), get_orbit_width(), MTK_COLOR_ORBIT_PLANET);
		register_space_curve(sc);
	}


	// ���邳�̊
	L00 = pow(80.0, 1.0 / StarImagePowIndex);

	return true;
}




bool
is_S_star_selected(int idx)
{
	return vS_stars[idx].bSelected;
}



//---------
//  Orbit
//---------
void
make_S_stars_orbit_curve(space_curve& sc, int div_N)
{
	const orbital_elements&  oe = vS_stars[sc.get_related_object().get_index()].oe;

	sc.clear();
	for (int j = 0; j<div_N; j++) {
		double phi = 2.0 * M_PI * double(j) / div_N;
		sc.add_point( M_C2E * oe.calc_orbit_pos(phi) + get_our_galaxy_position() );
	}
}




//------------
//  Position
//------------
void
update_S_stars_position(const double& t)
{
	for (std::vector<S_star>::iterator it = vS_stars.begin(); it != vS_stars.end(); it++) {
		it->oe.update_pos_and_velocity(t);
	}
}


vector3d
get_S_star_position(int idx)
{
	return M_C2E * vS_stars[idx].oe.get_pos() + get_our_galaxy_position();
}




//--------------
//  Visibility
//--------------
const double star_lc1 = 0.01 * _1AU_pc;
const double star_lc2 = 0.03 * _1AU_pc;


bool
is_S_star_label_visible(const mtk_draw_struct& ds, int idx, bool bTgt)
{
	if (!(swStar & STAR_DISP))  return false;
	if (!(swStar & STAR_NAME))  return false;
	if (bTgt) return true;
	if ((swStar & STAR_NAME_SELECT) && !vS_stars[idx].bSelected)  return false;


	const double lc = norm(ds.camera_base.get_pos() - get_our_galaxy_position());

	return  true;
}


float
get_S_star_label_alpha(const mtk_draw_struct& ds, int idx)
{
	const vector3d  cpos = ds.camera.get_pos();
	float  Dth = 7000.0f * _1AU_pc / norm(get_our_galaxy_position() - cpos);

	return  calc_alpha_by_apparent_diameter(Dth);
}




//---------
//  Alpha
//---------
float
get_S_star_alpha(const mtk_draw_struct& ds, int idx)
{
	bool bTgt = (get_target().get_type() == OBJ_TYPE_S_STAR && get_target().get_index() == idx);

	// ��͌n���S����̐��̈ʒu
	const vector3d  pos = M_C2E * vS_stars[idx].oe.get_pos();

	const float R1 = 0.01;
	const float R2 = 0.04;
	const float R3 = 1;
	const float R4 = 4;
	float R = norm(pos) / ds.scale;
	if (!bTgt && R > R4)  return  0.0;
	if (R < R1)  return  0.0;


	float af = 1.0;
	float sz = 1.5f / (ds.rad_per_pixel / 0.001);
	if (sz < 1) {
		af *= sz;
		sz = 1;
	}

	if (!bTgt && R > R3)	{ af *= 1 - (R - R3) / (R4 - R3); }
	if (R < R2)				{ af *= (R - R1) / (R2 - R1); }


	if (af < 0.05)  return 0.0;
	return  af;
}



float
get_S_star_object_orbit_alpha(const mtk_draw_struct& ds, int idx)
{
	const vector3d  cpos = ds.camera.get_pos();
	float  Dth = 20000.0f * _1AU_pc / norm(get_our_galaxy_position() - cpos);

	return  calc_alpha_by_apparent_diameter(Dth);
}


//---------
//  Scale
//---------
double
get_scale_S_star(int idx)
{
	return  200.0 * _1AU_pc;
}




//
//
//
void
prepare_draw_S_stars_2(const mtk_draw_struct &ds)
{
	float L0 = L00;

	switch (get_display_mode()) {
	case DISPMODE_SIDE_BY_SIDE:
	case DISPMODE_TOP_AND_BOTTOM:
		L0 *= 2.5;
		break;
	case DISPMODE_DOME_MASTER:
		L0 *= 4.0;
		break;
	default:
		break;
	}

	
	Lv0_ss = L0 / get_L_ref();
}




//----------------
//  Prepare draw
//----------------
double
get_star_radius_ss(const mtk_draw_struct& ds, const vector3d& rpos, const float& L)
{
	const float R1 = 1.0e-7;	// �P������ 1pc �ł̃C���[�W���a


	//  Radius
	double dist2 = norm2(rpos);
	float dist = sqrt(dist2);

	return pow_for_stars.eval(float(Lv0_ss*L / dist2)) * R1 * dist;
}


bool
prepare_draw_S_star(const mtk_draw_struct& ds, int idx, double* pRb, double* pRh)
{
	//---  Visible
	bool bFlg;
	bFlg = (swStar & STAR_DISP);

	if (!bFlg)  return false;
	

	//---  Radius
	vector3d rpos;
	double L;

	rpos = get_S_star_position(idx) - ds.camera_base.get_pos();
	L = Lv0_ss;

	*pRh = *pRb = get_star_radius_ss(ds, rpos, L);


	return  true;
}




//--------
//  Draw
//--------
void
draw_a_S_star(const mtk_draw_struct& ds, int idx, const vector3d& rpos, const double& scl, const double& Rb, float alp)
{
	double R2;
	R2 = norm2(rpos);
	if (R2 < 1e-100)  return;


	const float sz = 2.0 * scl * Rb;



	if (true) {
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glBindTexture(GL_TEXTURE_2D, texStar);
		glColor3f(1, 1, 1);

		draw_a_star_TempIdx_2(ds, rpos, sz, vS_stars[idx].TempIdx, 1);
	}
	else {
		glPointSize(sz);
		glBegin(GL_POINTS);
		glVertex3dv(rpos.get_pointer());
		glEnd();
	}
}




//--------
//  Info
//--------
void
make_object_info_S_star(const mtk_draw_struct& ds, int idx, std::vector<std::string>& text)
{
	const S_star&  ss = vS_stars[idx];


	//-- name
	text.push_back( get_string(ss.key.c_str()) );

	//-- type
	add_info_line(text, "INFO_TYPE", "INFO_TYPE_S_STAR");


	switch (ss.SpcType) {
	case 'E':
		add_info_line(text, "INFO_STAR_SPECTRAL_TYPE", "INFO_STAR_SPECTRAL_TYPE_EARLY");
		break;
	case 'L':
		add_info_line(text, "INFO_STAR_SPECTRAL_TYPE", "INFO_STAR_SPECTRAL_TYPE_LATE");
		break;
	case 'K':
		add_info_line_str(text, "INFO_STAR_SPECTRAL_TYPE", "K-M");
		break;
	default:
		add_info_line_char(text, "INFO_STAR_SPECTRAL_TYPE", ss.SpcType);
		break;
	}
}


