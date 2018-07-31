/******************************************************************************

Copyright (c) 2007   Tsunehiko Kato
Copyright (c) 2014   Tsunehiko Kato  and  4D2U Project, National Astronomical Observatory of Japan

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
#include  <stdio.h>
#include  "cf96.h"



//==========================================================================
// Reference:
//   Planetary Series (1996)
//   by J.Chapront and G. Francou 
//   URL:http://xml.gsfc.nasa.gov/archive/catalogs/6/6087/index_long.html
//==========================================================================


//---------
//  block
//---------
bool
CF96_block::load(FILE* fp)
{
	for (int i=0; i<3; i++) {
		sec[i].clear();
	}
	vvvct.clear();
	vvvst.clear();
	double buf[300];
	for (int iv=0; iv<3; iv++) {
		//-- secular coef
		int sec_n;
		fread(&sec_n, sizeof(int), 1, fp);
		fread(buf, sizeof(double), sec_n, fp);
		for (int i=0; i<sec_n; i++) {
			sec[iv].push_back(buf[i]);
		}
		//-- Poisson coef
		vector<vector<double> >  vvct, vvst;
		int m_n;
		fread(&m_n, sizeof(int), 1, fp);
		for (int m=0; m<m_n; m++) {
			vector<double>	vct, vst;
			int nf;
			fread(&nf, sizeof(int), 1, fp);
			fread(buf, sizeof(double), nf, fp);
			for (int i=0; i<nf; i++) {
				vct.push_back(buf[i]);
			}
			fread(buf, sizeof(double), nf, fp);
			for (int i=0; i<nf; i++) {
				vst.push_back(buf[i]);
			}
			vvct.push_back(vct);
			vvst.push_back(vst);
		}
		vvvct.push_back(vvct);
		vvvst.push_back(vvst);
	}
	return  true;
}


vector3d
CF96_block::eval_pos(const double& x, const double& fx, const vector<int>& nf, const vector<vector<double> >& vfq)  const
{
	const int max_sec = sec[0].size()-1;
	const int mx = vvvct[0].size()-1;
	double xyz[3];
	for (int iv=0; iv<3; iv++) {
		double sum, xx;

		//-- Secular series
		sum = 0.0;
		xx = 1.0;
		for (int i=0; i<=max_sec; i++) {
			sum += sec[iv][i] * xx;
			xx *= x;
		}
		xyz[iv] = sum;

		//-- Poisson series
		sum = 0.0;
		xx = 1.0;
		for (int m=0; m<=mx; m++) {
			for (int i=0; i<nf[m]; i++) {
				double th = vfq[m][i] * fx;
				sum += (vvvct[iv][m][i] * cos(th) +	vvvst[iv][m][i] * sin(th)) * xx;
			}
			xx *= x;
		}
		xyz[iv] += sum;
	}

	return  vector3d(xyz[0],xyz[1],xyz[2]);
}


vector3d
CF96_block::eval_vel(const double& dt, const double& x, const double& fx, const vector<int>& nf, const vector<vector<double> >& vfq)  const
{
	const double wt = 2.0 / dt;
	const int max_sec = sec[0].size() - 1;
	const int mx = vvvct[0].size() - 1;
	double vxyz[3];


	for (int iv = 0; iv<3; iv++) {
		double sum, xx, _xx;

		//-- Secular series
		sum = 0.0;
		xx = 1.0;
		for (int i = 1; i <= max_sec; i++) {
			sum += double(i) * sec[iv][i] * xx;
			xx *= x;
		}
		vxyz[iv] = wt*sum;

		//-- Poisson series
		sum = 0.0;
		_xx = 0.0;
		xx = 1.0;
		for (int m = 0; m <= mx; m++) {
			for (int i = 0; i<nf[m]; i++) {
				const double fw = vfq[m][i];
				double th = fw * fx;
				const double cs = cos(th);
				const double sn = sin(th);
				const double ctw = vvvct[iv][m][i];
				const double stw = vvvst[iv][m][i];

				sum += fw * (stw*cs - ctw*sn) * xx;
				if (m > 0) {
					sum += double(m)*wt*(ctw*cs + stw*sn)*_xx;
				}
			}
			_xx = xx;
			xx *= x;
		}
		vxyz[iv] += sum;
	}

	return  vector3d(vxyz[0], vxyz[1], vxyz[2]);
}



//----------
//  planet
//----------
bool
CF96_planet::load(FILE* fp)
{
	double buf[300];
	nf.clear();
	vfq.clear();
	vBlock.clear();

	fread(&t0, sizeof(double), 1, fp);
	fread(&dt, sizeof(double), 1, fp);
	fread(&mx, sizeof(int), 1, fp);
	fread(&imax, sizeof(int), 1, fp);
	fread(&iblock, sizeof(int), 1, fp);
	max_sec = 2*imax-1;

	//-- nf, fq
	int ibuf[10];
	fread(ibuf, sizeof(int), mx+1, fp);
	for (int i=0; i<=mx; i++) {
		nf.push_back(ibuf[i]);
	}

	for (int i=0; i<=mx; i++) {
		fread(buf, sizeof(double), nf[i], fp);
		
		vector<double>  fq;		
		for (int j=0; j<nf[i]; j++) {
			fq.push_back(buf[j]);
		}
		vfq.push_back(fq);
	}

	//-- block
	for (int i=0; i<iblock; i++) {
		CF96_block  blk;
		blk.load(fp);
		vBlock.push_back(blk);
	}

	return  true;
}


vector3d
CF96_planet::eval_pos(const double& t)  const
{
	//-- �u���b�N�����߂�
	int nb;
	nb = (t-t0)/dt;
	if (nb < 0) nb = 0;
	if (nb >= iblock)  nb = iblock - 1;

	const double t00 = t0 + nb*dt;
	const double x  = 2.0 * (t-t00)/dt -1;
	const double fx = 0.5*x*dt;

	return  1.0e-10 * vBlock[nb].eval_pos(x, fx, nf, vfq);	// AU �P�ʂɒ����ĕԂ�
}


vector3d
CF96_planet::eval_vel(const double& t)  const
{
	//-- �u���b�N�����߂�
	int nb;
	nb = (t - t0) / dt;
	if (nb < 0) nb = 0;
	if (nb >= iblock)  nb = iblock - 1;

	const double t00 = t0 + nb*dt;
	const double x = 2.0 * (t - t00) / dt - 1;
	const double fx = 0.5*x*dt;

	return  1.0e-10 * vBlock[nb].eval_vel(dt, x, fx, nf, vfq);	// AU �P�ʂɒ����ĕԂ�
}





//-----------
//  planets
//-----------
bool
CF96_planets::load(FILE* fp)
{
	bLoad = false;
	vPS.clear();

	//-- header
	char cbuf[4];
	fread(cbuf, sizeof(char), 4, fp);
	if (cbuf[0]!='C' || cbuf[1]!='F' || cbuf[2]!='9' || cbuf[3]!='6')  {
		return  false;
	}

	int pln_num;
	fread(&pln_num, sizeof(int), 1, fp);

	for (int i=0; i<pln_num; i++) {
		CF96_planet  ps;
		ps.load(fp);
		vPS.push_back(ps);
	}

	bLoad = true;
	return true;
}


bool
CF96_planets::load(const char* fn)
{
	FILE* fp = fopen(fn, "rb");
	if (fp == NULL)  return false;

	bool res = load(fp);
	fclose(fp);

	return res;
}


vector3d
CF96_planets::eval_pos(int idx, const double& t) const
{
	if (!bLoad)  return vector3d(0,0,0);
	if (idx < 0 || idx >= get_planet_num())  return vector3d(0,0,0);

	return vPS[idx].eval_pos(t);
}


vector3d
CF96_planets::eval_vel(int idx, const double& t) const
{
	if (!bLoad)  return vector3d(0, 0, 0);
	if (idx < 0 || idx >= get_planet_num())  return vector3d(0, 0, 0);

	return vPS[idx].eval_vel(t);
}
