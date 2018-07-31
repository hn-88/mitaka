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
#include  "mtk_solar_system.h"
#include  "mtk_colors.h"
#include  "mtk_sun.h"
#include  "mtk_planets.h"
#include  "mtk_satellites.h"
#include  "mtk_minor_planets.h"
#include  "mtk_oort_cloud.h"
#include  "mtk_label.h"
#include  "mtk_objects.h"
#include  "mtk_object_position.h"
#include  "mtk_time.h"
#include  "mtk_mode.h"



//-------------
//  Text size
//-------------
static float  ObjectNameSize, SpecialObjectNameSize;

//---------
//  Orbit
//---------
static float OrbitWidth;

//-------------
//  Zoom mode
//-------------
const zoom_rate ZoomRate[5] = {
	{   1,  1,  1,   1,  1},
	{   2,  2,  1,   2,  2},
	{ 500,  2, 25,  20,  2},
	{1000, 25, 50, 100,  5},
	{1500, 50, 70, 200, 10}
};

static char ZoomMode = 0;


//------------
//  Lighting
//------------
static float LightingAmbient;
float LightingAmbientDef = 0.1;


void	set_side_light(const mtk_draw_struct& ds);



//-----------------
//  Load textures
//-----------------
void
load_solar_system_textures()
{
	//-- Sun
	load_sun_textures();

	//-- Planets
	load_planets_textures();

	//-- Satellite
	load_satellites_textures();

	//-- Minor planets
	load_minor_planet_textures();

	//-- Ring
	load_ring_textures();
}







//-------------
//  Zoom rate
//-------------
zoom_rate
get_zoom_rate()
{
	return ZoomRate[ZoomMode];
}

void
set_zoom_mode(char zoom_mode)
{
	ZoomMode = zoom_mode;
	const double  t = get_time_JD();

	char mode = get_mode();
	set_satellites_zoom_rate(mode);

	update_objects_position_and_orientation(t, true);
	update_space_curves(t);
}

char
get_zoom_mode()
{
	return ZoomMode;
}




//-------------
//  Text size
//-------------
float
get_object_name_size()
{
	return  ObjectNameSize;
}

float
get_special_object_name_size()
{
	return  SpecialObjectNameSize;
}

void
set_object_name_size(float sz)
{
	ObjectNameSize = sz;
}

void
set_special_object_name_size(float sz)
{
	SpecialObjectNameSize = sz;
}


//---------
//  Orbit
//---------
float
get_orbit_width()
{
	return  OrbitWidth;
}

void
set_orbit_width(float w)
{
	OrbitWidth = w;
}


//------------
//  Lighting
//------------
float
get_lighting_ambient()
{
	return  LightingAmbient;
}

void
set_lighting_ambient(float amb)
{
	LightingAmbient = amb;
}




//--------
//  Draw
//--------
void
draw_celestial_object(float Rx, float Ry, float Rz, float tex_shift)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glPushMatrix();
		if (tex_shift != 0.0f) {
			glRotatef(tex_shift, 0, 0, 1);
		}
		glScalef(Rx, Ry, Rz);
		glEnable(GL_CULL_FACE);
		gl_draw_LOD_sphere();
	glPopMatrix();
}





