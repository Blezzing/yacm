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
        std::cout << "Saving elements of group [" << name << "]" << std::endl; 
        for (const auto& element : elements){
            element.save();
        }
        std::cout << std::endl;
    }

    void load() const{
        std::cout << "Loading elements of group [" << name << "]" << std::endl; 
        for (const auto& element : elements){
            element.load();
        }
        std::cout << std::endl;
    }
};