#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>

namespace FNet
{
	class SocketException { };

	class FNet
	{
	public:
		FNet()
		{
			WSADATA wsaData;
			if ( 0 != ::WSAStartup(MAKEWORD(2,2), &wsaData) )
				throw SocketException();
		}

		~FNet()
		{
			::WSACleanup();
		}
	};

	class DataSocket
	{
	private:
		SOCKET m_socket;

	public:
		DataSocket()
			: m_socket(INVALID_SOCKET)
		{
		}

		DataSocket(SOCKET s)
			: m_socket(s)
		{
		}

		~DataSocket()
		{
			if ( m_socket != INVALID_SOCKET )
				::closesocket(m_socket);
		}

		void setSocket(SOCKET s) { m_socket = s; }

		void connect(const char *ipaddr, int port)
		{
			sockaddr_in sa;
			sa.sin_family = AF_INET;
			sa.sin_addr.s_addr = ::inet_addr(ipaddr);
			sa.sin_port = ::htons(port);

			if ( m_socket == INVALID_SOCKET )
			{
				m_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
				if ( m_socket == INVALID_SOCKET ) throw SocketException();
			}

			if ( 0 != ::connect(m_socket, (SOCKADDR*)&sa, sizeof(sa)) ) throw SocketException();
		}
	};

	class ListenSocket
	{
	private:
		SOCKET m_socket;

	public:
		ListenSocket()
		{
			m_socket = INVALID_SOCKET;
		}

		~ListenSocket()
		{
			if ( m_socket != INVALID_SOCKET )
				::closesocket(m_socket);
		}

		void listen(const char *ipaddr, int port)
		{
			m_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if ( m_socket == INVALID_SOCKET ) throw SocketException();

			sockaddr_in sa;
			::ZeroMemory(&sa, sizeof(sa));
			sa.sin_family      = AF_INET;
			sa.sin_addr.s_addr = ::inet_addr(ipaddr);
			sa.sin_port        = ::htons((unsigned short)port);

			if ( 0 != ::bind(m_socket, (SOCKADDR*)&sa, sizeof(sa)) ) throw SocketException();
			if ( 0 != ::listen(m_socket, SOMAXCONN) ) throw SocketException();
		}

		bool hasAccept()
		{
			fd_set set;
			FD_ZERO(&set);
			FD_SET(m_socket, &set);

			timeval tv;
			::ZeroMemory(&tv, sizeof(tv));

			int res = ::select(0, &set, NULL, NULL, &tv); 
			if ( res == SOCKET_ERROR ) throw SocketException();

			if ( FD_ISSET(m_socket, &set) ) return true;
			else return false;
		}

		SOCKET accept()
		{
			sockaddr_in sa;
			int slen = sizeof(sa);
			SOCKET s = ::accept(m_socket, (SOCKADDR *)&sa, &slen);
			if ( s == INVALID_SOCKET ) throw SocketException();
			return s;
		}
	};
}