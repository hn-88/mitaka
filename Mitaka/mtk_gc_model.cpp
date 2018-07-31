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
#include  <windows.h>
#include  <gl/gl.h>
#include  "mtk_gc_model.h"
#include  "mtk_globular_clusters.h"
#include  "stars.h"
#include  "mtk_stars.h"
#include  "mtk_objects.h"




//*********
extern GLuint  texStar;
//**********




//--------------------------
//  Globular Cluster Model
//--------------------------
struct GC_model : public star
{
	vector3f  pos;
	float  B_V, sz;
	RGBf	rgb;
};

const int BV_N = 5;
const float GC_BV[BV_N] = {
	-0.5, 0.0, 0.5, 1.0, 1.5
};
const float GC_BV_MID[BV_N] = {
	-0.25, 0.25, 0.75, 1.25, 1.75
};

RGBf vGC_color[BV_N];
vector<GC_model>  vGCM[BV_N], vGCM_large;


//--------
//  Load
//--------
bool
load_GC_model(const directory& dir)
{
	FILE* fp;
	fp = dir.fopen("GC_model.dat", "rb");
	if (fp==NULL)  return false;

	float Lv  = 20;
	const float M5_dist = 25000 * _1ly_pc;
	const float V0 = 19.0;
	const mtk_object& obj = get_object("M13");
	const float Rh = get_globular_cluster_Rh( obj.get_index() );


	int N;
	fread(&N, sizeof(N), 1, fp);
	for (int i=0; i<BV_N; i++) {
		vGCM[i].clear();
	}
	vGCM_large.clear();

	for (int i=0; i<N; ++i) {
		GC_model gcm;
		float V, B_V;

		vector3f pos;
		fread(pos.get_pointer(), sizeof(vector3f), 1, fp);
		fread(&V,                sizeof(float),    1, fp);
		fread(&B_V,              sizeof(float),    1, fp);
		
		RGBf rgb = correct_RGB0( blackbody_color( B_V_to_Tc(B_V) ) );

		float Lrel = exp(0.4*(V0-V));
		float sz;
		if (Lrel < 1.0)  sz = 1.0;
		else			 sz = sqrt(Lrel);
		
		gcm.pos = Rh * pos;
		gcm.B_V = B_V;
		gcm.rgb = rgb;
		gcm.sz = sz;
		gcm.set_B_V(B_V);
		gcm.make_picture_cache(Lrel, get_star_picture());

		if (sz > 3) {
			vGCM_large.push_back(gcm);
		} else {
			int idx;
			for (idx=BV_N-1; idx>=0; idx--) {
				if (B_V > GC_BV[idx]) break;
			}
			vGCM[idx].push_back(gcm);
		}
	}
	fclose(fp);

	for (int i=0; i<BV_N; i++) {
		vGC_color[i] = correct_RGB0( blackbody_color( B_V_to_Tc( GC_BV[i] ) ) );
	}

	return  true;
}



//--------
//  Draw
//--------
void
draw_GC_model(const mtk_draw_struct& ds, int idx, const vector3d& rpos, const double& scl, float alp0)
{
	const float scl_min = 4;
	const float scl_max = 3000;
	const float scale = ds.scale;

	if (scale < scl_min) return;
	if (scale > 10*scl_max)  return;
	

	float af;
	af = alp0;
	if (scale > 5*scl_max) {
		af *= (10*scl_max - scale) / (5*scl_max);
	}

	float scl0 = 100;
	float sclf = scale/scl0;
	if (sclf > 1.0)	af /= sclf*sclf;
	const float af0 = 0.03;
	float af1 = af;
	float ptsz = 1.0 / (ds.rad_per_pixel / 0.001);
	if (ptsz < 1) {
		af1 *= ptsz;
		ptsz = 1;
	}

	int skp = 1;
	while (af1 < af0) { af1 += af; skp++; }

	const mtk_object& obj = get_object("M13");
	
	GLdouble MM[16];
	glGetDoublev( GL_MODELVIEW_MATRIX,  MM);
	
	
	//--
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);



	//--  �_  --
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_TEXTURE_2D);
	glPointSize( ptsz );

	//---  �J�������W�n�̎�
	vector3d  eF, eU, eR;
	vector3f  eUf;
	ds.camera.get_axes(eF, eU, eR);
	const double a = 0.1 / get_screen_z() * get_screen_w() / 180;
	eUf = vector3f( eU );



	for (int idx=0; idx<BV_N; idx++) {
		const int N = vGCM[idx].size();
		const RGBf&  rgb = vGC_color[idx];
		glColor4f(rgb.r, rgb.g, rgb.b, af1);

		if (true) {
			// QUAD

			glBegin(GL_QUADS);
			for (int i=0; i<N; i+=skp) {
				const GC_model& gcm = vGCM[idx][i];
				vector3f  rr = gcm.pos + rpos;

				//---  �l�p���W�n�̎�
				vector3f  eF2, eU2, eR2;
				float R2, R;
				R2 = norm2(rr);
				if (R2 < 1e-100)  continue;
				
				R = sqrt(R2);
				eF2 = rr / R;
				eR2 = normalize( cross_product(eF2, eUf) );
				eU2 = cross_product( eR2, eF2 );

				float sz;
				sz = a * R;

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
			glEnd();
		} else {
			glBegin(GL_POINTS);
			for (int i=0; i<N; i+=skp) {
				const GC_model& gcm = vGCM[idx][i];
				const vector3f  pos = gcm.pos + rpos;
				glVertex3fv(pos.get_pointer());
			}
			glEnd();
		}
	}

	//--  �r���{�[�h  --
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texStar);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	for (int i=0; i<vGCM_large.size(); i+=skp) {
		const GC_model& gcm = vGCM_large[i];
		const vector3f  pos = gcm.pos + rpos;

		// ���_���W�n�ł̈ʒu�x�N�g��
		float x, y, z;
		z = -(MM[2]*pos.x + MM[6]*pos.y + MM[10]*pos.z + MM[14]);
		if (z < get_z_near())		continue;
		x = MM[0]*pos.x + MM[4]*pos.y + MM[8] *pos.z + MM[12];
		y = MM[1]*pos.x + MM[5]*pos.y + MM[9] *pos.z + MM[13];

		// �X�N���[���̊p�x�ɂ��J�����O
		char bf = ds.get_frustum().cull_test(x, y, z);
		if (bf & (FRUSTUM_CULL_XY)) {
			continue;
		}
		const star_draw_cache& sdc = gcm.get_picture_cache();
		draw_a_star_TempIdx(ds, pos, sdc.sz, sdc.RGBcr, sdc.TempIdx, af1);
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
}

