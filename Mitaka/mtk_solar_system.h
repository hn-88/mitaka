/**************************************************************************************************

Copyright (c) 2007-2012   Tsunehiko Kato  and  4D2U Project, National Astronomical Observatory of Japan

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
#ifndef __MTK_SOLAR_SYSTEM_H__
#define __MTK_SOLAR_SYSTEM_H__
#include  "../common/math/vector3.h"
#include  "mtk_draw_struct.h"



//-----------
//  Defines
//-----------
struct zoom_rate {
	double zmhPlanet, zmhSun, zmhMoonOrb;		// 3D���[�h�ł̊g�嗦
	double zmpPlanet, zmpSunMoon;				// �v���l�^���E�����[�h�ł̊g�嗦
};

const float ObjectNameSizeDef			= 2.8f;
const float SpecialObjectNameSizeDef	= 3.8f;
const float OrbitWidthDef				= 1.2f;




//-------------
//  Functions
//-------------

//--  Load textures  --
void	load_solar_system_textures();

//--  Zoom rate  --
zoom_rate	get_zoom_rate();
void	set_zoom_mode(char mode);
char	get_zoom_mode();

//--  Text size  --
float	get_object_name_size();
float	get_special_object_name_size();
void	set_object_name_size(float sz);
void	set_special_object_name_size(float sz);

//--  Orbit  --
float	get_orbit_width();
void	set_orbit_width(float w);

//--  Lighting  --
float	get_lighting_ambient();
void	set_lighting_ambient(float amb);


//--  Draw  --
void	draw_celestial_object(float Rx, float Ry, float Rz, float tex_shift);



#endif




