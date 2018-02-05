#pragma once

#include <experimental/filesystem>
namespace fs = std::experimental::filesystem::v1;

#include "include/cpptoml.h"

#include "output.h"

//Use the cpptoml library to parse the toml file if it exist.
std::shared_ptr<cpptoml::table> parseToml(const Arguments& args){
    //Check if file exist in current directory
    if(!fs::exists(args.configPath)){
        printErrorAndExit("No locations.toml found in your current directory");
    }

    std::shared_ptr<cpptoml::table> ptr;

    try{
        ptr = cpptoml::parse_file(args.configPath);
    }
    catch(cpptoml::parse_exception e){
        printErrorAndExit(e.what());
    }

    return ptr;
}

//Replace the first character of a string with the home path, if it is a '~'
std::string unfoldHomePath(const std::string& str){
    std::string ret(str);
    char tilde = '~';
    char const* home = getenv("HOME");
    if (home or ((home = getenv("USERPROFILE")))) {
        if(ret[0] == tilde) ret.replace(0, 1, home);
    }

    return ret;
}

std::vector<std::string> getFilesInDirectoryRecursively(const fs::path& path){
    std::vector<std::string> paths;
    try{
        for(auto& p : fs::recursive_directory_iterator(path)){
            if(fs::is_regular_file(p)){
                paths.push_back(p.path());
            }
        }
    }
    catch(...){}
    return paths;
}

std::string removeStartingSubstring(const std::string& str, const std::string& sub){
    if(sub.length() >= str.length()){
        return "";
    }
    else{
        return std::string(str.begin()+sub.length(), str.end());
    }
}

bool isFilesIdentical(const fs::path& p1, const fs::path& p2){
    std::ifstream lFile(p1.c_str(), std::ifstream::in | std::ifstream::binary);
    std::ifstream rFile(p2.c_str(), std::ifstream::in | std::ifstream::binary);

    //If an open failed, then it probably doesn't even exist
    if(!lFile.is_open() || !rFile.is_open())
    {
        return false;
    }

    if (std::equal(std::istreambuf_iterator<char>(lFile.rdbuf()), std::istreambuf_iterator<char>(),
                   std::istreambuf_iterator<char>(rFile.rdbuf()), std::istreambuf_iterator<char>())) {
        return true;
    }
    else {
        return false;
    }
}

void createPathToFileIfNeeded(const std::string& filePath){
    try{
        fs::create_directories(fs::path(filePath).remove_filename());
    }
    catch(fs::filesystem_error e){
        printErrorAndExit(e.what());
    }
}