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
#ifndef  __COLOR_H__
#define  __COLOR_H__
#include  <windows.h>
#include  <math.h>
#include  "../math/vector3.h"


//--- Data types ---
typedef	vector3<float>	XYZf;
typedef	vector3<float>	RGBf;

class RGBAf
{
public:
	union {
		float	rgba[4];
		struct {
			RGBf	rgb;
			float	a;
		};
	};
public:
	RGBAf()																	{}
	RGBAf(const float& R, const float& G, const float& B, const float& A)	{ rgb.r=R; rgb.g=G; rgb.b=B; a=A; }
	RGBAf(const RGBf& _RGB, const float& A)									{ rgb = _RGB; a=A; }
	void	set(const float& R, const float& G, const float& B, const float& A) { rgb.r=R; rgb.g=G; rgb.b=B; a=A; }
	float*			get_pointer()		{ return rgba; }
	const float*	get_pointer() const	{ return rgba; }
};


//----------
//  Common
//----------
inline  void
clamp(float& x)
{
	if (x>1.0f) x=1.0f;	else if (x<0.0f) x=0.0f;
}


//--------
//  RGBf
//--------
inline void clamp(RGBf& rgb)
{
	clamp(rgb.r);
	clamp(rgb.g);
	clamp(rgb.b);
}

inline float
max_level(const RGBf& rgb) {
	float mx = (rgb.r >= rgb.g) ? rgb.r : rgb.g;
	mx = (mx > rgb.b) ? mx : rgb.b;
	return mx;
}

inline float
min_level(const RGBf& rgb) {
	return  min( min(rgb.r, rgb.g), rgb.b);
}


//---------
//  RGBAf
//---------
inline void clamp(RGBAf& rgba)
{
	clamp(rgba.rgb);
	clamp(rgba.a);
}

inline float
max_level(const RGBAf& rgba) {
	return  max( max_level(rgba.rgb), rgba.a);
}

inline float
min_level(const RGBAf& rgba) {
	return  min( min_level(rgba.rgb), rgba.a);
}


//-------------
//  Transform
//-------------

// sRGB (IEC 61966-2-1, see http://www.srgb.com/basicsofsrgb.htm )
inline  RGBf
XYZ_to_RGB(const XYZf& xyz)
{
	const matrix3f  A(	 3.2406f,	-1.5372f,	-0.4986f,
						-0.9689f,	 1.8758f,	 0.0415f,
						 0.0557f,	-0.2040f,	 1.0570f );
	return  A * xyz;
}

inline  XYZf
RGB_to_XYZ(const RGBf& rgb)
{
	const matrix3f A(	0.4124f,	0.3576f,	0.1805f,
						0.2126f,	0.7152f,	0.0722f,
						0.0193f,	0.1192f,	0.9505f );
	return  A * rgb;
}

XYZf	get_XYZ_value(const float& lmd);
XYZf	make_XYZ_color(float f(const float& lmd));

// gamma correction
float	correct_gamma(const float& val);


// RGB
inline	RGBf	get_RGB_value(const float& lmd)				{ return  XYZ_to_RGB( get_XYZ_value(lmd)); }
inline	RGBf	make_RGB_color(float f(const float& lmd))	{ return  XYZ_to_RGB( make_XYZ_color(f) ); }
inline  RGBf
scale_by_max(const RGBf& rgb0, bool bForce) {
	RGBf rgb = rgb0;
	float  col_max = max_level(rgb);	// ��ԃ��x�������������ŋK�i������
	if (bForce || col_max > 1.0) {
		rgb /= col_max;
	}
	return rgb;
}

RGBf	correct_RGB0(const RGBf& rgb);		// �K���}�␳�i���x��D��j
RGBf	correct_RGB(const RGBf&  rgb);		// �K���}�␳�i�F������D��j


// Black body
float	PlanckLaw(const float& T, const float& lmd_nm);
XYZf	blackbody_color_XYZ(const float T);
inline  RGBf	blackbody_color(const float T)				{ return  XYZ_to_RGB(blackbody_color_XYZ(T)); }
float	bb_max_level(const float T1, const float T2);



#endif