/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TrafficLights.h
 * Author: John Franklin
 *
 * Created on 24 August 2016, 19:09
 */

#ifndef TRAFFICLIGHTS_H
#define TRAFFICLIGHTS_H

#include <string>
#include <thread>
#include <mutex>

class TrafficLights
{
public:
	enum State
	{
		OFF,
		RED,
		RED_AMBER,
		GREEN,
		AMBER,
	};
	
public:
	TrafficLights();
	TrafficLights(const TrafficLights& orig);
	virtual ~TrafficLights();
	
	int initGPIO();
	
	void setState(State state);
	void setState(const std::string& state);
	
	State getState();
	std::string getStateString();
	
	void run();
	void runAsync();
	
	static void runThreadFunc(TrafficLights* self);
	
private:
	int m_amberToGreenDelay = 1000;
	int m_amberToRedDelay = 1000;
	
	int m_redTime = 10000;
	int m_greenTime = 10000;
	
	int m_timeUntilStateChange = 0;
	
	State m_currentState = OFF;
	
	std::thread m_thread;
	std::mutex m_mutex;
	
	
	static const char* m_stateStrings[];
	
public:
	void setRedTime(int time) {
		m_redTime = time;
	}
	void setGreenTime(int time) {
		m_greenTime = time;
	}
	void setAmberToGreenDelay(int time) {
		m_amberToGreenDelay = time;
	}
	void setAmberToRedDelay(int time) {
		m_amberToRedDelay = time;
	}
	
	int getRedTime() {return m_redTime;}
	int getGreenTime() {return m_greenTime;}
	int getAmberToGreenDelay() {return m_amberToGreenDelay;}
	int getAmberToRedDelay() {return m_amberToRedDelay;}
};

#endif /* TRAFFICLIGHTS_H */

