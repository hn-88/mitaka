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
#include  "gl_triangle_mesh.h"



void
gl_draw_triangle_mesh(const TriMesh& tm, bool bNormal, bool bTexCoord, bool bSmooth,
					  int iBegin, int iEnd)
{
	if (!tm.support_normal())	bNormal	  = false;
	if (!tm.support_texture())	bTexCoord = false;
	if (!bNormal)				bSmooth	  = false;

	const int  tri_num = tm.get_tri_num();
	if (iBegin > iEnd) swap(iBegin, iEnd);
	if (iBegin>=tri_num || iEnd < 0)  return;
	if (iBegin < 0) iBegin = 0;
	if (iEnd > tri_num) iEnd = tri_num;

	const vector3f* pt = tm.get_vertex_pointer();
	const vector<vector3f>&  nv = tm.get_nv_pointer();
	const vector<vector2f>&  uv = tm.get_uv_pointer();
	glBegin( GL_TRIANGLES );
		for (int i=iBegin; i<iEnd; ++i) {
			const triangle&  tri = tm.get_triangle(i);
			if (bSmooth) {
				for (int j=0; j<3; ++j) {
					int pidx  = tri.pidx[j];
					int nidx  = tri.nidx[j];
					int uvidx = tri.uvidx[j];
					if (bNormal)	{
						if (nidx==-1) {	// �O�p�`�̖@�����g���ꍇ
							glNormal3fv( tri.nv.get_pointer() );
						} else {
							const vector3f& _nv = nv[nidx];
							if (_nv.x >= -1.01f)	glNormal3fv( _nv.get_pointer() );
							else					glNormal3fv( tri.nv.get_pointer() );	// �X���[�W���O���Ȃ����_
						}
					}
					if (bTexCoord)	glTexCoord2fv( uv[uvidx].get_pointer() );
					glVertex3fv( pt[pidx].get_pointer() );
				}
			} else {
				for (int j=0; j<3; ++j) {
					int pidx  = tri.pidx[j];
					int uvidx = tri.uvidx[j];
					if (bNormal)	glNormal3fv( tri.nv.get_pointer() );
					if (bTexCoord)	glTexCoord2fv( uv[uvidx].get_pointer() );
					glVertex3fv( pt[pidx].get_pointer() );
				}
			}
		}
	glEnd();
}
