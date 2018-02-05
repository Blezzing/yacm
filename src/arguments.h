#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem::v1;

#include "output.h"
#include "helpers.h"

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
struct Arguments{
    ProgramMode programMode;
    SelectionMode selectionMode;
    std::vector<std::string> keys;

    std::string configPath;
    std::string backupPath;
    std::string locationsFile;

    Arguments(int argc, char* argv[]){
        selectionMode = SelectionMode::All;

        configPath = unfoldHomePath("~/.config/yacm/config");
        auto tomlName = "locations.toml";

        //determine if current directory is a valid backup
        if(fs::exists(tomlName) && fs::is_regular_file(tomlName)){
            backupPath = fs::current_path();
        }
        //determine if (the maybe) mentioned path in config file is valid
        else if(fs::exists(configPath)){ //TODO: make this handle if config file is malformed
            std::ifstream config(configPath);
            config >> backupPath;
        }
        //give up
        else{
            printErrorAndExit("Need to have a directory with a locations.toml, either as working directory, or mentioned in the config file to work.");
        }

        locationsFile = backupPath + "/" + tomlName;

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