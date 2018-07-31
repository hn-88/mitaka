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
#ifndef  __CONSTELLATION_H__
#define  __CONSTELLATION_H__
#include  "astrometry.h"




//----------------------
//  Constellation Name
//----------------------
enum CNST_ID {
	CNST_AND,
	CNST_MON,
	CNST_SGR,
	CNST_DEL,
	CNST_IND,
	CNST_PSC,
	CNST_LEP,
	CNST_BOO,
	CNST_HYA,
	CNST_ERI,
	CNST_TAU,
	CNST_CMA,
	CNST_LUP,
	CNST_UMA,
	CNST_VIR,
	CNST_ARI,
	CNST_ORI,
	CNST_PIC,
	CNST_CAS,
	CNST_DOR,
	CNST_CNC,
	CNST_COM,
	CNST_CHA,
	CNST_CRV,
	CNST_CRB,
	CNST_TUC,
	CNST_AUR,
	CNST_CAM,
	CNST_PAV,
	CNST_CET,
	CNST_CEP,
	CNST_CEN,
	CNST_MIC,
	CNST_CMI,
	CNST_EQU,
	CNST_VUL,
	CNST_UMI,
	CNST_LMI,
	CNST_CRT,
	CNST_LYR,
	CNST_CIR,
	CNST_ARA,
	CNST_SCO,
	CNST_TRI,
	CNST_LEO,
	CNST_NOR,
	CNST_SCT,
	CNST_CAE,
	CNST_SCL,
	CNST_GRU,
	CNST_MEN,
	CNST_LIB,
	CNST_LAC,
	CNST_HOR,
	CNST_VOL,
	CNST_PUP,
	CNST_MUS,
	CNST_CYG,
	CNST_OCT,
	CNST_COL,
	CNST_APS,
	CNST_GEM,
	CNST_PEG,
	CNST_SER,
	CNST_OPH,
	CNST_HER,
	CNST_PER,
	CNST_VEL,
	CNST_TEL,
	CNST_PHE,
	CNST_ANT,
	CNST_AQR,
	CNST_HYI,
	CNST_CRU,
	CNST_PSA,
	CNST_CRA,
	CNST_TRA,
	CNST_SGE,
	CNST_CAP,
	CNST_LYN,
	CNST_PYX,
	CNST_DRA,
	CNST_CAR,
	CNST_CVN,
	CNST_RET,
	CNST_FOR,
	CNST_SEX,
	CNST_AQL,
	CNST_NONE
};

int	get_constellation_ID(const char* str);
const char*	get_constellation_abbr(const int CnstID);



#endif
