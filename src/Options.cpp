/*
 * Options.cpp
 *
 *  Created on: 27.10.2014
 *      Author: zokp
 */
#include <cstdlib>
#include <getopt.h>

#include "Logger.h"
#include "Options.h"

static const uint32_t WIN_WIDTH  = 2560;
static const uint32_t WIN_HEIGHT = 1080;
static const uint32_t ANIM_SPEED = 1000;

Options::Options Options::parseOptions(int argc, char** argv)
{
    Options optData = {0};
    //defaults
    optData.width  = WIN_WIDTH;
    optData.height = WIN_HEIGHT;
    optData.speed = ANIM_SPEED;

    static const option LONGOPTIONS[] =
    {
        {"mesh",       required_argument, 0, 'm'},
        {"t1",         required_argument, 0,  1 },
        {"t2",         required_argument, 0,  2 },
        {"t3",         required_argument, 0,  3 },
        {"t4",         required_argument, 0,  4 },
        {"fullscreen", required_argument, 0, 'f'},
        {"width",      required_argument, 0, 'w'},
        {"height",     required_argument, 0, 'h'},
        {"wireframe",  required_argument, 0, 'l'},
        {"light",  	   required_argument, 0,  5 },
		{"speed",      required_argument, 0, 's'},
		{"map",        required_argument, 0,  6},
        {0,            0,                 0,  0}
    };

    int opt;
    int optIdx;
    while ((opt = getopt_long(argc, argv, "m:fw:h:ls:a:",
                              LONGOPTIONS, &optIdx)) != -1)
    {
        switch (opt)
        {
            case 1: optData.text1FileName = optarg; break;
            case 2: optData.text2FileName = optarg; break;
            case 3: optData.text3FileName = optarg; break;
            case 4: optData.text4FileName = optarg; break;
            case 'm': optData.meshFileName = optarg; break;
            case 'f': optData.fullScreen = true; break;
            case 'w': optData.width = ::atoi(optarg); break;
            case 'h': optData.height = ::atoi(optarg); break;
            case 'l': optData.wireframe = true; break;
            case 's': optData.speed = atoi(optarg); break;
            case 5: optData.text4FileName = optarg; LOG_INF("Parsing 5: " << optarg); break;
            case 6: optData.mapFileName = optarg; break;
        }
    }

    LOG_DEB("Parsing options:");
    LOG_DEB("option geometry file name: " << optData.meshFileName);
    LOG_DEB("option texture 1 file name: " << optData.text1FileName);
    LOG_DEB("option texture 2 file name: " << optData.text2FileName);
    LOG_DEB("option texture 3 file name: " << optData.text3FileName);
    LOG_DEB("option texture 4 file name: " << optData.text4FileName);
    LOG_DEB("option full screen: " << optData.fullScreen);
    LOG_DEB("option width: " << optData.width);
    LOG_DEB("option height: " << optData.height);
    LOG_DEB("option wire frame mode: " << optData.wireframe);
    LOG_DEB("option speed: " << optData.speed);
    LOG_DEB("option map file name: " << optData.mapFileName);

    return optData;
}
