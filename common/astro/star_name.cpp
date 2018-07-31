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
#include  <ctype.h>
#include  <string.h>
#include  "star_name.h"


//----------------
//  Greek letter
//----------------
const static char GreekLetter[][4] = {
	"Alp", "Bet", "Gam", "Del",
	"Eps", "Zet", "Eta", "The",
	"Iot", "Kap", "Lam", "Mu",
	"Nu",  "Xi",  "Omi", "Pi",
	"Rho", "Sig", "Tau", "Ups",
	"Phi", "Chi", "Psi", "Ome"
};

const static char GreekLetter_J[][4] = {
	"��", "��", "��", "��",
	"��", "��", "��", "��",
	"��", "��", "��", "��",
	"��", "��", "��", "��",
	"��", "��", "��", "��",
	"��", "��", "��", "��"
};


char
get_Greek_ID(const char* str) {
	for (char i=0; i<24; i++) {
		if (stricmp(str, GreekLetter[i])==0)  return i;
	}
	return GREEK_NONE;
}

const char*
get_Greek_letter(const char GreekID) {
	if (GreekID >= 24)  return 0;
	return  GreekLetter[GreekID];
}

const char*
get_Greek_letter_J(const char GreekID) {
	if (GreekID >= 24)  return 0;
	return  GreekLetter_J[GreekID];
}


//---------------------------------
//  Bayer Name / Framsteed Number
//---------------------------------
inline const char*
skip_white_space(const char* p)
{
	if (*p == '\0')  return p;

	char ch;
	while ((ch=*p)== ' ' || ch=='\n' || ch=='\t')  { p++; }
	return p;
}


//-- �w�肵��������̃o�C�G������ID�����߂�
char
get_Bayer_ID(const char* str, char* BayerSubNo)
{
	*BayerSubNo = -1;

	const char* p = str;
	p = skip_white_space(p);	// �󔒂��X�L�b�v

	//--- Bayer ID
	char buf[4];
	int count=0;
	while ( count<3 && isalpha(*p) )  { buf[count]=*p; p++; count++; }
	if (count != 2 && count != 3)  return -1;	// �M���V�������̗����͂Q�������R����

	buf[count] = '\0';
	char BayerID = get_Greek_ID(buf);
	if (BayerID == GREEK_NONE)  return -1;
	p = skip_white_space(p);	// �󔒂��X�L�b�v
	if (*p=='\0')  return BayerID;
	
	//--- Bayer Sub Number
	*BayerSubNo = (isdigit(*p)) ? *p-'0' : -1;

	//--- ���Ƃɉ����������Ƃ��`�F�b�N����
	p++;
	p = skip_white_space(p);
	if (*p != '\0')  return -1;

	return BayerID;
}


//-- �w�肵��������̃t�����X�`�[�h�ԍ��𓾂�
short
get_Framsteed_No(const char* str)
{
	const char* p = str;
	p = skip_white_space(p);	// �󔒂��X�L�b�v

	//--- Framsteed Number
	char buf[4];
	int count = 0;
	while (count<3 && isdigit(*p)) { buf[count]=*p; p++; count++; }
	if (count==0)  return -1;

	buf[count] = '\0';
	short Framsteed;
	sscanf(buf, "%hd", &Framsteed);
	if (*p=='\0')  return Framsteed;

	//--- ���Ƃɉ����������Ƃ��`�F�b�N����
	p++;
	p = skip_white_space(p);
	if (*p != '\0')  return -1;

	return Framsteed;
}

