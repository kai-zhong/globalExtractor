#pragma once

#include <string>
#include <chrono>
#include <openssl/sha.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "../util/cmdline.h"
#include "../configuration/types.h"

struct cmdOptions
{
    std::string filename;
    VertexID query;
    uint k;
    uint khop;
    uint maxcapacity;
};

cmdOptions parseCmdLineArgs(int argc, char* argv[]);

void digestPrint(const unsigned char* digest);