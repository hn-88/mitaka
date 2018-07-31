/******************************************************************************

Copyright (c) 2007-2015   Tsunehiko Kato

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
#ifndef __MODEL3D_H__
#define __MODEL3D_H__
#include  "triangle_mesh.h"
#include  "bounding_box.h"


//-----------------
//  Material list
//-----------------
struct material_list
{
	int						material_ID;
	vector<unsigned int>	vFaceNo;
};


//=============
//  3D Object
//=============
class Object3D  :  public TriMesh
{
private:
	char	name[256];					// �I�u�W�F�N�g�̖��O
	vector<material_list>	matL;		// �}�e���A���E���X�g
	bool	bVisible;					// �\�����邩
	vector3f	ex, ey, ez, r0;			// �Ǐ����W�n
public:
	Object3D() : TriMesh(), matL(), bVisible(true), ex(1,0,0), ey(0,1,0), ez(0,0,1), r0(0,0,0)							{};
	Object3D(unsigned char _type) : TriMesh(_type), matL(), bVisible(true), ex(1,0,0), ey(0,1,0), ez(0,0,1), r0(0,0,0)	{};
	void	set_name(const char* obj_name)						{ strncpy(name, obj_name, 254); }
	const char*	get_name() const								{ return  name; }
	void	add_material_list(const material_list& ml)			{ matL.push_back(ml); }
	int		get_material_num() const							{ return matL.size(); }
	const material_list&	get_material_list(int idx) const	{ return matL[idx]; }
	void	set_visible(bool bFlg)		{ bVisible = bFlg; }
	bool	is_visible() const			{ return bVisible; }
	bbox3f	get_bounding_box() const;
};




//============
//  Material
//============
struct Material
{
	char	szName[256];

	char	szTexFileName[256], szSpecMapFileName[256];
	bool	bHasTexture, bHasSpecMap;

	int		texture_ID, SpecMap_ID;
	bool	bTwoSided;		// ���ʕ\��
	bool	bFlat;			// �X���[�Y�V�F�[�f�B���O���Ȃ�
	bool	bDecal;			// �e�N�X�`���[�� DECAL �ŕ\��
	float	ambient_RGBAf[4];
	float	diffuse_RGBAf[4];
	float	specular_RGBAf[4];
	float	shininess;
	float	emission_RGBAf[4];
};




//================
//  Texture File
//================
struct TextureFile {
	char	fullpath[_MAX_PATH];
	char	filename[_MAX_PATH];
	bool	bLoad;
};



//============
//  3D Model
//============
class  Model3D
{
private:
	vector<Object3D>	vObject;		// �I�u�W�F�N�g
	vector<Material>	vMaterial;		// �}�e���A��
	vector<TextureFile> vTextureFile;	// �e�N�X�`���̃t�@�C�����̃��X�g
	char	name[256];					// ���f���̖��O
	vector3f			r0;
	float				scale, maxL;
public:
	Model3D() : r0(0,0,0), scale(1) {};
	void	clear()						{ vObject.clear(); vMaterial.clear(); vTextureFile.clear(); r0.set(0,0,0); scale=1.0f; }
	int	get_objects_num() const			{ return vObject.size(); }
	int get_total_vtx_num() const;
	int get_total_normal_num() const;
	int get_total_uv_coord_num() const;
	int get_total_tri_num() const;
	int	get_materials_num() const		{ return vMaterial.size(); }
	int	get_textures_num() const		{ return vTextureFile.size(); }
	void		set_name(const char* model_name)				{ strncpy(name, model_name, 254); }
	const char*	get_name() const								{ return  name; }
	Object3D&			add_new_object(const Object3D& obj);
	Material&			add_new_material(const Material& mat);
	Object3D&			get_object(int idx)						{ return  vObject[idx]; }
	const Object3D&		get_object(int idx) const				{ return  vObject[idx]; }
	Material&			get_material(int idx)					{ return  vMaterial[idx]; }
	const Material&		get_material(int idx) const				{ return  vMaterial[idx]; }
	TextureFile&		get_texture_file_info(int idx)			{ return  vTextureFile[idx]; }
	const TextureFile&	get_texture_file_info(int idx) const	{ return  vTextureFile[idx]; }
	bbox3f		get_bounding_box() const;
	void		update_triangles();
	void		calc_normals();
	int			register_textures(const char *fn);
	void		make_texture_list();
	void		set_scale(float scl)			{ scale = scl; }
	float		get_scale() const				{ return scale; }
	void		set_r0(const vector3f& _r0)		{ r0 = _r0; }
	vector3f	get_r0() const					{ return r0; }
	void		fit_size(const float& size);
	float		get_maxL() const				{ return maxL; }
};


#endif