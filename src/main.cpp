#include "selection.h"
#include "arguments.h"
#include "output.h"
#include "helpers.h"


//Let's go!
int main(int argc, char* argv[]){
    Arguments args(argc, argv);
    std::shared_ptr<cpptoml::table> toml(parseToml(args));
    Selection selection{*toml, args};

    switch(args.programMode){
        case ProgramMode::List: selection.list(); break;
        case ProgramMode::Save: selection.save(); break;
        case ProgramMode::Load: selection.load(); break;
        default: printErrorAndExit("The selected option is not yet implemented!"); break;
    }

    return 0;
}