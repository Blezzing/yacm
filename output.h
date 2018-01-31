#pragma once

#include <iostream>
#include <string>

//Used whenever the user input was malformed
void printUsageAndExit(){
    std::cout << 
    "Syntax: " << std::endl << 
    "\tyacm [option]            to handle all files described in ./locations.toml " << std::endl <<
    "\tyacm [option] [group(s)] to handle only specified group(s) described in ./locations.toml" << std::endl <<
    "Where option is one of:" << std::endl <<
    "\tlist                     to list all parsed groups and elements from ./location.toml" << std::endl <<
    "\tsave                     to copy from the system paths to the backup paths" << std::endl <<
    "\tload                     to copy from the backup paths to the system paths" << std::endl;
    exit(1);
}

//Used whenever an unrecoverable error occured
void printErrorAndExit(const char* description){
    std::cerr << "ERROR: " << description << std::endl;
    exit(1);
}

//Used by list
void printGroupName(const std::string& groupName){
    std::cout << "==== " << groupName << std::endl;
}

//Used by list
void printElementName(const std::string& elementName){
    std::cout << "  -- " << elementName << std::endl;
}

//Used whenever a yes/no choise have to be made
bool booleanPromt(const std::string& question){
    char response;
    while(true){
        std::cout << question << " (y/n):";
        std::cin >> response;
        switch(std::tolower(response)){
            case 'y': return true; break;
            case 'n': return false; break;
            default: break;
        }
        std::cout << "Did not understand that response, try again." << std::endl;
    }
}