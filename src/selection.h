#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "include/cpptoml.h"
#include "output.h"
#include "arguments.h"
#include "helpers.h"
#include "group.h"

//Represents everything a user want us to perform an action on
class Selection{
private:
    std::vector<Group> groups;

    //Use the cpptoml library to parse the toml file if it exist.
    std::shared_ptr<cpptoml::table> parseToml(const Arguments& args){
        //Check if file exist in current directory
        if(!fs::exists(args.locationsFile)){
            printErrorAndExit("No locations.toml found.");
        }

        std::shared_ptr<cpptoml::table> ptr;

        try{
            ptr = cpptoml::parse_file(args.locationsFile);
        }
        catch(cpptoml::parse_exception e){
            printErrorAndExit(e.what());
        }

        return ptr;
    }

public:
    Selection(const Arguments& args){
        std::shared_ptr<cpptoml::table> toml(parseToml(args));
    
        switch(args.selectionMode){
            case SelectionMode::All:{
                for(const auto& groupTableToml : *toml){
                    groups.emplace_back(args, groupTableToml.first, *(groupTableToml.second->as_table()));
                }
                break;
            }
            case SelectionMode::Groups:{
                for (const auto& key : args.keys){
                    auto groupTableToml = toml->get_table(key);
                    if(groupTableToml){
                        groups.emplace_back(args, key, *(groupTableToml->as_table()));
                    }
                }
                break;
            }
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