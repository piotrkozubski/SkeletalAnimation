/*
 * Options.h
 *
 *  Created on: 27 paü 2014
 *      Author: zokp
 */

#ifndef OPTIONS_H_
#define OPTIONS_H_

#include <stdint.h>

/**
 * Options namespace contains functions/data related to application's
 * command line options.
 */
namespace Options
{
    /**
     * Structure contains options data parsed from the command line
     */
    struct Options
    {
        char* meshFileName;
        char* text1FileName;
        char* text2FileName;
        char* text3FileName;
        char* text4FileName;
        bool  fullScreen;
        uint32_t width;
        uint32_t height;
        bool wireframe;
        uint32_t speed;
        char* mapFileName;
    };

    /**
     * Parses command line options and returns OptionsData structure
     */
    Options parseOptions(int argc, char** argv);
}

#endif /* OPTIONS_H_ */
