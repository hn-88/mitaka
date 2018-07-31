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
#include  <string.h>
#include  "Constellation.h"



//-----------------
//  Constellation
//-----------------
const static char ConstellationAbbr[88][4] = {
	"And", "Mon", "Sgr", "Del", "Ind",
	"Psc", "Lep", "Boo", "Hya", "Eri",
	"Tau", "CMa", "Lup", "UMa", "Vir",
	"Ari", "Ori", "Pic", "Cas", "Dor",
	"Cnc", "Com", "Cha", "Crv", "CrB",
	"Tuc", "Aur", "Cam", "Pav", "Cet",
	"Cep", "Cen", "Mic", "CMi", "Equ",
	"Vul", "UMi", "LMi", "Crt", "Lyr",
	"Cir", "Ara", "Sco", "Tri", "Leo",
	"Nor", "Sct", "Cae", "Scl", "Gru",
	"Men", "Lib", "Lac", "Hor", "Vol",
	"Pup", "Mus", "Cyg", "Oct", "Col",
	"Aps", "Gem", "Peg", "Ser", "Oph",
	"Her", "Per", "Vel", "Tel", "Phe",
	"Ant", "Aqr", "Hyi", "Cru", "PsA",
	"CrA", "TrA", "Sge", "Cap", "Lyn",
	"Pyx", "Dra", "Car", "CVn", "Ret",
	"For", "Sex", "Aql"
};


int
get_constellation_ID(const char* str) {
	for (int i=0; i<88; i++) {
		if (stricmp(str, ConstellationAbbr[i])==0)  return i;
	}
	return CNST_NONE;
}


const char*
get_constellation_abbr(const int CnstID) {
	if (CnstID >= 88)  return 0;
	return  ConstellationAbbr[CnstID];
}
