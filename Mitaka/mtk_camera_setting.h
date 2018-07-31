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
#ifndef __MTK_CAMERA_SETTING_H__
#define __MTK_CAMERA_SETTING_H__
#include  <stdio.h>
#include  "../common/math/vector3.h"
#include  "../common/math/quaternion.h"
#include  "../common/graphics/3d_common.h"






// ��n�ɑ΂���J�����̎p���E�ʒu
void		set_local_camera(const quaterniond& ori, const vector3d& r0);

void		init_local_camera_angle(const float angle, const float eye_offset);
void		reset_local_camera_angle(const float eye_offset);

void		set_local_camera_pos(const vector3d& r0);
void		set_local_camera_pos(const float eye_offset);


quaterniond	get_local_camera_orientation();
vector3d	get_local_camera_pos();



const double*	get_local_camera_matrix_GL();
const double*	get_local_camera_matrix_GL_inv();

vector3d	get_eU_of_base_on_local_camera();
vector3d	get_base_origin_on_local_camera();

bool	load_local_camera_setting(FILE* fp);

bool	load_frustum(FILE* fp, float z_near, float z_mid, float z_far);
bool	isFrustumLoaded();
frustum	get_loaded_frustum_near();
frustum	get_loaded_frustum_far();

bool	load_glrc_cfg(FILE* fp, float z_near, float z_mid, float z_far);



// ��n
void			set_base_camera(const quaterniond& ori);
quaterniond		get_base_camera_ori();
bool			load_base_camera_setting(FILE* fp);




// �����̒���
float	get_eye_distance_2();
void	set_eye_distance_2(float d, char CLR);




#endif
