/**************************************************************************************************

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
#include  "mitaka.h"
#include  "mtk_floating_object.h"
#include  "mtk_objects.h"
#include  "mtk_target.h"
#include  "mtk_switches.h"


void	OnSetZoom(double val);



//*********
extern mtk_camera  g_camera;

void	OnTarget(int tgt_ID);
void	OnChangeFloatingObjectPos(const vector3f& pos);



static int floating_object_ID;
static int previous_object_ID;
static int next_object_ID;
static double prev_log_scale;
static double next_log_scale;
static double duration;

static bool bChangingTarget = false;
static float frac;
static vector3d  floating_object_pos;
static double  floating_object_scale;

static bool bChangingScale = false;





//------------
//  Register
//------------
void
register_floating_object()
{
	mtk_object  obj;

	obj.init(OBJ_TYPE_FLOATING_OBJECT, 0, 0.0f, false);
	floating_object_ID = register_object(obj, "FloatingObject");
}



//------------
//  Position
//------------
void
set_floating_object_position_and_scale(double t)
{
	const mtk_object&  obj1 = get_object( previous_object_ID );
	const mtk_object&  obj2 = get_object( next_object_ID );

	const double f = next_log_scale - prev_log_scale;
	const double S = exp(f*t);
	double s;
	if (fabs(f) < 0.01) {
		s = t;
	} else {
		s = 1.0 / (exp(f) - 1.0) * (S - 1);
	}


	vector3d  eF, eU, eR;
	g_camera.get_axes(eF, eU, eR);


	vector3d  pos1 = obj1.get_pos() - eF * obj1.get_landing_radius();
	vector3d  pos2 = obj2.get_pos() - eF * obj2.get_landing_radius();

	floating_object_pos = (1.0 - s) * pos1 + s * pos2;
	floating_object_scale = exp(prev_log_scale) * S;
}



vector3d
get_floating_object_position()
{
	return  floating_object_pos;
}

void
set_floating_object_position(const vector3d& pos)
{
	floating_object_pos = pos;
}

double
get_floating_object_scale()
{
	return  floating_object_scale;
}


//-------------------
//  Changing target
//-------------------
void
set_changing_target(int prev_ID, int next_ID)
{
	OnTarget( floating_object_ID );


	previous_object_ID = prev_ID;
	next_object_ID = next_ID;

	double prev_scale = get_current_scale();
	prev_log_scale = log( prev_scale );

	double  scale2 = get_object( next_ID ).get_scale();
	if (scale2 > prev_scale ) {
		next_log_scale = log( scale2 );
	} else {
		next_log_scale = prev_log_scale;
	}

	duration = 1.0 * fabs( next_log_scale - prev_log_scale ) / 4.6;
	if (duration < 0.5)  duration = 0.5;

	frac = 0.0;
	set_floating_object_position_and_scale( frac );
	bChangingTarget = true;
}



bool
is_changing_target()
{
	return  bChangingTarget;
}




void
process_changing_target()
{
	if (frac >= 1.0f) {
		bChangingTarget = false;
		OnTarget( next_object_ID );
		return;
	}

	const float dt = get_timer_interval() / 1000.0f;
	frac += dt / duration;
	if (frac > 1.0)  frac = 1.0;
	set_floating_object_position_and_scale( frac );

	reset_selected_object();

	OnChangeFloatingObjectPos( floating_object_pos );
	OnSetZoom( floating_object_scale );
}







//------------------
//  Changing scale
//------------------
void
set_scale_frac(double t)
{
	double ln;
	ln  = (1.0 - t) * prev_log_scale + t * next_log_scale;
	floating_object_scale = exp(ln);
}


void
set_changing_scale(double final_scale)
{
	prev_log_scale = log( get_current_scale() );
	next_log_scale = log( final_scale );

	duration = 1.0 * fabs( next_log_scale - prev_log_scale ) / 4.6;
	if (duration < 0.5)  duration = 0.5;

	frac = 0.0;
	set_scale_frac( frac );
	bChangingScale = true;
}


bool
is_changing_scale()
{
	return  bChangingScale;
}



void
process_changing_scale()
{
	if (frac >= 1.0f) {
		bChangingScale = false;
		return;
	}

	const float dt = get_timer_interval() / 1000.0f;
	frac += dt / duration;
	if (frac > 1.0)  frac = 1.0;
	set_scale_frac( frac );

	OnSetZoom( floating_object_scale );
}
