/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   NetworkConnection.cpp
 * Author: John Franklin
 * 
 * Created on 20 September 2016, 09:57
 */

#include "NetworkConnection.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <cassert>

#include "NetworkException.h"

NetworkConnection::NetworkConnection()
{
}

NetworkConnection::NetworkConnection(int socketFD)
{
	m_clientSocketFD = socketFD;
}

NetworkConnection::NetworkConnection(NetworkConnection&& orig)
{
	*this = orig;
}

NetworkConnection& NetworkConnection::operator=(NetworkConnection&& orig)
{
	m_clientSocketFD = orig.m_clientSocketFD;
}

NetworkConnection::~NetworkConnection()
{
	closeConnection();
}

NetworkConnection::NetworkConnection(const NetworkConnection& orig) 
{
	assert(0);
}

NetworkConnection& NetworkConnection::operator=(const NetworkConnection& orig) 
{
	assert(0);
}

void NetworkConnection::takeSocket(int socket)
{
	if (isValidSocket())
	{
		closeConnection();
	}
	
	if (socket >= 0)
	{
		m_clientSocketFD = socket;
	}
}

bool NetworkConnection::isValidSocket()
{
	return m_clientSocketFD != INVALID_SOCKET;
}

std::string NetworkConnection::readFromConnection()
{
	const size_t BUFFER_SIZE = 256;
	char buffer[BUFFER_SIZE];
	bzero(buffer,BUFFER_SIZE);
	int n = read(m_clientSocketFD, buffer, BUFFER_SIZE-1);
	if (n == 0)
	{
		// Connection closed
		connectionClosedByClient();
	}
	if (n < 0)
	{
		error("ERROR reading from socket");
	}
	
	return std::string(buffer);
}

void NetworkConnection::writeToConnection(std::string data)
{
	int n = write(m_clientSocketFD, data.c_str(), data.length());
	if (n == 0)
	{
		// Connection closed
		connectionClosedByClient();
	}
	if (n < 0)
	{
		error("ERROR writing to socket");
	}
}

void NetworkConnection::closeConnection()
{
	if (isValidSocket())
	{
		close(m_clientSocketFD);
		m_clientSocketFD = INVALID_SOCKET;
	}
}

void NetworkConnection::error(const char *msg)
{
    perror(msg);
	throw NetworkException(msg);
}

void NetworkConnection::connectionClosedByClient()
{
	throw ConnectionClosed();
}