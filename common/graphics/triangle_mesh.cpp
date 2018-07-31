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
#include  "triangle_mesh.h"
#include  <assert.h>


//=================
//  class TriMesh
//=================

//-----------------------
// Name: add_vertex
// Desc: ���_��ǉ�����
// Retn: �ǉ��������_�̔ԍ�
//-----------------------
int
TriMesh::add_vertex(const vector3f& _pos)
{
	//-- allocate memory --
	int size = pos.size();
	if (size >= pos.capacity()) {
		size_t  reserve_size = (size < TRIANGLES_RESERVE_SIZE/2) ? TRIANGLES_RESERVE_SIZE : size*2;
		pos.reserve(reserve_size);
		vtx_tri_list.reserve(reserve_size);
	}

	//-- add vertex
	pos.push_back(_pos);
	vtx_tri_list.push_back( vector<int>() );
	bVtxTriListInit = false;

	return  get_vtx_last_idx();
}

void
TriMesh::add_normal(const vector3f& _nv)
{
	if (!(type & TRIMESH_NORMAL)) return;

	//-- allocate memory --
	int size = nv.size();
	if (size >= nv.capacity()) {
		size_t  reserve_size = (size < TRIANGLES_RESERVE_SIZE/2) ? TRIANGLES_RESERVE_SIZE : size*2;
		nv.reserve(reserve_size);
	}

	//-- add normal
	nv.push_back(_nv);
}

void
TriMesh::add_uv_coord(const vector2f& _uv)
{
	if (!(type & TRIMESH_TEXTURE)) return;

	//-- allocate memory --
	int size = uv.size();
	if (size >= uv.capacity()) {
		size_t  reserve_size = (size < TRIANGLES_RESERVE_SIZE/2) ? TRIANGLES_RESERVE_SIZE : size*2;
		uv.reserve(reserve_size);
	}

	//-- add uv coord
	uv.push_back(_uv);
}


void
TriMesh::add_triangle(const triangle& tri)
{
	//-- allocate memory --
	int  size = vTri.size();
	if (size >= vTri.capacity()) {
		size_t  reserve_size = (size < TRIANGLES_RESERVE_SIZE/2) ? TRIANGLES_RESERVE_SIZE : size*2;
		vTri.reserve(reserve_size);
	}

	vTri.push_back(tri);
	bVtxTriListInit = false;
}


void
TriMesh::make_vtx_tri_list()
{
	const int vtx_num = get_vtx_num();

	for (int i=0; i<vtx_num; i++) {
		vtx_tri_list[i].clear();
	}

	const int tri_num = get_tri_num();
	for (int i=0; i<tri_num; i++) {
		triangle&  tri = get_triangle(i);

		//-- �N���X���t�@�����X���X�g�ɒǉ�����
		vtx_tri_list[ tri.pidx[0] ].push_back(i);
		vtx_tri_list[ tri.pidx[1] ].push_back(i);
		vtx_tri_list[ tri.pidx[2] ].push_back(i);
	}

	bVtxTriListInit = true;
}


//--- ���_�̖@���������I�Ɍv�Z���� ---
void
TriMesh::calc_normals()
{
	if (!support_normal()) return;
	if (!bVtxTriListInit)	make_vtx_tri_list();
	const int vtx_num = get_vtx_num();
	//-- calc normal of vertices
	for (int i=0; i<vtx_num; ++i) {
		vector3f nv_sum(0,0,0), tu_sum(0,0,0);
		const vector<int>&  vtl = vtx_tri_list[i];
		for (vector<int>::const_iterator it=vtl.begin(); it!=vtl.end(); ++it) {
			const triangle&  tri = get_triangle(*it);
			nv_sum += tri.nv;
		}
		
		if (support_normal()) {
			float nrm = norm(nv_sum);
			if (nrm != 0.0) {
				nv[i] = nv_sum / nrm;
			} else {
				nv[i] = vector3f(-10,0,0);	// �X���[�W���O���Ȃ��t���O�Ƃ��Ďg��
			}
		} 
	}
}



//------------------------
//  �X�P�[�����O�A�ړ��A��]
//------------------------
void
TriMesh::scale(const float& scale)
{
	const int vtx_num = get_vtx_num();

	for (int i=0; i<vtx_num; i++) {
		pos[i] *= scale;
	}
}

void
TriMesh::translate(const vector3f& dr)
{
	const int vtx_num = get_vtx_num();

	for (int i=0; i<vtx_num; i++) {
		pos[i] += dr;
	}
}


