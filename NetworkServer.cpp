/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   NetworkServer.cpp
 * Author: John Franklin
 * 
 * Created on 24 August 2016, 19:23
 */

#include "NetworkServer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include "NetworkException.h"

NetworkServer::NetworkServer() {
}

NetworkServer::NetworkServer(const NetworkServer& orig) {
}

NetworkServer::~NetworkServer() {
}

void NetworkServer::SetupSocket(int portNo)
{
	struct sockaddr_in serverAddr;
	
	m_listenSocketFD = socket(AF_INET, SOCK_STREAM, 0);
	if (m_listenSocketFD < 0)
	{
	   error("ERROR opening socket");
	}

	bzero((char *) &serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(portNo);
	if (bind(m_listenSocketFD, (struct sockaddr *) &serverAddr,
			 sizeof(serverAddr)) < 0) 
	{
		error("ERROR on binding");
	}
}

NetworkConnection NetworkServer::ListenForConnection()
{
	struct sockaddr_in clientAddr;
	
	listen(m_listenSocketFD, 5);
	socklen_t clientLen = sizeof(clientAddr);
	int clientSocketFD = accept(m_listenSocketFD, 
				(struct sockaddr *) &clientAddr, 
				&clientLen);
	if (clientSocketFD < 0)
	{
		error("ERROR on accept");
	}
	
	return NetworkConnection(clientSocketFD);
}

void NetworkServer::CloseSocket()
{
	close(m_listenSocketFD);
}

void NetworkServer::error(const char *msg)
{
    perror(msg);
	throw NetworkException(msg);
}
