#include "globalExtractor/globalExtractor.h"

Graph globalExtractor::subgraphExtract(const Graph& G, uint khop, VertexID vid)
{
    if(G.getVertexNum() == 0)
    {
        std::cerr << "The graph is empty." << std::endl;
        throw std::runtime_error("The graph is empty.");
    }
    if(!G.hasVertex(vid))
    {
        std::cerr << "Vertex " << vid << " is not in the graph." << std::endl;
        throw std::runtime_error("Vertex" + std::to_string(vid) + "is not in the graph.");
    }
    Graph subgraph;
    std::unordered_map<VertexID, bool> visited;
    std::queue<std::pair<VertexID, uint>> bfsQueue; // (当前节点，跳数)

    bfsQueue.push(std::make_pair(vid, 0));
    visited[vid] = true;

    while(!bfsQueue.empty())
    {
        std::pair<VertexID, uint> current = bfsQueue.front();
        bfsQueue.pop();

        VertexID currentVid = current.first;
        // std::cout<< "Current vertex: " << currentVid << std::endl;
        uint currentHop = current.second;

        if(currentHop > khop)
        {
            break;
        }

        Vertex currentVertex = G.getVertex(currentVid);
        const std::vector<VertexID>& currentVNeighbors = currentVertex.getNeighbors();

        subgraph.addVertex(currentVid, false, false);

        // 遍历当前节点的邻居
        if(G.getVertexDegree(currentVid) > 0)
        {
            
            for (std::vector<VertexID>::const_iterator it = currentVNeighbors.begin(); it != currentVNeighbors.end(); ++it)
            {
                VertexID neighborVid = *it;

                if(subgraph.hasVertex(neighborVid))
                {
                    subgraph.addEdge(currentVid, neighborVid, false, false);
                }

                if(currentHop < khop && !visited[neighborVid])
                {
                    visited[neighborVid] = true;
                    bfsQueue.push(std::make_pair(neighborVid, currentHop + 1));
                }
            }
        }
    }
    subgraph.buildInvertedIndex();
    return subgraph;
}

Graph globalExtractor::kcoreExtract(Graph G, uint k, VertexID queryVid)
{
    VertexID minDegreeV;
    uint deg;
    bool satisfyKcore = false;
    Graph kcoreG;
    while(true)
    {
        minDegreeV = G.getMinDegreeVertexID();
        deg = G.getVertexDegree(minDegreeV);
        if(deg < k)
        {
            if(minDegreeV == queryVid)
            {
                break;
            }
            G.removeVertex(minDegreeV, true, false);
        }
        else
        {
            satisfyKcore = true;
            break;
        }
    }
    if(!satisfyKcore)
    {
        std::cout << "The graph does not satisfy k-core property." << std::endl;
        return G;
    }
    // 将所有不连通节点删除
    return subgraphExtract(G, 1000, queryVid);
}