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
#include  "mtk_sun.h"
#include  "mtk_mode.h"
#include  "../common/astro/astrometry.h"
#include  "mtk_params.h"
#include  "mtk_gl_common.h"
#include  "mtk_OpenGL.h"
#include  "mtk_objects.h"
#include  "mtk_solar_system.h"
#include  "mtk_trajectory.h"
#include  "mtk_label.h"
#include  "mtk_switches.h"
#include  "mtk_planetarium.h"
#include  "mtk_dialog_box.h"
#include  "mtk_font.h"
#include  "mtk_strings.h"
#include  "mtk_make_info.h"
#include  "mtk_eclipse.h"
#include  "mtk_atmosphere.h"
#include  "mtk_planets.h"
#include  "mtk_planet_position.h"
#include  "mtk_planet_orientation.h"
#include  "mtk_satellites.h"




extern float  landing_lon, landing_lat;


//
static mtk_trajectory trajSun;		// Sun
static vector3d r_sun(0, 0, 0);		// ���z�̑��z�n�d�S���W�n�ł̈ʒu

const double sun_draw_scale_max = 5e5;




//--------------------
//  Color / Textures
//--------------------
const float  B_V_sun = 0.65;
const float  Tc_sun = B_V_to_Tc(B_V_sun);
const RGBf  col_sun( correct_RGB( blackbody_color(Tc_sun) ) );
static GLuint	texSun;

const char* szSunGlareColorDef = "#FFF2EE";
static RGBf colSunGlare;




double
get_sun_radius(int mode)
{
	if (mode == MODE_3D) {
		return R_sun_km * _1km_pc * get_zoom_rate().zmhSun;
	} else {
		return R_sun_km * _1km_pc * get_zoom_rate().zmpSunMoon;
	}
}

double
get_sun_glare_radius(int mode)
{
	return  get_sun_radius(mode) * get_fparam(MTK_FLOAT_SUN_GLARE_SIZE);
}





//------------
//  Register
//------------
void
register_object_sun()
{
	mtk_object  obj;

	//  register object
	obj.init(OBJ_TYPE_SUN, 0, 0.0f, true);
	int obj_idx = register_object(obj, "SUN");
	
	// register label
	float R   = 1.4 * _1km_pc * R_sun_km;
	float thc = 5 * D2R;	//**

	add_label( "SUN", obj_idx, MTK_COLOR_NAME_SPECIAL_OBJECT, 1.0f, get_special_object_name_size(), R, thc, true);
}




//-----------------
//  Load Textures
//-----------------
void
load_sun_textures()
{
	SetSplashText("LOADING_SUN_PLANET_TEXTURE");
	texSun = LoadTexture("sun.jpg", TexShrinkRate, false);
}



//------------------
//  Landing radius
//------------------
double
get_landing_radius_sun()
{
	return  get_sun_radius( MODE_3D );
}



//----------------
//  Prepare draw
//----------------
bool
prepare_draw_sun(const mtk_draw_struct& ds, double* pRb, double* pRh)
{
	if (ds.scale >= sun_draw_scale_max)  return false;

	*pRb = get_sun_glare_radius(ds.tyMode);
	*pRh = get_sun_radius(ds.tyMode);

	return  true;
}



//------------
//  Position
//------------

//--- ���z�̈ʒu�̓ǂݍ���
bool
load_Sun_position(const char *filename)
{
	return trajSun.load(filename, 100);
}


void
update_Sun_position(const double& t)
{
	if (trajSun.isLoaded() && trajSun.isInRange(t)) {
		r_sun = trajSun.get_pos(t);
	}
	else {
		r_sun.set(0, 0, 0);
	}
}


vector3d
get_sun_position()
{
	return  r_sun;
}



//---------
//  Scale
//---------
double
get_scale_sun()
{
	return  30.0 * get_sun_radius(MODE_3D);
}



//---------
//  Label
//---------
bool
is_sun_label_visible()
{
	return  test_disp_switch(DISPSW_PLANET, DISPM_NAME);
}



//---------------
//  Label alpha
//---------------
float
get_sun_label_alpha(const mtk_draw_struct& ds)
{
	const float cnorm = ds.cnorm;
	float af;

	float scl_max = 5e4;
	if (cnorm >= scl_max)  return 0;

	if (cnorm >= 3e2)  {
		af = 1 - cnorm/scl_max;
		if (af <= 0) return 0;
		af *= af;
	} else {
		af = 1.0;
	}

	return  af;
}



//---------
//  Glare
//---------
void
set_sun_glare_color(const RGBf& col)
{
	colSunGlare = col;
}






//--------
//  Draw
//--------
void
draw_sun0(const float& R, const RGBf& col)
{
	float mat_d[] = {1,1,1,1}, mat_a[] = {1,1,1,1};
	mat_d[0] = col.r;
	mat_d[1] = col.g;
	mat_d[2] = col.b;
	mat_a[0] = col.r;
	mat_a[1] = col.g;
	mat_a[2] = col.b;

	glMaterialfv( GL_FRONT_AND_BACK , GL_DIFFUSE,   mat_d );
	glMaterialfv( GL_FRONT_AND_BACK , GL_AMBIENT,   mat_a );

	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glColor3f(1,1,1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texSun);
	glDisable(GL_BLEND);

	// ���_�̈ʒu����Ɩ����Ƃ炷
	glPushMatrix();
		toward_eye_pos();
		float pos[4];
		pos[0] = 0;
		pos[1] = 0;
		pos[2] = 1;
		pos[3] = 0.0f;
		glLightfv( GL_LIGHT1, GL_POSITION, pos );
	glPopMatrix();

	glPushMatrix();
		glScalef(R, R, R);
		gl_draw_LOD_sphere();
	glPopMatrix();

	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT1);
	glEnable(GL_LIGHT0);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_NORMALIZE);
}

void
draw_sun_glare(const float& Rd, const float alp)
{
	// �O���A�̕\��
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDepthMask( GL_FALSE );

	glPushMatrix();
		glScalef(Rd, Rd, Rd);
		toward_eye_pos2();
		
		glBegin(GL_TRIANGLE_FAN);
		glColor4f(colSunGlare.r, colSunGlare.g, colSunGlare.b, 0.7*alp);
		glVertex3f(0,0,0);
		for (int i=0; i<=360; i+=20) {
			float x = cos(D2R*i);
			float y = sin(D2R*i);
			glColor4f(colSunGlare.r, colSunGlare.g, colSunGlare.b, 0);
			glVertex3f(x,y,0);
		}
		glEnd();
	glPopMatrix();

	glDepthMask( GL_TRUE );
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}




void
draw_sun(const mtk_draw_struct& ds, int idx, const vector3d& rpos, const double& scl)
{
	const float scale			= ds.scale;

	float af;
	if (scale >= 3e4)  {
		af = 1 - scale / sun_draw_scale_max;
		if (af <= 0) return;	//*****
	} else {
		af = 1.0;
	}
	
	RGBAf rgba;
	rgba.rgb = col_sun;
	rgba.a   = af;


	// �����a
	const float Re = scl * get_sun_radius(ds.tyMode);
	double th = Re/rpos.norm();
	bool bPoint = (th < 2*0.001);


	glDepthMask(GL_TRUE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPushMatrix();
		glTranslated(rpos.x, rpos.y, rpos.z);

		//---  Draw  ---


		//-- Glare
		const double D = norm(rpos);
		float aa = 400.0 / D;	// 100pc
		if (aa > 0.02) {
			if (aa > 1) aa=1;		
			float Rd = Re * get_fparam(MTK_FLOAT_SUN_GLARE_SIZE);
			const double thc = 1.0e-2;
			if (Rd/D < thc) {
				Rd = thc * D;
			}

			draw_sun_glare(Rd, aa);
		}

		//-- Sun
		if (!bPoint) {
			draw_sun0(Re, RGBf(1,1,1));
		} else {
			glEnable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_LIGHTING);
			glPointSize( 5/(ds.rad_per_pixel/0.001)  );
			glBegin(GL_POINTS);
				glColor4fv( rgba.get_pointer() );
				glVertex3f(0,0,0);
			glEnd();
		}

	glPopMatrix();
}



void
draw_sun_planetarium_2(const mtk_draw_struct& ds, const double& Rc, const double& I0)
{
	const int Nr = 20;
	const int Nth = 50;

	const mtk_object&  tgt = get_object(ds.tgt_ID);
	const float scale = ds.scale;

	const vector3d r_sun = get_sun_position();
	vector3d  rpos = r_sun - ds.camera.get_pos();
	double L = rpos.norm();
	vector3d  pos0 = rpos / L;

	double sin_max = get_sun_radius(MODE_PLANETARIUM) / L;



	vector3d ex, ez;
	ex = get_a_normal_unit_vector(pos0);
	ez = cross_product(pos0, ex);


	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);


	int i, j;



	//---  Attenuation  ---
	RGBf col(1, 1, 1);
	if (tgt.get_type() == OBJ_TYPE_PLANET && tgt.get_index() == 2 && has_atmosphere(PLNT_EARTH)) {
		vector3d r0 = ds.camera.get_pos() - get_planet_position(2);

		const atmosphere& atm = get_atmosphere(PLNT_EARTH);

		float zrt = 1;
		const double R = atm.R * zrt;
		const float  H = atm.H * zrt;		// �X�P�[���n�C�g(H=8km)
		const float n0 = atm.n0 / zrt;		// number density

		vector3d  lv = normalize(rpos);


		//-- ���ˌ��̌����� --
		col = calc_attenuated_light_intensity(R, R + 10 * H, n0, H, I0*1.5, r0*_1pc_km * 1000, lv, 100);
		col = correct_RGB(col);
	}



	//---  Sun  ---
	glColor3fv(col.get_pointer());
	glBegin(GL_TRIANGLE_STRIP);
	for (i = 0; i<Nr; i++) {
		double r1, r2;
		r2 = (i + 1) * sin_max / Nr;
		r1 = i * sin_max / Nr;

		for (j = 0; j<Nth; j++) {
			double th = 2 * 3.1415 * j / (Nth - 1);
			vector3d ev = cos(th)*ex + sin(th)*ez;
			ev = ev.normalized();

			vector3d rr;
			rr = Rc * (pos0 + r1 * ev);
			glVertex3f(rr.x, rr.y, rr.z);
			rr = Rc * (pos0 + r2 * ev);
			glVertex3f(rr.x, rr.y, rr.z);
		}
	}
	glEnd();



	//---  Corona  ---
	if (tgt.get_type() != OBJ_TYPE_PLANET || tgt.get_index() != 2)  return;

	bool bEclipse = false;
	if (swEclipse & ECLIPSE_DISP) {
		vector3d r1, r2;
		double R1, R2, Rs, Rx, Ry, Rz;
		eclipse_info  eci;


		r1 = get_planet_position(2);
		R1 = get_planet_Re(ds.tyMode, 2) * _1km_pc;
		r2 = get_satellite_position(0);
		get_satellite_size(MODE_PLANETARIUM, 0, Rx, Ry, Rz);
		R2 = Rx * _1km_pc;
		Rs = R_sun_km * _1km_pc;

		eci = calc_eclipse_info(r1, R1, r2, R2, r_sun, Rs);
		if (!eci.bFlg) return;


		vector3d pos = R1 * get_planet_nv_ec(2, landing_lon, landing_lat) + r1;


		// �����a
		double thm, ths, R;
		thm = asin(R2 / norm(pos - r2));
		ths = asin(Rs / norm(pos - r_sun));
		R = thm / ths;

		// ���S�Ԃ̋���
		double Dlt;
		vector3d nm = normalize(r2 - pos);
		vector3d ns = normalize(r_sun - pos);
		Dlt = acos(dot_product(nm, ns)) / ths;

		// ���������v�Z����
		double f = calc_eclipse_fraction(R, Dlt);
		if (f <= 0.0)  f = 0;

		if (f <= 0.995)  return;



		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		double Br0 = (f - 0.995) / 0.005;

		glBegin(GL_TRIANGLE_STRIP);
		for (i = 0; i<Nr; i++) {
			double r1, r2, a1, a2, c1, c2;
			r2 = (2 * (i + 1) + Nr) * sin_max / Nr;
			r1 = (2 * i + Nr) * sin_max / Nr;

			a1 = double(i) / Nr;
			a2 = double(i + 1) / Nr;

			c1 = 1 - a1;
			c1 = Br0 * c1 * c1 * c1 * c1 * c1 * c1 * c1;
			c2 = 1 - a2;
			c2 = Br0 * c2 * c2 * c2 * c2 * c2 * c2 * c2;


			for (j = 0; j<Nth; j++) {
				double th = 2 * 3.1415 * j / (Nth - 1);
				vector3d ev = cos(th)*ex + sin(th)*ez;
				ev = ev.normalized();

				vector3d rr;
				RGBf cc;

				rr = Rc * (pos0 + r1 * ev);
				cc = correct_RGB(float(c1) * col);
				glColor3fv(cc.get_pointer());
				glVertex3f(rr.x, rr.y, rr.z);

				rr = Rc * (pos0 + r2 * ev);
				cc = correct_RGB(float(c2) * col);
				glColor3fv(cc.get_pointer());
				glVertex3f(rr.x, rr.y, rr.z);
			}
		}
		glEnd();

	}
}




void
draw_sun_label_planetarium(const mtk_draw_struct& ds, const double& Rc)
{
	if (!test_disp_switch(DISPSW_PLANET, DISPM_NAME))  return;


	vector3d  rpos = get_sun_position() - ds.camera.get_pos();
	double L = rpos.norm();
	vector3d  pos = Rc * rpos / L;
	float RR = get_sun_radius(MODE_PLANETARIUM) / L;


	glPushMatrix();
	glTranslated(pos.x, pos.y, pos.z);

	prepare_3Dtext_1(1.7*RR*ds.plnt_R, 45, get_special_object_name_size()*ds.ltf, 1.0f, false);

	const RGBf &col = get_color(MTK_COLOR_NAME_SPECIAL_OBJECT);
	glColor3f(col.r, col.g, col.b);

	glDisable(GL_DEPTH_TEST);
	draw_text(get_string("SUN"));
	glPopMatrix();
}





//--------------
//  Draw label
//--------------
void
draw_sun_label(const mtk_draw_struct& ds, const RGBf& col, float af)
{
	const float cnorm = ds.cnorm;


	//*****
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);


	//---  �X�P�[���ɂ��\����ς���  ---
	const double scl1 = 5;
	const double scl2 = 20;

	if (cnorm < scl1) {
		// Sun
		glColor4f(col.r, col.g, col.b, af);
		draw_text(get_string("SUN"));

	}
	else if (cnorm > scl2) {
		// Solar System
		glColor4f(col.r, col.g, col.b, af);
		draw_text(get_string("SOLAR_SYSTEM"));
	}
	else {
		// �؂�ւ��̏ꍇ
		const double logscl = log(cnorm);
		const double logscl1 = log(scl1);
		const double logscl2 = log(scl2);
		float t;
		float af1, af2;
		t = (logscl - logscl1) / (logscl2 - logscl1);
		af1 = (1 - t) * af;
		af2 = t * af;
		// ���z
		glColor4f(col.r, col.g, col.b, af1);

		glPushMatrix();
			draw_text(get_string("SUN"));
		glPopMatrix();
		// ���z�n
		glColor4f(col.r, col.g, col.b, af2);
		glPushMatrix();
			glTranslatef(0, 0, 0.02);
			draw_text(get_string("SOLAR_SYSTEM"));
		glPopMatrix();
	}
}


float
get_sun_label_size(const mtk_draw_struct& ds)
{
	const float cnorm = ds.cnorm;
	const double scl1 = 6;


	bool bFlg = (cnorm < scl1) ? false : true;
	const std::string& str = (bFlg) ? get_string("SOLAR_SYSTEM") : get_string("SUN");


	float w, h;
	get_text_size(str, w, h);


	return  w;
}




void
set_light_from_sun(const vector3d& pos)
{
	vector3d n = normalize( pos );
	float lpos[4];
	lpos[0] = -n.x;
	lpos[1] = -n.y;
	lpos[2] = -n.z;
	lpos[3] = 0.0f;
	glLightfv( GL_LIGHT0, GL_POSITION, lpos );
}





//--------
//  Info
//--------
void
make_object_info_sun(const mtk_draw_struct& ds, std::vector<std::string>& text)
{
	//---  Name  ---
	text.push_back(get_string("SUN"));

	//---  Type  ---
	add_info_line(text, "INFO_TYPE", "INFO_TYPE_STAR");
	add_info_line_str(text, "INFO_STAR_SPECTRAL_TYPE", "G2");
}
