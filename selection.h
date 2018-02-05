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