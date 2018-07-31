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
#include  <windows.h>
#include  <gl/gl.h>
#include  <vector>
#include  <map>
#include  "../common/astro/constellation.h"
#include  "../common/math/spline.h"
#include  "../common/graphics/color.h"
#include  "mitaka.h"
#include  "mtk_mode.h"
#include  "mtk_font.h"
#include  "mtk_text.h"
#include  "mtk_switches.h"
#include  "mtk_gl_common.h"
#include  "mtk_constellation.h"
#include  "mtk_stars.h"
#include  "mtk_colors.h"
#include  "mtk_objects.h"
#include  "mtk_strings.h"
#include  "mtk_selection.h"
using namespace std;



//-----------------
//  Constellation
//-----------------
struct constellation
{
	std::string	key;		// key
	char abbr_name[5];		// ����
	char abbr_name2[5];		// �����i���E�p�j
	bool	bSelected;		// �I�����
	float RAdeg, DEdeg;		// �T���ʒu�i�Ԍo�A�Ԉ܁F�x�P�ʁj
	vector3f	pos0;		// �T���ʒu�̓V���ʏ�̈ʒu
	vector<vector<long> >	lines_HIP;	// ��
	vector<RA_DEC>			boundary;
};





//----------------
//  �����f�[�^
//----------------
static vector<constellation>			CNST2;				// �����f�[�^

//---------
//  �����G
//---------
struct constellation_illust
{
	int cnst_ID;
	vector<vector<vector3f> >		vLine;		// �|�����C��
};
static vector<constellation_illust> vCnstIllust;		// �����G

//---------
//  �T�C�Y
//---------
float ConstellationNameSize;



//-------------------
//  �����P��ǂݍ���
//-------------------
bool
read_a_constellation(FILE* fp, constellation& cst)
{
	int count;
	unsigned char  len;
	char dummy[1024];


	//-------
	//  ���O
	//-------
	// ����
	count = fread(&len, sizeof(unsigned char), 1, fp);
	if (count != 1)  return false;
	fread(cst.abbr_name, 1, len, fp);
	cst.abbr_name[len] = '\0';
	// �����Q
	fread(&len, sizeof(unsigned char), 1, fp);
	fread(cst.abbr_name2, 1, len, fp);
	cst.abbr_name2[len] = '\0';

	// Key
	cst.key = std::string("CNST_") + cst.abbr_name;
	// Selection
	cst.bSelected = is_selected(cst.key);


	// �ȉ��̓X�L�b�v
	// �p��
	fread(&len, sizeof(unsigned char), 1, fp);
	fread(dummy, 1, len, fp);
	// �p�����L�i
	fread(&len, sizeof(unsigned char), 1, fp);
	fread(dummy, 1, len, fp);
	// ���{���i�Ђ炪�ȁj
	fread(&len, sizeof(unsigned char), 1, fp);
	fread(dummy, 1, len, fp);
	// ���{���i�����j
	fread(&len, sizeof(unsigned char), 1, fp);
	fread(dummy, 1, len, fp);




	//----------
	//  �T���ʒu
	//----------
	fread(&cst.RAdeg, sizeof(float), 1, fp);
	fread(&cst.DEdeg, sizeof(float), 1, fp);

	const float a = M_PI / 180.0f;
	float x,y,z,rho,th,phi;
	th  = a * cst.DEdeg;
	phi = a * cst.RAdeg;
	z   = sin(th);
	rho = cos(th);
	x = rho * cos(phi);
	y = rho * sin(phi);
	cst.pos0.set(x,y,z);


	//---------
	//  ������
	//---------
	unsigned char num;
	fread(&num, sizeof(unsigned char), 1, fp);
	cst.lines_HIP.clear();
	cst.lines_HIP.resize(num);
	for (unsigned char i=0; i<num; ++i) {
		vector<long> vLine;

		unsigned char lines;
		fread(&lines, sizeof(unsigned char), 1, fp);
		for (unsigned char j=0; j<lines; ++j) {
			long HIP;
			fread(&HIP, sizeof(HIP), 1, fp);
			vLine.push_back(HIP);
		}
		cst.lines_HIP.push_back(vLine);
	}


	//---------
	//  ���E��
	//---------
	//--- boundary
	short bnum;
    fread(&bnum, sizeof(bnum), 1, fp);
	cst.boundary.resize(bnum);
	for (int i=0; i<bnum; ++i) {
		RA_DEC  pos;
		fread(&pos.RAdeg, sizeof(float), 1, fp);
		fread(&pos.DEdeg, sizeof(float), 1, fp);
		pos.RAdeg *= 180.0f/12.0f;
		cst.boundary[i] = pos;
	}

	return  true;
}

bool
load_constellations(const directory& dir)
{
	FILE* fp = dir.fopen("constellation_HIP.dat", "rb");
	if (fp == NULL)  return false;

	CNST2.clear();
	while (true) {
		constellation cst;
		if (!read_a_constellation(fp, cst))  break;
		CNST2.push_back( cst );
	}

	fclose(fp);
	return  true;
}




//---------
//  �����G
//---------
bool
load_constellation_illust_line(FILE* fp, vector<vector<vector2f> >& CI)
{
	int line_num;
	fread(&line_num, sizeof(line_num), 1, fp);

	for (int i=0; i<line_num; ++i) {

		//--- �_��ǂݍ��� ---
		int pt_num;
        vector2f xy[2000];

		fread(&pt_num, sizeof(pt_num), 1, fp);
		fread(xy, pt_num*sizeof(vector2f), 1, fp);

		//--- �o�^���� ---
		vector<vector2f>  vVec2;
		for (int j=0; j<pt_num; ++j) {
			vVec2.push_back( vector2f(xy[j].x, xy[j].y) );
		}
		CI.push_back(vVec2);
	}

	return  true;
}

bool
load_constellation_illust_spline(FILE* fp, const int detail, vector<vector<vector2f> >& CI)
{
	NURBS2<float>  nbs;
	
	int spline_num;
	fread(&spline_num, sizeof(spline_num), 1, fp);

	for (int i=0; i<spline_num; ++i) {

		//--- ������ǂݍ��� ---
		int order;
		fread(&order, sizeof(order), 1, fp);
		nbs.set_K(order + 1);

		//--- �m�b�g��ǂݍ��� ---
		int knot_num;
		float knot[1000];
		fread(&knot_num, sizeof(knot_num), 1, fp);
		fread(knot, knot_num * sizeof(float), 1, fp);
		nbs.set_knots(knot_num, knot);


		//--- ����_��ǂݍ��� ---
		int pt_num;
		vector2f xy[1000];
		fread(&pt_num, sizeof(pt_num), 1, fp);
		fread(xy, pt_num*sizeof(vector2f), 1, fp);
		nbs.set_control_points(pt_num, xy);


		//--- �X�v���C����Ԃ����Đ����𐶐� ---
		int eval_num = (pt_num-1) * detail;
		vector<vector2f>  vVec2;
		float t_ini, t_fin;
		t_ini = nbs.t_ini();
		t_fin = nbs.t_fin();
		
		float t, dt;

		dt = (t_fin - t_ini) / float(eval_num);
		for (t=t_ini; t<t_fin; t+=dt) {
			vVec2.push_back( nbs.eval(t) );
		}
		vVec2.push_back( nbs.eval(t_fin - fabs(t_fin)*1e-5) );

		CI.push_back(vVec2);
	}

	return  true;
}

bool
load_constellation_illust_circle(FILE* fp, vector<vector<vector2f> >& CI)
{
	int circle_num;
	fread(&circle_num, sizeof(circle_num), 1, fp);

	for (int i=0; i<circle_num; ++i) {
        vector2f cpos;
		float    r;

		fread(&cpos, sizeof(cpos), 1, fp);
		fread(&r,    sizeof(r),    1, fp);

		//--- �|�����C�������ēo�^���� ---
		vector<vector2f>  vVec2;
		const int jmax = 25;
		for (int j=0; j<jmax; ++j) {
			float x,y,th;
			th = 2 * M_PI * j / float(jmax-1);
			x = r * cos(th) + cpos.x;
			y = r * sin(th) + cpos.y;
			vVec2.push_back( vector2f(x,y) );
		}
		CI.push_back(vVec2);
	}

	return  true;
}

bool
load_constellation_illust_ellipse(FILE* fp, vector<vector<vector2f> >& CI)
{
	int ellipse_num;
	fread(&ellipse_num, sizeof(ellipse_num), 1, fp);

	for (int i=0; i<ellipse_num; ++i) {
        vector2f cpos, ma1;
		float    b_a;

		fread(&cpos, sizeof(cpos), 1, fp);
		fread(&ma1,  sizeof(ma1),  1, fp);
		fread(&b_a,  sizeof(b_a),    1, fp);

		//--- ���W���
		float a, b;
		vector2f  ema, emi;
		ema = normalize(ma1);
		emi.set(-ema.y, ema.x);
		a = ma1.norm();
		b = b_a * a;

		//--- �|�����C�������ēo�^���� ---
		vector<vector2f>  vVec2;
		const int jmax = 25;
		for (int j=0; j<jmax; ++j) {
			float x, y, th;
			vector2f  pos;

			th = 2 * M_PI * j / float(jmax-1);
			x = cos(th);
			y = sin(th);
			pos = a*x*ema + b*y*emi + cpos;

			vVec2.push_back( pos );
		}
		CI.push_back(vVec2);
	}

	return  true;
}


bool
load_a_constellation_illust(FILE* fp)
{
	constellation_illust  ci;


	//--- �����̗��� ---
	char len, abbr_name[10];
	if (fread(&len, sizeof(len), 1, fp) != 1)  return false;
	fread(abbr_name, len, 1, fp);
	ci.cnst_ID = get_constellation_ID(abbr_name);


	//--- �X�P�[���ƕϊ��s�� ---
	matrix3f T;
	float sc;
	fread(&sc, sizeof(sc), 1,      fp);
	fread(T.m, 9*sizeof(float), 1, fp);


	vector<vector<vector2f> > CI;
	while (true) {
		int type;
		bool res;

		if (fread(&type, sizeof(type), 1, fp) < 1)  break;
		switch (type) {
			case 0:		goto exit_loop;
			case 1:		res = load_constellation_illust_line(fp, CI);		break;
			case 2:		res = load_constellation_illust_spline(fp, 5, CI);	break;
			case 3:		res = load_constellation_illust_circle(fp, CI);		break;
			case 4:		res = load_constellation_illust_ellipse(fp, CI);	break;
			default:	res = true;											break;
		}
	}

exit_loop:

	// �|�����C����ԓ����W�ɕϊ�����
	for (int i=0; i<CI.size(); ++i) {
		vector<vector2f>& vVec2 = CI[i];
		vector<vector3f>  vVec3;
		for (int j=0; j<vVec2.size(); ++j) {
			vector2f& pos2 = vVec2[j];
			vector3f  pos3;
			pos3 = normalize( vector3f( pos2.x, pos2.y, sc ) );
			vVec3.push_back( normalize( M_R2E * T * pos3 ) );
		}
		ci.vLine.push_back( vVec3 );
	}

	vCnstIllust.push_back(ci);
	return  true;
}


bool
load_constellation_illusts(const directory& dir)
{
	vCnstIllust.clear();

	FILE* fp = dir.fopen("cnst_illust.dat", "rb");
	if (fp==NULL)  return false;

	while (true) {
		if ( !load_a_constellation_illust(fp) )  break;
	}

	fclose(fp);
	return  true;
}




//--------------
//  ��������`��
//--------------
void
draw_constellation_line_2(const mtk_draw_struct& ds, const constellation& cnst, const float Rmax, bool flg)
{
	const vector3d camera_pos = ds.camera.get_pos();
	const float Rmax_sqr = Rmax * Rmax;

	for (int i=0; i<cnst.lines_HIP.size(); ++i) {
		const vector<long>&  vHIP = cnst.lines_HIP[i];

		glBegin(GL_LINE_STRIP);
		for (int j=0; j<vHIP.size(); ++j) {
			vector3f xyz;
			long HIP = vHIP[j];

			if (HIP == -2) {
				// Omega Cen �̏ꍇ
				const mtk_object&  obj = get_object("GC_NGC_5139");
				xyz = obj.get_pos() - camera_pos;
			} else {
				if ( !is_HIP_present(HIP) )  continue;

				const mtk_star& st = get_star_by_HIP( HIP );
				xyz = st.get_pos() - camera_pos;
			}

			const float l_sqr = xyz.norm2();
			if (!flg || l_sqr > Rmax_sqr) {
				xyz *= float(Rmax / sqrt(l_sqr));
			}

			glVertex3fv( xyz.get_pointer() );
		}
		glEnd();
	}
}


void
draw_constellation_lines(const mtk_draw_struct& ds)
{
	float Rmax, alp0;

	alp0 = 1;
	if (ds.tyMode == MODE_3D) {
		Rmax = get_Rmax();
		float scl = ds.scale;

		if (scl > Scl_inner) {
			float t = (scl - Scl_inner)/(Scl_star2 - Scl_inner);
			alp0 = 1-t;
		}
	} else {
		Rmax = 1.001;
	}




	const RGBf& col = get_color(MTK_COLOR_CONSTELLATION_LINE);

	const float lw = ds.line_width_fac / (ds.rad_per_pixel / 0.001);
	glLineWidth(lw);
	for (int i=0; i<CNST2.size(); ++i) {
		float alp;

		if (CNST2[i].bSelected) {
			alp = 1.0;
		}
		else {
			alp = alp0;
		}

		if (alp <= 0.0)  continue;
		glColor4f(col.r, col.g, col.b, alp);
		draw_constellation_line_2(ds, CNST2[i], Rmax, (ds.tyMode == MODE_3D));
	}
	glLineWidth(1.0);
}


//--------------
//  �����G��`��
//--------------
void
draw_constellation_illust(const mtk_draw_struct& ds)
{
	if (vCnstIllust.size() == 0)  return;

	float Rmax, alp;
	if (ds.tyMode == MODE_PLANETARIUM) {
		Rmax = 1.0015;
		alp = 1;
	} else {
		if (ds.tgt_pos.norm() > Scl_star2) return;
		Rmax = get_Rmax();
		const float scl = ds.scale;

		alp = 1;
		if (scl > Scl_inner) {
			float t = (scl - Scl_inner)/(Scl_star2 - Scl_inner);
			alp = 1-t;
		}
	}
	if (alp <= 0.0) return;


	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	const float lw = ds.line_width_fac / (ds.rad_per_pixel / 0.001);
	glLineWidth(lw);
	glColor4f(0.1,0.4,0.6,alp);
	for (int i=0; i<vCnstIllust.size(); ++i) {
		const constellation_illust& ci = vCnstIllust[i];

		for (int j=0; j<ci.vLine.size(); ++j) {
			const vector<vector3f>&  vVec3 = ci.vLine[j];

			glBegin(GL_LINE_STRIP);
			for (int k=0; k<vVec3.size(); ++k) {
				const vector3f xyz = Rmax * vVec3[k];
				glVertex3fv( xyz.get_pointer() );
			}
			glEnd();
		}
	}


	glLineWidth(1.0);
	glEnable(GL_DEPTH_TEST);
}




//-----------------
//  �����̖��O������
//-----------------
void
draw_constellation_name(const mtk_draw_struct& ds)
{
	float Rmax, alp;
	if (ds.tyMode == MODE_PLANETARIUM) {
		Rmax = 0.995;
		alp = 1;
	} else {
		if (ds.tgt_pos.norm() > Scl_star2) return;

		Rmax = get_Rmax();
		const float scl = ds.scale;
		
		alp = 1;
		if (scl > Scl_inner) {
			float t = (scl - Scl_inner)/(Scl_star2 - Scl_inner);
			alp = 1-t;
		}
	}
	if (alp <= 0.0) return;


	const RGBf& col = get_color(MTK_COLOR_CONSTELLATION_NAME);
	glColor4f(col.r, col.g, col.b, alp);


	// ���̒��S�̍��W�𓾂�
	float M[16];
	vector3f  rc;
	glGetFloatv( GL_MODELVIEW_MATRIX, M);
	rc.x = M[12];
	rc.y = M[13];
	rc.z = M[14];


	for (int i=0; i<CNST2.size(); ++i) {
		const constellation& cst = CNST2[i];
		
		glPushMatrix();
			vector3d xyz = double(Rmax) * (M_R2E * (cst.pos0 - vector3f(0,0,0)));	////
			glTranslated(xyz.x, xyz.y, xyz.z);

			prepare_3Dtext_2(rc, 0, ConstellationNameSize*ds.ltf, get_screen_w(), get_screen_z(), true, false);
			
			const std::string& name = get_string(CNST2[i].key);
			draw_text_centering(name);
		glPopMatrix();
	}
}


//-------------------
//  �����̋��E����`��
//-------------------
void
draw_constellation_boundary(const mtk_draw_struct& ds)
{
	float Rmax, alp;
	if (ds.tyMode == MODE_PLANETARIUM) {
		Rmax = 1.0015;
		alp = 1;
	} else {
		if (ds.tgt_pos.norm() > Scl_star2) return;

		Rmax = get_Rmax();
		const float scl = ds.scale;


		alp = 1;
		if (scl > Scl_inner) {
			float t = (scl - Scl_inner)/(Scl_star2 - Scl_inner);
			alp = 1-t;
		}
	}
	if (alp <= 0.0) return;


	const RGBf& col = get_color(MTK_COLOR_CONSTELLATION_BOUNDARY);
	glColor4f(col.r, col.g, col.b, alp);
	
	const float lw = ds.line_width_fac / (ds.rad_per_pixel / 0.001);
	glLineWidth(lw);
	for (int i=0; i<CNST2.size(); ++i) {
		const constellation&  cst = CNST2[i];
		const vector<RA_DEC>&  vpos = cst.boundary;
		vector3f xyzE0;

		//glBegin(GL_LINE_LOOP);
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j<vpos.size(); ++j) {
			const RA_DEC& eqPos = vpos[j];

			vector3f xyz = Rmax * vector3f( eqPos.get_dirc_vec() );
			vector3f xyzE = M_R2E * xyz;
			glVertex3fv( xyzE.get_pointer() );

			if (j == 0) {
				xyzE0 = xyzE;
			}
		}
		glVertex3fv(xyzE0.get_pointer());
		glEnd();
	}

	glLineWidth(1.0);
}




