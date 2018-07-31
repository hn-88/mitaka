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
#ifndef __GL_MODEL3D_H__
#define __GL_MODEL3D_H__
#include  "Model3D.h"
#include  "../win_file.h"



// �����_�����O���[�h
#define MODEL_RENDERING_DEFAULT			0
#define MODEL_RENDERING_WIREFRAME		1
#define MODEL_RENDERING_FLAT_SHADING	2
#define MODEL_RENDERING_TWO_SIDED		4


//=======================
//  3D Model for OpenGL
//=======================
class glModel3D  :  public Model3D
{
private:
	GLuint			texname[256];
	unsigned char	rendering_mode;
public:
	glModel3D()  :  Model3D(), rendering_mode(MODEL_RENDERING_DEFAULT)	{}
	void	clear()  { clear_textures(); Model3D::clear(); }
	void			set_rendering_mode(unsigned char Flg)	{ rendering_mode = Flg; }
	unsigned char	get_rendering_mode() const				{ return rendering_mode; }
	bool	get_wireframe() const		{ return (rendering_mode & MODEL_RENDERING_WIREFRAME); }
	bool	get_flat_shading() const	{ return (rendering_mode & MODEL_RENDERING_FLAT_SHADING); }
	bool	get_two_sided() const		{ return (rendering_mode & MODEL_RENDERING_TWO_SIDED); }
	void	bind_textures(const directory& dir);
	void	clear_textures();
	void	draw_object(int obj_idx)  const;
	void	draw()  const;
	void	draw_triangle_mesh(int obj_idx) const;
};


#endif