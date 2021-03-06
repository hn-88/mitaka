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
#ifndef __MTK_PLANET_ORIENTATION_H__
#define __MTK_PLANET_ORIENTATION_H__
#include  "../common/math/vector3.h"
#include  "../common/math/quaternion.h"





void	calc_planet_ec_to_pl_matrix(int idx, const double& t, matrix3d& M0, matrix3d& M);



const matrix3d&	get_planet_orientation(int idx, bool bRotation);
void	set_planet_orientation_GL(int idx);


inline  void
get_planet_bases(int idx, vector3d& ex, vector3d& ey, vector3d& ez)
{
	get_planet_orientation(idx, true).get_bases_tr(ex, ey, ez);
}





vector2d	get_planet_lon_lat_ec(int idx, const vector3d& nv);
vector3d	get_planet_nv_ec(int idx, const double& lon, const double& lat);

quaterniond	get_ori_from_planetarium_dirc(int idx, const double& lon, const double& lat, const double& thd);
quaterniond	get_planetarium_ori(int idx, const double& lon, const double& lat, const double& azim, const double& alti);

double		get_dirc_from_3D_ori(int idx, const quaterniond& ori);




#endif
