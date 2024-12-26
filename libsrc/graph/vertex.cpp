#include "graph/vertex.h"

Vertex::Vertex() : id(0), degree(0) {}

Vertex::Vertex(VertexID vid) : id(vid), degree(0) {}

Vertex::Vertex(const Vertex& other)
{
    id = other.id;
    degree = other.degree;
    neighbors = other.neighbors;
}

Vertex::~Vertex() {}

VertexID Vertex::getVid() const
{
    return id;
}

uint Vertex::getDegree() const
{
    return degree;
}

const std::vector<VertexID>& Vertex::getNeighbors() const
{
    return neighbors;
}

bool Vertex::hasNeighbor(VertexID neighbor_vid) const
{
    return std::find(neighbors.begin(), neighbors.end(), neighbor_vid) != neighbors.end();
}

void Vertex::addNeighbor(VertexID neighbor_vid)
{
    if(!hasNeighbor(neighbor_vid))
    {
        neighbors.emplace_back(neighbor_vid);
        degree++;
    }
}

void Vertex::removeNeighbor(VertexID neighbor_vid)
{
    auto it = std::find(neighbors.begin(), neighbors.end(), neighbor_vid);
    if(it != neighbors.end())
    {
        neighbors.erase(it);
        degree--;
    }
}

bool Vertex::operator==(const Vertex& other) const
{
    return id == other.id;
}

bool Vertex::operator>(const Vertex& other) const
{
    return id > other.id;
}

bool Vertex::operator<(const Vertex& other) const
{
    return id < other.id;
}

void Vertex::printInfo() const
{
    std::cout << "Vertex ID: " << id << std::endl;
    std::cout << "Degree: " << degree << std::endl;
    std::cout << "Neighbors: ";
    for (auto neighbor : neighbors)
    {
        std::cout << neighbor << " ";
    }
    std::cout << std::endl;
    std::cout << PRINT_SEPARATOR << std::endl;
}