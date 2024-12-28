#include "globalExtractor/globalExtractor.h"

globalExtractor::globalExtractor()
{
    mbptree = nullptr;
}

globalExtractor::~globalExtractor()
{
    if(mbptree != nullptr)
    {
        delete mbptree;
    }
}

void globalExtractor::buildMbpTree(const Graph& graph, const uint maxcapacity)
{
    if(mbptree != nullptr)
    {
        delete mbptree;
        mbptree = nullptr;
    }
    mbptree = new MbpTree(maxcapacity);
    for(const std::pair<uint, Vertex>& nodepair : graph.getNodes())
    {
        VertexID vid = nodepair.first;
        mbptree->setVertexDigest(vid, graph.getVertexDigest(vid));
    }
    mbptree->getRoot()->digestCompute();
    // mbptree->printMbpTreeInfo();
}

std::map<VertexID, std::string> globalExtractor::serializeGraphInfo(const Graph& graph, const Graph& subgraph, std::vector<VertexID>& subgraphVids)
{
    std::map<VertexID, std::string> serializedInfo;
    std::ostringstream oss;

    for(const std::pair<uint, Vertex>& nodepair : subgraph.getNodes())
    {
        unsigned char splitter = '/';
        oss = std::ostringstream();
        VertexID vid = nodepair.first;
        const Vertex& subgraphNode = nodepair.second;
        const Vertex& graphNode = graph.getVertex(vid);
        const std::vector<VertexID>& subgraphNeighbors = subgraphNode.getNeighbors();
        const std::vector<VertexID>& graphNeighbors = graphNode.getNeighbors();

        subgraphVids.emplace_back(vid);

        oss << vid;
        for(const VertexID& subNeighbor : subgraphNeighbors)
        {
            oss << splitter << subNeighbor;
        }
        oss << '|' << vid;
        for(const VertexID& graphNeighbor : graphNeighbors)
        {
            oss << splitter << graphNeighbor;
        }

        serializedInfo[vid] = oss.str();
    }
    return serializedInfo;
}

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

    std::vector<VertexID> subgraphVids;
    subgraphVids.reserve(subgraph.getVertexNum());
    std::map<VertexID, std::string> serializedInfo = serializeGraphInfo(G, subgraph, subgraphVids);
    mbptree->constructVO(vo, subgraphVids, serializedInfo);
    std::cout << "VO has been constructed." << std::endl;
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

void globalExtractor::getRootDigest(unsigned char* _digest)
{
    mbptree->getRoot()->getDigest(_digest);
}

void globalExtractor::vertify(Graph& subgraph, std::queue<VOEntry>& VO, unsigned char* partDigest)
{
    unsigned char vertifyDigest[SHA256_DIGEST_LENGTH];
    SHA256_CTX ctx;
    SHA256_Init(&ctx);
    while(!VO.empty())
    {
        VOEntry entry = VO.front();
        VO.pop();
        if(entry.type == VOEntry::NODEDATA)
        {
            if(entry.nodeData != nullptr)
            {
                // 构建子图
                std::string nodeDataStr(entry.nodeData);
                std::pair<std::string, std::string> infoPair = splitStringtoTwoParts(nodeDataStr, "|");
                std::string subgraphNodeDataStr = infoPair.first;
                std::string graphNodeDataStr = infoPair.second;
                std::vector<VertexID> subvertexInfo;
                splitString(subgraphNodeDataStr, "/", subvertexInfo);
                VertexID vid = subvertexInfo[0];
                for(size_t i = 1; i < subvertexInfo.size(); i++)
                {
                    subgraph.addEdge(vid, subvertexInfo[i], false, false);
                }

                // 计算摘要
                unsigned char subgraphVertexDigest[SHA256_DIGEST_LENGTH];
                SHA256_CTX minictx;
                SHA256_Init(&minictx);
                SHA256_Update(&minictx, (unsigned char*)graphNodeDataStr.c_str(), graphNodeDataStr.size());
                SHA256_Final(subgraphVertexDigest, &minictx);

                // std::cout << "Vertex " << vid  << ": ";
                // std::cout << graphNodeDataStr << " -> ";
                // digestPrint(subgraphVertexDigest);

                SHA256_Update(&ctx, subgraphVertexDigest, SHA256_DIGEST_LENGTH);
            }
        }
        else if(entry.type == VOEntry::DIGEST)
        {
            // std::cout << "VOENTRY_DIGEST: ";
            // digestPrint(entry.digest);
            SHA256_Update(&ctx, entry.digest, SHA256_DIGEST_LENGTH);
        }
        else if(entry.type == VOEntry::SPECIAL)
        {
            if(entry.specialChar == '[')
            {
                unsigned char innerDigest[SHA256_DIGEST_LENGTH];
                vertify(subgraph, VO, innerDigest);
                if(VO.empty())
                {
                    memcpy(vertifyDigest, innerDigest, SHA256_DIGEST_LENGTH);
                    break;
                }
                else
                {
                    SHA256_Update(&ctx, innerDigest, SHA256_DIGEST_LENGTH);
                }
            }
            else if(entry.specialChar == ']')
            {
                SHA256_Final(partDigest, &ctx);
                return ;
            }
        }
        else
        {
            std::cerr << "Unknown VOEntry type: " << entry.type << std::endl;
            throw std::runtime_error("Unknown VOEntry type.");
        }
    }
    std::cout << "Vertify digest: ";
    digestPrint(vertifyDigest);
    unsigned char rootDigest[SHA256_DIGEST_LENGTH];
    getRootDigest(rootDigest);
    std::cout << "Root digest: ";
    digestPrint(rootDigest);
    std::cout << std::endl;
}

void globalExtractor::calculateVOSize()
{
    // size_t totalSize = sizeof(vo); // vector 内部结构的占用
    size_t totalSize = 0; // 仅计算 VOEntry 占用的大小
    for(const auto& entry : vo)
    {
        totalSize += sizeof(entry); // 每个 VOEntry 的占用
        if(entry.type == VOEntry::NODEDATA && entry.nodeData != nullptr)
        {
            totalSize += std::strlen(entry.nodeData) + 1; // 动态分配的 nodeData 大小
        }
    }
    std::cout << "Total size of vo:" << std::endl;
    std::cout << "  Bytes: " << totalSize << " B" << std::endl;
    std::cout << "  Kilobytes: " << totalSize / 1024.0 << " KB" << std::endl;
    std::cout << "  Megabytes: " << totalSize / (1024.0 * 1024.0) << " MB" << std::endl;
    std::cout << std::endl;
}

const std::vector<VOEntry>& globalExtractor::getVO() const
{
    return vo;
}

void globalExtractor::printVO()
{
    for(size_t i = 0; i < vo.size(); i++)
    {
        vo[i].printVOEntry();
        if(i < vo.size() - 1 && vo[i].type != VOEntry::SPECIAL && vo[i+1].type != VOEntry::SPECIAL)
        {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;
}