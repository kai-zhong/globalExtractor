#include "util/common.h"

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

void digestPrint(const unsigned char* digest)
{
    for (size_t i = 0; i < SHA256_DIGEST_LENGTH; i++) 
    {
        printf("%02x", digest[i]);
    }
    std::cout << std::endl;
}