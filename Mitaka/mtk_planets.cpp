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
#include  "mitaka.h"
#include  "mtk_mode.h"
#include  "mtk_planet.h"
#include  "mtk_planets.h"
#include  "mtk_objects.h"
#include  "mtk_object_position.h"
#include  "mtk_object_orientation.h"
#include  "mtk_planet_position.h"
#include  "mtk_planet_orientation.h"
#include  "mtk_solar_system.h"
#include  "mtk_sun.h"
#include  "mtk_satellites.h"
#include  "mtk_minor_planets.h"
#include  "mtk_planetarium.h"
#include  "mtk_planet_eclipse.h"
#include  "mtk_label.h"
#include  "mtk_calc_alpha.h"
#include  "mtk_time.h"
#include  "mtk_stars.h"
#include  "mtk_places.h"
#include  "mtk_draw_places.h"
#include  "mtk_switches.h"
#include  "mtk_atmosphere.h"
#include  "mtk_font.h"
#include  "mtk_text.h"
#include  "mtk_gl_common.h"
#include  "mtk_draw_common.h"
#include  "mtk_planetary_ring.h"
#include  "mtk_dialog_box.h"
#include  "mtk_OpenGL.h"
#include  "../common/astro/astro_const.h"
#include  "../common/graphics/gl_triangle_mesh.h"
#include  "mtk_triangle_mesh.h"
#include  "mtk_colors.h"
#include  "mtk_eclipse.h"
#include  "mtk_target.h"


#include  "../common/graphics/gl_bmp_font.h"
#include  "mtk_strings.h"
#include  "mtk_make_info.h"
#include  "mtk_topography.h"



//-----------
//  Externs
//-----------
extern bool bDrawLowRes;	//****
vector2d	get_landing_pos();





//------------
//  Lighting
//------------
float AtmosphereIntensity;
float AtmosphereIntensityDef = 6.0;
float EarthSpecular;
float EarthSpecularDef = 6.0;
float EarthShininess;
float EarthShininessDef = 60;



//----------
//  Others
//----------
int EarthCityLightsIniDef = 0;
bool EarthCityLightsIni   = false;





//------------
//  �f���f�[�^
//------------
const int PLANET_NUM = 9;

struct planet_info {
	const char* key;	// Key
	double	Re;			// �ԓ����a(km)
	double  f;			// �G��
	int		ring_idx;	// �ւ̔ԍ��i-1�Ŋ������Ȃ��j
	const char* tex_fn;	// �e�N�X�`���̃t�@�C����
};

const planet_info plnti[PLANET_NUM] = {
	{ "PLNT_MERCURY", 2439.7,    0.0,       -1, "mercury.jpg"},	// ����
	{ "PLNT_VENUS",   6051.8,    0.0,       -1, "venus.jpg"},	// ����
	{ "PLNT_EARTH",   6378.1366, 0.0033528, -1, "earth.jpg"},	// �n��
	{ "PLNT_MARS",    3396.19,   0.0058860, -1, "mars.jpg"},	// �ΐ�
	{ "PLNT_JUPITER", 71492,     0.064874,  -1, "jupiter.jpg"},	// �ؐ�
	{ "PLNT_SATURN",  60268,     0.097962,   0, "saturn.jpg"},	// �y��
	{ "PLNT_URANUS",  25559,     0.022927,   1, "uranus.jpg"},	// �V����
	{ "PLNT_NEPTUNE", 24764,     0.017081,  -1, "neptune.jpg"},	// �C����
	{ "PLNT_PLUTO",   1185,      0.0,       -1, "pluto.jpg"}	// ������
};




//---------------
//  �����O�f�[�^
//---------------
const int RING_NUM = 2;


const ring_info ringi[RING_NUM] = {
	{1.236, 2.270, 1,   "saturn_rings.png" },	// �y��
	{1.487, 2.001, 0.2, "uranus_rings.png" }	// �V����
};




//---------------------
//  ���̂ق��̃e�N�X�`��
//---------------------
const int EARTH_DETAIL_TEXTURE_NUM = 3;
const char szEarthDetailTexFileName[EARTH_DETAIL_TEXTURE_NUM][30] = {
	"earth_japan.png",		// ���{����
	"earth_hawaii.png",		// �n���C������
	"earth_chile.png"		// �`���AALMA����
};





//-----------
//  Objects
//-----------

//--  Planets
static std::vector<planet> vPlanets;

//--  Textures  --
GLuint	texEarthCloud, texEarthLights, texEarthSpecular, texEarthSpecular2;
GLuint	texEarthDetail[6];	//****




//
//
//
planet&
get_planet(int idx)
{
	return  vPlanets[idx];
}




//--------------------
//  Register objects
//--------------------
void
register_object_planets()
{
	mtk_object  obj;
	for (int i=0; i<PLANET_NUM; i++) {



		//--  init
		planet pnt;
		const planet_info& pif = plnti[i];
		pnt.init(pif.key, pif.Re, pif.f, pif.tex_fn);
		
		// Ring
		if (pif.ring_idx >= 0) {
			const ring_info& ri = ringi[pif.ring_idx];

			int ring_idx;
			ring_idx = add_particle_ring(ri.Rr1, ri.Rr2, ri.ring_alp, ri.tex_fn);
			pnt.add_ring(ring_idx);
		}

		// Orientation
		int ori_idx = add_orientation(pnt.get_key(), ORIENTATION_PLANET, i);
		pnt.set_orientation_index(ori_idx);


		vPlanets.push_back(pnt);



		//--  key
		const std::string& key = pnt.get_key();


		//--  register planets
		obj.init(OBJ_TYPE_PLANET, i, 0.0f, true);
		int obj_idx = register_object(obj, key);

		//--  register labels
		float R   = 1.4 * _1km_pc * get_planet_Re(MODE_3D, i);
		float thc = 5 * D2R;

		int color_no;
		float name_size;
		if (i==PLNT_PLUTO) {
			color_no  = MTK_COLOR_NAME_DWARF_PLANET;
			name_size = get_dwarf_planet_name_size();
		} else {
			color_no  = MTK_COLOR_NAME_OBJECT;
			name_size = get_object_name_size();
		}

		add_label( key, obj_idx, color_no, 1.0f, name_size, R, thc, true);
	}
}





//--------------
//  Atmosphere
//--------------
void
init_atmospehre()
{
	atmosphere atm;


	//---  Earth
	atm.R = get_planet_Re(MODE_3D, PLNT_EARTH) * 1000.0;
	atm.H = 8000;
	atm.dp = 6.0;
	atm.n0 = 2.69e25;
	atm.I0 = AtmosphereIntensity;
	add_atmosphere(PLNT_EARTH, atm);

	//---  Mars
	atm.R = get_planet_Re(MODE_3D, PLNT_MARS) * 1000.0;
	atm.H = 11000;
	atm.dp = 8.0;
	atm.n0 = 4e23;
	atm.I0 = AtmosphereIntensity;
	add_atmosphere(PLNT_MARS, atm);
}


//---------------
//  �ԓ����a(km)
//---------------
double
get_planet_Re(int mode, int idx)
{
	const planet& pnt = get_planet(idx);
	double fac = (mode == MODE_3D) ? get_zoom_rate().zmhPlanet : get_zoom_rate().zmpPlanet;
	return  fac * pnt.get_Re();
}



//------------------
//  Landing radius
//------------------
double
get_landing_radius_planet(int idx)
{
	planet& pnt = vPlanets[idx];

	double R = get_planet_Re(MODE_3D, idx) * _1km_pc;

	//---  �n�`����̏ꍇ
	TopoSphere *tp = pnt.get_topography();
	if (tp != 0 && tp->is_loaded()) {
		vector2d  lon_lat = get_landing_pos();
		R += tp->get_elv_fac() * tp->get_elevation(lon_lat.x, lon_lat.y) * _1km_pc / 1000.0;
	}

	return  R;
}



//----------------
//  Prepare draw
//----------------
bool
prepare_draw_planet(const mtk_draw_struct& ds, int idx, double* pRb, double* pRh)
{
	const planet& pnt = vPlanets[idx];

	//---  Visible
	bool flg;
	if (idx==PLNT_PLUTO)	flg = test_disp_switch( DISPSW_TNO,		DISPM_DISP);	//--  Pluto
	else					flg = test_disp_switch( DISPSW_PLANET,	DISPM_DISP);
	if (!flg)  return false;

	if (ds.scale > 500 * get_planet_a(idx) * _1AU_pc)  return false;


	//---  Radius
	*pRb = *pRh = get_planet_Re(ds.tyMode, idx) * _1km_pc;
	
	if (pnt.hasRing()) {
		const particle_ring& pr = pnt.get_ring();
		*pRb *= pr.get_R2();
	}

	return  true;
}



//---------
//  Scale
//---------
double
get_scale_planet(int idx)
{
	return  get_planet_Re(MODE_3D, idx) * _1km_pc * 30.0;
}



//---------
//  Label
//---------
bool
is_planet_label_visible(int idx)
{
	int dispsw_idx;

	if (idx == PLNT_PLUTO)	dispsw_idx = DISPSW_TNO;
	else					dispsw_idx = DISPSW_PLANET;

	if ( !test_disp_switch(dispsw_idx, DISPM_DISP) || !test_disp_switch(dispsw_idx, DISPM_NAME) )  return  false;

	return  true;
}






//-----------------
//  Load Textures
//-----------------
void
set_earth_texture0(TopoSphere& tp, GLuint texname, int px1, int py1, int lv, int pw, int ph, int x1, int y1, int tex_w, int tex_h)
{
	const int img_w = 8192;
	const int img_h = 4096;

	float x1p, y1p;
	x1p = float(px1) / 180.0 * img_w;
	y1p = float(py1) /  90.0 * img_h;
	const float u1 = (x1p-x1)/tex_w;
	const float v1 = 1.0 - (y1p-y1)/tex_h;
	const float du =  float(img_w/180.0)/tex_w;
	const float dv = -float(img_h/90.0) /tex_h;

	float v = v1;
	for (int iy=0; iy<ph; iy++) {
		float u = u1;
		for (int ix=0; ix<pw; ix++) {
			patch_texture_info  pti;
			pti.texname = texname;
			pti.u1 = u;
			pti.v1 = v;
			pti.u2 = u + du;
			pti.v2 = v + dv;
			tp.set_patch_texture_info(px1+ix, py1+iy, lv, pti);
			u += du;
		}
		v += dv;
	}

}

void
set_earth_texture()
{
	planet& pnt = vPlanets[PLNT_EARTH];

	TopoSphere *tp = pnt.get_topography();
	if (tp == 0 || !tp->is_loaded())  return;
	
	tp->set_default_texname(pnt.get_texture_index());
	set_earth_texture0(*tp, texEarthDetail[0],  148, 17, 4, 22,22, 6735,  773, 1024,1024);		// ���{����
	set_earth_texture0(*tp, texEarthDetail[1],   9, 33, 4, 10,5,   409, 1501,  256, 128);		// �n���C������
	set_earth_texture0(*tp, texEarthDetail[2],   53, 51, 4, 5,10,  2412, 2321,  256, 512);		// �`���AALMA����
}



void
load_planets_textures()
{
	//-- Planets
	for (int i = 0; i<vPlanets.size(); ++i) {
		planet& pnt = vPlanets[i];

		// planet
		pnt.load_texture(TexShrinkRate);
	}


	//-- Earth Detail
	SetSplashText("LOADING_EARTH_DETAILED_TEXTURE");
	for (int i=0; i<EARTH_DETAIL_TEXTURE_NUM; i++) {
		texEarthDetail[i] = LoadTexture(szEarthDetailTexFileName[i], TexShrinkRate, false);
	}
	set_earth_texture();

	//-- Earth cloud
	SetSplashText("LOADING_EARTH_CLOUD_TEXTURE");
	texEarthCloud = LoadTexture("earth_cloud.jpg", TexShrinkRate, false);

	//-- Earth lights
	SetSplashText("LOADING_EARTH_CITY_LIGHT_TEXTURE");
	texEarthLights = LoadTexture("earth_lights.jpg", TexShrinkRate, false);

	//-- Earth specular
	SetSplashText("LOADING_EARTH_SPECULAR_MAP");
	texEarthSpecular  = LoadTexture("earth_specular_with_cloud.jpg",    TexShrinkRate, false);
	texEarthSpecular2 = LoadTexture("earth_specular_without_cloud.jpg", TexShrinkRate, false);
}



//-------------------------
//  Places and topography
//-------------------------
void
init_planet_places_and_topography()
{
	//---  Places and topography
	for (int i = 0; i < vPlanets.size(); i++) {
		planet& pnt = vPlanets[i];

		const std::string& key = pnt.get_key();
		pnt.set_places( get_places(key) );
		pnt.set_topography( get_topography(key) );
	}


	//---  Eclipse
	const vector3f col(0, 0, 0);

	// Earth
	{
		planet& pnt = get_planet(PLNT_EARTH);
		pnt.add_eclipse("SAT_E_MOON", col);
	}
	// Jupiter
	{
		const char j_sat_key[4][20] = {"SAT_J_IO", "SAT_J_EUROPA", "SAT_J_GANYMEDE", "SAT_J_CALLISTO"};
		planet& pnt = get_planet(PLNT_JUPITER);

		for (int i = 0; i < 4; i++) {
			pnt.add_eclipse(j_sat_key[i], col);
		}

	}
	// Saturn
	{
		planet& pnt = get_planet(PLNT_SATURN);
		pnt.add_eclipse("SAT_S_TITAN", col);
	}
}











void
set_planet_light(int idx)
{
	set_light_from_sun( get_planet_position(idx) );
}


//-----------------
// ����̓V�̂�`��
//-----------------
void
draw_cloud_sphere(float Rx, float Ry, float Rz)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glPushMatrix();
		glScalef(Rx, Ry, Rz);
		gl_draw_LOD_sphere(true);
	glPopMatrix();
	glEnable(GL_CULL_FACE);
}


//-------------------
//  �f���̎��]����`��
//-------------------
void
draw_planet_axis(const float Rz)
{
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glLineWidth(1.5);
	glColor3f(0.8, 0.6, 0.2);
	glBegin(GL_LINES);
		glVertex3f(0,0, Rz*1.3);
		glVertex3f(0,0,-Rz*1.3);
	glEnd();
}


//-------
//  �n��
//-------
vector2d	get_landing_pos();

void
draw_Earth(const mtk_draw_struct& ds, bool bPointer, bool bWire)
{
	planet& pnt = vPlanets[PLNT_EARTH];
	const frustum frs = ds.get_frustum();
	const vector3d  r0 = get_planet_position(PLNT_EARTH);


	glLineWidth(1.0f);
	glColor3f(0,1,0);


	TopoSphere *tp = pnt.get_topography();
	if (tp!=0 && tp->is_loaded()) {
		tp->draw(frs, bWire, bDrawLowRes);
	} else {
		glPushMatrix();
			glEnable(GL_LIGHTING);
			glEnable(GL_NORMALIZE);

			glBindTexture(GL_TEXTURE_2D, pnt.get_texture_index());
			glRotatef(180, 0, 0, 1);	//***
			gl_draw_LOD_sphere();
		glPopMatrix();
	}

	const float spc = EarthSpecular;
	float mat0[] = {0,0,0, 1};
	float mat_s[] = {spc, spc, spc, 1};
	glPushAttrib(GL_LIGHTING_BIT | GL_ENABLE_BIT);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
		glEnable(GL_NORMALIZE);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glMaterialfv( GL_FRONT_AND_BACK , GL_AMBIENT,    mat0 );
		
		// ��i
		float difi = 1.0;
		if ( !test_disp_switch(DISPSW_PLANET, DISPM_CITY_LIGHTS) ) {
			const float difi0 = 0.3;
			const double R  = norm(ds.camera.get_pos() - r0) * _1pc_meter;
			const double Re = get_planet_Re(ds.tyMode, 2) * 1000;
			const double h = R - Re;
			difi = difi0 * (1 - h/(1.7*Re));
			if (difi > 1.0) difi = 1.0;
			if (difi < 0.0) difi = 0.0;
		} else {
			float cosphi = dot_product( normalize(r0), normalize(ds.camera.get_pos() - r0) );
			if (cosphi <= 0.0)  difi = 0;
			else				difi *= cosphi;
		}

		if (difi > 0.0) {
			float mat_d[4];
			for (int i=0; i<3; i++) {
				mat_d[i] = difi;
			}
			mat_d[3] = 1.0;

			glMaterialfv( GL_FRONT_AND_BACK , GL_DIFFUSE,    mat_d );
			glBindTexture(GL_TEXTURE_2D, texEarthLights);
			glRotatef(180, 0, 0, 1);	//***
			gl_draw_LOD_sphere(false, true);
			glRotatef(180, 0, 0, 1);	//***
		}
		
		// �C�ʂ̋��ʔ���
		RGBf hl_col;
		{
			const float zrt = get_zoom_rate().zmhPlanet;
			const float H  = 8000 * zrt;		// �X�P�[���n�C�g(H=8km)
			const float n0 = 2.69e25 / zrt;		// number density
			RGBf F = calc_intensity_of_highlight(ds.camera.get_pos(), r0, get_sun_position(), get_planet_Re(ds.tyMode, 2)*1000, 10, H, n0, 1, 30);
			F = spc * F;

			float Fmax = max(F.r, max(F.g, F.b));
			if (Fmax > 1.0) {
				F = correct_RGB(F/Fmax) * Fmax;
			} else {
				F = correct_RGB(F);
			}

			hl_col = F;
		}
		glPushMatrix();
			if (test_disp_switch(DISPSW_PLANET, DISPM_CLOUD)) {
				glBindTexture(GL_TEXTURE_2D, texEarthSpecular);		// �_����
			}
			else {
				glBindTexture(GL_TEXTURE_2D, texEarthSpecular2);	// �_�Ȃ�
			}

			glRotatef(180, 0, 0, 1);	//***
			draw_planet_highlight(ds, 2, hl_col);
		glPopMatrix();

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPopAttrib();

	// ��C�̕`��
	if (AtmosphereMode != ATMOSPHERE_NONE && has_atmosphere(PLNT_EARTH)) {
		glRotatef(180, 0, 0, 1);	//***
		draw_atmosphere(get_atmosphere(PLNT_EARTH), get_zoom_rate().zmhPlanet, (AtmosphereMode == ATMOSPHERE_MULTIPLE));
		glRotatef(180, 0, 0, 1);	//***
	}

	// �_�̕\��
	if ( test_disp_switch( DISPSW_PLANET, DISPM_CLOUD ) ) {
		glBindTexture(GL_TEXTURE_2D, texEarthCloud);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glDepthMask(GL_FALSE);
		const double h = 10.0/6378.1;	// �_�̔��a�̘f�����a�Ƃ̔�
		float Rc = 1.0 + h * get_topo_zoom_rate();

		draw_celestial_object(Rc, Rc, Rc, 180.0f);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_TRUE);
	}

	// ���H���̉e
	if (swEclipse & (ECLIPSE_DISP | ECLIPSE_BORDER)) {
		draw_planet_eclipse(ds, PLNT_EARTH, 1.0);
	}

	
	// �n��
	if ( test_disp_switch(DISPSW_PLANET, DISPM_NAME)  &&  ds.tySubMode != SUBMODE_3DCHART) {
		Places* plc = get_places("PLNT_EARTH");
	
		if (plc != 0) {
			bool bHorizontalMode = (get_landing_mode() == LANDING_MODE_HORIZONTAL);
			draw_places_on_TopoSphere(*plc, tp, get_screen_w(), get_screen_z(), ds.ltf, bHorizontalMode);
		}
	}
	

	// �|�C���^
	if (bPointer) {
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		vector2d lon_lat = get_landing_pos();
		draw_pointer_on_TopoSphere(tp, get_screen_w(), get_screen_z(), lon_lat.x, lon_lat.y, ds.ltf);
	}

	glEnable( GL_DEPTH_TEST );
	glDisable(GL_NORMALIZE);
}




//------------------------------
//  �f��
//   �ʒu�Ǝ��]�������킹�Ă���Ă�
//------------------------------
void
draw_planet(const mtk_draw_struct& ds, int idx, bool bAxis, bool bFar)
{
	planet& pnt = vPlanets[idx];
	const mtk_object&  tgt = get_target();
	bool bTarget = (tgt.get_type() == OBJ_TYPE_PLANET && tgt.get_index() == idx);

	const float screen_w = get_screen_w();
	const float screen_z = get_screen_z();
	const float Rz = 1.0 / (1.0f + pnt.get_f());


	//---  ���]���̕`��  ---
	glDisable(GL_BLEND);
	if (bAxis) draw_planet_axis(Rz);



	//---  �f���{�̂̕`��  ---
	const frustum frs = ds.get_frustum();
	bool bWire = false;	//


	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glBindTexture(GL_TEXTURE_2D, pnt.get_texture_index());


	glPushMatrix();
	if (ds.tyMode == MODE_3D && (idx == 2 || idx == 3)) {

		//--- �n�� ---
		if (idx==2) {
			draw_Earth(ds, bSurfacePointer && bTarget && ds.tySubMode != SUBMODE_3DCHART, false);
		}

		//--- �ΐ� ---
		if (idx==3) {
			glLineWidth(1.0f);
			glColor3f(0,1,0);


			TopoSphere *tp = pnt.get_topography();
			if (tp != 0) {
				tp->draw(frs, bWire, bDrawLowRes);
			}
			else {
				draw_celestial_object(1, 1, Rz, 0.0f);
			}

			if ( test_disp_switch( DISPSW_PLANET, DISPM_NAME )  &&  ds.tySubMode != SUBMODE_3DCHART) {
				Places *plc = pnt.get_places();
				
				if (plc != 0) {
					bool bHorizontalMode = (get_landing_mode() == LANDING_MODE_HORIZONTAL);
					draw_places_on_TopoSphere(*plc, tp, screen_w, screen_z, ds.ltf, bHorizontalMode);
				}
			}

			if (bSurfacePointer && bTarget && ds.tySubMode != SUBMODE_3DCHART) {
				glDisable(GL_LIGHTING);
				glDisable(GL_TEXTURE_2D);
				glEnable(GL_BLEND);
				vector2d lon_lat = get_landing_pos();
				draw_pointer_on_TopoSphere(tp, screen_w, screen_z, lon_lat.x, lon_lat.y, ds.ltf);
			}

			// ��C
			if (AtmosphereMode != ATMOSPHERE_NONE && has_atmosphere(PLNT_MARS)) {
				draw_atmosphere(get_atmosphere(PLNT_MARS), get_zoom_rate().zmhPlanet, false);
			}
			glDisable(GL_NORMALIZE);
		}
	} else {


		if (pnt.hasRing()) {
			vector3d lv;
			if (bSideLight) {
				lv = get_side_light_vector(ds);
			}
			else {
				lv = normalize(get_sun_position() - get_planet_position(idx));
			}
			const particle_ring& pr = pnt.get_ring();
			vector3d r0, ex, ey, ez;
			double R, f;
			r0 = get_planet_position(idx);
			R = get_planet_Re(ds.tyMode, idx) * _1km_pc;
			get_planet_bases(idx, ex, ey, ez);
			f = pnt.get_f();

			draw_ring_shaded_sphere(r0, R, f, ex, ey, ez, lv, pr, pnt.get_texture_index(), RGBf(1,1,1));
		}
		else {
			draw_celestial_object(1, 1, Rz, 180.0f);
		}


		//  Eclipse
		if ((swEclipse & (ECLIPSE_DISP | ECLIPSE_BORDER))) {
			draw_planet_eclipse(ds, idx, 1.0);
		}

		
		// �n�\�|�C���^�̕`��
		if (bSurfacePointer && bTarget && ds.tySubMode != SUBMODE_3DCHART) {
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_LIGHTING);
			glEnable(GL_BLEND);
			glLineWidth(1.0f);
			glColor3f(0,1,0);
			vector2d lon_lat = get_landing_pos();
			draw_pointer_on_sphere(screen_w, screen_z, lon_lat.x, lon_lat.y, ds.ltf);
		}
	}
	glPopMatrix();


	// �f���̊̕`��
	if (pnt.hasRing()) {
		vector3d lv;
		if (bSideLight) {
			lv = get_side_light_vector(ds);
		}
		else {
			lv = normalize(get_sun_position() - get_planet_position(idx));
		}

		particle_ring& ring = pnt.get_ring();

		vector3d r0, ex, ey, ez;
		double R, f;

		r0 = get_planet_position(idx);
		R  = get_planet_Re(ds.tyMode, idx) * _1km_pc;
		get_planet_bases(idx, ex, ey, ez);

		// �f�������ɂȂ�悤�Ɏ������ɃX�P�[���������W�n�ł̌����̈ʒu�����߂�
		f = pnt.get_f();



		ring.calc_shadow_factor(ds, r0, R, f, ex, ey, ez, lv);
		ring.draw(ds, r0, R, ex, ey, lv);
	}

	glDisable(GL_NORMALIZE);
}




//---------------
//  3D �`���[�g�p
//---------------
void
draw_planet2(int idx)
{
	const planet& pnt = vPlanets[idx];

	glBindTexture(GL_TEXTURE_2D, pnt.get_texture_index());
	const float Rz = 1.0/(1.0f + pnt.get_f());
	glDisable(GL_BLEND);


	float tex_shift = (idx == PLNT_MARS) ? 0.0f : 180.0f;


	//---  �f���{�̂̕`��  ---
	glEnable(GL_TEXTURE_2D);
	draw_celestial_object(1, 1, Rz, tex_shift);

	// �n���̉_��\��
	if ( test_disp_switch(DISPSW_PLANET, DISPM_CLOUD) && idx==PLNT_EARTH) {
		glBindTexture(GL_TEXTURE_2D, texEarthCloud);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
		float Rc = 1.0 + 8 * get_topo_zoom_rate() / get_planet(PLNT_EARTH).get_Re();
		draw_celestial_object(Rc, Rc, Rc, tex_shift);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	if (pnt.hasRing()) {
		const particle_ring& ring = pnt.get_ring();
		glPushMatrix();
			glScalef(1, 1, 1.0/(1.0f + pnt.get_f()));
			ring.draw_3Dchart();
		glPopMatrix();
	}

	glDisable(GL_NORMALIZE);
}




//-----------
//  �f���O��
//-----------
void
register_planets_orbit_curves()
{
	for (int i=0; i<PLANET_NUM; i++) {	//***
		int color_no = (i==PLNT_PLUTO) ? MTK_COLOR_ORBIT_DWARF_PLANET : MTK_COLOR_ORBIT_PLANET;
		space_curve  sc( mtk_object(OBJ_TYPE_PLANET, i, 0.0f, false), get_orbit_width(), color_no );
		register_space_curve( sc );
	}
}


void
make_planets_orbit_curve(space_curve& sc, int div_N)
{
	const int idx = sc.get_related_object().get_index();

	sc.clear();
	for (int i=0; i<div_N; i++) {
		double phi = 2.0 * M_PI * double(i) / div_N;
		sc.add_point( calc_planet_orbit_pos(idx, phi) );
	}
}


float
get_planet_label_alpha(const mtk_draw_struct& ds, int idx)
{
	float scl = 1.0f;
	const vector3d  rpos = get_planet_position( idx ) - ds.camera.get_pos();
	float  Rv = scl * calc_visible_radius_planet( idx );
	float  Dth = 2.0f * Rv / rpos.norm();

	return  calc_alpha_by_apparent_diameter( Dth );
}

float
get_planet_orbit_alpha(const mtk_draw_struct& ds, int idx)
{
	const float dscl = 0.035 * _1AU_pc;
	float  alp = calc_alpha_by_disapear_scale_inside( ds.scale, dscl );
	float scl = 1.0f;
	const vector3d  rpos = get_planet_position( idx ) - ds.camera.get_pos();
	float  Rv = scl * calc_visible_radius_planet( idx );
	float  Dth = 2.0f * Rv / rpos.norm();

	return  alp * calc_alpha_by_apparent_diameter( Dth );
}





//------------------
//  Visible radius
//------------------
double
calc_visible_radius_planet(int idx)
{
	return  get_planet_position(idx).norm();
}



//-----------
//  Planets
//-----------
void
draw_a_planet(const mtk_draw_struct& ds, int idx, const vector3d& rpos, const double& scl)
{
	glEnable(GL_LIGHT0);
	float mat_d[] = {1,1,1,1}, mat_a[] = {0.5,0.5,0.5,1};
	mat_a[0] = mat_a[1] = mat_a[2] = get_lighting_ambient();
	glMaterialfv( GL_FRONT_AND_BACK , GL_DIFFUSE,   mat_d );
	glMaterialfv( GL_FRONT_AND_BACK , GL_AMBIENT,   mat_a );


	// �Ɩ��̐ݒ�
	if (!bSideLight)	{ set_planet_light(idx); }
	else				{ set_side_light(ds); }

	const float Re = scl * get_planet_Re(ds.tyMode, idx) * _1km_pc;
	glPushMatrix();
		glTranslated(rpos.x, rpos.y, rpos.z);

		set_planet_orientation_GL(idx);	// ���]������щ�]�p�Z�b�g����

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);

		glScalef(Re, Re, Re);
		draw_planet(ds, idx, test_disp_switch(DISPSW_PLANET, DISPM_AXIS), !ds.is_near_region() );
	glPopMatrix();

	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
}





//------------
//  Hit-test
//------------
bool
hit_test_planet(const mtk_draw_struct& ds, int idx, const vector3d& rpos, const vector3d& nv)
{
	const planet& pnt = vPlanets[idx];
	matrix3d  M_E2PLNT;
	vector3d  _rpos, _nv;
	M_E2PLNT = get_planet_orientation(idx, true);
	_rpos = M_E2PLNT * rpos;
	_nv = M_E2PLNT * nv;

	// �G����
	double f;
	f = 1.0 - pnt.get_f();

	_rpos.z /= f;
	_nv.z /= f;
	_nv = normalize(_nv);

	// ��_
	double R, b, c, D, t;
	R = get_planet_Re(MODE_3D, idx) * _1km_pc;
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
make_object_info_planet(const mtk_draw_struct& ds, int idx, std::vector<std::string>& text)
{
	const planet& pnt = get_planet(idx);

	//---  Name  ---
	text.push_back(get_string(pnt.get_key()));


	//---  Type  ---
	if (idx != PLNT_PLUTO) {
		add_info_line(text, "INFO_TYPE", "INFO_TYPE_PLANET");
	}
	else {
		add_info_line(text, "INFO_TYPE", "INFO_TYPE_PLUTOID");
		add_info_line_int(text, "INFO_MP_NUMBER", 134340);
		add_info_line(text, "INFO_MP_ORBIT_TYPE", "MP_ORBIT_TYPE_TNO");	//  Orbit Type
	}
}




//---------------
//  Planetarium
//---------------
void
draw_a_planet_planetarium(const mtk_draw_struct& ds, int idx)
{
	planet& pnt = vPlanets[idx];

	if (ds.tgt_ID == get_object_ID(pnt.get_key()))  return;



	vector3d  ppos = get_planet_position(idx);


	//---  ���e�ʒu�ƃT�C�Y  ---
	vector3d  rpos = ppos - ds.camera.get_pos();
	double  L = rpos.norm();
	// �ʒu
	const double R0 = 1.002;
	vector3d pos = R0 * rpos/L;
	// �����a
	double Re, Rp;
	Re = get_planet_Re(ds.tyMode, idx) * _1km_pc;
	Rp = Re / L;


	// ����p
	double th = Rp/pos.norm();
	th *= get_planetarium_zooming_factor();

	const double thc1 = 1*ds.rad_per_pixel;
	bool bPoint = (th < thc1);


	//---  Draw  ---
	glDisable(GL_DEPTH_TEST);	//****

	glPushMatrix();
		glTranslatef(pos.x, pos.y, pos.z);
	
		if (!bPoint) {
			//---  ���T�C�Y�ŕ`��  ---
			glPushMatrix();
				set_light_from_sun( ppos );
				set_planet_orientation_GL(idx);

				glEnable(GL_NORMALIZE);
				glEnable(GL_LIGHTING);
				glScalef(Rp, Rp, Rp);
				draw_planet(ds, idx, false, false);
			glPopMatrix();
		} else {
			//---  �_�ŕ`��  ---
			glDisable(GL_LIGHTING);
			glDisable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);
			glPointSize(1.5);
			glBegin(GL_POINTS);
				glColor3f(1,1,1);
				glVertex3f(0,0,0);
			glEnd();
		}

		// ���O
		if ( test_disp_switch(DISPSW_PLANET, DISPM_NAME) ) {
			float sz;
			int col_idx;

			if (idx != PLNT_PLUTO) {
				sz = get_object_name_size();
				col_idx = MTK_COLOR_NAME_OBJECT;
			} else {
				sz = get_dwarf_planet_name_size();
				col_idx = MTK_COLOR_NAME_DWARF_PLANET;
			}

			prepare_3Dtext_1(Rp*1.2*ds.plnt_R, 45, sz*ds.ltf, 1.0f, false);
			
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_LIGHTING);
			glEnable(GL_BLEND);

			const RGBf col = get_color(col_idx);
			glColor4f(col.r, col.g, col.b, col_star_name.a);
		
			
			draw_string( pnt.get_key() );
		}
	glPopMatrix();
}




void
draw_planets_planetarium(const mtk_draw_struct& ds)
{
	float mat_d[] = {1,1,1,1};
	float mat_a[] = {0.5,0.5,0.5,1};
	mat_a[0] = mat_a[1] = mat_a[2] = get_lighting_ambient();
	glMaterialfv( GL_FRONT_AND_BACK , GL_DIFFUSE,   mat_d );
	glMaterialfv( GL_FRONT_AND_BACK , GL_AMBIENT,   mat_a );

	glEnable(GL_LIGHT0);

	for (int i=0; i<PLANET_NUM; i++) {
		draw_a_planet_planetarium(ds, i);
	}
}

