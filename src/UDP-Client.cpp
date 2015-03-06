//============================================================================
// Name        : UDP-Client.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
using namespace std;

#define PORT 1234

#define MAXDATASIZE 100

int main(int argc, char *argv[]) {
	int sockfd, num;
	struct hostent *he, peer;
	struct sockaddr_in server;
	socklen_t addrlen;
	char buf[MAXDATASIZE];
	if (argc != 3) {
		printf("ip%s\n", argv[1]);
		return 1;
	}

	if ((he = gethostbyname(argv[1])) == NULL) {
		printf("gethostbyname() error");
		return 1;
	} else {
		printf("hostname:%s", he->h_name);
	}

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket() error.");
		return 1;
	}

	bzero(&server, sizeof(server));

	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr = *((struct in_addr *) he->h_addr);
	cout << endl << server.sin_addr.s_addr << endl;

	sendto(sockfd, argv[2], strlen(argv[2]), 0, (struct sockaddr *) &server,
			 sizeof(server));
	cout<< "Sent Hello!" <<endl;

	addrlen = sizeof(server);

	while (1) {
		if ((num = recvfrom(sockfd, buf, MAXDATASIZE, 0,
				(struct sockaddr *) &peer, &addrlen)) == -1) {
			perror("recvfrom() error");
			return 1;
		};

		if (addrlen != sizeof(server)
				|| memcmp((const void *) &server, (const void*) &peer,
						addrlen)) {
			printf("not from target server\n");
			continue;
		}
		buf[num] = '\0';
		printf("message:%s", buf);
		break;
	}

	close(sockfd);
	return 0;
}
