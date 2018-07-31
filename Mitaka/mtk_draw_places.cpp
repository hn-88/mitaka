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
#include  "mtk_draw_places.h"
#include  "../common/graphics/gl_common.h"
#include  "mtk_gl_common.h"
#include  "mtk_colors.h"
#include  "mtk_strings.h"
#include  "mtk_font.h"


const float D2Rf = M_PI/180.0f;



float LocationNameSize;



void
prepare_3Dtext_place_name(const vector3f& rc, const vector3f& rp, float w, const float sz, const float screen_w, const float screen_z, bool bHorizontalMode)
{
	double M[16];

	// �J�������W�n�ł̈ʒu���W���擾
	glGetDoublev(GL_MODELVIEW_MATRIX, M);

	vector3f dr = rp - rc;
	vector3f eF, eU, eR;

	quaternionf ori1, ori2, ori;

	// �����ꍇ
	eF = normalize(-dr);
	eU = get_eU_of_base_on_local_camera();	// �J�������W�n�ł̊���W�n�̎�
	eR = normalize(cross_product(eU, -eF));
	eU = normalize(cross_product(-eF, eR));
	ori1 = axes_to_quaternion(eF, eU);

	if (bHorizontalMode) {
		// �߂��ꍇ
		eF = normalize(rp);
		eU = normalize(dr);
		
		vector3f _eR = cross_product(eU, -eF);
		float nrm = _eR.norm();
		const float c1 = 0.2;
		const float c2 = 0.4;
		if (nrm < c1) {
			w = 1;
		} else if (nrm < c2) {
			w = 1 - (1-w) * ((nrm - c1) / (c2 - c1));
		}

		eR = normalize(_eR);
		eU = normalize(cross_product(-eF, eR));
		ori2 = axes_to_quaternion(eF, eU);

		// ���ʐ��`���
		ori = slerp(ori2, ori1, w);
	}
	else {
		ori = ori1;
	}

	quaternion_to_axes(ori, eF, eU, eR);
	M[0] = eR.x;
	M[1] = eR.y;
	M[2] = eR.z;
	M[3] = 0;
	M[4] = eU.x;
	M[5] = eU.y;
	M[6] = eU.z;
	M[7] = 0;
	M[8] = -eF.x;
	M[9] = -eF.y;
	M[10] = -eF.z;
	M[11] = 0;
	glLoadMatrixd(M);


	// �T�C�Y�̕��������炷
	float fac0 = rp.norm() / screen_z;
	float scl2 = sz*fac0 * screen_w / 180.0;
	glScalef(scl2, scl2, scl2);

}




vector3f
get_pos_from_lon_lat(float lon, float lat)
{
	float th, phi, x, y, z, rho;
	th = D2Rf * (90.0f - lat);
	phi = D2Rf * lon;
	z = cos(th);
	rho = sin(th);
	x = rho * cos(phi);
	y = rho * sin(phi);
	return  vector3f(x, y, z);
}



void
draw_places_on_TopoSphere_sub(const Places &places, const TopoSphere* tp, float screen_w, float screen_z, float ltf, bool bHorizontalMode)
{
	const float thr1    = 1.5 * D2Rf;
	const float thr2    = 2.5 * D2Rf;
	const float thr1sqr = thr1*thr1;
	const float thr2sqr = thr2*thr2;
	const double fac = 0.0005;   //***
	const float R0 = (tp != 0) ? tp->get_R0() : 1.0f;

	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);


	float M[16];
	vector3f  rc;
	glGetFloatv( GL_MODELVIEW_MATRIX, M);
	rc.x = M[12];
	rc.y = M[13];
	rc.z = M[14];

	//rc -= get_base_origin_on_local_camera();	// ���_�̂��炵�����ǂ�

	const float R2 = M[0]*M[0] + M[1]*M[1] + M[2]*M[2];


	// ���`��Ԃ̏d��
	float w;
	//w = 3 * (rc.norm() - sqrt(R2)) / rc.norm();
	w = 3.5 * (rc.norm() - sqrt(R2)) / rc.norm();
	if (w > 1) { w = 1; }
	if (w < 0) { w = 0; }

	float ww = 0.3;
	w = (w - ww) / (1 - ww);
	if (w < 0) { w = 0; }




	for (int i=0; i<places.size(); i++) {
		const PlaceItem&  pli = places[i];


		vector3f  r = (tp != 0) ? tp->get_pos(pli.lon, pli.lat) : get_pos_from_lon_lat(pli.lon, pli.lat);
		r *= 1.002f;


		glPushMatrix();
			glPushMatrix();
				glTranslatef(r.x, r.y, r.z);
				glGetFloatv( GL_MODELVIEW_MATRIX, M);
			glPopMatrix();
			
			vector3f rp(M[12], M[13], M[14]);
			rp -= get_base_origin_on_local_camera();	// ���_�̂��炵�����ǂ�

			const float L2 = rp.norm2();
			float th1_2 = R2 * pli.R1*pli.R1 / L2;

			if (th1_2 > thr1sqr) {
				float af = (th1_2 - thr1sqr) / (thr2sqr - thr1sqr);
				if (af > 1.0f)  af = 1.0f;


				// �߂�����ꍇ�t�F�[�h�A�E�g����
				float w2 = L2 / R2;
				if (w2 < 0.01) {
					af *= w2/(0.01*0.01);
				}


				if (af > 0.05) {

					//-- ���O
					if (pli.bDispLabel) {
						r *= (1+fac);
						glTranslatef(r.x, r.y, r.z);


						/**** �����̉����Ɋ֌W�������� *****/
						/*
						if (fnt.get_polygon_mode()) {
							const RGBAf& col = pli.col1;
							glColor4f(col.rgb.r, col.rgb.g, col.rgb.b, col.a*af);
						}
						else {
							const RGBAf& col = pli.col2;
							glColor4f(col.rgb.r, col.rgb.g, col.rgb.b, col.a*af);
						}
						*/

						//**** ���� col1 �����ŏ���
						const RGBAf& col = pli.col1;
						glColor4f(col.rgb.r, col.rgb.g, col.rgb.b, col.a*af);


						float fnt_sz = LocationNameSize * pli.size * ltf / 1.2;
						prepare_3Dtext_place_name(rc, rp, w, fnt_sz, screen_w, screen_z, bHorizontalMode);


						const std::string& str = get_string(pli.key);


						switch (pli.align) {
						case 'L':	draw_text_right(str);						break;
						case 'R':
						default:	glTranslatef(0.2, 0, 0);	draw_text(str);	break;
						}
					}
				}
			}
		glPopMatrix();
	}
}





void
draw_places_on_TopoSphere(const Places& places, const TopoSphere* tp, float screen_w, float screen_z, float ltf, bool bHorizontalMode)
{
	draw_places_on_TopoSphere_sub(places, tp, screen_w, screen_z, ltf, bHorizontalMode);
	/*
	// �����̂��߁A������ɕ����ĕ`��
	fnt.set_polygon_mode(true);
	draw_places_on_TopoSphere_sub(places, tp, screen_w, screen_z, ltf, bHorizontalMode);
	fnt.set_polygon_mode(false);
	draw_places_on_TopoSphere_sub(places, tp, screen_w, screen_z, ltf, bHorizontalMode);
	fnt.set_polygon_mode(true);
	*/
}







//--------------
//  �n�\�|�C���^
//--------------
void
draw_pointer_on_sphere(float screen_w, float screen_z, float lon, float lat, float ltf)
{
	const float thR = tan(20.0f * D2Rf);		// �|�C���^��\�����鎞�̋��̎����a
	const float thR2 = thR * thR;

	// �����̔���
	GLfloat MM[16];
	glGetFloatv( GL_MODELVIEW_MATRIX,  MM);

	const float L2  = MM[12]*MM[12] + MM[13]*MM[13] + MM[14]*MM[14];	// ���S�܂ł̋���
	const float R2  = MM[0] *MM[0]  + MM[1] *MM[1]  + MM[2] *MM[2];		// ���a
		
	//-- ���̒��̏ꍇ
	float L02 = L2 - R2;
	if (L02 <= 0.0f)	{ return; }

	//--- �����ꍇ�́A����\��
	if (R2/L2 < thR2)	{ return; }


	glPushMatrix();
		float af = (R2/L2)/thR2 - 1.0f;
		if (af > 1.0f) af = 1.0f;

		vector3f  r0(MM[12], MM[13], MM[14]);

		const double th  = D2R * (90.0 - lat);
		const double phi = D2R * lon;
		const double rho = sin(th);
		vector3f  pos(rho*cos(phi), rho*sin(phi), cos(th));
		//pos *= 1.004;

		glTranslatef(pos.x, pos.y, pos.z);

		glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_DEPTH_TEST);

		// �N���X�}�[�N�̕`��
		prepare_3Dtext_2(r0, 0, 4.0*ltf/1.2, screen_w, screen_z, false, true);

		glColor4f(0,1,0,af);
		glLineWidth(2.0f);
		const float d = 0.3;
		glBegin(GL_LINES);
			glVertex3f(-d,0,0);
			glVertex3f( d,0,0);
			glVertex3f(0,-d,0);
			glVertex3f(0, d,0);
		glEnd();

		// �o�x�E�ܓx��\��
		char szText[100];
		char EW = (lon >= 0.0f)  ?  'E' : 'W';
		char NS = (lat >= 0.0f)  ?  'N' : 'S';
		
		sprintf(szText, "%3.1f%c, %3.1f%c", fabs(lon), EW, fabs(lat), NS);
		glPushMatrix();
			glTranslatef(1,0,0);
			draw_text(szText);
		glPopMatrix();

		glPopAttrib();

	glPopMatrix();

}



void
draw_pointer_on_TopoSphere(const TopoSphere* tp, float screen_w, float screen_z, float lon, float lat, float ltf)
{
	if (tp==0 || !tp->is_loaded()) {
		glPushMatrix();
			draw_pointer_on_sphere(screen_w, screen_z, lon, lat, ltf);
		glPopMatrix();
		return;
	}


	const float thR = tan(20.0f * D2Rf);		// �|�C���^��\�����鎞�̋��̎����a
	const float thR2 = thR * thR;

	//����
	GLfloat MM[16];
	glGetFloatv( GL_MODELVIEW_MATRIX,  MM);

	// �����̔���
	const float L2  = MM[12]*MM[12] + MM[13]*MM[13] + MM[14]*MM[14];	// ���S�܂ł̋���
	const float R2  = MM[0] *MM[0]  + MM[1] *MM[1]  + MM[2] *MM[2];		// ���a
		
	//-- ���̒��̏ꍇ
	float L02 = L2 - R2;
	if (L02 <= 0.0f)	return;

	//--- �����ꍇ
	if (R2/L2 < thR2)	return;

	glPushMatrix();
		float af = (R2/L2)/thR2 - 1.0f;
		if (af > 1.0f) af = 1.0f;

		vector3f  r0(MM[12], MM[13], MM[14]);	// ���̒��S�̈ʒu
		vector3f  pos = tp->get_pos(lon, lat);
		float elv = float(tp->get_elevation(lon, lat));

		glTranslatef(pos.x, pos.y, pos.z);

		glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_DEPTH_TEST);

		// �N���X�}�[�N�̕`��
		prepare_3Dtext_2(r0, 0, 4.0*ltf/1.2, screen_w, screen_z, false, true);

		glColor4f(0,1,0,af);
		glLineWidth(2.0f);
		const float d = 0.3;
		glBegin(GL_LINES);
			glVertex3f(-d,0,0);
			glVertex3f( d,0,0);
			glVertex3f(0,-d,0);
			glVertex3f(0, d,0);
		glEnd();

		// �o�x�E�ܓx��\��
		char szText[100];
		char EW = (lon >= 0.0f)  ?  'E' : 'W';
		char NS = (lat >= 0.0f)  ?  'N' : 'S';
		
		sprintf(szText, "%3.1f%c, %3.1f%c", fabs(lon), EW, fabs(lat), NS);
		glPushMatrix();
			glTranslatef(1,0,0);
			draw_text(szText);
		glPopMatrix();

		sprintf(szText, "%4.1fm", elv);
		glPushMatrix();
			glTranslatef(1,-1,0);
			draw_text(szText);
		glPopMatrix();
		glPopAttrib();
	glPopMatrix();
}




