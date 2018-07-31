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
#ifndef __CF96_H__
#define __CF96_H__
#include  <vector>
using namespace std;
#include  "../math/vector3.h"


//=================================
// Reference:
//   Planetary Series (1996)
//   by J.Chapront and G. Francou 
//=================================

//---------
//  const
//---------
const double  CF96_t1 = 2415020.5;
const double  CF96_t2 = 2487980.5;


//---------
//  block
//---------
class CF96_block {
	vector<double>						sec[3];
	vector<vector<vector<double> > >	vvvct;
	vector<vector<vector<double> > >	vvvst;
public:
	bool	load(FILE* fp);
	vector3d	eval_pos(const double& x, const double& fx, const vector<int>& nf, const vector<vector<double> >& vfq)  const;
	vector3d	eval_vel(const double& dt, const double& x, const double& fx, const vector<int>& nf, const vector<vector<double> >& vfq)  const;
};


//----------
//  planet
//----------
class CF96_planet
{
	double	t0, dt;
	int		mx, imax, iblock, max_sec;
	vector<int>	nf;
	vector<vector<double> > vfq;
	vector<CF96_block>	vBlock;
public:
	bool	load(FILE* fp);
	vector3d	eval_pos(const double& t)  const;
	vector3d	eval_vel(const double& t)  const;
};


//-----------
//  planets
//-----------
class CF96_planets
{
	bool	bLoad;
	vector<CF96_planet>  vPS;
public:
	CF96_planets() : bLoad(false) {};
	bool		isLoaded() const { return bLoad; }
	bool		isInRange(const double& t)	{ return (t >= CF96_t1 && t <= CF96_t2); }
	bool		load(FILE* fp);
	bool		load(const char* fn);
	int			get_planet_num() const	{ return vPS.size(); }
	vector3d	eval_pos(int idx, const double& t) const;
	vector3d	eval_vel(int idx, const double& t) const;
};


#endif