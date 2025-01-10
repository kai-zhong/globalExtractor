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
        void mbpTreeDigestCompute();
        void mbpTreeAddUpdate(const Vertex& src, const Vertex& dst);
        void mbpTreeDeleteEdgeUpdate(const Vertex& v); // 节点未被删除，但是节点信息发生改变，需要更新节点的摘要
        void mbpTreeDeleteVertexUpdate(const VertexID& vid); // 节点被删除，需要删除mbp树中该节点的摘要

        std::map<VertexID, std::string> serializeGraphInfo(const Graph& graph, const Graph& subgraph, std::vector<VertexID>& subgraphVids);

        Graph subgraphExtract(const Graph& G, uint khop, VertexID vid);
        Graph kcoreExtract(const Graph& originG, Graph G, uint k, VertexID queryVid);

        void getRootDigest(unsigned char* _digest);

        void vertify(Graph& subgraph, std::queue<VOEntry>& VO, unsigned char* partdigest);

        void calculateVOSize();

        const std::vector<VOEntry>& getVO() const;

        void printVO();
};

