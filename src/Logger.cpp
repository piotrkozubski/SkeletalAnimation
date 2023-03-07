/*
 * Logger.cpp
 *
 *  Created on: 11.10.2014
 *      Author: zokp
 */
#include "Logger.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <ctime>
#include <cstdio>
#include <sys/timeb.h>
#include <sys/time.h>
#include <time.h>

static const int WIDTH_LEVEL = 5;
static const int WIDTH_FILE  = 24;
static const int WIDTH_LINE  = 6;
static const int WIDTH_FUNC  = 20;
static const int WIDTH_STR 	 = 300;

static const char* LevelToStr(const Logger::Level level);


void Logger::log(const Level level, const char* file, const int line,
		const char* function, const std::stringstream& sstream)
{
	timeval tv;
	time_t currTime;

	gettimeofday(&tv, NULL);
	currTime = tv.tv_sec;

	tm* now = localtime(&currTime);
	std::ostringstream oss;
	oss << line;
	std::string levelString(LevelToStr(level));
	std::string fileString(file);
	std::string funcString(function);
	std::cout
	    << std::setfill('0') << std::right
	    << std::setw(2) << now->tm_hour << ":"
	    << std::setw(2) << now->tm_min << ":"
	    << std::setw(2) << now->tm_sec << ":"
		<< std::setw(3) << tv.tv_usec << " | "
	    << std::setfill(' ') << std::left
	    << std::setw(WIDTH_LEVEL) << levelString.substr(0, WIDTH_LEVEL) << " | "
	    << std::setw(WIDTH_FILE) << fileString.substr(0, WIDTH_FILE) << " | "
		<< std::setw(WIDTH_LINE) << oss.str().substr(0, WIDTH_LINE) << " | "
//		<< std::setw(WIDTH_FUNC) << funcString.substr(0, WIDTH_FUNC) << " | "
		<< std::setw(WIDTH_STR) << sstream.str().substr(0, WIDTH_STR) << std::endl;
}

static const char* LevelToStr(const Logger::Level level)
{
	const char* result = "";
	switch (level)
	{
		case Logger::LOG_INFO:
			result = "INFO";
			break;
		case Logger::LOG_DETAIL:
			result = "DETAIL";
			break;
		case Logger::LOG_DEBUG:
			result = "DEBUG";
			break;
		case Logger::LOG_WARNING:
			result = "WARNING";
			break;
		case Logger::LOG_ERROR:
			result = "ERROR";
			break;
		default:
			result = "UNKNOWN";
			break;
	}

	return result;
}
