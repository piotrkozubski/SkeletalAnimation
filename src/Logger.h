/*
 * Logger.h
 *
 *  Created on: 11.10.2014
 *      Author: zokp
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <sstream>
#include <cstdlib>

#define LOG(level, sstream) \
{ \
	std::stringstream ss; \
	ss << sstream; \
	Logger::log(level, __FILE__, __LINE__, __FUNCTION__, ss); \
}
#define LOG_INF(sstream) LOG(Logger::LOG_INFO, sstream);
#define LOG_DET(sstream) LOG(Logger::LOG_DETAIL, sstream);
#define LOG_DEB(sstream) //LOG(Logger::LOG_DEBUG, sstream);
#define LOG_WAR(sstream) LOG(Logger::LOG_WARNING, sstream);
#define LOG_ERR(sstream) LOG(Logger::LOG_ERROR, sstream);

#define ASSERT(cond) LOG_ASSERT(cond, "")
#define LOG_ASSERT(cond, sstream) \
{ \
	if (!(cond)) \
	{ \
        LOG_ERR(sstream); \
        exit(0); \
	} \
}

/**
 * Logger namespace
 */
namespace Logger
{
	/**
	 * Defines the levels of the log
	 */
	enum Level
	{
	    LOG_ERROR = 0,
	    LOG_WARNING,
	    LOG_INFO,
	    LOG_DETAIL,
	    LOG_DEBUG
	};

	/**
	 * Puts the log information into the logger
	 */
	void log(const Level level, const char* file, const int line,
			const char* function, const std::stringstream& sstream);
}


#endif /* LOGGER_H_ */
