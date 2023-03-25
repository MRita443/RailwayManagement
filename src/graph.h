//
// Created by rita on 28-02-2023.
//

#ifndef RAILWAYMANAGEMENT_GRAPH_H
#define RAILWAYMANAGEMENT_GRAPH_H


#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>

#include "vertex.h"
#include "station.h"

class Graph {
protected:
    std::vector<Vertex *> vertexSet;    // vertex set
    StationMap<int> stationToVertex; //Maps each station to the id of the vertex representing it

    [[nodiscard]] unsigned int findVertexIdx(const int &id) const;

    bool path(const int &source, const int &target);

    [[nodiscard]] double findBottleneck(const int &source, const int &target) const;

    void augmentPath(const int &source, const int &target, const double &value) const;

public:
    [[nodiscard]] Vertex *findVertex(const int &id) const;

    bool addVertex(const int &id);

    [[nodiscard]] bool addBidirectionalEdge(const int &source, const int &dest, double c) const;

    [[nodiscard]] unsigned int getNumVertex() const;

    [[nodiscard]] std::vector<Vertex *> getVertexSet() const;

    void edmondsKarp(const int &source, const int &target);
};


#endif //RAILWAYMANAGEMENT_GRAPH_H
