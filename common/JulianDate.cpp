/******************************************************************************

Copyright (c) 2007-2012   Tsunehiko Kato

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
#include  <windows.h>
#include  "JulianDate.h"
#include  <time.h>
#include  <math.h>

// <time.h> �� __time64_t �Ȃǂ́A1970�N����3000�N�܂ł��������Ȃ��̂ŁA�g�p���Ȃ��B

//-------
//  �萔
//-------
const double  _1day_min = 1440.0;											// �P���̕�
const double  _1day_sec = 86400.0;											// �P���̕b��
const int days1[12] = { 0,31,59,90, 120,151,181,212, 243,273,304,334 };		// ���N�̒ʓ�
const int days2[12] = { 0,31,60,91, 121,152,182,213, 244,274,305,335 };		// �[�N�̒ʓ�
const JD	JD_111	 = 1721425.5;											// ����P�N�P���P���O���̃����E�X��
const JD	JD_UTC0 = 2415020 + 25567 + 0.5;								// UTC�̊(1970�N1��1���j��JD


//------------------------
//  UTC���烆���E�X���𓾂�
//------------------------
JD
get_JD(int year, int month, int day, int hour, int min, double sec)
{
	while (month<=0)	{ year--; month+=12; }
	while (month>12)	{ year++; month-=12; }

	double y = double(year-1);
	bool   leap_year = (year%4==0) && ((year%100!=0) || (year%400==0));

	double a,b,c;
	a = floor(365.25*y) - floor(y/100.0) + floor(y/400.0) + 1721425.0;
	b = (leap_year) ? double(days2[month-1]) : double(days1[month-1]);
	c = double(day) + double(hour)/24.0 + double(min)/_1day_min + sec/_1day_sec; 

	return a + b + c - 0.5;
}

JD
UTC_to_JD(const UTC& utc)
{
	return  get_JD(utc.year, utc.month, utc.day, utc.hour, utc.min, utc.sec);
}


//---------------------------
//  �����E�X������UTC�𓾂�
//  (*** year > -4713 ***)
//---------------------------
UTC
JD_to_UTC(const JD& jd)
{
	double	JD1, JD2, sec;
	int     JD0, L, N, I, J;
	int		year, mon, day, hour, min;
	
	JD0 = int(jd+0.5);
	JD1 = double(JD0)-0.5;

	L = JD0 + 68569;
	N = (4*L) / 146097;
	L = L - (146097*N + 3)/4;
	I = (4000*(L+1)) / 1461001;
	L = L - (1461*I)/4 + 31;
	J = (80*L)/2447;
	day = L - (2447*J)/80;
	L = J/11;
	mon = J+2-12*L;
	year = 100*(N-49) + I + L;

	JD2 = jd - JD1;
	hour = int(JD2*24.0);
	min  = int(JD2*1440.0 - hour*60.0);
	sec  = JD2*86400.0 - hour*3600.0 - min*60.0;

	UTC utc;
	utc.year  = year;
	utc.month = mon;
	utc.day   = day;
	utc.hour  = hour;
	utc.min   = min;
	utc.sec   = sec;
	return  utc;
}


//-----------------------
//  ���݂̃����E�X���𓾂�
//-----------------------
JD
get_present_JD()
{
	SYSTEMTIME st;
	GetSystemTime(&st);
	UTC utc;
	utc.year	= st.wYear;
	utc.month	= st.wMonth;
	utc.day		= st.wDay;
	utc.hour	= st.wHour;
	utc.min		= st.wMinute;
	utc.sec		= double(st.wSecond) + 0.001 * double(st.wMilliseconds);
	return  UTC_to_JD(utc);
}


//-----------------
//  ���݂�UTC�𓾂�
//-----------------
UTC
get_present_UTC()
{
	return  JD_to_UTC( get_present_JD() );
}


//--------------------------
//  ���݂̃��[�J���^�C���𓾂�
//--------------------------
UTC
get_present_LocalTime(const double& DT)
{
	return  JD_to_UTC( get_present_JD() + DT/24.0 );
}
