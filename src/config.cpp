#include"mini/ini.h"
#include"config.hpp"


namespace gol
{
    // int gol::turnPeriod = TURN_PERIOD;

    Config* cfg = nullptr;


    void loadConfig(const std::string filename)
    {
        cfg = new Config(filename);

        return;
    }

    Config::Config(const std::string filename)
    {
        mINI::INIFile file(filename);
        mINI::INIStructure ini;
        file.read(ini);

        screenWidth  = std::stoi(ini["sizes"]["screen_width" ]);
        screenHeight = std::stoi(ini["sizes"]["screen_height"]);
        worldWidth   = std::stoi(ini["sizes"]["world_width"  ]);
        worldHeight  = std::stoi(ini["sizes"]["world_height" ]);

        turnPeriod = std::stoi(ini["run"]["turn_period"]);
        turnMax    = std::stoi(ini["run"]["turn_max"   ]);

        server_ip_addr =           ini["server"]["ip_addr"];
        server_port    = std::stoi(ini["server"]["port"   ]);
    }
}
