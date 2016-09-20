/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   NetworkConnection.h
 * Author: John Franklin
 *
 * Created on 20 September 2016, 09:57
 */

#ifndef NETWORKCONNECTION_H
#define NETWORKCONNECTION_H

#include <string>

class NetworkConnection 
{
public:
	class ConnectionClosed
	{};
public:
	NetworkConnection();
	NetworkConnection(int socketFD);
	NetworkConnection(NetworkConnection&& orig);
	NetworkConnection& operator=(NetworkConnection&& orig);
	virtual ~NetworkConnection();
	// Forbid copying. socket FD should only be stored in one place
private:
	NetworkConnection(const NetworkConnection& orig);
	NetworkConnection& operator=(const NetworkConnection& orig);
	
public:
	
	void takeSocket(int socket);
	
	bool isValidSocket();
	
	std::string readFromConnection();
	void writeToConnection(std::string data);
	
	void closeConnection();
	
private:
	const int INVALID_SOCKET = -1;
	int m_clientSocketFD = INVALID_SOCKET;
	
private:
	void error(const char *msg);
	void connectionClosedByClient();
};

#endif /* NETWORKCONNECTION_H */

