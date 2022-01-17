#include "commonarguments.h"

CommonArguments::CommonArguments()
{
    _parser.addPositionalArgument(
        "ff8frPackDirectory", "FF8.Fr Pack Installation root directory", "<FF8frPackDirectory>"
    );
    _parser.addPositionalArgument(
        "ff8Directory", "FF8 Installation root directory", "<FF8Directory>"
    );
}
