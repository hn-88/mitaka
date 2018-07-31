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
#include  "astrometry.h"
#include  <time.h>



//-------------------------------
//  B1950.0 ���� J2000.0 �ւ̕ϊ�
//-------------------------------
// �u�V�̂̈ʒu�v�Z�vpg.238
//-------------------------------

//-- �ʒu
const matrix3d M_B1950_to_J2000_1(0.9999256782, -0.0111820610, -0.0048579477,
								  0.0111820609,  0.9999374784, -0.0000271765,
								  0.0048579479, -0.0000271474,  0.9999881997);
const matrix3d M_B1950_to_J2000_2(0.0000024239502, -0.0000000271066, -0.0000000117766,
								  0.0000000271066,  0.0000024239788, -0.0000000000659,
								  0.0000000117766, -0.0000000000658,  0.0000024241017);
//-- �ŗL���x
const matrix3d M_B1950_to_J2000_3(-0.00055, -0.23854,  0.43574,
								   0.23849, -0.00267, -0.00854,
								  -0.43562,  0.01225,  0.00212);
const matrix3d M_B1950_to_J2000_4(0.99994704, -0.01118251, -0.00485767,
								  0.01118251,  0.99995883, -0.00002718,
								  0.00485767, -0.00002714,  1.00000956);

//--- �ʒu�̕ϊ� ---

// �ŗL�^������
//  mua0  in sec    per 100 Bessel year
//  mud0  in arcsec per 100 Bessel year
//  Ra0   in km/sec
//  Plx0  in arcsec
RA_DEC
B1950_to_J2000(const RA_DEC& posB1950, const float& mua0, const float& mud0, const float& Ra0, const float& Plx0)
{
	const double D2R = M_PI / 180.0;

	vector3d X0, V0;
	

	double alp0, dlt0, sina0, cosa0, sind0, cosd0;
	alp0 = D2R * posB1950.RAdeg;
	dlt0 = D2R * posB1950.DEdeg;
	sina0 = sin(alp0);
	cosa0 = cos(alp0);
	sind0 = sin(dlt0);
	cosd0 = cos(dlt0);

	// �����x�N�g��(double)
	X0.set(cosd0*cosa0, cosd0*sina0, sind0);

	// �ŗL�^�����x
	V0 = double(mud0) * vector3d(-sind0*cosa0, -sind0*sina0, cosd0)
	   + 15.0 * double(mua0) * vector3d(-cosd0*sina0, cosd0*cosa0, 0.0)
	   + 21.094502 * Plx0 * Ra0 * X0;

	//
	const double  q = dot_product(X0, vector3d(-1.62557, -0.31919, -0.13843));
	const double dq = dot_product(X0, vector3d( 1.245, -1.580, -0.659));

	vector3d X1, V1;
	X1 = X0 + 1e-6 * vector3d(1.62557, 0.31919, 0.13843) + q*1e-6*X0;
	V1 = V0 + 1e-3 * vector3d(-1.245, 1.580, 0.659) + 1e-3*dq*X0;
	
	RA_DEC pos2000;
	vector3d X2000 = M_B1950_to_J2000_1 * X1 + M_B1950_to_J2000_2 * V1;
	pos2000.set_dirc_vec( X2000 );

	return  pos2000;
}

//--- �ŗL�^���̕ϊ� ---
RA_DEC
B1950_to_J2000_pm(const RA_DEC& pos1950, const RA_DEC& pm1950)
{
	RA_DEC pm2000;
	vector3d V2000 = M_B1950_to_J2000_3 * pos1950.get_dirc_vec()
					+M_B1950_to_J2000_4 * pm1950.get_dirc_vec();
	pm2000.set_dirc_vec( V2000 );

	return  pm2000;
}


//-----------
//  �ϊ��s��
//-----------
// �ԓ����W�Ɖ������W
const double incE = 23.43870583;						// �n���̌X��
const double thE  = -incE * M_PI/180;
const double csE( cos(thE) );
const double snE( sin(thE) );
const matrix3d  M_E2R(1,0,0, 0,csE, snE, 0,-snE,csE);	// �������W����ԓ����W�ւ̕ϊ��s��
const matrix3d	M_R2E(1,0,0, 0,csE,-snE, 0, snE,csE);	// �ԓ����W���物�����W�ւ̕ϊ��s��
// ��͍��W������S�������W��
const vector3f  vGC( GC_RA_DEC.get_dirc_vec() );
const vector3f  vGN( GN_RA_DEC.get_dirc_vec() );
const vector3f  vGO( cross_product(vGN, vGC) );
const matrix3d	M_G2R(vGC, vGO, vGN);
const matrix3d	M_R2G( M_G2R.transposed() );
const matrix3d	M_G2E( M_R2E * M_G2R );
const matrix3d	M_E2G( M_G2E.transposed() );
