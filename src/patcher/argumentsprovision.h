#pragma once

#include "commonarguments.h"

class ArgumentsProvision : public CommonArguments
{
public:
    ArgumentsProvision();
private:
    void parse();
};
