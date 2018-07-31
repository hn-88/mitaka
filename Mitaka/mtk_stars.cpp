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
#include <windows.h>
#include  "mitaka.h"
#include  "mtk_mode.h"
#include  "mtk_gl_common.h"
#include  "mtk_stars.h"
#include  "mtk_switches.h"
#include  "mtk_font.h"
#include  "mtk_text.h"
#include  "mtk_draw_struct.h"
#include  "mtk_colors.h"
#include  "mtk_solar_system.h"
#include  "../common/astro/constellation.h"
#include  "../common/astro/star_name.h"
#include  "../common/math/random.h"
#include  "../common/text/string_util.h"
#include  "mtk_strings.h"
#include  "mtk_data_file.h"
#include  "mtk_selection.h"
#include  <string>







//-----------
//  Externs
//-----------
extern float ini_obj_z;
float	calc_alpha(const float r1, const float r2, const float r, const float fac);


//-------------
//  Constants
//-------------
const float  plx_err_thr = 0.575f;	// �g�p����f�[�^�̑��Ό덷�̍ő�l

//-----------
//  Objects
//-----------
static vector<mtk_star>		vStars;			// ���̃e�[�u��
static map<long, long>		HIP_table;		// HIP �� vStars �̃C���f�b�N�X�̎Q�ƃe�[�u��
static vector<star_name>	StarNames;		// ���̌ŗL���̃e�[�u��
static vector<string>		StarNames2;		// �n�O�f���������̖��O

static set<int>				HippExoplanets;	// �n�O�f���������̃��X�g
//-- Luminosity reference
static float  g_L_ref		= L_ref_ini;		// ����~�m�V�e�B�[
static float  g_log_L_ref0;
//---  ���̕\���p���  ---
static star_picture  STP(2000, 40000, 16, 255);
static vector<star>  STARIF;
GLuint  texStar;
//
float StarImagePowIndex;
float StarImagePowIndexDef = 0.3;

fast_pow  pow_for_stars;



//-----------------
//  Init fast pow
//-----------------
void
init_pow_for_stars()
{
	pow_for_stars.init(float(StarImagePowIndex));
}



//--------------
//  Make names
//--------------
string
make_Bayer_name(int BayerID, int BayerSub, int FramsteedNo, char ADScomp, const string& szCnst)
{
	//-- �o�C�G�����A�t�����X�`�[�h�L���̕\������������
	std::string name;

	if (BayerID != -1) {
		const char* szBayer = get_Greek_letter_UTF8(BayerID);
		char szBayer2[10];

		name = string(szBayer);
		
		if (BayerSub != -1)		{ name = name + "\\S" + to_string(BayerSub); }
		if (FramsteedNo != -1)	{ name = to_string(FramsteedNo) + name; }
		name = name + szCnst;
	}
	else if (FramsteedNo != -1) {
		name = to_string(FramsteedNo) + szCnst;
	}

	if (name != "") {
		if (ADScomp != ' ')		{ name = name + ADScomp; }
	}

	return name;
}



//  ���̖��O�̃e�L�X�g���A�b�v�f�[�g����
void
mtk_star::make_name(char _swStar, char _swCnst)
{
	std::string szCnst;
	int _BayerID, _FramsteedNo;
	float _w, _h;


	//  Bayer name
	_BayerID = (_swStar & STAR_BAYER) ? BayerID : -1;
	_FramsteedNo = (_swStar & STAR_FRAMSTEED) ? Framsteed : -1;
	szCnst = (_swCnst & CNST_BND) ? "" : get_constellation_abbr(cnstID);

	name_S = make_Bayer_name(_BayerID, BayerSub, _FramsteedNo, ADScomp, szCnst);



	bNameLarge = (_swStar & STAR_NAME) && is_defined_key(key) && (!(_swStar & STAR_NAME_SELECT) || is_selected(key));
	bNameSmall = (name_S != "");
	label_width = 0.0;


	if (bNameLarge) {
		name_L = get_string(key);
		get_text_size(name_L, _w, _h);
		label_width += _w;
	}
	else {
		name_L = "";
	}

	if (bNameSmall) {
		if (bNameLarge) {
			name_S = "(" + name_S + ")";
		}
		get_text_size(name_S, _w, _h);
		label_width += 4.0 / 5.0 * _w;
	}
	else {
		name_S = "";
	}
}



// ���ׂĂ̐��̖��O�̃e�L�X�g���A�b�v�f�[�g����
void
make_star_name_text_2(char _swStar, char _swCnst)
{
	for (vector<mtk_star>::iterator it = vStars.begin(); it != vStars.end(); ++it) {
		it->make_name(_swStar, _swCnst);
	}
}





//-------------
//  Load Data
//-------------
int
read_a_Hipparcos_star(FILE* fp, mtk_star& st)
{
	long HIP;
	float RAdeg, DEdeg;
	float Plx, err_Plx;
	float pmRA, pmDE;
	float Vmag, B_V;
	char SpcType, SpcIdx2;
	bool bDouble;
	short Framsteed;
	char BayerID, BayerSub;
	char CnstID;
	char ADScomp;

	//--- Read one line ---
	int count;
	count = fread(&HIP,	sizeof(HIP),		1, fp);
	if (count < 1)  return FILE_END;

	fread(&RAdeg,		sizeof(RAdeg),		1, fp);
	fread(&DEdeg,		sizeof(DEdeg),		1, fp);
	fread(&Plx,			sizeof(Plx),		1, fp);
	fread(&err_Plx,		sizeof(err_Plx),	1, fp);
	fread(&pmRA,		sizeof(pmRA),		1, fp);
	fread(&pmDE,		sizeof(pmDE),		1, fp);
	fread(&Vmag,		sizeof(Vmag),		1, fp);
	fread(&B_V,			sizeof(B_V),		1, fp);
	fread(&SpcType,		sizeof(SpcType),	1, fp);
	fread(&SpcIdx2,		sizeof(SpcIdx2),	1, fp);
	fread(&bDouble,		sizeof(bDouble),	1, fp);
	fread(&Framsteed,	sizeof(Framsteed),	1, fp);
	fread(&BayerID,		sizeof(BayerID),	1, fp);
	fread(&BayerSub,	sizeof(BayerSub),	1, fp);
	fread(&CnstID,		sizeof(CnstID),		1, fp);
	fread(&ADScomp,		sizeof(ADScomp),	1, fp);
	
	//--- Error in parallax
	// �덷�����e�͈͂��傫���ꍇ�̓X�L�b�v����i�f�l�u0.57)
	if (fabs(err_Plx) > plx_err_thr * Plx) {
		return FILE_SKIP;
	}

	//--- Distance  (in pc)
	double distance = 1.0 / (Plx * 1e-3);

	//--- Position
	const double D2R = M_PI/180.0;
	const double alp = D2R * RAdeg;
	const double dlt = D2R * DEdeg;
	const double cosd = cos(dlt);
	vector3d pos = distance * vector3d(cosd*cos(alp), cosd*sin(alp), sin(dlt));
	
	pos = M_R2E * pos;


	//--- Absolute Magnitude (in mag)
	float Mag;
	Mag = Vmag + 5.0 - 5.0 * log10(distance);

	//--- Luminosity
	const float L = pow(10.0, -0.4*Mag);


	//--- Key
	string key, key2;
	key  = "HIP_" + to_string(HIP);
	key2 = key + "_BAYER";


	//--- Set
	st.set_key(key);
	st.set_pos(pos);
	st.set_L(L);
	st.set_B_V(B_V);
	st.set_spectrum_type(SpcType, SpcIdx2);
	st.set_HIP(HIP);
	st.set_double(bDouble);
	st.set_name_info(CnstID, BayerID, BayerSub, Framsteed, ADScomp);
	st.bPlanet = false;

	return  FILE_SUCCESS;
}

int
read_a_star_with_exoplanet(FILE* fp, mtk_star& st)
{
	key_data kd;
	float RAdeg, DEdeg, distance;
	float Vmag, B_V;
	int BinFlg;
	bool bDouble;


	kd = read_a_key_data(fp);
	if (kd.key == "") {
		return FILE_END;
	}


	RAdeg = stof(kd.data[0]);
	DEdeg = stof(kd.data[1]);
	distance = stof(kd.data[2]);
	B_V  = stof(kd.data[3]);
	Vmag = stof(kd.data[4]);
	BinFlg = stoi(kd.data[5]);
	bDouble = (BinFlg != 0);


	//--- Position
	const double D2R = M_PI / 180.0;
	const double alp = D2R * RAdeg;
	const double dlt = D2R * DEdeg;
	const double cosd = cos(dlt);
	vector3d pos = double(distance) * vector3d(cosd*cos(alp), cosd*sin(alp), sin(dlt));

	pos = M_R2E * pos;


	//--- Absolute Magnitude (in mag)
	float Mag;
	Mag = Vmag + 5.0 - 5.0 * log10(distance);

	//--- Luminosity
	const float L = pow(10.0, -0.4*Mag);


	//--- Set
	st.set_key(kd.key);
	st.set_pos(pos);
	st.set_L(L);
	st.set_B_V(B_V);
	st.set_spectrum_type(0, 0);
	st.set_HIP(-1);
	st.set_double(bDouble);
	st.set_name_info(-1, -1, 0, -1, 0);
	st.bPlanet = true;

	return  FILE_SUCCESS;
}



bool
load_Hipparcos_data_2(FILE* fp)
{
	mtk_star st;

	//--  �P���f�[�^�̓ǂݍ���  --
	vStars.clear();
	while (true) {
		int res = read_a_Hipparcos_star(fp, st);
		if (res == FILE_FAILED)  break;
		if (res == FILE_SKIP) continue;
		vStars.push_back(st);
	}

	//--  HIP �Q�ƃe�[�u���̍쐬  --
	HIP_table.clear();
	for (int i=0; i<vStars.size(); i++) {
		const mtk_star& st = vStars[i];
		HIP_table.insert( pair<long,long>(st.get_HIP(), i) );
	}

	return  true;
}

// �n�O�f���������̃��X�g�̓ǂݍ��� (HIP star)
bool
load_exoplanet_list(const char* filename)
{
	FILE* fp;
	if ((fp=fopen(filename, "rt")) == NULL)  return false;

	HippExoplanets.clear();
	while (true) {
		char buf[100];
		char* rs;
		rs = fgets(buf, 100, fp);
		if (rs == NULL)  break;

		int HIP;
		int count;
		count = sscanf(buf, "%d", &HIP);
		if (count != 1)  continue;

		HippExoplanets.insert(HIP);
	}
	fclose(fp);


	// �f���t���O���Z�b�g
	for (int i = 0; i < vStars.size(); i++) {
		mtk_star &st = vStars[i];
		set<int>::const_iterator it = HippExoplanets.find(st.get_HIP());
		if (it != HippExoplanets.end())  {
			st.bPlanet = true;
		}
	}


	return true;
}


// �n�O�f���������̃f�[�^�̓ǂݍ��� (HIP star�ȊO)
bool
load_stars_with_exoplanet(FILE* fp)
{
	//--  �n�O�f���������̒ǉ��f�[�^
	while (true) {
		mtk_star st;
		int res = read_a_star_with_exoplanet(fp, st);
		if (res == FILE_FAILED)  break;
		if (res == FILE_SKIP) continue;
		vStars.push_back(st);
	}

	return  true;
}



//-------
//  HIP
//-------
bool
is_HIP_present(int HIP)
{
	map<long, long>::const_iterator it = HIP_table.find(HIP);
	if (it == HIP_table.end())  return false;
	return  true;
}

int
get_star_index_by_HIP(int HIP)
{
	return  HIP_table[HIP];
}

//--------
//  Size
//--------
int
get_stars_num()
{
	return  vStars.size();
}

//------------
//  Position
//------------
vector3d
get_star_position(int idx)
{
	return  vStars[idx].get_pos();
}

vector3d
get_star_position_by_HIP(int HIP)
{
	return  get_star_position( get_star_index_by_HIP(HIP) );
}


//
mtk_star&
get_star(int idx)
{
	return  vStars[idx];
}

const mtk_star&
get_star_by_HIP(int HIP)
{
	return  get_star( get_star_index_by_HIP(HIP) );
}


//-------------------
//  ����~�m�V�e�B�[
//-------------------
float
get_L_ref()
{
	return g_L_ref;
}

void
set_L_ref(float L_ref)
{
	g_L_ref = L_ref;
}

float
get_log_L_ref()
{
	return g_log_L_ref0;
}



//-----------------------
//  ���̃e�N�X�`���̏�����
//-----------------------
void
init_star_textures()
{
	image img;
	glGenTextures(1, &texStar);
	STP.create_texture_image(img, 128, 0.35, 4.0);
	set_texture(texStar, img);
}

const star_picture&
get_star_picture()
{
	return  STP;
}



//--------
//  Draw
//--------
void
draw_a_star(const mtk_draw_struct& ds, const vector3f& rpos, float Rb, const RGBf& RGBcr, const int idx)
{
	const mtk_star& st = get_star(idx);

	vector3f r, dx, dy;
	if (ds.bRotateBillboard) {
		dx = Rb * st.eR;
		dy = Rb * st.eU;
	}
	else {
		dx = Rb * ds.eRf;
		dy = Rb * ds.eUf;
	}
	float u1, u2;
	u1 = st.u1;
	u2 = st.u2;


	glBegin(GL_QUADS);
	r = rpos + dx;
	glTexCoord2f(u2, 1);		glVertex3fv(r.get_pointer());
	r = rpos + dy;
	glTexCoord2f(u1, 1);		glVertex3fv(r.get_pointer());
	r = rpos - dx;
	glTexCoord2f(u1, 0);		glVertex3fv(r.get_pointer());
	r = rpos - dy;
	glTexCoord2f(u2, 0);		glVertex3fv(r.get_pointer());
	glEnd();
}



void
draw_a_star_TempIdx(const mtk_draw_struct& ds, const vector3f& rpos, float sz, const RGBf& RGBcr, const int TempIdx, const float af0)
{
	if (sz <= 1.0)  return;		//***


	const star_picture& sp = get_star_picture();
	const int TempN = sp.get_TempN();


	float du, u1, u2;
	du = 1.0f / TempN;
	u1 = du*TempIdx;
	u2 = du*(TempIdx + 1);


	float R;
	R = rpos.norm();



	float fac = R / get_screen_z();
	float _sz;
	_sz = sz * fac * get_screen_w() / 180.0;
	_sz *= 0.2 * 1.333;


	vector3f r, dx, dy;
	//**** �r���{�[�h����]����ꍇ�́H
	dx = _sz * ds.eRf;
	dy = _sz * ds.eUf;


	glColor3f(af0, af0, af0);

	glBegin(GL_QUADS);
	r = rpos + dx;
	glTexCoord2f(u2, 1);		glVertex3fv(r.get_pointer());
	r = rpos + dy;
	glTexCoord2f(u1, 1);		glVertex3fv(r.get_pointer());
	r = rpos - dx;
	glTexCoord2f(u1, 0);		glVertex3fv(r.get_pointer());
	r = rpos - dy;
	glTexCoord2f(u2, 0);		glVertex3fv(r.get_pointer());
	glEnd();
}



void
draw_a_star_TempIdx_2(const mtk_draw_struct& ds, const vector3f& rpos, float sz, const int TempIdx, const float af0)
{
	//if (sz <= 1.0)  return;		//***

	const star_picture& sp = get_star_picture();
	const int TempN = sp.get_TempN();

	//----------------------
	//  Textured billboard
	//----------------------
	vector3f  r, dx, dy;
	dx = sz * ds.eRf;
	dy = sz * ds.eUf;

	/*
	if (ds.bRotateBillboard) {
	dx = sz * st.eR;
	dy = sz * st.eU;
	}
	else {
	dx = sz * ds.eRf;
	dy = sz * ds.eUf;
	}
	*/

	float du, u1, u2;
	du = 1.0f / TempN;
	u1 = du*TempIdx;
	u2 = du*(TempIdx + 1);


	glBegin(GL_QUADS);
	r = rpos + dx;
	glTexCoord2f(u2, 1);		glVertex3fv(r.get_pointer());
	r = rpos + dy;
	glTexCoord2f(u1, 1);		glVertex3fv(r.get_pointer());
	r = rpos - dx;
	glTexCoord2f(u1, 0);		glVertex3fv(r.get_pointer());
	r = rpos - dy;
	glTexCoord2f(u2, 0);		glVertex3fv(r.get_pointer());
	glEnd();
}



//------------------
//  ���̃}�[�N��\��
//------------------
void
draw_star_mark(const vector3f& xyz, const float sz)
{
	glDisable(GL_TEXTURE_2D);

	glPushMatrix();
		glTranslatef( xyz.x, xyz.y, xyz.z );
		
		// �ʂ̌�����ݒ肷��
		float r;
		r = toward_eye_pos();
		float fac = r/get_screen_z() / 2.0*0.6;
		float _sz;
		_sz = sz * fac * get_screen_w() / 180.0;
		_sz += 2 * get_screen_w() / 180.0;

		glScalef(_sz,_sz,1);
		glBegin(GL_LINE_STRIP);
			glVertex3f(1, 0, 0);
			glVertex3f(0, 1, 0);
			glVertex3f(-1, 0, 0);
			glVertex3f(0, -1, 0);
			glVertex3f(1, 0, 0);
		glEnd();
	glPopMatrix();

	glEnable(GL_TEXTURE_2D);
}



//----------------
//  �A���n���}�[�N
//----------------
void
draw_double_star_mark(const mtk_draw_struct& ds, const vector3f& xyz, const mtk_star& st, const float sz, const float alpha)
{
	if (!st.is_double())  return;
	glColor4f(0,1,0,alpha);
	glLineWidth( 0.5 * ds.line_width_fac / (ds.rad_per_pixel / 0.001) );
	draw_star_mark(xyz, sz);
}

//------------------------
//  �n�O�f�����������}�[�N
//------------------------
void
mark_stars_with_planets(const mtk_draw_struct& ds, const vector3f& xyz, const mtk_star& st, const float sz0, const float alpha)
{
	if (!st.bPlanet)  return;

	float sz = sz0;
	if (sz < 1) sz=1;
	glColor4f(1,0.9,0,alpha);
	glLineWidth( 1.5 * ds.line_width_fac / (ds.rad_per_pixel / 0.001) );
	draw_star_mark(xyz, sz);
}







