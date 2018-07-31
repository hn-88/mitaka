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
#ifndef  __ASTRO_CONST_H__
#define  __ASTRO_CONST_H__
#include <math.h>


//-----------------------------------------------
//  IAU(1976) System of Astronomical Constatnts
//-----------------------------------------------
const double speed_of_light = 299792458.0;		// Speed of light  [m/s]
const double k_Gauss		= 0.01720209895;	// Gaussian gravitational constant
const double G_grav			= 6.672e-11;		// Constant of gravitation [m^3 kg^-1 s^-2]

const double	J2000 = 2451545.0;					// (J2000.0 = JD2451545.0)
const double	epsJ2000 = 84381.448 / (60.0*60.0);	// obliquity at J2000.0

//-- IERS numerical standard
const double _1AU_sec   = 499.0047838061;
const double _1AU_meter = speed_of_light * _1AU_sec;

//
const double _1ly_meter = speed_of_light * 365.25 * 24.0 * 60.0 * 60.0;
const double _1pc_meter = _1AU_meter / sin(M_PI/180.0 /60.0/60.0);

//
const float _1AU_km = _1AU_meter * 1e-3;
const float _1km_AU = 1.0 / _1AU_km;
const float _1ly_km = _1ly_meter * 1e-3;
const float _1km_ly = 1.0 / _1ly_km;
const float _1pc_km = _1pc_meter * 1e-3;
const float _1km_pc = 1.0 / _1pc_km;

//
const float _1AU_pc = _1AU_meter/_1pc_meter;
const float _1pc_AU = _1pc_meter/_1AU_meter;
const float _1pc_ly = _1pc_meter/_1ly_meter;
const float _1ly_pc = _1ly_meter/_1pc_meter;


#endif
