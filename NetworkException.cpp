/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   NetworkException.cpp
 * Author: osmc
 * 
 * Created on 20 September 2016, 10:34
 */

#include "NetworkException.h"

NetworkException::NetworkException() {
}

NetworkException::NetworkException(const std::string& message)
{
	m_message = message;
}

NetworkException::NetworkException(const NetworkException& orig) {
}

NetworkException::~NetworkException() {
}

