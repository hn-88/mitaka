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
//---------------------------------
//  TCP_UDP.cpp
//   written by T.N.Kato  2002/6
//   2003/2/15
//---------------------------------
#include  "tcp_udp.h"



//==========
//  common
//==========
bool
init_winsock( void __cdecl close_func() )
{
	WSADATA wsaData;
	WORD version = MAKEWORD(2,0);

	if ( WSAStartup( version, &wsaData ) != 0)	return false;
	if ( version != wsaData.wVersion )			return false;
	if ( atexit( close_func ) != 0)				return false;
	
	return true;
}


struct hostent*
get_host_entry(const char* server_address_or_name)
{
	unsigned long  svraddr;
	struct hostent* hostentry;

	svraddr = inet_addr(server_address_or_name);
	if (svraddr != INADDR_NONE) {
		// �L����IP�A�h���X�̏ꍇ
		hostentry = gethostbyaddr((const char*)&svraddr, sizeof(svraddr), AF_INET);
	} else {
		// server_addres_or_name �𖼑O�Ƃ݂Ȃ�
		hostentry = gethostbyname(server_address_or_name);
	}

	return  hostentry;
}



bool
get_server_address(const char* server_address_or_name, int server_port, struct sockaddr_in* svr)
{
	struct hostent* hostentry;
	hostentry = get_host_entry(server_address_or_name);
	if (hostentry == NULL)	return false;

	memset((void*)svr, 0, sizeof(sockaddr_in));
	svr->sin_family = AF_INET;														// TCP/IP���g��
	svr->sin_addr.S_un.S_addr = *((unsigned long*)(hostentry->h_addr_list[0]));		// �T�[�o�̃A�h���X
	svr->sin_port = htons(server_port);												// �T�[�o�̃|�[�g

	return  true;
}


bool
bind_inaddr_any(SOCKET soc, int port)
{
	struct  sockaddr_in	 socadr;

	memset((void*)&socadr, 0, sizeof(socadr));
	socadr.sin_family = AF_INET;
	socadr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	socadr.sin_port = htons( port );
	if ( bind(soc, (struct sockaddr *)&socadr, sizeof(socadr)) == SOCKET_ERROR )  return false;
	return  true;
}




//=======
//  TCP
//=======
SOCKET
init_TCP_server(int port)
{
	SOCKET soc;

	//-- Create TCP socket --
	if ( (soc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET )  return INVALID_SOCKET;

	//-- Set socket option --
	int opt = 1;
	if ( setsockopt(soc, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) != 0 )  {
		closesocket(soc);
		return INVALID_SOCKET;
	}
    
	//-- Bind with socket --
	if ( !bind_inaddr_any(soc, port) )  {
		closesocket(soc);
		return INVALID_SOCKET;
	}

	return  soc;
}


SOCKET
init_TCP_client(const char* server_name, int server_port)
{
	SOCKET  soc;
	struct sockaddr_in  svradr;

	//-- Create TCP socket --
	if ( (soc = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET )  return INVALID_SOCKET;

	//-- Connect to server --
	if ( !get_server_address(server_name, server_port, &svradr) )  {
		closesocket(soc);
		return INVALID_SOCKET;
	}
	if ( connect(soc, (struct sockaddr *)&svradr, sizeof(svradr)) == SOCKET_ERROR )  {
		closesocket(soc);
		return INVALID_SOCKET;
	}

	return soc;
}


//--- �Œ蒷���b�Z�[�W��ǂݎ�� ---
int
readn_TCP(SOCKET soc, char* buf, const size_t& len)
{
	int count;
	count = len;
	while (count > 0) {
		int rcvn = recv(soc, buf, count, 0);
		
		if (rcvn == SOCKET_ERROR || rcvn == 0) {
			return  rcvn;
		}
		buf += rcvn;
		count -= rcvn;
	}
	return len;
}


int
send_TCP_packet(SOCKET soc, char* buf, int size)
{
	char FAR* p = buf;
	int msg_size = size;
	int sent_bytes = 0;
	int len;
	len = send(soc, (const char FAR*)&size, sizeof(int), 0);
	if (len == SOCKET_ERROR)  return len;
	sent_bytes += len;
	while (size > 0) {
	   len = send(soc, p, msg_size, 0);
	   if (len == SOCKET_ERROR)	 return len;
	   if (len < msg_size && len > 0) msg_size = len;

	   p += len;
	   size -= len;
	   sent_bytes += len;
	}
	return sent_bytes;
}


int
recv_TCP_packet(SOCKET soc, char* buf)
{
	int len;
	//-- �p�P�b�g�T�C�Y��ǂ�
	if ( readn_TCP(soc, (char*)&len, sizeof(len)) < 0) return -1;
	//-- �f�[�^�{�̂�ǂ�
	return readn_TCP(soc, buf, len);
}


//=======
//  UDP
//=======
SOCKET
init_UDP_server(int port)
{
	SOCKET soc;
	//-- Create UDP socket --
	if ( (soc = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)  return INVALID_SOCKET;
	//-- Bind with socket --
	if ( !bind_inaddr_any(soc, port) )  {
		closesocket(soc);
		return INVALID_SOCKET;
	}
	return  soc;
}

SOCKET
init_UDP_client()
{
	SOCKET soc;
	//-- Create UDP socket --
	if ( (soc = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)  return INVALID_SOCKET;
	return soc;
}
