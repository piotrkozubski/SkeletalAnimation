/*
 * OGLProgramUniformsSuite.cpp
 *
 *  Created on: 15.07.2017
 *      Author: Piotr Kozubski
 */
#include <cstring>
#include "OGLProgramUniformsSuite.h"
#include "Logger.h"

bool OGLProgramUniformsSuite::set(UniformName pUnfName, const void* data)
{

	for (uint32_t idx = 0; idx < mArraySize; ++idx)
	{
		if (mArray[idx].enumName == pUnfName)
		{
			mArray[idx].data = const_cast<void*>(data);
			return true;
		}
	}

	LOG_ERR("Unable to set " << pUnfName << " uniform");
	return false;
}

void validateOGLUniformSuite(const OGLProgramUniformsSuite& suite,
							 const char* suiteName)
{
#define ERRSTR "validating " << suiteName << ": "

	LOG_INF("validating " << suiteName << " uniform suite");
	for (uint32_t unfIdx = 0; unfIdx < suite.mArraySize; ++unfIdx)
	{
		const OGLProgramUniform& uniform = suite.mArray[unfIdx];
		LOG_ASSERT(uniform.data, ERRSTR << "incorrect data");
		LOG_ASSERT(uniform.enumName > UNFNAME_NONE,
				ERRSTR << "incorrect enumName");
		LOG_ASSERT(uniform.location > -1, ERRSTR << "incorrect location");
		LOG_ASSERT(uniform.name, ERRSTR << "incorrect name");
		LOG_ASSERT(strlen(uniform.name), ERRSTR << "incorrect name, 0 length");
		LOG_ASSERT(uniform.type > OGLProgramUniform::UNIFORMTYPE_NONE,
				ERRSTR << "incorrect type");
		LOG_ASSERT(uniform.count > 0, ERRSTR << "incorrect number");
	}
#undef ERRSTR
}
