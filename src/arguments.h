#pragma once

#include <string>
#include <vector>

#include "output.h"

//Defines the options that decide what the purpose of the program execution is
enum class ProgramMode{
    Setup,
    List,
    Save,
    Load,
};

//Defines how the search for relevan groups should be
enum class SelectionMode{
    Groups,
    All
};

//Container for all the things that can change how the execution should be.
class Arguments{
public:
    ProgramMode programMode;
    SelectionMode selectionMode;
    std::vector<std::string> keys;
    std::string configPath;

    Arguments(int argc, char* argv[]){
        selectionMode = SelectionMode::All;
        configPath = "locations.toml";

        if (argc <= 1){
            printUsageAndExit();
        }
        if (argc > 1){
            auto arg = std::string(argv[1]);
            if (arg == "list") programMode = ProgramMode::List;
            else if (arg == "save") programMode = ProgramMode::Save;
            else if (arg == "load") programMode = ProgramMode::Load;
            else if (arg == "setup") programMode = ProgramMode::Setup;
            else printUsageAndExit();
        }
        if (argc > 2){
            selectionMode = SelectionMode::Groups;
            for (int i = 2; i < argc; i++){
                keys.emplace_back(argv[i]);
            }
        }
    }
};