#pragma once
#ifndef CONFIG_HPP
#define CONFIG_HPP

/**
 * Configurations
 */

// Follow the size as the convention
// Ref: https://stackoverflow.com/q/4651012
#define SCREEN_WIDTH  80
#define SCREEN_HEIGHT 24

#define WORLD_WIDTH   28
#define WORLD_HEIGHT  19

#define TURN_PERIOD   200
#define TURN_MAX      10000


namespace gol
{
    extern int turnPeriod;
}

#endif
