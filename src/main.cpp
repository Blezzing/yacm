#include "selection.h"
#include "arguments.h"
#include "output.h"
#include "helpers.h"
#include "setup.h"


//Let's go!
int main(int argc, char* argv[]){
    Arguments args(argc, argv);

    switch(args.programMode){
        case ProgramMode::List: {
            Selection selection(args); 
            selection.list();
            break;
        }
        case ProgramMode::Save: {
            Selection selection(args); 
            selection.save(); 
            break;
        }
        case ProgramMode::Load: {
            Selection selection(args); 
            selection.load(); 
            break;
        }
        case ProgramMode::Setup: {
            setup::guide();
            break;
        }
        default: {
            printErrorAndExit("The selected option is not yet implemented!"); 
            break;
        }
    }

    return 0;
}