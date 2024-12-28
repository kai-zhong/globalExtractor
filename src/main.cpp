#include <iostream>

#include "./graph/vertex.h"
#include "./graph/graph.h"
#include "./globalExtractor/globalExtractor.h"
#include "./mbptree/mbptree.h"
#include "./util/common.h"

int main(int argc, char* argv[])
{
    cmdOptions options = parseCmdLineArgs(argc, argv);

    Graph graph;
    globalExtractor extractor;

    graph.loadGraphfromFile(options.filename);
    std::cout << "Graph Vertex Num : " << graph.getVertexNum() << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    extractor.buildMbpTree(graph, options.maxcapacity);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Tree building Time taken: " << duration.count() << " ms" << std::endl << std::endl;

    start = std::chrono::high_resolution_clock::now();

    Graph subgraph = extractor.subgraphExtract(graph, options.khop, options.query, true);
    std::cout << "Subgraph has extracted" << std::endl;
    std::cout << "SubGraph Vertex Num : " << subgraph.getVertexNum() << std::endl << std::endl;
    Graph getGraph;
    unsigned char vertifyDigest[SHA256_DIGEST_LENGTH];
    std::queue<VOEntry> queueVO = convertVectorToQueue(extractor.getVO());
    extractor.vertify(getGraph, queueVO, vertifyDigest);

    Graph kcoreGraph = extractor.kcoreExtract(subgraph, options.k, options.query);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Computation Time taken: " << duration.count() << " ms" << std::endl << std::endl;
    extractor.calculateVOSize();
    return 0;
}
