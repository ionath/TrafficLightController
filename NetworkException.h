/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   NetworkException.h
 * Author: osmc
 *
 * Created on 20 September 2016, 10:34
 */

#ifndef NETWORKEXCEPTION_H
#define NETWORKEXCEPTION_H

#include <string>

class NetworkException {
public:
	NetworkException();
	NetworkException(const std::string& message);
	NetworkException(const NetworkException& orig);
	virtual ~NetworkException();
private:
	std::string m_message;
};

#endif /* NETWORKEXCEPTION_H */

