#include <iostream>
#include <cstring>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#endif

void printAddrinfo(addrinfo* addresInfo)
{
	std::cout << "\tFlags: " << addresInfo->ai_flags;

	std::cout << ", Family: ";
	switch (addresInfo->ai_family)
	{
	case AF_UNSPEC:
		std::cout << "Unspecified";
		break;
	case AF_INET:
		std::cout << "AF_INET (IPv4)";
		break;
	case AF_INET6:
		std::cout << "AF_INET6 (IPv6)";
		break;
	default:
		std::cout << "Other" << addresInfo->ai_family;
		break;
	}

	std::cout << ", Socket type: ";
	switch (addresInfo->ai_socktype)
	{
	case 0:
		std::cout << "Unspecified";
		break;
	case SOCK_STREAM:
		std::cout << "SOCK_STREAM (stream)";
		break;
	case SOCK_DGRAM:
		std::cout << "SOCK_DGRAM (datagram)";
		break;
	case SOCK_RAW:
		std::cout << "SOCK_RAW (raw)";
		break;
	case SOCK_RDM:
		std::cout << "SOCK_RDM (reliable message datagram)";
		break;
	case SOCK_SEQPACKET:
		std::cout << "SOCK_SEQPACKET (pseudo-stream packet)";
		break;
	default:
		std::cout << "Other " << addresInfo->ai_socktype;
		break;
	}

	std::cout << ", Protocol: ";
	switch (addresInfo->ai_protocol)
	{
	case 0:
		std::cout << "Unspecified";
		break;
	case IPPROTO_TCP:
		std::cout << "IPPROTO_TCP (TCP)";
		break;
	case IPPROTO_UDP:
		std::cout << "IPPROTO_UDP (UDP)";
		break;
	default:
		std::cout << "Other" << addresInfo->ai_protocol;
		break;
	}
	std::cout << "\n\tLength of this sockaddr: " << addresInfo->ai_addrlen;
	if (addresInfo->ai_canonname != nullptr)
		std::cout << "\n\tCanonical name: " << addresInfo->ai_canonname;

	void* addr = nullptr;
	switch (addresInfo->ai_family)
	{
	case AF_INET:
	{
		struct sockaddr_in* ipv4 = (struct sockaddr_in*)addresInfo->ai_addr;
		addr = &(ipv4->sin_addr);
		break;
	}
	case AF_INET6:
	{
		struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)addresInfo->ai_addr;
		addr = &(ipv6->sin6_addr);
		break;
	}
	}
	char ipstr[INET6_ADDRSTRLEN];
	inet_ntop(addresInfo->ai_family, addr, ipstr, sizeof ipstr);
	std::cout << "\n\tAddress: " << ipstr << "\n";
}

void printAddrinfo(char* host, char* service, int ai_family, int ai_socktype, int ai_flags)
{
	struct addrinfo hints, * res;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = ai_family;
	hints.ai_socktype = ai_socktype;
	hints.ai_flags = ai_flags;

	int status;
	if ((status = getaddrinfo(host, service, &hints, &res)) != 0)
	{
		std::cout << "getaddrinfo error: " << gai_strerror(status) << "\n";
		return;
	}

	std::cout << "\nIP addresses for host: " << ((host != nullptr) ? host : "null") << " service: " << ((service != nullptr) ? service : "null") << "\n";

	for (struct addrinfo* addresInfo = res; addresInfo != NULL; addresInfo = addresInfo->ai_next)
		printAddrinfo(addresInfo);

	freeaddrinfo(res);
}

int main(int argc, char* argv[])
{
#ifdef _WIN32
	// Initialize Winsock
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		std::cout << "WSAStartup failed: " << iResult << "\n";
		return 1;
	}

#endif
	std::cout << "########################################\n";
	std::cout << "adress: google.com";
	printAddrinfo((char*)"google.com", (char*)"8000", AF_UNSPEC, SOCK_STREAM, AI_PASSIVE);
	printAddrinfo((char*)"google.com", (char*)"8000", AF_INET, SOCK_STREAM, 0);
	printAddrinfo((char*)"google.com", (char*)"8000", AF_INET6, SOCK_STREAM, 0);

	std::cout << "########################################\n";
	std::cout << "adress: empty string";
	printAddrinfo((char *)"", (char*)"8000", AF_UNSPEC, SOCK_STREAM, AI_PASSIVE);
	printAddrinfo((char*)"", (char*)"8000", AF_INET, SOCK_STREAM, 0);
	printAddrinfo((char*)"", (char*)"8000", AF_INET6, SOCK_STREAM, 0);

	std::cout << "########################################\n";
	std::cout << "adress: NULL";
	printAddrinfo(NULL, (char*)"8000", AF_UNSPEC, SOCK_STREAM, 0 );
	printAddrinfo(NULL, (char*)"8000", AF_INET, SOCK_STREAM, 0);
	printAddrinfo(NULL, (char*)"8000", AF_INET6, SOCK_STREAM, 0);

	std::cout << "########################################\n";
	std::cout << "adress: NULL PASSIVE";
	printAddrinfo(NULL, (char*)"8000", AF_UNSPEC, SOCK_STREAM, AI_PASSIVE);
	printAddrinfo(NULL, (char*)"8000", AF_INET, SOCK_STREAM, AI_PASSIVE);
	printAddrinfo(NULL, (char*)"8000", AF_INET6, SOCK_STREAM, AI_PASSIVE);

#ifdef _WIN32
	WSACleanup();
#endif

	return 0;
}



//#undef UNICODE
//
//#include <winsock2.h>
//#include <ws2tcpip.h>
//#include <stdio.h>
//
//// link with Ws2_32.lib
//#pragma comment(lib, "Ws2_32.lib")
//
//int __cdecl main(int argc, char** argv)
//{
//
//    //-----------------------------------------
//    // Declare and initialize variables
//    WSADATA wsaData;
//    int iResult;
//    INT iRetval;
//
//    DWORD dwRetval;
//
//    int i = 1;
//
//    struct addrinfo* result = NULL;
//    struct addrinfo* ptr = NULL;
//    struct addrinfo hints;
//
//    struct sockaddr_in* sockaddr_ipv4;
//    //    struct sockaddr_in6 *sockaddr_ipv6;
//    LPSOCKADDR sockaddr_ip;
//
//    char ipstringbuffer[46];
//    DWORD ipbufferlength = 46;
//
//    // Validate the parameters
//    if (argc != 3) {
//        printf("usage: %s <hostname> <servicename>\n", argv[0]);
//        printf("       provides protocol-independent translation\n");
//        printf("       from an ANSI host name to an IP address\n");
//        printf("%s example usage\n", argv[0]);
//        printf("   %s www.contoso.com 0\n", argv[0]);
//        return 1;
//    }
//
//    // Initialize Winsock
//    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
//    if (iResult != 0) {
//        printf("WSAStartup failed: %d\n", iResult);
//        return 1;
//    }
//
//    //--------------------------------
//    // Setup the hints address info structure
//    // which is passed to the getaddrinfo() function
//    ZeroMemory(&hints, sizeof(hints));
//    hints.ai_family = AF_UNSPEC;
//    hints.ai_socktype = SOCK_STREAM;
//    hints.ai_protocol = IPPROTO_TCP;
//
//    printf("Calling getaddrinfo with following parameters:\n");
//    printf("\tnodename = %s\n", argv[1]);
//    printf("\tservname (or port) = %s\n\n", argv[2]);
//
//    //--------------------------------
//    // Call getaddrinfo(). If the call succeeds,
//    // the result variable will hold a linked list
//    // of addrinfo structures containing response
//    // information
//    dwRetval = getaddrinfo(argv[1], argv[2], &hints, &result);
//    if (dwRetval != 0) {
//        printf("getaddrinfo failed with error: %d\n", dwRetval);
//        WSACleanup();
//        return 1;
//    }
//
//    printf("getaddrinfo returned success\n");
//
//    // Retrieve each address and print out the hex bytes
//    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
//
//        printf("getaddrinfo response %d\n", i++);
//        printf("\tFlags: 0x%x\n", ptr->ai_flags);
//        printf("\tFamily: ");
//        switch (ptr->ai_family) {
//        case AF_UNSPEC:
//            printf("Unspecified\n");
//            break;
//        case AF_INET:
//            printf("AF_INET (IPv4)\n");
//            sockaddr_ipv4 = (struct sockaddr_in*)ptr->ai_addr;
//            printf("\tIPv4 address %s\n",
//                inet_ntop(sockaddr_ipv4->sin_addr));
//            break;
//        case AF_INET6:
//            printf("AF_INET6 (IPv6)\n");
//            // the InetNtop function is available on Windows Vista and later
//            // sockaddr_ipv6 = (struct sockaddr_in6 *) ptr->ai_addr;
//            // printf("\tIPv6 address %s\n",
//            //    InetNtop(AF_INET6, &sockaddr_ipv6->sin6_addr, ipstringbuffer, 46) );
//
//            // We use WSAAddressToString since it is supported on Windows XP and later
//            sockaddr_ip = (LPSOCKADDR)ptr->ai_addr;
//            // The buffer length is changed by each call to WSAAddresstoString
//            // So we need to set it for each iteration through the loop for safety
//            ipbufferlength = 46;
//            iRetval = WSAAddressToString(sockaddr_ip, (DWORD)ptr->ai_addrlen, NULL,
//                ipstringbuffer, &ipbufferlength);
//            if (iRetval)
//                printf("WSAAddressToString failed with %u\n", WSAGetLastError());
//            else
//                printf("\tIPv6 address %s\n", ipstringbuffer);
//            break;
//        case AF_NETBIOS:
//            printf("AF_NETBIOS (NetBIOS)\n");
//            break;
//        default:
//            printf("Other %ld\n", ptr->ai_family);
//            break;
//        }
//        printf("\tSocket type: ");
//        switch (ptr->ai_socktype) {
//        case 0:
//            printf("Unspecified\n");
//            break;
//        case SOCK_STREAM:
//            printf("SOCK_STREAM (stream)\n");
//            break;
//        case SOCK_DGRAM:
//            printf("SOCK_DGRAM (datagram) \n");
//            break;
//        case SOCK_RAW:
//            printf("SOCK_RAW (raw) \n");
//            break;
//        case SOCK_RDM:
//            printf("SOCK_RDM (reliable message datagram)\n");
//            break;
//        case SOCK_SEQPACKET:
//            printf("SOCK_SEQPACKET (pseudo-stream packet)\n");
//            break;
//        default:
//            printf("Other %ld\n", ptr->ai_socktype);
//            break;
//        }
//        printf("\tProtocol: ");
//        switch (ptr->ai_protocol) {
//        case 0:
//            printf("Unspecified\n");
//            break;
//        case IPPROTO_TCP:
//            printf("IPPROTO_TCP (TCP)\n");
//            break;
//        case IPPROTO_UDP:
//            printf("IPPROTO_UDP (UDP) \n");
//            break;
//        default:
//            printf("Other %ld\n", ptr->ai_protocol);
//            break;
//        }
//        printf("\tLength of this sockaddr: %d\n", ptr->ai_addrlen);
//        printf("\tCanonical name: %s\n", ptr->ai_canonname);
//    }
//
//    freeaddrinfo(result);
//    WSACleanup();
//
//    return 0;
//}