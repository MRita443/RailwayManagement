//
// Created by rita on 28-02-2023.
//

#ifndef RAILWAYMANAGEMENT_GRAPH_H
#define RAILWAYMANAGEMENT_GRAPH_H


#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <tuple>
#include <list>
#include <algorithm>

#include "vertex.h"
#include "station.h"

class Graph {
protected:
    unsigned int numEdges;

    std::vector<Vertex *> vertexSet;    // vertex set

    [[nodiscard]] unsigned int findVertexIdx(const std::string &id) const;

    bool path(const std::list<std::string> &source, const std::string &target);

    void visitedDFS(Vertex *source);

    [[nodiscard]] double findBottleneck(const std::string &target) const;

    void augmentPath(const std::string &target, const unsigned int &value) const;

    [[nodiscard]] std::list<std::string> findEndOfLines(const std::string& stationId) const;

public:
    [[nodiscard]] Vertex *findVertex(const std::string &id) const;

    bool addVertex(const std::string &id);

    [[nodiscard]] bool addBidirectionalEdge(const std::string &source, const std::string &dest, double c, Service service);

    [[nodiscard]] unsigned int getNumVertex() const;

    [[nodiscard]] std::vector<Vertex *> getVertexSet() const;
    
    unsigned int edmondsKarp(const std::list<std::string> &source, const std::string &target);

    unsigned int incomingFlux(const std::string &station);

    std::pair<std::list<std::pair<std::string,std::string>>,unsigned int> calculateNetworkMaxFlow();

    unsigned int getNumEdges();
};


#endif //RAILWAYMANAGEMENT_GRAPH_H
