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
#include  "mitaka.h"
#include  "mtk_oort_cloud.h"
#include  "../common/math/vector3.h"
#include  "../common/math/random.h"
#include  "../common/astro/astro_const.h"
#include  "mtk_objects.h"
#include  "mtk_calc_alpha.h"
#include  "mtk_label.h"
#include  "mtk_colors.h"
#include  "mtk_switches.h"
#include  "mtk_solar_system.h"
#include  "mtk_sun.h"
#include  "mtk_draw_common.h"




const float RO1 = 2000*_1AU_pc;		// �I�[���g�̉_�̔��a�i�����j
const float RO2 = 20000*_1AU_pc;	// �i���ԁj
const float RO3 = 200000*_1AU_pc;	// �i�O���j
const float RO  = RO2;
const float RK  = 50*_1AU_pc;		// �J�C�p�[�x���g�̓����̔��a

static std::vector<vector3f>		PT;		// �_�̈ʒu
//const int PT_N = 40000;
const int PT_N = 10000;
//const int PT_N = 8000;





//----------------------------
//  �I�[���g�̉_�̃��f���̏�����
//----------------------------
void
init_Oort_cloud()
{
	int PT_N1 = PT_N;


	//-- init random seed
	rndInit(5);


	//--
	const float alp = 2;  // �����ׂ̂�
	const float bet = 4;  // �O���ׂ̂�

	float ratio;
	ratio = (bet-1.0) / (alp-1.0) * (1.0 - pow(RO1/RO2, -alp+1)) / (pow(RO3/RO2, -bet+1) - 1);
	const float P_out = 1.0 / (1.0 + ratio);

	//--  Oort cloud
	for (int i=0; i<PT_N1; ++i) {
		float phi = rndUni(0,2*pi); // isotropic
		float mu  = rndUni(-1,1);
		float x,y,z,rho, R;

		double rn = rnd();
		if (rnd() > P_out) {
			// inner region
			R = RO1 * pow(1.0 - rn * ( 1.0 - pow(double(RO2/RO1), -alp+1.0)), 1.0/(-alp+1.0));
		} else {
			// outer region
			R = RO2 * pow(1.0 - rn * ( 1.0 - pow(double(RO3/RO2), -bet+1.0)), 1.0/(-bet+1.0));
		}

		z   = R * mu;
		rho = R * sqrt(1.0-mu*mu);
		x   = rho*cos(phi);
		y   = rho*sin(phi);
		
		PT.push_back(vector3f(x,y,z));
	}
}




//------------
//  Register
//------------
void
register_object_Oort_cloud()
{
	mtk_object  obj;

	//  register object
	obj.init(OBJ_TYPE_OORT_CLOUD, 0, -0.1f, false);
	int obj_idx = register_object(obj, "LABEL_OORT_CLOUD");

	// register label
	float thc = 10 * D2R;
	add_label("LABEL_OORT_CLOUD", obj_idx, MTK_COLOR_NAME_OBJECT, 1.0f, get_object_name_size(), 0.2*RO, thc, false);
}




double
get_Oort_cloud_radius()
{
	return  1.1 * RO;
}



//---------
//  Label
//---------
bool
is_Oort_cloud_label_visible()
{
	return  bOortCloud && test_disp_switch(DISPSW_PLANET, DISPM_NAME);
}


float
get_Oort_cloud_label_alpha(const mtk_draw_struct& ds)
{
	const float  scl = 0.2f;
	const vector3d  rpos = get_sun_position() - ds.camera.get_pos();
	float  Dth = scl * 2.0f * RO / rpos.norm();

	return  calc_alpha_by_apparent_diameter( Dth );
}



//----------------
//  Prepare draw
//----------------
const double oc_lc1 = 0.005;
const double oc_lc2 = 0.03;

bool
prepare_draw_Oort_cloud(const mtk_draw_struct& ds, int idx, double* pRb, double* pRh)
{
	//--  Visible
	if (!bOortCloud)  return false;
	if (3*RO3/ds.scale < 0.01)  return false;
	if (ds.cnorm < oc_lc1)  return  false;

	//--  Radius
	*pRb = *pRh = RO;
	return  true;
}



//--------
//  Draw
//--------
void
draw_Oort_cloud(const mtk_draw_struct& ds, int idx, const vector3d& rpos, const double& scl)
{
	const float scale = ds.scale;
	const RGBf& col = get_color(MTK_COLOR_OORT);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	float  Dth0 = 40 * D2R;
	float  Dth  = RO / rpos.norm();
	float  a = Dth / Dth0;
	float  af0  = (a > 1)  ?  1  :  a*a;

	const double lc = ds.cnorm;


	if (lc < oc_lc2) {
		af0 *= (lc - oc_lc1) / (oc_lc2 - oc_lc1);
	}

	af0 *= 0.3;

	int N = PT.size();
	int skip = 1;
	while (af0 < 0.05f) {
		skip *= 2;
		af0 *= 2;
	}
	if (skip > N/8) return;


	glColor4f(col.r, col.g, col.b, af0);
	

	
	if (true) {
		const double a = 0.002;


		glBegin(GL_QUADS);
		if (ds.bRotateBillboard) {
			const vector3f eUf = ds.eUf;

			for (int i = 0; i < N; i += skip) {
				const vector3f&  xyz = PT[i];
				vector3f  rr = xyz + rpos;

				float R2, R;
				R2 = norm2(rr);
				if (R2 < 1e-38)  continue;
				R = sqrt(R2);

				//---  �l�p���W�n�̎�
				vector3f  eF2, eU2, eR2;
				eF2 = rr / R;
				eR2 = normalize(cross_product(eF2, eUf));
				eU2 = cross_product(eR2, eF2);

				draw_point(ds, a, R, rr, eR2, eU2);
			}
		}
		else {
			for (int i = 0; i < N; i += skip) {
				const vector3f&  xyz = PT[i];
				vector3f  rr = xyz + rpos;



				float R2, R;
				R2 = norm2(rr);
				if (R2 < 1e-38)  continue;
				R = sqrt(R2);

				draw_point(ds, a, R, rr, ds.eRf, ds.eUf);
			}
		}
		glEnd();

	} else {
		//glPointSize(sz);
		glBegin(GL_POINTS);
		for (int i=0; i<N; i+=skip) {
			const vector3f&  xyz = PT[i];
			vector3f  rr = xyz + rpos;
			glVertex3fv(rr.get_pointer());
		}
		glEnd();
	}

	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
