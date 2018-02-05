#pragma once

#include <iostream>
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem::v1;

#include "helpers.h"
#include "output.h"

namespace setup{
    void createConfigurationFile(const std::string& where){
        auto filepath = unfoldHomePath("~/.config/yacm/config");
        createPathToFileIfNeeded(filepath);
        std::ofstream config(filepath);

        config << where << std::endl;
    }

    void createLocationsTemplate(const std::string& where){
        auto filepath = unfoldHomePath(where + "/locations.toml");
        createPathToFileIfNeeded(filepath);
        std::ofstream locations(filepath);

        locations <<
            "[yacm]" << std::endl <<
            "config = ['~/.config/yacm/config', 'yacm/config']" << std::endl;
    }

    std::string selectBackupLocation(){
        while(true){
            auto whereToPutBackupQuestion = "Where do you want to locate your backup directory (eg. ~/git/dotfiles)?";
            auto whereToPutBackup = unfoldHomePath(stringPrompt(whereToPutBackupQuestion));
            if (fs::is_directory(whereToPutBackup)){
                std::cout << "\tSelected already existing directory " << whereToPutBackup << std::endl;
                return whereToPutBackup;
            }
            else if (!fs::exists(whereToPutBackup)){
                createPathToFileIfNeeded(whereToPutBackup + "/");
                std::cout << "\tCreated directory " << whereToPutBackup << std::endl;
                return whereToPutBackup;
            }
            else{
                std::cout << "\tThe chosen location was not valid, try again. (it could be an existing file)" << std::endl;
            }
        }
    }

    bool optLocationsTemplate(const std::string& location){
        auto wantTemplateLocationsFileQuestion = "Do you want to have a template locations.toml generated in your backup directory?";
        auto wantTemplateLocationsFile = booleanPromt(wantTemplateLocationsFileQuestion);
        if (wantTemplateLocationsFile){
            createLocationsTemplate(location);
            std::cout << "\tGenerated " << location << "/locations.toml" << std::endl;
            return true;
        }
        return false;
    }

    bool optConfigurationFile(const std::string& location){
        auto wantConfigurationFileQuestion = "Do you want to store a configuration file in ~/.config/yacm so you can use yacm from anywhere?";
        auto wantConfigurationFile = booleanPromt(wantConfigurationFileQuestion);
        if (wantConfigurationFile){
            createConfigurationFile(location);
            std::cout << "\tGenerated " << unfoldHomePath("~/.config/yacm/config") << std::endl;
            return true;
        }
        return false;
    }

    void guide(){
        auto location = selectBackupLocation();
        optLocationsTemplate(location);
        optConfigurationFile(location);
    }
}