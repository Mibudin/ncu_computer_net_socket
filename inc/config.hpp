#pragma once
#ifndef CONFIG_HPP
#define CONFIG_HPP

#include<string>


/**
 * Configurations
 */

// Follow the size as the convention
// Ref: https://stackoverflow.com/q/4651012
// #define SCREEN_WIDTH  80
// #define SCREEN_HEIGHT 24

// #define WORLD_WIDTH   28
// #define WORLD_HEIGHT  19

// #define TURN_PERIOD   200
// #define TURN_MAX      10000

#define DEFAULT_CONFIG_FILE "./config.ini"


namespace gol
{
    #define MINI_CASE_SENSITIVE


    // extern int turnPeriod;


    void loadConfig(const std::string filename);


    class Config
    {
    public:
        Config(const std::string filename);
        int screenWidth;
        int screenHeight;
        int worldWidth;
        int worldHeight;
        int turnPeriod;
        int turnMax;
    };

    extern Config* cfg;
}

#endif
