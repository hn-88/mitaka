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
#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__
#include  <dinput.h>


//-----------------------------------------------------
//  class joystick
//    ����: ��������� SetCooperativeLevel �����s���邱��
//-----------------------------------------------------
class joystick
{
private:
	LPDIRECTINPUT8			pDI;		// DirectInput8�f�o�C�X 
	LPDIRECTINPUTDEVICE8	pJoystick;	// Joystick�f�o�C�X
	int	nAxis, nButton;
public:
	joystick();
	~joystick();
	HRESULT	SetCooperativeLevel(HWND hWnd, DWORD dwFlags);
	bool	isValid() const	{ return ((pDI!=NULL) && (pJoystick!=NULL)); }
	HRESULT	GetJoystickState(DIJOYSTATE2* pJs) const;
	int		get_axis_num() const	{ return nAxis; }
	int		get_button_num() const	{ return nButton; }
	HRESULT	Acquire()	{ if (!isValid()) return E_FAIL;  return pJoystick->Acquire(); }
	HRESULT	Unacquire()	{ if (!isValid()) return E_FAIL;  return pJoystick->Unacquire(); }
private:
	static BOOL CALLBACK EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance,
												VOID* pContext );
	static BOOL CALLBACK EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi,
											  VOID* pContext );
};


//-------------------------------
// Input (ELECOM �p�̃L�[�o�C���h�j
//-------------------------------
enum axis_type
{
	AXIS_X,
	AXIS_Y,
	AXIS_RZ,
	AXIS_Z
};

enum button_type
{
	BTN_TRI,
	BTN_CIR,
	BTN_CRS,
	BTN_SQR,
	BTN_L2,
	BTN_R2,
	BTN_L1,
	BTN_R1,
	BTN_PLAY,
	BTN_SELECT,
	BTN_L3,
	BTN_R3,
	BTN_UP,
	BTN_RIGHT,
	BTN_DOWN,
	BTN_LEFT
};


class axis_state
{
public:
	int		value, last_value;
	bool	bToggle;
	int		axis_no;
	int		thr_val, max_val;
	int		key_code1, key_code2;
	int		key_val1, key_val2;
	float	power_index;
public:
	axis_state() : power_index(1)  {};
	void	set(int _axis_no, int _thr_val, int _max_val, int _key_code1, int _key_val1, int _key_code2, int _key_val2);
	void	key_in();
	void	joystick_in(DIJOYSTATE2& djs);
	float	get_normalized_value() const  { return float(value)/(max_val+1); }
	void	set_power_index(float pwi)	{ power_index = pwi; }
	float	get_power_index() const		{ return power_index; }
};


class button_state
{
public:
	bool	value, last_value;
	bool	bToggle;
	int		button_no;
	int		key_code;
public:
	void	set(int _button_no, int _key_code)  { button_no=_button_no; key_code=_key_code; }
	void	key_in();
	void	joystick_in(DIJOYSTATE2& djs);
};


class joystick_state
{
public:
	axis_state		axis[4];
	button_state	button[16];
public:
	void update(joystick& jstk);
};


#endif
