#pragma once
#ifndef CONFIG_H
#define CONFIG_H

/**
 * Configurations
 */

#define SCREEN_WIDTH  84
#define SCREEN_HEIGHT 24

#define WORLD_WIDTH   30
#define WORLD_HEIGHT  20

#define TURN_PERIOD   200
#define TURN_MAX      10000


namespace gol
{
    extern int turnPeriod;
}

#endif
