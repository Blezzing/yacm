#pragma once

#include <vector>
#include <string>
#include <iostream>

#include "helpers.h"
#include "element.h"

//Represents a group of elements, and is little more than a link between those and a Selection
class Group{
private:
    std::vector<Element> elements;

public:
    std::string name;

    Group(const Arguments& args, const std::string& groupName, const cpptoml::table& groupContentToml){
        name = groupName;
        
        for (const auto& elementToml : groupContentToml){
            elements.emplace_back(args, elementToml.first, *(elementToml.second->as_array()));
        }
    }

    void list() const{
        std::cout << "[" << name << "]" << std::endl; 
        for (const auto& element : elements){
            element.list();
        }
        std::cout << std::endl;
    }

    void save() const{
        std::cout << "[" << name << "]" << std::endl;
        for (const auto& element : elements){
            element.save();
        }
        std::cout << std::endl;
    }

    void load() const{
        std::cout << "[" << name << "]" << std::endl;
        for (const auto& element : elements){
            element.load();
        }
        std::cout << std::endl;
    }
};