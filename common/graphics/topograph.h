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
#ifndef __TOPOGRAPH_H__
#define __TOPOGRAPH_H__
#include  "../math/vector3.h"
#include  "../graphics/gl_common.h"
#include  <stdio.h>





const int patch_grid_max = 2*128;
const int lvN_max = 10;


//  Texture info
struct patch_texture_info {
	GLuint	texname;		// �e�N�X�`����(0 �Ńf�t�H���g�j
	float	u1, v1, u2, v2;	// �l���̃e�N�X�`�����W
};


//-------------
//  PatchGrid
//-------------
class PatchGrid
{
private:
	int		Nx, Ny;
	short	*data;								// �W���f�[�^
	float	*CosTh, *SinTh, *CosPhi, *SinPhi;	// �O���b�h�ł̎O�p�֐�
	patch_texture_info	tex_info;				// �e�N�X�`�����
	// �`��p�̃e���|�����o�b�t�@
	static vector3f rbuf[patch_grid_max+1][patch_grid_max+1], nvbuf[patch_grid_max+1][patch_grid_max+1];
public:
	PatchGrid();
	~PatchGrid();
	void	alloc(int _Nx, int _Ny);
	void	free();
	int		get_Nx() const		{ return Nx; }
	int		get_Ny() const		{ return Ny; }
	short*	get_pointer() const	{ return data; }
	short	get_elevation(int ix, int iy) const	{ return data[(iy+1)*(Nx+3) + ix+1]; }
	vector3f	get_nv(float x, float y) const;
	GLuint	get_texname() const	{ return tex_info.texname; }
	void	set_texture_info(const patch_texture_info& pti)		{ tex_info = pti; }
	void	shift_texcoord(float du);
	void	load(FILE* fp, float lon1, float lat1, float lon2, float lat2);
	void	draw(const float fac, bool bWire);
private:
	void	calc_buffers(const float fac);
	void	draw_poly();
	void	draw_wire();
};



//-------------
//  TopoPatch
//-------------
class TopoPatch
{
private:
	int		lvN;
	float	lon1, lon2, lat1, lat2;
	vector3f	nv[4], nv_mid;
	float		D;
	PatchGrid*	grid[lvN_max];
public:
	TopoPatch();
	~TopoPatch();
	void	free();
	int		get_lvN()  const	{ return lvN;  }
	float	get_lon1() const	{ return lon1; }
	float	get_lon2() const	{ return lon2; }
	float	get_lat1() const	{ return lat1; }
	float	get_lat2() const	{ return lat2; }
	float	get_D() const		{ return D; }
	vector3f	get_nv(float lon, float lat) const;
	float	get_anguler_diam_sqr(const float MM[16]) const;
	short	get_elevation(float lon, float lat)  const;
	GLuint	get_texname(int lv)  const;
	void	set_texture_info(int lv, const patch_texture_info& pti);
	void	shift_texcoord(float du);
	bool	cull_test(const float MM[16], const frustum& frs);
	void	draw(float fac, int lv, bool bWire);
	bool	load(FILE* fp);
private:
	void	calc_nv();
};


//--------------
//  TopoSphere
//--------------
class TopoSphere
{
private:
	float	R0;					// ���ϔ��a
	float	unit;				// �W���f�[�^�̒P��
	float	elv_fac;			// �n�`�̔{��
	GLuint	default_texneame;
	float	tex_shift;			
	int		PatchNx, PatchNy;
	TopoPatch*	patch;
	float	th_thr2[lvN_max];
public:
	TopoSphere() : patch(0), R0(1.0f), elv_fac(1.0f), default_texneame(0), tex_shift(0.0f)  {};
	~TopoSphere() { free(); };
	void	free();
	void	set_R0(float _R0)	{ R0 = _R0; }
	float	get_R0() const		{ return R0; }
	void	set_elv_fac(float _elv_fac)		{ elv_fac = _elv_fac; }
	float	get_elv_fac() const				{ return  elv_fac; }
	void	set_default_texname(GLuint texname)	{ default_texneame = texname; }
	void	set_patch_texture_info(int px, int py, int lv, const patch_texture_info& pti);
	void	shift_texture_coord(float shift_deg);	// �e�N�X�`���[�̃V�t�g�i�x�j
	float	get_texture_shift() const	{ return tex_shift; }
	short		get_elevation(float lon, float lat) const;
	short		get_elevation_W(float lonW, float lat) const;
	vector3f	get_pos(float lon, float lat) const;
	vector3f	get_nv(float lon, float lat) const;
	vector3f	get_interp_nv(float lon, float lat) const;
	void	draw(const frustum& frs, bool bWire, bool bLowRes);
	bool	load(const char* filename);
	bool	is_loaded() const	{ return !(patch==0); }
	void	draw_special(float lon1, float lat1, float u1, float v1, float lon2, float lat2, float u2, float v2, int Nx, int Ny) const;
private:
	void	draw_sphere();
};




#endif