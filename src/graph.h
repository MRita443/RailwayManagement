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

    [[nodiscard]] unsigned int findVertexIdx(const std::string &id) const;

    bool path(const std::string &source, const std::string &target);

    [[nodiscard]] double findBottleneck(const std::string &source, const std::string &target) const;

    void augmentPath(const std::string &source, const std::string &target, const double &value) const;

    [[nodiscard]] std::vector<Vertex *> findEndOfLines(const std::string &stationId) const;

public:
    [[nodiscard]] Vertex *findVertex(const std::string &id) const;

    bool addVertex(const std::string &id);

    [[nodiscard]] bool addBidirectionalEdge(const std::string &source, const std::string &dest, double c, Service service) const;

    [[nodiscard]] unsigned int getNumVertex() const;

    [[nodiscard]] std::vector<Vertex *> getVertexSet() const;

    void edmondsKarp(const std::string &source, const std::string &target);
};


#endif //RAILWAYMANAGEMENT_GRAPH_H
