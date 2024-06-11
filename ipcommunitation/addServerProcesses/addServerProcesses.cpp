#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>


#define PORT "????"
#define BACKLOG 10

void* get_in_addr(struct sockaddr* sa)
{
	if (sa->sa_family == AF_INET)
		return &(((struct sockaddr_in*)sa)->sin_addr);
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void client(int clientSocket)
{
	int a, b;
	if (recv(clientSocket, &a, sizeof(a), 0) == -1)
	{
		std::cout << "Error recv.\n";
		return;
	}

	std::cout << "Received a= " << a << ".\n";

	if (recv(clientSocket, &b, sizeof(b), 0) == -1)
	{
		std::cout << "Error recv.\n";
		return;
	}

	std::cout << "Received b= " << b << ".\n";

	int sum = a + b;

	std::cout << "Send sum = " << sum << ".\n";
	if (send(clientSocket, &sum, sizeof(sum), 0) == -1)
	{
		std::cout << "Error send.\n";
		return;
	}
}


int main(void)
{
	struct addrinfo hints;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	struct addrinfo* servinfo;
	int result;
	if ((result = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0)
	{
		std::cout << "retaddrinfo error: " << gai_strerror(result) << ".\n";
		return 1;
	}

	if (servinfo == nullptr)
	{
		std::cout << "No interface to listen.\n";
		return 1;
	}

	int listenSocket;
	if ((listenSocket = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1)
	{
		freeaddrinfo(servinfo);
		std::cout << "Error creatind socket.\n";
		return 1;
	}

	int yes = 1;
	if (setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
	{
		freeaddrinfo(servinfo);
		std::cout << "Error setsockopt.\n";
		return 1;
	}

	if (bind(listenSocket, servinfo->ai_addr, servinfo->ai_addrlen) == -1)
	{
		freeaddrinfo(servinfo);
		close(listenSocket);
		std::cout << "Error bind.";
		return 1;
	}

	freeaddrinfo(servinfo);

	if (listen(listenSocket, BACKLOG) == -1)
	{
		std::cout << "Error listen.\n";
		return 1;
	}

	std::cout << "Server waiting for connections.\n";

	while (true)
	{
		struct sockaddr_storage their_addr;
		socklen_t sin_size = sizeof(their_addr);
		int clientSocket;
		clientSocket = accept(listenSocket, (struct sockaddr*)&their_addr, &sin_size);
		if (clientSocket == -1)
			std::cout << "Error accept.";
		else
		{
			char s[INET6_ADDRSTRLEN];
			inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr*)&their_addr), s, sizeof s);
			std::cout << "Server got connection from " << s << ".\n";
			if (fork() == 0)
			{
				// this is the child process
				close(listenSocket);
				client(clientSocket);
				close(clientSocket);
				exit(0);
			}
			else
			{
				// parent
				close(clientSocket);
			}
		}
	}
	return 0;
}
