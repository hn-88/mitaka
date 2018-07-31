/******************************************************************************

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

�ȉ��ɒ�߂�����ɏ]���A�{�\�t�g�E�F�A����ъ֘A�����̃t�@�C���i�ȉ��u�\�t�g�E�F
�A�v�j�̕������擾���邷�ׂĂ̐l�ɑ΂��A�\�t�g�E�F�A�𖳐����Ɉ������Ƃ𖳏��ŋ�
���܂��B����ɂ́A�\�t�g�E�F�A�̕������g�p�A���ʁA�ύX�A�����A�f�ځA�Еz�A�T�u
���C�Z���X�A�����/�܂��͔̔����錠���A����у\�t�g�E�F�A��񋟂��鑊��ɓ�����
�Ƃ������錠�����������Ɋ܂܂�܂��B

��L�̒��쌠�\������і{�����\�����A�\�t�g�E�F�A�̂��ׂĂ̕����܂��͏d�v�ȕ�����
�L�ڂ�����̂Ƃ��܂��B

�\�t�g�E�F�A�́u����̂܂܁v�ŁA�����ł��邩�Öقł��邩���킸�A����̕ۏ؂���
���񋟂���܂��B�����ł����ۏ؂Ƃ́A���i���A����̖ړI�ւ̓K�����A����ь�����N
�Q�ɂ��Ă̕ۏ؂��܂݂܂����A����Ɍ��肳�����̂ł͂���܂���B ��҂܂��͒�
�쌠�҂́A�_��s�ׁA�s�@�s�ׁA�܂��͂���ȊO�ł��낤�ƁA�\�t�g�E�F�A�ɋN���܂���
�֘A���A���邢�̓\�t�g�E�F�A�̎g�p�܂��͂��̑��̈����ɂ���Đ������؂̐����A��
�Q�A���̑��̋`���ɂ��ĉ���̐ӔC������Ȃ����̂Ƃ��܂��B
*******************************************************************************/
#ifndef  __ASTROMETRY_H__
#define  __ASTROMETRY_H__
#include  <vector>
#include  "../math/vector3.h"
#include  "../astro/astro_const.h"
using namespace std;
const char  star_spectral_type[] = {'O', 'B', 'A', 'F', 'G', 'K', 'M' };


//--- �ϊ��s�� ---
extern const matrix3d	M_E2R;		// �������W����ԓ����W�ւ̕ϊ��s��
extern const matrix3d	M_R2E;		// �ԓ����W���物�����W�ւ̕ϊ��s��
extern const matrix3d	M_R2G;		// �ԓ����W�����͍��W�ւ̕ϊ��s��
extern const matrix3d	M_G2R;		// ��͍��W����ԓ����W�ւ̕ϊ��s��
extern const matrix3d	M_E2G;		// �������W�����͍��W�ւ̕ϊ��s��
extern const matrix3d	M_G2E;		// ��͍��W���物�����W�ւ̕ϊ��s��


//------------
//  �Ԍo�E�Ԉ�
//------------
struct  RAhms {
	short	h, m;
	float	s;
};

struct  DEdms {
	bool	b_minus;	// �����id,m,s�ɂ͐�Βl������Bd=-0�Ώ��p�j
	short	d, m;
	float	s;
};

//----------------
//  class RA_DEC
//----------------
class RA_DEC
{
public:
	float  RAdeg, DEdeg;
public:
	RA_DEC() {};
	RA_DEC(const float& alp, const float& dlt) : RAdeg(alp), DEdeg(dlt) {};
	RA_DEC(const RAhms& RA, const DEdms& DEC) { set_RAhms(RA); set_DEdms(DEC); }
	//-- �����]�� --
	vector3d	get_dirc_vec() const;
	void	set_dirc_vec(const vector3d& v);
	//-- hms/dms �\�L����̕ϊ� --
	void	set_RAhms(const RAhms& RA);
	void	set_DEdms(const DEdms& DEC);
};

inline  vector3d
RA_DEC::get_dirc_vec() const
{
	const double D2R = M_PI/180.0;
	const double alp = D2R * RAdeg;
	const double dlt = D2R * DEdeg;
	const double cosd = cos(dlt);
	return  vector3d(cosd*cos(alp), cosd*sin(alp), sin(dlt));
}

inline  void
RA_DEC::set_dirc_vec(const vector3d& v)
{
	const double R2D = 180.0 / M_PI;
	double rho = sqrt(v.x*v.x + v.y*v.y);
	RAdeg = R2D * atan2(v.y, v.x);
	while (RAdeg < 0.0) { RAdeg += 360.0; }
	DEdeg = R2D * atan2(v.z, rho);
	while (DEdeg < 0.0) { DEdeg += 360.0; }
}

inline  void
RA_DEC::set_RAhms(const RAhms& RA)
{
	RAdeg = 15.0f*RA.h + 0.25f*RA.m + 0.25f/60.0f*RA.s;
}

inline  void
RA_DEC::set_DEdms(const DEdms& DEC)
{
	if (!DEC.b_minus)	{ DEdeg = DEC.d + 1.0f/60.0f * DEC.m + 1.0f/3600.0f * DEC.s; }
	else				{ DEdeg = DEC.d - 1.0f/60.0f * DEC.m - 1.0f/3600.0f * DEC.s; }
}


//--- B1950.0 ���� J2000.0 �ւ̕ϊ� ---
// �ʒu�̕ϊ��i�ŗL�^������j
RA_DEC	B1950_to_J2000(const RA_DEC& posB1950, const float& mua0, const float& mud0, const float& Ra0, const float& Plx0);	
// �ʒu�̕ϊ��i�ŗL�^���Ȃ��j
inline RA_DEC  B1950_to_J2000(const RA_DEC& posB1950)  { return B1950_to_J2000(posB1950, 0, 0, 0, 0); }

// �ŗL�^���̕ϊ�
RA_DEC	B1950_to_J2000_pm(const RA_DEC& pos1950, const RA_DEC& pm1950);


//-----------------------------------------------------
//  RA-DEC position of Galactic center/ northern pole
//  SgrA*, Yusef-Zadeh & Choate, 1999, ApJ, 518, L33
//-----------------------------------------------------
const RAhms		GC_RAhms ={ 17, 45, 40.0383 };
const DEdms		GC_DEdms={true,  -29, 00, 28.069 };
const RAhms		GN_RAhms ={ 12, 51, 24};
const DEdms		GN_DEdms={false,  27,  8,  0};
const RA_DEC	GC_RA_DEC(GC_RAhms, GC_DEdms);	// ��͒��S
const RA_DEC	GN_RA_DEC(GN_RAhms, GN_DEdms);	// ��̖͂k��
extern const vector3f  vGC, vGN, vGO;


//--------------------
//  Astrometric data
//--------------------
struct astrometric_data {
	RA_DEC		eqPos;			// Right ascension/Declination (in degree)
	float		Plx;			// ���� (in milli arc sec)
	float		e_Plx;			// �����̌덷
	float		Vmag;			// V mag (Johnson V)
	float		B_V;			// Johnson B-V colour
};


//------------------
//  Hipparcos data
//------------------
struct Hipparcos_data : public astrometric_data
{
	long		HIP;			// Identifier (HIP number)
	float		pmRA, pmDE;
	float		distance;		// ���� (in pc)
	float		Mag;			// ��Γ���
	char		SpcType;		// �X�y�N�g���^ (O,B,A,F,G,K,M)
	char		SpcIdx2;		// �X�y�N�g���^�̌�̐��� (0-9)
	bool		bDouble;		// Double/Multiple System Flag
	short		Framsteed;
	char		BayerID, BayerSub;
	char		ConstellationID;
	char		ADScomp;
};


//--------------
//  Tycho data
//--------------
struct Tycho_data : public astrometric_data
{
	long		HIP;			// Identifier (HIP number)
};


//------------
//  NGC data
//------------
struct NGC_data
{
	short	NGC_NO;				// NGC or IC number
	bool	bNGC;				// NGC? (or IC)
	RA_DEC	eqPos;
	char	type;				// object classification
	float	size;				// largest dimension (in arcmin)
	float	mag;				// magnitude visual or photograhic
	bool	bPhotographicMag;	//
};

enum {
	NGC_TY_NONE,
	NGC_TY_GALAXY,
	NGC_TY_OPEN_STAR_CLUSTER,
	NGC_TY_GLOBULAR_STAR_CLUSTER,
	NGC_TY_EMISSION_OR_REFLECTION_NEBULA,
	NGC_TY_PLANETARY_NEBULA,
	NGC_TY_CLUSTER_WITH_NEBULOSITY,
	NGC_TY_ASTERISM,
	NGC_TY_KNOT_IN_EXTERNAL_GALAXY,
	NGC_TY_TRIPLE_STAR,
	NGC_TY_DOUBLE_STAR,
	NGC_TY_SINGLE_STAR
};


//-------------
//  Star Name
//-------------
struct star_name
{
	long HIP;			// HIP number
	char flg;			// Flag
	char name_E[30];	// �p��
	char name_J[100];	// ���{��
};


//  Functuons
inline  float
B_V_to_Tc(float B_V)
{
	return  7300.0f / (B_V + 0.73f);	// color temparature, see [2] pg.124 eq.(4-75)
}



//-- Refs -------------------------------------------
//  [1] The Hipparcos Catalogue,
//        ESA, 1997, ESA SP-1200
//  [2] Astrophysical concepts,
//        M.Harwit, 1997
//---------------------------------------------------


#endif
