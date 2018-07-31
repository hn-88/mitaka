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
#include  "triangulation.h"
#include  <vector>
#include  <list>
using namespace std;


//------------
//  Triangle
//------------
class dtTriangle  :  public Triangle
{
	vector2f	rc;				// circumcircle center
	float		Rc, Rc2;		// square of circumcircle radius
	float		xmax;
public:
	dtTriangle() {}
	void	calcCircumCircle(const vector2f* vVtx);
	int		checkCircumCircle(const vector2f& r);
	vector2f	get_rc() const		{ return rc; }
	float		get_Rc() const		{ return Rc; }
};


inline void
dtTriangle::calcCircumCircle(const vector2f* vVtx)
{
	const vector2f r1 = vVtx[p[0]];
	const vector2f r2 = vVtx[p[1]];
	const vector2f r3 = vVtx[p[2]];
	const vector2f v1 = r2 - r1;
	const vector2f v2 = r3 - r1;
	const vector2f v3 = r3 - r2;
	const vector2f vp(-v1.y, v1.x);
	const vector2f dr = 0.5f*v1 + 0.5f *(dot_product(v3,v2) / dot_product(vp,v2) ) * vp;
	Rc2 = norm2(dr);
	Rc  = sqrt(Rc2);
	rc  = r1 + dr;
	const float D = 1.0001f * Rc;
	xmax = rc.x + D;
}


inline  int
dtTriangle::checkCircumCircle(const vector2f& r)
{
	if (norm2(r-rc) <= Rc2) {
		return 0;
	}
	
	if (xmax < r.x) {
		return 2;
	}

	return 1;
}


//--------
//  Edge
//--------
struct  dtEdge {
	int p1, p2;
};


//-------------
//  Temporary
//-------------
vector<dtTriangle>	vTri;
vector<dtEdge>		vEdge(1000);


//====================================
//  Delaunay Triangulation
//------------------------------------
//  vVtx �� n+3 �v�f���̃X�y�[�X���K�v
//------------------------------------
int
DelaunayTriangulation(const int n, vector2f* vVtx, Triangle* vtri)
{
	const int tri_order[3] = {1, 2, 0};
	if (vTri.size() < 3*n) {
		vTri.resize(3*n);
	}

	//-----------------
	//  Supertriangle
	//-----------------
	float xmin, xmax, ymin, ymax;
	xmin = xmax = vVtx[0].x;
	ymin = ymax = vVtx[0].y;
	for (int i=1; i<n; i++) {
		const vector2f& r = vVtx[i];
		const float x = r.x;
		const float y = r.y;

		if (x < xmin)		xmin = x;
		else if (x > xmax)	xmax = x;
		if (y < ymin)		ymin = y;
		else if (y > ymax)	ymax = y;
	}

	// ���_��o�^
	float dx = xmax - xmin;
	float dy = ymax - ymin;
	float x1 = xmin - 0.1*dx;
	float y1 = ymin - 0.1*dy;
	float x2 = xmax + 1.1*dx;
	float y2 = ymax + 1.1*dy;
	vVtx[n  ].x = x1;
	vVtx[n  ].y = y1;
	vVtx[n+1].x = x2;
	vVtx[n+1].y = y1;
	vVtx[n+2].x = x1;
	vVtx[n+2].y = y2;

	// �O�p�`��o�^
	vTri[0].p[0] = n;
	vTri[0].p[1] = n+1;
	vTri[0].p[2] = n+2;
	vTri[0].calcCircumCircle(vVtx);
	
	int nTri  = 1;	// work�p
	int nTri2 = 0;	// �o�͗p
	//--------------------------
	//  Delaunay Triangulation
	//--------------------------
	for (int i=0; i<n; i++) {
		vector2f r = vVtx[i];

		//------------------------------------------
		//  �O�ډ~���ɐV�����_���܂ގO�p�`�̕ӂ�o�^����
		//------------------------------------------
		int edge_num = 0;
		for (int j=0; j<nTri; j++) {
			int stat = vTri[j].checkCircumCircle(r);
			if (stat==1) continue;
			dtTriangle&  tri = vTri[j];
			if (stat==2) {
				if (tri.p[0] < n && tri.p[1] < n && tri.p[2] < n) {
					// �o�͗p�z��ɃR�s�[
					vtri[nTri2++] = vTri[j];
				}
				// ���̎O�p�`�͍폜
				vTri[j] = vTri[nTri-1];
				nTri--;
				j--;
				continue;
			}

			//-------------------
			//  �R�̕ӂ�o�^����
			//-------------------
			int p2;
			p2 = tri.p[0];
			for (int k=0; k<3; k++) {
				int p1 = p2;
				p2 = tri.p[ tri_order[k] ];
				
				bool reg = true;
				for (int l=0; l<edge_num; l++) {
					if (vEdge[l].p1 == p2 && vEdge[l].p2 == p1) {
						// ���łɓo�^����Ă��� -> ������폜
						vEdge[l].p1 = -1;
						reg = false;
						break;
					}
				}
				if (reg) {
					// �o�^����Ă��Ȃ� -> �o�^����
					if (vEdge.size() <= edge_num) {
						vEdge.resize( 2*edge_num );
					}
					
					vEdge[edge_num].p1 = p1;
					vEdge[edge_num].p2 = p2;
					edge_num++;
				}
			}
			
			//------------------
			//  ���̎O�p�`���폜
			//------------------
			vTri[j] = vTri[nTri-1];		// ��Ԍ��̗v�f�������Ă���
			nTri--;
			j--;
		}
		
		//------------------------------------------
		//  �o�^���ꂽ�ӂƐV�����_�ō����O�p�`��o�^
		//------------------------------------------
		for (int l=0; l<edge_num; l++) {
			if (vEdge[l].p1 == -1)  continue;
			dtTriangle&  tri = vTri[nTri];
			tri.p[0] = vEdge[l].p1;
			tri.p[1] = vEdge[l].p2;
			tri.p[2] = i;
			tri.calcCircumCircle(vVtx);
			nTri++;
		}
	}

	// �c���Ă�����̂��R�s�[
	for (int i=0; i<nTri; i++) {
		const dtTriangle&  tri = vTri[i];
		if (tri.p[0] < n && tri.p[1] < n && tri.p[2] < n) {
			vtri[nTri2++] = vTri[i];
		}
	}

	return  nTri2;
}
