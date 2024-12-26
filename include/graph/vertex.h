#pragma once

#include <vector>
#include <algorithm>
#include <iostream>
#include <string>

#include "../configuration/types.h"
#include "../configuration/config.h"

class Vertex
{
    private:
        VertexID id;
        uint degree;
        std::vector<VertexID> neighbors;

    public:
        Vertex();
        Vertex(VertexID vid);
        Vertex(const Vertex& other);
        ~Vertex();

        VertexID getVid() const;
        uint getDegree() const;
        const std::vector<VertexID>& getNeighbors() const;

        bool hasNeighbor(VertexID neighbor_vid) const;

        void addNeighbor(VertexID neighbor_vid);
        void removeNeighbor(VertexID neighbor_vid);
        void printInfo() const;

        bool operator==(const Vertex& other) const;
        bool operator>(const Vertex& other) const;
        bool operator<(const Vertex& other) const;
};