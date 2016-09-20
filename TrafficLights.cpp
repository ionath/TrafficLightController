/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TrafficLights.cpp
 * Author: John Franklin
 * 
 * Created on 24 August 2016, 19:09
 */

#include "TrafficLights.h"

#include <bcm2835.h>

#include <chrono>
#include <thread>

#define PIN_GREEN RPI_BPLUS_GPIO_J8_11
#define PIN_YELLOW RPI_BPLUS_GPIO_J8_13
#define PIN_RED RPI_BPLUS_GPIO_J8_15

const char* TrafficLights::m_stateStrings[] = {
	"off",
	"red",
	"red_amber",
	"green",
	"amber",
};

TrafficLights::TrafficLights() {
}

TrafficLights::TrafficLights(const TrafficLights& orig) {
}

TrafficLights::~TrafficLights() {
}

int TrafficLights::initGPIO()
{
	if (!bcm2835_init())
	{
		return 1;
	}

    // Set the pins to be an outputs
    bcm2835_gpio_fsel(PIN_GREEN, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(PIN_YELLOW, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(PIN_RED, BCM2835_GPIO_FSEL_OUTP);
	
	// Set all light to off
	bcm2835_gpio_write(PIN_GREEN, LOW);
	bcm2835_gpio_write(PIN_YELLOW, LOW);
	bcm2835_gpio_write(PIN_RED, LOW);
}

void TrafficLights::setState(State state)
{
	m_mutex.lock();
	m_currentState = state;
	switch (state)
	{
	case RED:
		bcm2835_gpio_write(PIN_RED, HIGH);
		bcm2835_gpio_write(PIN_YELLOW, LOW);
		bcm2835_gpio_write(PIN_GREEN, LOW);
		m_timeUntilStateChange = m_redTime;
		break;
	case RED_AMBER:
		bcm2835_gpio_write(PIN_RED, HIGH);
		bcm2835_gpio_write(PIN_YELLOW, HIGH);
		bcm2835_gpio_write(PIN_GREEN, LOW);
		m_timeUntilStateChange = m_amberToGreenDelay;
		break;
	case GREEN:
		bcm2835_gpio_write(PIN_RED, LOW);
		bcm2835_gpio_write(PIN_YELLOW, LOW);
		bcm2835_gpio_write(PIN_GREEN, HIGH);
		m_timeUntilStateChange = m_greenTime;
		break;
	case AMBER:
		bcm2835_gpio_write(PIN_RED, LOW);
		bcm2835_gpio_write(PIN_YELLOW, HIGH);
		bcm2835_gpio_write(PIN_GREEN, LOW);
		m_timeUntilStateChange = m_amberToRedDelay;
		break;
	case OFF:
		bcm2835_gpio_write(PIN_RED, LOW);
		bcm2835_gpio_write(PIN_YELLOW, LOW);
		bcm2835_gpio_write(PIN_GREEN, LOW);
		m_timeUntilStateChange = m_amberToRedDelay;
		break;
	}
	m_mutex.unlock();
}

void TrafficLights::setState(const std::string& stateStr)
{
	if (stateStr == "red") {
		setState(RED);
	}
	else if (stateStr == "red_amber") {
		setState(RED_AMBER);
	}
	else if (stateStr == "green") {
		setState(GREEN);
	}
	else if (stateStr == "amber") {
		setState(AMBER);
	}
	else if (stateStr == "off") {
		setState(OFF);
	}
}

TrafficLights::State TrafficLights::getState()
{
	return m_currentState;
}

std::string TrafficLights::getStateString()
{
	return m_stateStrings[m_currentState];
}

inline void mySleep(int milliseconds)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

void TrafficLights::run()
{
	while (1)
	{
		int updateTime = 100;
		if (m_timeUntilStateChange <= 0)
		{
			// Change to next state
			switch (m_currentState)
			{
			case RED:
				setState(RED_AMBER);
				break;
			case RED_AMBER:
				setState(GREEN);
				break;
			case GREEN:
				setState(AMBER);
				break;
			case AMBER:
				setState(RED);
				break;
			case OFF:
				m_timeUntilStateChange = updateTime;
				break;
			}
		}
		else
		{
			m_timeUntilStateChange -= updateTime;
			mySleep(100);
		}
	}
}

void TrafficLights::runAsync()
{
	m_thread = std::thread(runThreadFunc, this);
}

void TrafficLights::runThreadFunc(TrafficLights* self)
{
	self->run();
}