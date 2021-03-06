/**************************************************************************************************

Copyright (c) 2007   Tsunehiko Kato  and  4D2U Project, National Astronomical Observatory of Japan

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

以下に定める条件に従い、本ソフトウェアおよび関連文書のファイル（以下「ソフトウェア」）の複製を取得す
るすべての人に対し、ソフトウェアを無制限に扱うことを無償で許可します。これには、ソフトウェアの複製を
使用、複写、変更、結合、掲載、頒布、サブライセンス、および/または販売する権利、およびソフトウェアを
提供する相手に同じことを許可する権利も無制限に含まれます。

上記の著作権表示および本許諾表示を、ソフトウェアのすべての複製または重要な部分に記載するものとしま
す。

ソフトウェアは「現状のまま」で、明示であるか暗黙であるかを問わず、何らの保証もなく提供されます。ここ
でいう保証とは、商品性、特定の目的への適合性、および権利非侵害についての保証も含みますが、それに限定
されるものではありません。 作者または著作権者は、契約行為、不法行為、またはそれ以外であろうと、ソフ
トウェアに起因または関連し、あるいはソフトウェアの使用またはその他の扱いによって生じる一切の請求、損
害、その他の義務について何らの責任も負わないものとします。
***************************************************************************************************/
#include  "mtk_fixed_object.h"
#include  "mtk_object.h"
#include  "mtk_calc_alpha.h"
#include  "mtk_switches.h"
#include  "mtk_our_galaxy.h"
#include  "../common/astro/astro_const.h"
#include  <vector>


struct fixed_object
{
	vector3d  pos;
	int  obj_type;
};

static  std::vector<fixed_object>	vFixedObjects;




//--------
//  Init
//--------
int
add_fixed_object(const vector3d& pos, int obj_type)
{
	fixed_object  fo;
	fo.pos = pos;
	fo.obj_type = obj_type;


	vFixedObjects.push_back( fo );
	return  vFixedObjects.size() - 1;
}


//------------
//  Position
//------------
vector3d
get_fixed_object_position(int idx)
{
	return  vFixedObjects[idx].pos;
}



//---------
//  Label
//---------
bool
is_fixed_object_label_visible(const mtk_draw_struct& ds, int idx)
{
	switch  ( vFixedObjects[idx].obj_type ) {
		case OBJ_TYPE_SMALL_OBJECT:
			return  (test_disp_switch(DISPSW_MINOR_PLANET, DISPM_DISP) && test_disp_switch(DISPSW_MINOR_PLANET, DISPM_NAME));
		case OBJ_TYPE_OUR_GALAXY:
			return  is_our_galaxy_label_visible(ds);
		default:	break;
	}

	return  true;
}


float
get_fixed_object_label_alpha(const mtk_draw_struct& ds, int idx)
{
	float alp = 1.0f;
	switch  ( vFixedObjects[idx].obj_type ) {
		case OBJ_TYPE_SMALL_OBJECT:
			alp =  calc_alpha_by_disapear_scale_inside( ds.scale, 2.3*_1AU_pc );
			break;
		case OBJ_TYPE_OUR_GALAXY:
			return  get_our_galaxy_label_alpha(ds);
		default:
			break;
	}

	const vector3d& pos  = vFixedObjects[idx].pos;
	const vector3d  rpos = pos - ds.camera.get_pos();
	float  Rv  = pos.norm();
	float  Dth = 2.0 * Rv / rpos.norm();

	return  alp * calc_alpha_by_apparent_diameter( Dth );
}



//--------
//  Type
//--------
int
get_fixed_object_type(int idx)
{
	return  vFixedObjects[idx].obj_type;
}



