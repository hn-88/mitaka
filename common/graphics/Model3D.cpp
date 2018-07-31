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
#include  "Model3D.h"


//============
//  Object3D
//============
bbox3f
Object3D::get_bounding_box() const
{
	bbox3f  bb;
	const int imax = get_vtx_num();
	for (int i=0; i<imax; ++i) {
		bb.add_point( get_vtx_pos(i) );
	}
	return bb;
}



//============
//  3D Model
//============
int
Model3D::get_total_vtx_num() const
{
	int vtx_num = 0;
	for (vector<Object3D>::const_iterator it=vObject.begin(); it!=vObject.end(); ++it) {
		vtx_num += it->get_vtx_num();
	}
	return  vtx_num;
}

int
Model3D::get_total_normal_num() const
{
	int normal_num = 0;
	for (vector<Object3D>::const_iterator it=vObject.begin(); it!=vObject.end(); ++it) {
		normal_num += it->get_normal_num();
	}
	return  normal_num;
}

int
Model3D::get_total_uv_coord_num() const
{
	int uv_coord_num = 0;
	for (vector<Object3D>::const_iterator it=vObject.begin(); it!=vObject.end(); ++it) {
		uv_coord_num += it->get_uv_coord_num();
	}
	return  uv_coord_num;
}

int
Model3D::get_total_tri_num() const
{
	int tri_num = 0;
	for (vector<Object3D>::const_iterator it=vObject.begin(); it!=vObject.end(); ++it) {
		tri_num += it->get_tri_num();
	}
	return  tri_num;
}

Object3D&
Model3D::add_new_object(const Object3D& obj)
{
	vObject.push_back(obj);
	return  vObject.back();
}

Material&
Model3D::add_new_material(const Material& mat)
{
	vMaterial.push_back(mat);
	return  vMaterial.back();
}

bbox3f
Model3D::get_bounding_box() const
{
	bbox3f  bb;
	const int obj_num = get_objects_num();
	for (int i=0; i<obj_num; ++i) {
		const Object3D& obj = get_object(i);
		if (!obj.is_visible())  continue;
		bb.merge( obj.get_bounding_box() );
	}
	return  bb;
}

void
Model3D::update_triangles()
{
	for (vector<Object3D>::iterator it=vObject.begin(); it!=vObject.end(); ++it) {
		it->update_triangles();
	}
}

void
Model3D::calc_normals()
{
	for (vector<Object3D>::iterator it=vObject.begin(); it!=vObject.end(); ++it) {
		it->calc_normals();
	}
}




int
Model3D::register_textures(const char *fn)
{
	for (int j = 0; j<vTextureFile.size(); j++) {
		if (stricmp(vTextureFile[j].fullpath, fn) == 0) {
			return j;
		}
	}

	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	_splitpath(fn, drive, dir, fname, ext);

	TextureFile tf;
	strcpy(tf.fullpath, fn);
	sprintf(tf.filename, "%s%s", fname, ext);

	tf.bLoad = false;
	vTextureFile.push_back(tf);

	return vTextureFile.size() - 1;
}


void
Model3D::make_texture_list()
{
	vTextureFile.clear();
	
	
	for (int i=0; i<get_materials_num(); ++i) {
		Material& mat = get_material(i);

		if (!mat.bHasTexture && !mat.bHasSpecMap)				  continue;


		if (mat.bHasTexture) {
			mat.texture_ID = register_textures(mat.szTexFileName);
		}

		if (mat.bHasSpecMap) {
			mat.SpecMap_ID = register_textures(mat.szSpecMapFileName);
		}

	}
}

void
Model3D::fit_size(const float& size)
{
	// �o�E���f�B���O�{�b�N�X���v�Z����
	bbox3f  bb = get_bounding_box();
	if (bb.isValid()) {
		vector3f  vc(bb.x1+bb.x2, bb.y1+bb.y2, bb.z1+bb.z2);
		vc *= 0.5f;
		set_r0(-vc);
		vector3f dd(bb.x2-bb.x1, bb.y2-bb.y1, bb.z2-bb.z1);
		maxL = dd.x;
		if (dd.y > maxL)	maxL = dd.y;
		if (dd.z > maxL)	maxL = dd.z;
		set_scale(size/maxL);
	}
}
