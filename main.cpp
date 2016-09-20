/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: John Franklin
 *
 * Created on 24 August 2016, 19:07
 */

#include <cstdlib>
#include <sstream>
#include <iostream>

#include "TrafficLights.h"
#include "NetworkServer.h"
#include "NetworkException.h"

/*
 * 
 */
int main(int argc, char** argv)
{
	std::cout << "Setting up traffic lights output\n";
	TrafficLights trafficLights;
	if (trafficLights.initGPIO() != 0)
		return 1;
	
	std::cout << "Initializing lights to Red\n";
	trafficLights.setState(TrafficLights::RED);
	
	std::cout << "Running lights\n";
	trafficLights.runAsync();
	
	std::cout << "Setting up server socket\n";
	NetworkServer netServer;
	netServer.SetupSocket(27015);
	
	while (1)
	{
		try {
			std::cout << "Waiting for connection\n";
			auto connection = netServer.ListenForConnection();

			while (1)
			{
				try
				{
					std::string commandLine = connection.readFromConnection();
					std::cout << "Command recieved: " << commandLine << "\n";
					std::istringstream strm(commandLine);

					std::string command;
					strm >> command;

					if (command == "disconnect")
					{
						std::cout << "disconnecting\n";
						connection.closeConnection();
						break;
					}
					else if (command == "set_state")
					{
						std::string state;
						strm >> state;
						std::cout << "Setting state to " << state << "\n";
						trafficLights.setState(state);
					}
					else if (command == "get_state")
					{
						auto state = trafficLights.getStateString();
						connection.writeToConnection(state);
					}
				}
				catch (NetworkConnection::ConnectionClosed e)
				{
					break;
				}
			}
		}
		catch (NetworkException e)
		{
			
		}
		catch (...)
		{
			std::cout << "Unknown exception\n";
		}
	}
	
	netServer.CloseSocket();
	
	return 0;
}

