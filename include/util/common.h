#pragma once
#include <string>
#include <chrono>
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

cmdOptions parseCmdLineArgs(int argc, char* argv[])
{
    cmdline::parser parser;
    parser.add<std::string>("filename", 'f', "Graph data file path", true);
    parser.add<VertexID>("query", 'q', "Query vertex ID", true);
    parser.add<uint>("k", 'k', "kmax of k-core subgraph", true);
    parser.add<uint>("khop", 'h', "k-hop neighborhood of query vertex", false, 6);
    parser.add<uint>("maxcapacity", 'c', "Maximum capacity of the Mbptree", false, 16);

    parser.parse_check(argc, argv);

    cmdOptions options;
    options.filename = parser.get<std::string>("filename");
    options.query = parser.get<VertexID>("query");
    options.k = parser.get<uint>("k");
    options.khop = parser.get<uint>("khop");
    options.maxcapacity = parser.get<uint>("maxcapacity");
    return options;
}