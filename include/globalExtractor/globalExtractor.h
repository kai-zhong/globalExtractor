#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <unordered_map>
#include "../graph/graph.h"
#include "../graph/vertex.h"
#include "../configuration/types.h"

class globalExtractor
{
    public:
        Graph subgraphExtract(const Graph& G, uint khop, VertexID vid);
        Graph kcoreExtract(Graph G, uint k, VertexID queryVid);
};

