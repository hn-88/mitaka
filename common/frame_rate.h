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
#ifndef __FRAME_RATE_H__
#define __FRAME_RATE_H__
#include  <windows.h>
#include  <Mmsystem.h>
#include  <vector>
using namespace std;


class fps_counter
{
	vector<float>	vFPS, vETM;
	int idx;
	DWORD	time_in;
	bool	bFull;

public:
	fps_counter()		: idx(0), vFPS(10), vETM(10), bFull(false) {};
	fps_counter(int N)	: idx(0), vFPS(N), vETM(N),  bFull(false) {};

	void	begin_frame()		{ time_in = timeGetTime(); }
	void	end_frame()		{

		DWORD dt = timeGetTime() - time_in;

		if (dt == 0) {
			vFPS[idx] = 10000.0f;
		}
		else {
			vFPS[idx] = 1000.0f / float(dt);
		}
		vETM[idx] = float(dt);

		idx++;
		if (idx >= vFPS.size()) { idx=0; bFull=true; }
	}

	float	get_fps() const {
		int N = (bFull)  ?  vFPS.size()  :  idx;
		if (N == 0)  return 0;
		
		float sum = 0.0f;
		for (int i=0; i<N; i++) {
			sum += vFPS[i];
		}

		return sum/N;
	}

	float get_elapsed_time() const {

		int N = (bFull) ? vETM.size() : idx;
		if (N == 0)  return 0;

		float sum = 0.0f;
		for (int i = 0; i<N; i++) {
			sum += vETM[i];
		}

		return sum / N;
	}
};



#endif
