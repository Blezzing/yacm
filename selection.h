#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem::v1;

#include "include/cpptoml.h"
#include "output.h"
#include "arguments.h"
#include "helpers.h"

//Represents a single configuration file, and the relation between its two locations
class Element{
private:
    fs::path systemPath;
    fs::path backupPath;

public:
    std::string name;

    Element(const std::string& elementName, const cpptoml::array& elementArray){
        name = elementName;

        auto systemString = elementArray.get_array_of<std::string>()->at(0);
        auto backupString = elementArray.get_array_of<std::string>()->at(1);

        unfoldHomePath(systemString);
        unfoldHomePath(backupString);

        systemPath = fs::path(systemString);
        backupPath = fs::path(backupString);
    }

    bool isFilesIdentical() const{
        std::ifstream lFile(systemPath.c_str(), std::ifstream::in | std::ifstream::binary);
        std::ifstream rFile(backupPath.c_str(), std::ifstream::in | std::ifstream::binary);

        //If an open failed, then it probably doesn't even exist
        if(!lFile.is_open() || !rFile.is_open())
        {
            return false;
        }

        const int BUFFER_SIZE = 1024;
        char lBuffer[BUFFER_SIZE];
        char rBuffer[BUFFER_SIZE];

        do {
            lFile.read(lBuffer, BUFFER_SIZE);
            rFile.read(rBuffer, BUFFER_SIZE);

            if (std::memcmp(lBuffer, rBuffer, BUFFER_SIZE) != 0)
            {
                return false;
            }
        } while (lFile.good() || rFile.good());

        return true;
    }

    void list() const{
        printElementName(name);
    }

    void save() const{
        try{
            fs::create_directories(fs::path(backupPath).remove_filename());
        }
        catch(fs::filesystem_error e){
            printErrorAndExit(e.what());
        }

        if(fs::exists(backupPath)){
            if(isFilesIdentical()){
                std::cout << "Skipped " << name << " as the version currently in backup is identical!" << std::endl;
                return;
            }

            auto question = "Want to overwrite existing [" + backupPath.string() + "]?";
            if (!booleanPromt(question)){
                return;
            }
        }

        //Recursive copy if it is directory, and allow overwrites
        auto options = fs::copy_options::recursive | fs::copy_options::overwrite_existing; 
        fs::copy(systemPath, backupPath, options);

        std::cout << "Copied " << name << " to backup" << std::endl;
    }

    void load() const{
        try{
            fs::create_directories(fs::path(systemPath).remove_filename());
        }
        catch(fs::filesystem_error e){
            printErrorAndExit(e.what());
        }

        if(fs::exists(backupPath)){
            if(isFilesIdentical()){
                std::cout << "Skipped " << name << " as the version currently in system is identical!" << std::endl;
                return;
            }

            auto question = "Want to overwrite existing [" + systemPath.string() + "]?";
            if (!booleanPromt(question)){
                return;
            }
        }

        //Recursive copy if it is directory, and allow overwrites
        auto options = fs::copy_options::recursive | fs::copy_options::overwrite_existing; 
        fs::copy(backupPath, systemPath, options);

        std::cout << "Copied " << name << " to system" << std::endl;
    }

    
};

//Represents a group of elements, and is little more than a link between those and a Selection
class Group{
private:
    std::vector<Element> elements;

public:
    std::string name;

    Group(const std::string& groupName, const cpptoml::table& groupContentToml){
        name = groupName;
        
        for (const auto& elementToml : groupContentToml){
            elements.emplace_back(elementToml.first, *(elementToml.second->as_array()));
        }
    }

    void list() const{
        printGroupName(name);
        for (const auto& element : elements){
            element.list();
        }
        std::cout << std::endl;
    }

    void save() const{
        for (const auto& element : elements){
            element.save();
        }
    }

    void load() const{
        for (const auto& element : elements){
            element.load();
        }
    }
};

//Represents everything a user want us to perform an action on
class Selection{
private:
    std::vector<Group> groups;

public:
    Selection(const cpptoml::table& completeToml, const Arguments& args){
        switch(args.selectionMode){
            case SelectionMode::All:{
                for(const auto& groupTableToml : completeToml){
                    groups.emplace_back(groupTableToml.first, *(groupTableToml.second->as_table()));
                }
            }
            break;
            case SelectionMode::Groups:{
                for (const auto& key : args.keys){
                    auto groupTableToml = completeToml.get_table(key);
                    if(groupTableToml){
                        groups.emplace_back(key, *(groupTableToml->as_table()));
                    }
                }
            }
            break;
        }
    }

    void list() const{
        for (const auto& group : groups){
            group.list();
        }
    }

    void save() const{
        for (const auto& group : groups){
            group.save();
        }
    }

    void load() const{
        for (const auto& group : groups){
            group.load();
        }
    }
};