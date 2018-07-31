/******************************************************************************

Copyright (c) 2007-2014   Tsunehiko Kato

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
#ifndef  __CELE_MECH_H__
#define  __CELE_MECH_H__
#include  "../math/vector3.h"
#include  "../JulianDate.h"


//----------------------------------------------
//  �O����̈ʒu�����߂�i�O�����W�n�F�œ_�����_�j
//    a:  �O�������a
//    e:  ���S��
//    M0: ���ϋߓ_���p�i�����A�x�j
//    n:  ���ω^���i�x�A�P�ʎ��Ԃ�����j
//    epoch: ����
//    t:  �ʒu�����߂鎞��
//----------------------------------------------
vector3d	calc_orbital_pos(const double& a, const double& e, const double& M0, const double& n, const JD& epoch, const JD& t);

//  �O�����W�n�ł̑��x�x�N�g�������߂�
vector3d	calc_orbital_velocity(const vector3d& pos, const double& a, const double& e, const double& n);


//---------------------------------------------------
//  �Q�̍��W�Ԃ̕ϊ�
//---------------------------------------------------
//   node: ����_���o, i: �O���X�Ίp, omg: ��(��)�_����
//  * �p�x�͑S�ēx��P�ʂƂ���
//---------------------------------------------------
inline matrix3d
calc_trans_matrix(const double& node, const double& i, const double& omg)
{
	return  Rzxz3(D2R*node, D2R*i, D2R*omg);
}

inline vector3d
calc_pos_on_ref_plane(const double& node, const double i, const double& omg, const vector3d& r)
{
	return  Rzxz3(D2R*node, D2R*i, D2R*omg) * r;
}


#endif
