/**************************************************************************************************

Copyright (c) 2015   Tsunehiko Kato  and  4D2U Project, National Astronomical Observatory of Japan

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
#include  "mtk_trajectory.h"
#include  "../common/astro/astro_const.h"




// polint: from NR 3.1
double
polint(const double xa[], const double ya[], int n, double x)
{
	int ns = 0;
	double y, dy, den, dif, dift, ho, hp, w;
	double c[4], d[4];

	dif = fabs(x - xa[0]);


	for (int i = 0; i < n; i++) {
		dift = fabs(x - xa[i]);
		if (dift < dif) {
			ns = i;
			dif = dift;
		}
		c[i] = ya[i];
		d[i] = ya[i];
	}

	y = ya[ns--];
	for (int m = 0; m < n - 1; m++) {
		for (int i = 0; i < n - m - 1; i++) {
			ho = xa[i] - x;
			hp = xa[i + m + 1] - x;
			w = c[i + 1] - d[i];
			den = ho - hp;
			if (den == 0.0) {
				return 0; // Error 
			}

			den = w / den;
			d[i] = hp*den;
			c[i] = ho*den;
		}

		dy = (2 * (ns + 1) < n - m - 1) ? c[ns + 1] : d[ns--];
		y += dy;
	}

	return y;
}




//----------------------------
//  class trajectory_segment
//----------------------------

bool
trajectory_segment::read_data(FILE *fp)
{
	if (fread(_t, sizeof(double), 4, fp) != 4)  return false;
	if (fread(_x, sizeof(double), 4, fp) != 4)  return false;
	if (fread(_y, sizeof(double), 4, fp) != 4)  return false;
	if (fread(_z, sizeof(double), 4, fp) != 4)  return false;

	return true;
}


void
trajectory_segment::scale_pos(const double& fac)
{
	for (int i = 0; i < 4; i++) {
		_x[i] *= fac;
		_y[i] *= fac;
		_z[i] *= fac;
	}
}


vector3d
trajectory_segment::get_pos(const double& t) const
{
	if (!is_inside(t)) {
		return vector3d(0, 0, 0);	// Error
	}


	vector3d pos;
	pos.x = polint(_t, _x, 4, t);
	pos.y = polint(_t, _y, 4, t);
	pos.z = polint(_t, _z, 4, t);

	return pos;
}




//------------------------
//  class mtk_trajectory
//------------------------

int
mtk_trajectory::find_segment(const double &t, int idx1, int idx2) const
{
	// �͈͓����ǂ������`�F�b�N
	if (t < vSeg[idx1].get_t_begin())	{ return -1; }
	if (t > vSeg[idx2].get_t_end())		{ return -1; }

	// t ���܂ރZ�O�����g��񕪖@�ɂ�茩����
	while (true) {
		int idxm;

		idxm = (idx1 + idx2) / 2;

		const trajectory_segment& ts = vSeg[idxm];
		if (ts.is_inside(t)) {
			return idxm;
		}

		if (idx2 == idx1 + 1) {
			return idx2;
		}

		if (t < ts.get_t_begin()) {
			idx2 = idxm;
		}

		if (t > ts.get_t_end()) {
			idx1 = idxm;
		}
	}
}


void
mtk_trajectory::make_index_table(int N)
{
	int Nseg;

	Nseg = vSeg.size();
	t_begin = vSeg[0].get_t_begin();
	t_end   = vSeg[Nseg - 1].get_t_end();

	int idx1, idx2;
	idx1 = 0;
	idx2 = Nseg - 1;

	double dt;
	dt = (t_end - t_begin) / N;

	vSegTable.resize(N + 1);
	for (int i = 0; i < N; i++) {
		double ti;
		int idx;

		ti = i*dt + t_begin;
		idx = find_segment(ti, idx1, idx2);
		vSegTable[i] = idx;

		idx1 = idx;
	}
	vSegTable[N] = idx2;
}


int
mtk_trajectory::find_segment(const double& t) const
{
	int N, idx_end, i, idx1, idx2;

	N = vSegTable.size() - 1;
	idx_end = vSeg.size() - 1;
	i = (t - t_begin) / (t_end - t_begin) * N;

	if (i<0 || i > N) { return -1; }
	if (i == N) { return idx_end; }

	idx1 = vSegTable[i];
	idx2 = vSegTable[i + 1];
	if (idx1 == idx2)  return idx1;

	return find_segment(t, idx1, idx2);
}


bool
mtk_trajectory::load(const char *fn, int Ntbl)
{
	FILE *fp;
	if ((fp = fopen(fn, "rb")) == NULL)  return  false;


	vSeg.clear();
	while (true) {
		trajectory_segment  ts;

		if (!ts.read_data(fp)) {
			break;
		}
		ts.scale_pos(_1km_pc);	// �P�ʂ� km ���� pc �ɂ���

		vSeg.push_back(ts);
	}
	fclose(fp);

	make_index_table(Ntbl);
	bLoaded = true;

	return  true;
}


vector3d
mtk_trajectory::get_pos(const double& t) const
{
	if (!bLoaded) {
		return vector3d(0, 0, 0);
	}

	if (t <= t_begin)	{ return vSeg[0].get_pos(t_begin); }
	if (t >= t_end)		{ return vSeg[ vSeg.size() - 1 ].get_pos(t_end); }

	int idx;
	idx = find_segment(t);
	if (idx == -1) {
		return  vector3d(0, 0, 0);
	}

	return  vSeg[idx].get_pos(t);
}
