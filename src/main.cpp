#include <iostream>

#include "./graph/vertex.h"
#include "./graph/graph.h"
#include "./globalExtractor/globalExtractor.h"
#include "./mbptree/mbptree.h"
#include "./util/common.h"



int main(int argc, char* argv[])
{
    cmdOptions options = parseCmdLineArgs(argc, argv);
    std::cout << "file path: "<< options.filename << std::endl;
    std::cout << "query vertex: "<< options.query << std::endl;
    std::cout << "k: " << options.k << std::endl;
    std::cout << "khop: " << options.khop << std::endl;
    std::cout << "max capacity: " << options.maxcapacity << std::endl;
    std::cout << PRINT_SEPARATOR << std::endl;


    VertexID minDegreeV;
    uint deg;

    Graph graph;
    graph.loadGraphfromFile(options.filename);
    std::cout << "Graph has loaded" << std::endl;
    // graph.printGraphInfoSimple();

    auto start = std::chrono::high_resolution_clock::now();
    globalExtractor extractor;
    Graph subgraph = extractor.subgraphExtract(graph, options.khop, options.query);
    std::cout << "Subgraph has extracted" << std::endl;

    Graph kcoreGraph = extractor.kcoreExtract(subgraph, options.k, options.query);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Computation Time taken: " << duration.count() << " ms" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    MbpTree mbptree(options.maxcapacity);
    for(uint i = 0; i < 1000000; i++)
    {
        mbptree.set(i, i);
    }
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Time taken: " << duration.count() << " ms" << std::endl;
    
    return 0;
}
