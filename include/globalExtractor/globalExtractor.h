#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <unordered_map>
#include <queue>

#include "../graph/graph.h"
#include "../graph/vertex.h"
#include "../mbptree/mbptree.h"
#include "../util/common.h"
#include "../configuration/types.h"

class MbpTree;
class MbpNode;
class Graph;

class globalExtractor
{
    private:
        MbpTree* mbptree;
        std::vector<VOEntry> vo;
    public:
        globalExtractor();
        ~globalExtractor();

        void buildMbpTree(const Graph& graph, const uint maxcapacity);

        std::map<VertexID, std::string> serializeGraphInfo(const Graph& graph, const Graph& subgraph, std::vector<VertexID>& subgraphVids);

        Graph subgraphExtract(const Graph& G, uint khop, VertexID vid, bool needVO);
        Graph kcoreExtract(Graph G, uint k, VertexID queryVid);

        void getRootDigest(unsigned char* _digest);

        void vertify(Graph& subgraph, std::queue<VOEntry>& VO, unsigned char* partdigest);

        void calculateVOSize();

        const std::vector<VOEntry>& getVO() const;

        void printVO();
};

