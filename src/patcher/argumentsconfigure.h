#pragma once

#include "commonarguments.h"

class ArgumentsConfigure : public CommonArguments
{
public:
    ArgumentsConfigure();
private:
    void parse();
};
