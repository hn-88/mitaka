/******************************************************************************

Copyright (c) 2007-2014   Tsunehiko Kato

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
#include  <windows.h>
#include  <stdio.h>
#include  "gl_common.h"
#include  "../math/vector3.h"
#include  "topograph.h"

const float D2Rf = M_PI/180.0f;


//-------------
//  PatchGrid
//-------------

// �ÓI�����o�̒�`
vector3f PatchGrid::rbuf[patch_grid_max+1][patch_grid_max+1];
vector3f PatchGrid::nvbuf[patch_grid_max+1][patch_grid_max+1];






PatchGrid::PatchGrid()
: Nx(0), Ny(0), data(0)  ///
, CosTh(0), SinTh(0), CosPhi(0), SinPhi(0)
{
}

PatchGrid::~PatchGrid()
{
	free();
}


void
PatchGrid::alloc(int _Nx, int _Ny)
{
	free();
	if (_Nx < 1)  _Nx = 1;
	if (_Ny < 1)  _Ny = 1;
	Nx = _Nx;
	Ny = _Ny;
	data   = new short[(Nx+3) * (Ny+3)];
	CosTh  = new float[Ny+3];
	SinTh  = new float[Ny+3];
	CosPhi = new float[Nx+3];
	SinPhi = new float[Nx+3];
}

void
PatchGrid::free()
{
	if (data   != 0)	{ delete [] data;   data   = 0; }
	if (CosTh  != 0)	{ delete [] CosTh;  CosTh  = 0; }
	if (SinTh  != 0)	{ delete [] SinTh;  SinTh  = 0; }
	if (CosPhi != 0)	{ delete [] CosPhi; CosPhi = 0; }
	if (SinPhi != 0)	{ delete [] SinPhi; SinPhi = 0; }
	Nx = Ny = 0;
}

void
PatchGrid::load(FILE* fp, float lon1, float lat1, float lon2, float lat2)
{
	// �w�b�_���
	int lv, _Nx, _Ny;
	fread(&lv, sizeof(int), 1, fp);
	fread(&_Nx, sizeof(int), 1, fp);
	fread(&_Ny, sizeof(int), 1, fp);
	// �̈�̊m��
	alloc(_Nx, _Ny);
	// �W���f�[�^�̓ǂݍ���
	int NNx = Nx+3;
	int NNy = Ny+3;
	for (int iy=0; iy<NNy; iy++) {
		fread(&data[iy*NNx], sizeof(short), NNx, fp);
	}
	// �O�p�֐��̌v�Z
	const float dlat = lat2 - lat1;
	for (int iy=-1; iy<Ny+2; iy++) {
		float thd;
		thd  = lat1 + dlat * float(iy) / float(Ny);
		thd  = 90.0 - thd;
		float th;
		th = D2Rf * thd;
		CosTh[iy+1] = cos(th);
		SinTh[iy+1] = sin(th);
	}

	const float dlon = lon2 - lon1;
	for (int ix=-1; ix<Nx+2; ix++) {
		float phid;
		phid  = lon1 + dlon * float(ix) / float(Nx);
		if (phid < 0.0)		phid += 360.0;
		if (phid > 360.0)	phid -= 360.0;

		float phi;
		phi = D2Rf * phid;
		CosPhi[ix+1] = cos(phi);
		SinPhi[ix+1] = sin(phi);
	}

	// �e�N�X�`�����̐ݒ�
	tex_info.texname = 0;	// �f�t�H���g�e�N�X�`��
	tex_info.u1 = (lon1) / 360.0;
	tex_info.u2 = (lon2) / 360.0;
	tex_info.v1 = 0.5 + lat1 / 180.0;
	tex_info.v2 = 0.5 + lat2 / 180.0;
}


void
PatchGrid::shift_texcoord(float du)
{
	float ddu = tex_info.u2 - tex_info.u1;
	tex_info.u1 += du;
	if (tex_info.u1 >= 1.0f)  tex_info.u1 -= 1.0f;
	if (tex_info.u1 <  0.0f)  tex_info.u1 += 1.0f;
	tex_info.u2 = tex_info.u1 + ddu;
}


//--- �O���b�h�̈ʒu�Ɩ@�����v�Z����
void
PatchGrid::calc_buffers(const float fac)
{
	static vector3f rbuf0[patch_grid_max+3][patch_grid_max+3];	// �@���̌v�Z�p
	const int NNx = Nx+3;
	const int NNy = Ny+3;

	//--- �ɂ��܂ނ��ǂ����̔���
	char ty=0;
	if (SinTh[0] + SinTh[1] < 0.0) {
		ty = 1;		// �k��
	} else if (SinTh[NNy-2] + SinTh[NNy-1] < 0.0) {
		ty = 2;		// ���
	}

	//--- rbuf0, rbuf
	for (int iy=0; iy<NNy; iy++) {
		const float z   = CosTh[iy];
		const float rho = SinTh[iy];
		const int idx0 = iy*NNx;
		for (int ix=0; ix<NNx; ix++) {
			const float x = rho * CosPhi[ix];
			const float y = rho * SinPhi[ix];
			const vector3f  nv0(x,y,z);
			rbuf0[iy][ix] = ( 1.0f + fac * float(data[idx0 + ix]) ) * nv0;
			if (ix==0 || ix==NNx-1 || iy==0 || iy==NNy-1)  continue;
			rbuf[iy-1][ix-1] = rbuf0[iy][ix];
		}
	}

	//--- nvbuf
	for (int iy=0; iy<Ny+1; iy++) {
		int iym, iyp;
		iym = iy-1;
		iyp = iy+1;

		//--- �ɂ̏ꍇ
		if ((iy==0 &&  ty==1 ) || (iy==Ny && ty==2)) {
			for (int ix=0; ix<Nx+1; ix++) {
				nvbuf[iy][ix] = normalize( rbuf0[iy+1][ix+1] );
			}
			continue;
		}

		for (int ix=0; ix<Nx+1; ix++) {
			int ixm, ixp;
			ixm = ix-1;
			ixp = ix+1;
			vector3f  u, v;
			u = rbuf0[iy +1][ixp+1] - rbuf0[iy +1][ixm+1];
			v = rbuf0[iyp+1][ix +1] - rbuf0[iym+1][ix +1];
			nvbuf[iy][ix] = -normalize( cross_product(u, v) );
		}
	}
}

//--- �`�悷��
void
PatchGrid::draw_poly()
{
	// �e�N�X�`�����W
	const float du = (tex_info.u2-tex_info.u1)/Nx;
	const float dv = (tex_info.v2-tex_info.v1)/Ny;
	// �`��
	float v = tex_info.v1;
	for (int iy=0; iy<Ny; iy++) {
		int iyp = iy+1;
		glBegin(GL_TRIANGLE_STRIP);
		float u = tex_info.u1;
		for (int ix=0; ix<=Nx; ix++) {
			glTexCoord2f(  u,v );
			glNormal3fv(   nvbuf[iy ][ix ].get_pointer() );
			glVertex3fv(   rbuf [iy ][ix ].get_pointer() );
			glTexCoord2f(  u, v+dv );
			glNormal3fv(   nvbuf[iyp][ix ].get_pointer() );
			glVertex3fv(   rbuf [iyp][ix ].get_pointer() );

			u += du;
		}
		glEnd();
		v += dv;
	}
}


void
PatchGrid::draw_wire()
{
	// �`��
	for (int iy=0; iy<=Ny; iy++) {
		glBegin(GL_LINE_STRIP);
		for (int ix=0; ix<=Nx; ix++) {
			glVertex3fv(   rbuf [iy][ix].get_pointer() );
		}
		glEnd();
	}
	for (int ix=0; ix<=Nx; ix++) {
		glBegin(GL_LINE_STRIP);
		for (int iy=0; iy<=Ny; iy++) {
			glVertex3fv(   rbuf [iy][ix].get_pointer() );
		}
		glEnd();
	}
}

void
PatchGrid::draw(const float fac, bool bWire)
{
	// �ʒu�Ɩ@���̌v�Z
	calc_buffers(fac);
	if (bWire)	draw_wire();
	else		draw_poly();
}

vector3f
PatchGrid::get_nv(float x, float y)  const
{
	int xf = x * (Nx-1);
	int yf = y * (Ny-1);
	int ix = int(xf);
	int iy = int(yf);

	float wx1, wx2, wy1, wy2;
	wx2 = xf - ix;
	wx1 = 1 - wx2;
	wy2 = yf - iy;
	wy1 = 1 - wy2;

	vector3f nv;
	nv  = wx1 * wy1 * nvbuf[ix][iy];
	nv += wx2 * wy1 * nvbuf[ix+1][iy];
	nv += wx2 * wy2 * nvbuf[ix+1][iy+1];
	nv += wx1 * wy1 * nvbuf[ix][iy+1];

	return  nv;
}


//-------------
//  TopoPatch
//-------------
TopoPatch::TopoPatch()
{
	for (int i=0; i<lvN_max; i++) {
		grid[i] = 0;
	}
}

TopoPatch::~TopoPatch()
{
	free();
}

void
TopoPatch::free()
{
	for (int i=0; i<lvN_max; i++) {
		if (grid[i] != 0)  { delete grid[i];  grid[i] = 0; }
	}
}

//
GLuint
TopoPatch::get_texname(int lv)  const
{
	if (lv<0 || lv>lvN)	 return  0;
	return  grid[lv]->get_texname();
}

void
TopoPatch::set_texture_info(int lv, const patch_texture_info& pti)
{
	if (lv < 0) lv=0;
	if (lv > lvN)  return;
	for (int i=lv; i<=lvN; i++) {
		if (grid[i] != 0)  grid[i]->set_texture_info(pti);
	}
}

void
TopoPatch::shift_texcoord(float du)
{
	for (int lv=0; lv<=lvN; lv++) {
		grid[lv]->shift_texcoord(du);
	}
}

// �@���ƒ��a���v�Z����
void
TopoPatch::calc_nv()
{
	const float th1  = D2Rf * (90.0-lat1);
	const float th2  = D2Rf * (90.0-lat2);
	const float phi1 = D2Rf * (lon1);
	const float phi2 = D2Rf * (lon2);
	const float rho1 = sin(th1);
	const float rho2 = sin(th2);
	const float cos_th1  = cos(th1);
	const float cos_th2  = cos(th2);
	const float cos_phi1  = cos(phi1);
	const float cos_phi2  = cos(phi2);
	const float sin_phi1  = sin(phi1);
	const float sin_phi2  = sin(phi2);

	//--- �l���̖@���̌v�Z
	nv[0].set(rho1*cos_phi1, rho1*sin_phi1, cos_th1);
	nv[1].set(rho1*cos_phi2, rho1*sin_phi2, cos_th1);
	nv[2].set(rho2*cos_phi1, rho2*sin_phi1, cos_th2);
	nv[3].set(rho2*cos_phi2, rho2*sin_phi2, cos_th2);

	//--- �����̖@���̌v�Z
	const float th_mid  = D2Rf * (90.0  - 0.5 * (lat1 + lat2));
	const float phi_mid = D2Rf * (0.5 * (lon1 + lon2));
	const float rho_mid = sin(th_mid);
	nv_mid.set(rho_mid*cos(phi_mid), rho_mid*sin(phi_mid), cos(th_mid));

	//--- �p�b�`�̒��a�𓾂�i���̔��a���P�ʁj
	const float dphi  = D2Rf * fabs(lon2 - lon1);
	const float dth   = D2Rf * fabs(lat1 - lat2);
	float  a = dphi * sin(th_mid);
	float  b = dth;
	D = 2.0 * max(a, b);
}

short
TopoPatch::get_elevation(float lon, float lat)  const
{
	if (lon > 180.0f)   lon-=360.0f;
	float x = (lon-lon1) / (lon2-lon1);
	const float y = (lat1-lat) / (lat1-lat2);
	
	if (x < 0.0) { x += 1.0; }
	if (x < 0.0) { x += 1.0; }

	if (x<0.0 || x>1.0)  return 0;
	if (y<0.0 || y>1.0)  return 0;

	const PatchGrid& grd = *grid[lvN];
	const int Nx = grd.get_Nx();
	const int Ny = grd.get_Ny();
	int ix = int(x * Nx);
	int iy = int(y * Ny);

	// ���`�⊮
	float wx1, wx2, wy1, wy2;
	wx2 = (x * Nx) - ix;
	wx1 = 1 - wx2;
	wy2 = (y * Ny) - iy;
	wy1 = 1 - wy2;

	float elv;
	elv  = wx1*wy1 * grd.get_elevation(ix,   iy);
	elv += wx2*wy1 * grd.get_elevation(ix+1, iy);
	elv += wx2*wy2 * grd.get_elevation(ix+1, iy+1);
	elv += wx1*wy2 * grd.get_elevation(ix,   iy+1);

	return  elv;
}


// �J�����O�̃e�X�g
bool
TopoPatch::cull_test(const float MM[16], const frustum& frs)
{
	// �J�����O
	char bf = 0x3f;
	int c1 = 0;

	const vector3f  r0(MM[12], MM[13], MM[14]);
	for (int i=0; i<4; i++) {
		// ���_���W�n�ł̖@���x�N�g��
		const vector3f&  nv1 = nv[i];
		vector3f  nve;
		nve.x = MM[0]*nv1.x + MM[4]*nv1.y + MM[8] *nv1.z;
		nve.y = MM[1]*nv1.x + MM[5]*nv1.y + MM[9] *nv1.z;
		nve.z = MM[2]*nv1.x + MM[6]*nv1.y + MM[10]*nv1.z;
		// ���_���W�n�ł̓_�̈ʒu�x�N�g��
		const vector3f  r(nve + r0);
		// ���_���W�n�ł̓_�̈ʒu�x�N�g���Ɩ@���Ƃ̓���
		if (dot_product(r, nve) >= 0.0)	{ c1++; }
		// �X�N���[���̊p�x�ɂ��J�����O
		bf &= frs.cull_test(r.x, r.y, -r.z);
	}

	return  ((c1==4) || (bf != 0));
}


// �����a�̂Q��𓾂�
float
TopoPatch::get_anguler_diam_sqr(const float MM[16])  const
{
	const vector3f  r0(MM[12], MM[13], MM[14]);
	const vector3f&  nv1 = nv_mid;
	vector3f  nve;
	nve.x = MM[0]*nv1.x + MM[4]*nv1.y + MM[8] *nv1.z;
	nve.y = MM[1]*nv1.x + MM[5]*nv1.y + MM[9] *nv1.z;
	nve.z = MM[2]*nv1.x + MM[6]*nv1.y + MM[10]*nv1.z;
	const vector3f  r(nve + r0);
	return  D*D / r.norm2();
}


//--- �`�悷��
void
TopoPatch::draw(float fac, int lv, bool bWire)
{
	grid[lv]->draw(fac, bWire);
}


//--- �f�[�^���t�@�C������ǂݍ���
bool
TopoPatch::load(FILE* fp)
{
	// �w�b�_���̓ǂݍ���
	fread(&lon1, sizeof(float), 1, fp);
	fread(&lon2, sizeof(float), 1, fp);
	fread(&lat1, sizeof(float), 1, fp);
	fread(&lat2, sizeof(float), 1, fp);
	fread(&lvN, sizeof(int), 1, fp);
	// �e�K�w�̃f�[�^��ǂ�
	for (int lv=0; lv<=lvN; lv++) {
		grid[lv] = new PatchGrid;
		grid[lv]->load(fp, lon1, lat1, lon2, lat2);
	}
	calc_nv();
	return  true;
}

vector3f
TopoPatch::get_nv(float lon, float lat)	const
{
	float x = (lon - lon1) / (lon2 - lon1);
	float y = (lat - lat1) / (lat2 - lat1);
	return grid[lvN]->get_nv(x, y);
}



//--------------
//  TopoSphere
//--------------
void
TopoSphere::free()
{
	if (patch != 0)  { delete [] patch;  patch=0; }
}

void
TopoSphere::set_patch_texture_info(int px, int py, int lv, const patch_texture_info& pti)
{
	if (!is_loaded())  return;
	if (px < 0)			px = 0;
	if (px > PatchNx-1)	px = PatchNx-1;
	if (py < 0)			py = 0;
	if (py > PatchNy-1)	py = PatchNy-1;
	patch[py*PatchNx + px].set_texture_info(lv, pti);
}

void
TopoSphere::shift_texture_coord(float shift_deg)
{
	tex_shift = shift_deg;
	const float du = shift_deg/360.0f;
	for (int iy=0; iy<PatchNy; iy++) {
		for (int ix=0; ix<PatchNx; ix++) {
			TopoPatch&  pt = patch[iy*PatchNx+ix];
			pt.shift_texcoord(du);				
		}
	}
}

short
TopoSphere::get_elevation(float lon, float lat) const
{
	if (!is_loaded())  return 0;
	int ix = int(PatchNx * (lon + 180.0f) / 360.0f);
	int iy = int(PatchNy * ( 90.0f - lat)  / 180.0f);
	ix = ix % PatchNx;
	if (iy < 0)			iy = 0;
	if (iy > PatchNy-1)	iy = PatchNy-1;
	return  unit * patch[iy*PatchNx + ix].get_elevation(lon, lat);
}

short
TopoSphere::get_elevation_W(float lonW, float lat)  const
{
    return  get_elevation(360.0f - lonW, lat);
}

vector3f
TopoSphere::get_pos(float lon, float lat) const
{
	if (!is_loaded())  return get_nv(lon, lat);
	const float elv = get_elevation(lon, lat);
	return  (1.0f + elv*elv_fac/R0) * get_nv(lon, lat);
}

vector3f
TopoSphere::get_nv(float lon, float lat) const
{
	float th, phi, x,y,z,rho;
	th  = D2Rf * ( 90.0f - lat);
	phi = D2Rf * (lon);
	z	= cos(th);
	rho	= sin(th);
	x	= rho * cos(phi);
	y	= rho * sin(phi);
	return  vector3f(x,y,z);
}

vector3f
TopoSphere::get_interp_nv(float lon, float lat) const
{
	if (!is_loaded())  return get_nv(lon, lat);
	int ix = int(PatchNx * (lon + 180.0f) / 360.0f);
	int iy = int(PatchNy * ( 90.0f - lat)  / 180.0f);
	ix = ix % PatchNx;
	if (iy < 0)			iy = 0;
	if (iy > PatchNy-1)	iy = PatchNy-1;
	return  patch[iy*PatchNx + ix].get_nv(lon, lat);
}



//--------
//  Draw
//--------
void
TopoSphere::draw(const frustum& frs, bool bWire, bool bLowRes)
{
	const float thR = tan(6.0f * D2Rf);		// ����\�����鎋���a
	const float thR2 = thR * thR;


	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_BLEND);
	if (!bWire) {
		glEnable(GL_LIGHTING);
		glEnable(GL_NORMALIZE);
	} else {
		glDisable(GL_LIGHTING);
	}

	// �n�`�f�[�^�����[�h����Ă��Ȃ����͋���\��
	if (!is_loaded()) {
		glPushMatrix();
			draw_sphere();
		glPopMatrix();
		glPopAttrib();
		return;
	}

	//----------------
	//  �n���\�ʂ̕\��
	//----------------
	GLfloat MM[16];
	glPushMatrix();
		glGetFloatv( GL_MODELVIEW_MATRIX,  MM);
		const float L2  = MM[12]*MM[12] + MM[13]*MM[13] + MM[14]*MM[14];	// ���S�܂ł̋���
		const float R2  = MM[0] *MM[0]  + MM[1] *MM[1]  + MM[2] *MM[2];		// ���a
		float L02 = L2 - R2;
		if (L02 <= 0.0f) {
			glPopMatrix();
			glPopAttrib();
			return;
		}
		//--- �����ꍇ�́A����\��
		if (R2/L2 < thR2) {
			draw_sphere();
			glPopMatrix();
			glPopAttrib();
			return;
		}

		// �e�p�b�`�̕`��
		GLuint texname = 0;
		const float fac = unit*elv_fac / R0;
		for (int iy = 0; iy<PatchNy; iy++) {
			for (int ix=0; ix<PatchNx; ix++) {
				TopoPatch&  pt = patch[iy*PatchNx+ix];
				// �J�����O
				if ( pt.cull_test(MM, frs) )  continue;
				// �\�����x���̌���
				const int lvN = pt.get_lvN();
				const float D2 = R2 * pt.get_anguler_diam_sqr(MM);	// �����a(rad.)�̂Q��
				int lv;
				for (lv=lvN; lv>=0; lv--) {
					//****
					//float th = D2Rf * 0.2f * float(1<<lv);
					float th = D2Rf * 0.13f * float(1<<lv);		//****
					//****
					float th2 = th*th;
					if (D2 > th2)  break;
				}
				if (bLowRes)	lv--;
				if (lv < 0)  lv = 0;

				// �e�N�X�`���̐ݒ�
				GLuint texname1 = pt.get_texname(lv);
				if (texname1 == 0)  texname1 = default_texneame;
				if (texname != texname1) {
					glBindTexture(GL_TEXTURE_2D, texname1);
					texname = texname1;
				}

				// �`��
				pt.draw(fac, lv, bWire);
			}
		}
	glPopMatrix();
	glPopAttrib();
}


void
TopoSphere::draw_sphere()
{
	glRotatef(tex_shift, 0,0,1);
	gl_draw_LOD_sphere();
}


//------------------
//  �f�[�^�̓ǂݍ���
//------------------
bool
TopoSphere::load(const char* filename)
{
	FILE *fp;
	if ((fp=fopen(filename, "rb")) == NULL)  return  false;

	// �w�b�_���
	fread(&unit,    sizeof(float), 1, fp);
	fread(&R0,      sizeof(float), 1, fp);
	fread(&PatchNx, sizeof(int), 1, fp);
	fread(&PatchNy, sizeof(int), 1, fp);

	// �e�p�b�`�̓ǂݍ���
	free();
	patch = new TopoPatch[PatchNx * PatchNy];
	for (int iy=0; iy<PatchNy; iy++) {
		for (int ix=0; ix<PatchNx; ix++) {
			patch[iy*PatchNx+ix].load(fp);
		}
	}

	fclose(fp);
	return  true;
}


//------------------------------
//  ����̗̈悾�����𑜓x�ŕ`��
//------------------------------
void
TopoSphere::draw_special(float lon1, float lat1, float u1, float v1, float lon2, float lat2, float u2, float v2, int Nx, int Ny) const
{
	const float dlat = (lat2 - lat1)/float(Ny);
	const float dlon = (lon2 - lon1)/float(Nx);
	const float du   = (u2 - u1)/Nx;
	const float dv   = (v2 - v1)/Ny;

	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_NORMALIZE);
	glDisable(GL_BLEND);

	// �`��
	float v = v1;
	for (int iy=0; iy<Ny; iy++) {
		float lat  = lat1 + dlat * iy;
		glBegin(GL_TRIANGLE_STRIP);
		float u = u1;
		for (int ix=0; ix<=Nx; ix++) {
			vector3f  pos, nv;
			float lon = lon1 + dlon * ix;
			pos = 1.0001f * get_pos(lon, lat);
			nv  = get_interp_nv(lon, lat);
			glTexCoord2f( u, v );
			glNormal3fv(  nv.get_pointer() );
			glVertex3fv(  pos.get_pointer() );
			pos = 1.0001f * get_pos(lon, lat + dlat);
			nv  = get_interp_nv(lon, lat+dlat);
			glTexCoord2f( u, v+dv );
			glNormal3fv(  nv.get_pointer() );
			glVertex3fv(  pos.get_pointer() );
			u += du;
		}
		glEnd();
		v += dv;
	}
}
