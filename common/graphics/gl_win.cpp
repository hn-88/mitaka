/******************************************************************************

Copyright (c) 2007-2015   Tsunehiko Kato

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
#include  <stdio.h>
#include  <string.h>
#include  "gl_win.h"




BOOL(WINAPI *wglSwapIntervalEXT)(int) = NULL;





//===========
//  COpenGL
//===========

//--- initialize OpenGL ---
bool
COpenGL::init(HDC hDC, const PIXELFORMATDESCRIPTOR& _pfd)
{
	PIXELFORMATDESCRIPTOR  pfd = _pfd;
	int		pixelFormat;
	BOOL	succes;

	pixelFormat	=	ChoosePixelFormat( hDC, &pfd );
	succes		=	SetPixelFormat( hDC, pixelFormat, &pfd );
	hRC			=	wglCreateContext(hDC);

	return  (succes == TRUE);
}


bool
COpenGL::MakeCurrent(HDC hDC)
{
	if (hRC == NULL)	return false;

	BOOL res;

	if (hDC != NULL)	res = wglMakeCurrent(hDC,  hRC);
	else				res = wglMakeCurrent(NULL, NULL);

	return  (res==TRUE);
}


bool
initGL_Window(COpenGL& ogl, HDC hDC)
{
	PIXELFORMATDESCRIPTOR  pfd = 
	{
		sizeof(pfd),				//  size of pfd
		1,							//  version
		PFD_DRAW_TO_WINDOW |		//  write on window
			PFD_SUPPORT_OPENGL |	//  support OpenGL
			PFD_DOUBLEBUFFER,		//  support double buffer
		PFD_TYPE_RGBA,				//  RGBA type
		24,							//  24-bit color depth
		0, 0, 0, 0, 0, 0,			//  ignored
		0, 0, 0, 0, 0, 0, 0,		//  no alpha and accumulation buffer
		32,							//  32-bit z-buffer
		0, 0,						//  no stencil and axiliary buffer
		PFD_MAIN_PLANE,				//  main layer (ignored)
		0,							//  reserved
		0, 0, 0						//  layer masks (ignored)
	};
	return  ogl.init(hDC, pfd);
}


bool
initGL_Window_stencil(COpenGL& ogl, HDC hDC)
{
	PIXELFORMATDESCRIPTOR  pfd = 
	{
		sizeof(pfd),				//  size of pfd
		1,							//  version
		PFD_DRAW_TO_WINDOW |		//  write on window
			PFD_SUPPORT_OPENGL |	//  support OpenGL
			PFD_DOUBLEBUFFER,		//  support double buffer
		PFD_TYPE_RGBA,				//  RGBA type
		24,							//  24-bit color depth
		0, 0, 0, 0, 0, 0,			//  ignored
		0, 0, 0, 0, 0, 0, 0,		//  no alpha and accumulation buffer
		32,							//  32-bit z-buffer
		8, 0,						//  stencil and axiliary buffer
		PFD_MAIN_PLANE,				//  main layer (ignored)
		0,							//  reserved
		0, 0, 0						//  layer masks (ignored)
	};
	return  ogl.init(hDC, pfd);
}


bool
initGL_DIB(COpenGL& ogl, HDC hDC)
{
	PIXELFORMATDESCRIPTOR  pfd = 
	{
		sizeof(pfd),				//  size of pfd
		1,							//  version
		PFD_DRAW_TO_BITMAP |		//  write on BITMAP
			PFD_SUPPORT_OPENGL,		//  support OpenGL
		PFD_TYPE_RGBA,				//  RGBA type
		24,							//  24-bit color depth
		0, 0, 0, 0, 0, 0,			//  ignored
		0, 0, 0, 0, 0, 0, 0,		//  no alpha and accumulation buffer
		32,							//  32-bit z-buffer
		0, 0,						//  no stencil and axiliary buffer
		PFD_MAIN_PLANE,				//  main layer (ignored)
		0,							//  reserved
		0, 0, 0						//  layer masks (ignored)
	};
	return  ogl.init(hDC, pfd);
}


//-----------
//  2D mode
//-----------
void
gl_enter_2D_mode()
{
	int vp[4];
	glGetIntegerv(GL_VIEWPORT, vp);

	glPushAttrib(GL_TEXTURE_BIT | GL_ENABLE_BIT | GL_VIEWPORT_BIT);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();
		glOrtho(vp[0], vp[0]+vp[2], vp[1]+vp[3], vp[1], -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
}

void
gl_enter_2D_mode_vwpt(int x0, int y0, int w, int h)
{
	gl_enter_2D_mode();
	glViewport(x0,y0,w,h);
}




void
gl_leave_2D_mode()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glPopAttrib();
}




//--------------
//  Extensions
//--------------
void
gl_init_extensions()
{
	//---  wglSwapIntervalEXT
	if (strstr((char*)glGetString(GL_EXTENSIONS), "WGL_EXT_swap_control") != 0)
	{
		wglSwapIntervalEXT = (BOOL(WINAPI*)(int))wglGetProcAddress("wglSwapIntervalEXT");
		if (wglSwapIntervalEXT)
		{
			wglSwapIntervalEXT(0);
		}
	}
}


// ��������
void
gl_set_interval(bool bItvl)
{
	if (wglSwapIntervalEXT == 0)  return;

	int itvl = (bItvl) ? 1 : 0;
	wglSwapIntervalEXT(itvl);
}