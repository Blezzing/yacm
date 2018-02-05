#pragma once

#include <iostream>
#include <string>
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem::v1;

#include "helpers.h"

//Represents a single configuration file, and the relation between its two locations
class Element{
private:
    enum class elementKind {directory, file, mismatched, nonexisting, invalid};
    fs::path systemPath;
    fs::path backupPath;
    elementKind kind;
    std::string name;

    void saveAsFile() const{
        createPathToFileIfNeeded(backupPath);

        if(fs::exists(backupPath)){
            if(isFilesIdentical(systemPath, backupPath)){
                std::cout << "Skipped [" << name << "], current backup is identical." << std::endl;
                return;
            }

            auto question = "About [" + name + "]: Want to overwrite existing file [" + backupPath.string() + "]?";
            if (!booleanPromt(question)){
                std::cout << "Skipped [" << name << "], user decision." << std::endl;
                return;
            }
        }

        //Allow overwrites
        auto options = fs::copy_options::overwrite_existing; 
        fs::copy(systemPath, backupPath, options);

        std::cout << "Copied [" << name << "] to backup." << std::endl;
    }

    void saveAsDirectory() const{
        //find the relevant files
        auto filesInSystem = getFilesInDirectoryRecursively(systemPath);
        auto filesInBackup = getFilesInDirectoryRecursively(backupPath);

        //get a form of the path where we can relate the two paths to each other
        auto shortedFilesInSystem = filesInSystem;
        auto shortedFilesInBackup = filesInBackup;
        for(auto& file : shortedFilesInSystem){
            file = removeStartingSubstring(file, systemPath);
        }
        for(auto& file : shortedFilesInBackup){
            file = removeStartingSubstring(file, backupPath);
        }

        //the actual handling of each file
        for(auto fileIndex = 0u; fileIndex < filesInSystem.size(); fileIndex++){
            auto pathInSystem = filesInSystem[fileIndex];
            auto pathInBackup = std::string(backupPath) + shortedFilesInSystem[fileIndex];

            //does it exist in backup?
            bool isInBackupDirectory = shortedFilesInBackup.end() != std::find(shortedFilesInBackup.begin(), shortedFilesInBackup.end(), shortedFilesInSystem[fileIndex]);

            if(isInBackupDirectory){
                if(isFilesIdentical(pathInSystem, pathInBackup)){
                    std::cout << "Skipped [" << name << "] content [" << shortedFilesInSystem[fileIndex] << "], current backup is identical." << std::endl;
                    continue;
                }

                auto question = "About [" + name + "]: Want to overwrite existing [" + pathInBackup + "]?";
                if (!booleanPromt(question)){
                    std::cout << "Skipped [" << name << "] content [" << shortedFilesInSystem[fileIndex] << "], user decision." << std::endl;
                    continue;
                }
            }

            createPathToFileIfNeeded(pathInBackup);

            auto options = fs::copy_options::overwrite_existing; 
            fs::copy(pathInSystem, pathInBackup, options);

            std::cout << "Copied [" << name << "] content [" << shortedFilesInSystem[fileIndex] << "] to backup." << std::endl;
        }
    }

    void loadAsFile() const{
        createPathToFileIfNeeded(systemPath);

        if(fs::exists(backupPath)){
            if(isFilesIdentical(systemPath, backupPath)){
                std::cout << "Skipped [" << name << "], current system is identical." << std::endl;
                return;
            }

            auto question = "About [" + name + "]: Want to overwrite existing [" + systemPath.string() + "]?";
            if (!booleanPromt(question)){
                std::cout << "Skipped [" << name << "], user decision." << std::endl;
                return;
            }
        }

        //Allow overwrites
        auto options = fs::copy_options::overwrite_existing; 
        fs::copy(backupPath, systemPath, options);

        std::cout << "Copied [" << name << "] to system." << std::endl;
    }

    void loadAsDirectory() const{
        //find the relevant files
        auto filesInSystem = getFilesInDirectoryRecursively(systemPath);
        auto filesInBackup = getFilesInDirectoryRecursively(backupPath);

        //get a form of the path where we can relate the two paths to each other
        auto shortedFilesInSystem = filesInSystem;
        auto shortedFilesInBackup = filesInBackup;
        for(auto& file : shortedFilesInSystem){
            file = removeStartingSubstring(file, systemPath);
        }
        for(auto& file : shortedFilesInBackup){
            file = removeStartingSubstring(file, backupPath);
        }

        //the actual handling of each file
        for(auto fileIndex = 0u; fileIndex < filesInBackup.size(); fileIndex++){
            auto pathInBackup = filesInBackup[fileIndex];
            auto pathInSystem = std::string(systemPath) + shortedFilesInBackup[fileIndex];

            //does it exist in backup?
            bool isInSystemDirectory = shortedFilesInSystem.end() != std::find(shortedFilesInSystem.begin(), shortedFilesInSystem.end(), shortedFilesInBackup[fileIndex]);

            if(isInSystemDirectory){
                if(isFilesIdentical(pathInSystem, pathInBackup)){
                    std::cout << "Skipped [" << name << "] content [" << shortedFilesInBackup[fileIndex] << "], current system is identical." << std::endl;
                    continue;
                }

                auto question = "About [" + name + "]: Want to overwrite existing [" + pathInSystem + "]?";
                if (!booleanPromt(question)){
                    std::cout << "Skipped [" << name << "] content [" << shortedFilesInSystem[fileIndex] << "], user decision." << std::endl;
                    continue;
                }
            }

            createPathToFileIfNeeded(pathInSystem);

            auto options = fs::copy_options::overwrite_existing; 
            fs::copy(pathInBackup, pathInSystem, options);

            std::cout << "Copied [" << name << "] content [" << shortedFilesInBackup[fileIndex] << "] to system." << std::endl;
        }
    }

public:
    Element(const std::string& elementName, const cpptoml::array& elementArray)
        : systemPath(unfoldHomePath(elementArray.get_array_of<std::string>()->at(0)))
        , backupPath(unfoldHomePath(elementArray.get_array_of<std::string>()->at(1)))
        , name(elementName){

        //set kind
        if(fs::is_directory(systemPath)){
            if(!fs::exists(backupPath) || fs::is_directory(backupPath)){
                kind = elementKind::directory;
            }
            else{
                kind = elementKind::mismatched;
            }
        }
        else if(fs::is_regular_file(systemPath)){
            if(!fs::exists(backupPath) || fs::is_regular_file(backupPath)){
                kind = elementKind::file;
            }
            else{
                kind = elementKind::mismatched;
            }
        }
        else if(!fs::exists(systemPath)){
            if(!fs::exists(backupPath)){
                kind = elementKind::nonexisting;
            }
            else if(fs::is_directory(backupPath)){
                kind = elementKind::directory;
            }
            else if(fs::is_regular_file(backupPath)){
                kind = elementKind::file;
            }
            else{
                kind = elementKind::invalid;
            }
        }
        else{
            kind = elementKind::invalid;
        }
    }

    void list() const{
        std::cout << "[" << name << "] is a ";
        switch (kind){
            case elementKind::file: std::cout << "file."; break;
            case elementKind::directory: std::cout << "directory."; break;
            case elementKind::mismatched: std::cout << "mismatch between directory and a regular file. Please fix!"; break;
            case elementKind::nonexisting: std::cout << "nonexisting element on both system and backup."; break;
            default: std::cout << "invalid element."; break;
        }
        std::cout << std::endl;
    }

    void save() const{
        switch(kind){
            case elementKind::file: saveAsFile(); break;
            case elementKind::directory: saveAsDirectory(); break;
            case elementKind::nonexisting: std::cout << "Failed [" << name << "] due to both paths being nonexisting." << std::endl; break;
            case elementKind::mismatched: std::cout << "Failed [" << name << "] due to both paths not being either a file or a directory." << std::endl; break;
            default: std::cout << "Failed [" << name << "] is invalid!" << std::endl;
        }
    }

    void load() const{
        switch(kind){
            case elementKind::file: loadAsFile(); break;
            case elementKind::directory: loadAsDirectory(); break;
            case elementKind::nonexisting: std::cout << "Failed [" << name << "] due to both paths being nonexisting." << std::endl; break;
            case elementKind::mismatched: std::cout << "Failed [" << name << "] due to both paths not being either a file or a directory." << std::endl; break;
            default: std::cout << "Failed [" << name << "] is invalid!" << std::endl;
        }
    }  
};