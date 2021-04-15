#include <cstring>
#include <fstream>

#include "ParseUtils.h"
#include "Logger.h"

bool readLine(std::ifstream& is, std::string& s)
{
	bool result = static_cast<bool>(std::getline(is, s));
	if (result)
	{
		if (s.size() && s.at(0) == '#')
		{
			result = static_cast<bool>(std::getline(is, s));
		}
	}
	return result;
}

int parseInt(char *& p)
{
	int value;
	if (p)
	{
		value = std::atoi(p);
		p = std::strtok(NULL, ",");
	}

	return value;
}

float parseFloat(char *& p)
{
	float value;
	if (p)
	{
		value = std::atof(p);
		p = std::strtok(NULL, ",");
	}

	return value;
}

int parseLineParam(std::ifstream& file)
{
	std::string line;
    if (readLine(file, line))
    {
        return std::atoi(line.c_str());
    }
    else
    {
    	LOG_ERR("Error parsing parameter from text file.");
    }
    return 0;
}
