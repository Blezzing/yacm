#include <iostream>
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem::v1;

#include "include/cpptoml.h"
#include "selection.h"
#include "arguments.h"
#include "output.h"

std::shared_ptr<cpptoml::table> parseToml(const Arguments& args){
    std::shared_ptr<cpptoml::table> ptr;

    try{
        ptr = cpptoml::parse_file(args.configPath);
    }
    catch(cpptoml::parse_exception e){
        printErrorAndExit(e.what());
    }

    return ptr;
}

///////////////////////////////////////
//Main
int main(int argc, char* argv[]){
    Arguments args(argc, argv);
    std::shared_ptr<cpptoml::table> toml(parseToml(args));
    Selection selection{*toml, args};

    switch(args.programMode){
        case ProgramMode::List: selection.list(); break;
        case ProgramMode::Save: selection.save(); break;
        case ProgramMode::Load: selection.load(); break;
    }

    return 0;
}