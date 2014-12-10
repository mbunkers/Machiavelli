//=============================================================================
// socket.cpp
// (c) 8-12-2010, Frans Spijkerman, Avans Hogeschool
// enhanced for cross-platform use by Bob Polis (c) 2013, 2014
//
// Implementation of classes Socket, ServerSocket and CientSocket
//=============================================================================
#include "stdafx.h"

#include "Socket.h"
#include "throw.h"
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <stdexcept>
#include <memory>

#if defined(__APPLE__) || defined(__linux__)

	#include <sys/socket.h>
	#include <sys/ioctl.h>
	#include <net/route.h>
	#include <net/if.h>
	#include <arpa/inet.h>
	#include <netinet/in.h>
	#include <sys/types.h>
	#include <netdb.h>
	#include <ifaddrs.h>
	#include <unistd.h>

#else // Windows

	#include <ws2tcpip.h>

	// Tell linker to use these libraries
	#pragma comment (lib, "Ws2_32.lib")
	#pragma comment (lib, "Mswsock.lib")
	#pragma comment (lib, "AdvApi32.lib")

	//=============================================================================
	class WSA
	//=============================================================================
	// ONLY NEEDED IN MS-WINDOWS
	//
	// An instance of this class is created globally.
	// So constructor is automatically called before main()
	// and destructor is automatically called after main().
	// Result: Windows Socket API is active when needed.
	//=============================================================================
	{
	private:
		WSADATA data;
	public:
		WSA()
		{
			int iResult = WSAStartup(MAKEWORD(2, 2), &data);
			if (iResult != 0) {
				cerr << "WSAStartup failed with error: " << iResult << '\n';
			}
		}
		~WSA()
		{
			WSACleanup();
		}
	} wsa; // instance

#endif // Windows

#if defined(__APPLE__)
#pragma mark Socket
#endif

Socket::Socket(SOCKET sock, const struct sockaddr& address)
: sock {sock}
{
    switch (address.sa_family) {
        case AF_INET: {
            struct sockaddr_in* in_addr = reinterpret_cast<struct sockaddr_in*>(&addr);
            const struct sockaddr_in* ipv4 = reinterpret_cast<const struct sockaddr_in*>(&address);
            *in_addr = *ipv4;
            break;
        }
        case AF_INET6: {
            struct sockaddr_in6* in6_addr = reinterpret_cast<struct sockaddr_in6*>(&addr);
            const struct sockaddr_in6* ipv6 = reinterpret_cast<const struct sockaddr_in6*>(&address);
            *in6_addr = *ipv6;
            break;
        }
        default: {
            throw runtime_error("invalid type of socket address");
        }
    }
}

ssize_t Socket::read(char *buf, ssize_t maxlen)
{
	ssize_t len = 0;
	// might come in parts
	while (ssize_t n = ::recv(sock, buf + len, int(maxlen - len), 0)) {
		throw_if_min1((int)n);
		len += n;
		if (len >= maxlen) break;
	}
	return len;
}

string Socket::readline()
{
	// read a line: ignore '\r', stop at '\n'
	string line;
	char c;
	while (ssize_t n = ::recv(sock, &c, 1, 0)) {
		throw_if_min1((int)n);
		if (c == '\n') break;
		if (c != '\r') line += c;
	}
	return line;
}

void Socket::write(const string& msg)
{
	write(msg.c_str(), msg.length());
}

void Socket::write(const char *buf, size_t len)
{
	throw_if_min1((int)::send(sock, buf, (int)len, 0));
}

Socket::~Socket()
{
	if (sock > 0) close();
}

void Socket::close()
{
	cerr << "will close socket " << sock << endl;
#if defined(__APPLE__) || defined(__linux__)
	throw_if_min1(::close(sock));
#else
	::closesocket(sock);
#endif
	sock = 0;
}

string Socket::get_dotted_ip() const
{
    const char* result {nullptr};
    char textbuf[INET6_ADDRSTRLEN]; // large enough for both IPv4 and IPv6 addresses
    const struct sockaddr* addr_p = reinterpret_cast<const struct sockaddr*>(&addr);
    switch (addr_p->sa_family) {
        case AF_INET: {
            const struct sockaddr_in* in_p = reinterpret_cast<const struct sockaddr_in*>(addr_p);
            throw_if_null(result = ::inet_ntop(AF_INET, (void*)&in_p->sin_addr, textbuf, INET6_ADDRSTRLEN));
            break;
        }
        case AF_INET6: {
            const struct sockaddr_in6* in6_p = reinterpret_cast<const struct sockaddr_in6*>(addr_p);
            throw_if_null(result = ::inet_ntop(AF_INET6, (void*)&in6_p->sin6_addr, textbuf, INET6_ADDRSTRLEN));
            break;
        }            
        default:
            return "n/a";
    }
    return result;
}

#if defined(__APPLE__)
#pragma mark ServerSocket
#endif

ServerSocket::ServerSocket(int port)
{
	throw_if_min1(sock = ::socket(AF_INET, SOCK_STREAM, 0));

	struct sockaddr_in saServer;
	saServer.sin_family = AF_INET;
	saServer.sin_addr.s_addr = INADDR_ANY; 
	saServer.sin_port = htons(port);

	throw_if_min1(::bind(sock, (struct sockaddr*)&saServer, sizeof(struct sockaddr)));
	throw_if_min1(::listen(sock, 100));  // the number of clients that can be queued
}

Socket *ServerSocket::accept()
{
	struct sockaddr client_addr;
	client_addr.sa_family = AF_INET;
	socklen_t len = sizeof(client_addr);
	int fd;
	throw_if_min1(fd = ::accept(sock, &client_addr, &len));
	Socket* client = new Socket(fd, client_addr);
    cerr << "Connection accepted from " << client->get_dotted_ip() << ", with socket " << fd << endl;
	return client;
}

#if defined(__APPLE__)
#pragma mark ClientSocket
#endif

ClientSocket::ClientSocket(const char *host, int port)
{
    // construct network address for server
    struct addrinfo hint;
    memset(&hint, 0, sizeof(hint));
    hint.ai_family = AF_INET;
    hint.ai_socktype = SOCK_STREAM;
    struct addrinfo* infolist {nullptr};
    int gai_error = ::getaddrinfo(host, to_string(port).c_str(), &hint, &infolist);
    if (gai_error) {
        ostringstream oss;
        oss << "getaddrinfo error " << gai_error << ": " << gai_strerror(gai_error) << " (" << __FILE__ << ":" << __LINE__ << ")";
        throw runtime_error(oss.str());
    }
    // wrap our list pointer inside unique_ptr for auto cleanup
#if defined(__APPLE__) || defined(__linux__)
    using cleanup_func = void(*)(struct addrinfo*);
    unique_ptr<struct addrinfo, cleanup_func> list {infolist, ::freeaddrinfo};
#else // Windows
    using cleanup_func = void(__stdcall*)(PADDRINFOA);
    unique_ptr<struct addrinfo, cleanup_func> list(infolist, ::freeaddrinfo);
#endif
    // create socket
    throw_if_min1(sock = ::socket(list->ai_family, list->ai_socktype, list->ai_protocol));
    
    // connect to server
    throw_if_min1(::connect(sock, (struct sockaddr*)list->ai_addr, list->ai_addrlen));
}
