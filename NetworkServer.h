/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   NetworkServer.h
 * Author: John Franklin
 *
 * Created on 24 August 2016, 19:23
 */

#ifndef NETWORKSERVER_H
#define NETWORKSERVER_H

#include <string>

#include "NetworkConnection.h"

class NetworkServer
{
public:
	NetworkServer();
	NetworkServer(const NetworkServer& orig);
	virtual ~NetworkServer();
	
	void SetupSocket(int portNo);
	NetworkConnection ListenForConnection();
	
	void CloseSocket();
	
private:
	// File descriptors for server socket
	int m_listenSocketFD;
	
private:
	void error(const char *msg);
};

#endif /* NETWORKSERVER_H */

