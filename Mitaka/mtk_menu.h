/**************************************************************************************************

Copyright (c) 2007-2015   Tsunehiko Kato  and  4D2U Project, National Astronomical Observatory of Japan

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
#ifndef  __MTK_MENU_H__
#define  __MTK_MENU_H__
#include  <string.h>
#include  <typeinfo.h>
#include  <string>
#include  <vector>
#include  <assert.h>




//-------------------
//  ���j���[���ڂ̎��
//-------------------
enum menu_item_type {
	MENU_TY_BASE,
	MENU_TY_CHILD,
	MENU_TY_COMMAND,
	MENU_TY_CHECK,
	MENU_TY_FVALUE
};


//------------------------
//  ���j���[���ڂ̊�{�N���X
//------------------------
class menu;

class menu_item_base
{
	std::string name_key;

public:
	menu_item_base()										{}
	menu_item_base(const std::string& key)	{ set_name_key(key); }
	~menu_item_base()										{} 

	void	set_name_key(const std::string& key) {
		name_key = key;
	}

	virtual	menu_item_type	get_type() const = 0;
	
	const std::string& get_name_key() const	{ return name_key; }
};



//------------
//  �q���j���[
//------------
class menu_item_child  :  public menu_item_base
{
	menu* pChildMenu;
public:
	menu_item_child(const std::string& key, menu* pMenu) : menu_item_base(key), pChildMenu(pMenu)  {};

	menu_item_type	get_type() const					{ return MENU_TY_CHILD; }
	void			set_child_menu(menu* pMenu)			{ pChildMenu = pMenu; }
	menu*			get_child_menu()					{ return pChildMenu; }
};


//-----------
//  �R�}���h
//-----------
class menu_item_command  :  public menu_item_base
{
	int command_ID;
	int	idx;

public:
	menu_item_command(const std::string& key, int cmdID, int _idx) : menu_item_base(key), command_ID(cmdID), idx(_idx)  {};

	menu_item_type	get_type() const			{ return MENU_TY_COMMAND; }
	void			set_ID(int ID)				{ command_ID = ID; }
	int				get_ID() const				{ return command_ID; }
	void			set_idx(int _idx)			{ idx = _idx; }
	int				get_idx() const				{ return idx; }
};


//----------
//  �`�F�b�N
//----------
class menu_item_check  :  public menu_item_base
{
	bool	bCheck;
	int		check_ID;
	int		idx;

public:
	menu_item_check(const std::string& key, bool bChk, int ID, int _idx) : menu_item_base(key), bCheck(bChk), check_ID(ID), idx(_idx)  {};

	menu_item_type	get_type() const				{ return MENU_TY_CHECK; }
	void			set_check(bool bChk)			{ bCheck = bChk; }
	bool			get_check() const				{ return bCheck; }
	void			set_ID(int ID)					{ check_ID = ID; }
	int				get_ID() const					{ return check_ID; }
	void			set_idx(int _idx)				{ idx = _idx; }
	int				get_idx() const					{ return idx; }
};


//--------------
//  ���������_�l
//--------------
class menu_item_fvalue  :  public menu_item_base
{
	float	value;
public:
	menu_item_fvalue(const std::string& key, const float& val)  :  menu_item_base(key), value(val)  {};

	menu_item_type	get_type() const				{ return MENU_TY_FVALUE; }
	void			set_value(const float& val)		{ value = val; }
	float			get_value() const				{ return value; }
};


//--------------
//  class menu
//--------------
class menu  :  public  std::vector<menu_item_base*>
{
	menu* pParentMenu;
	std::string  title_key;
	
	int current_pos;
public:
	menu() : current_pos(0)  {};
	~menu()	{ free(); }
	void	free();

	//---
	void	add_child_menu(menu* pChildMenu, const std::string& key)	{
		this->push_back(new menu_item_child(key, pChildMenu) );
	}
	void	add_command(int command_id, int idx, const std::string& key) {
		this->push_back( new menu_item_command(key, command_id, idx) );
	}
	void	add_check(int check_id, const std::string& key, int idx, bool bInitCheck) {
		this->push_back( new menu_item_check(key, bInitCheck, check_id, idx) );
	}

	void	set_current_pos(int pos)	{ current_pos = pos; assert(pos>=0); }
	int		get_current_pos() const		{ return current_pos; }
	virtual void set_current_pos_auto()		{ set_current_pos(0); }

	void	set_title(const std::string& key)	{
		title_key = key;
	}

	const std::string& get_title_key() const	{ return  title_key; }
	void	set_check(int idx, bool bChk);
};


//---------------------
//  class menu_option
//---------------------
class menu_option  :  public menu
{
	int  check_pos;

public:
	menu_option() : check_pos(0)  {};
	menu_option(int idx) : check_pos(idx)  {};
	~menu_option() { menu::free(); }

	void set_current_pos_auto()	{
		assert(check_pos >= 0);
		set_current_pos(check_pos);
	}

	void	set_check(int idx)
		{
			assert(idx >= 0);
			check_pos = idx;
			int i=0;
			for (vector<menu_item_base*>::iterator it=this->begin(); it!=this->end(); ++it, ++i) {
				menu_item_check*  mi = dynamic_cast<menu_item_check*>(*it);
				if (mi != 0) {
					mi->set_check(i==idx);
				}
			}
		}

	void	set_check_by_index(int idx)
		{
			assert(idx >= 0);
			check_pos = 0;
			int i = 0;
			for (vector<menu_item_base*>::iterator it=this->begin(); it!=this->end(); ++it, ++i) {
				menu_item_check*  mi = dynamic_cast<menu_item_check*>(*it);
				if (mi != 0) {
					int  miidx = mi->get_idx();
					mi->set_check( miidx==idx );
					if (miidx == idx) {
						check_pos = i;
					}
				}
			}
		}


	int		get_check() const	{ return check_pos; }

	void	reset_check()	{
		for (vector<menu_item_base*>::iterator it=this->begin(); it!=this->end(); ++it) {
			menu_item_check*  mi = dynamic_cast<menu_item_check*>(*it);
			mi->set_check(false);
		}
	}
};


//------------------
//  menu selection
//------------------
class  menu_selection  :  public  std::vector<menu*>
{};



#endif
