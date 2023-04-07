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
  private:
    Graph(Graph &g);

    unsigned int numEdges;
    std::vector<Vertex *> vertexSet;    // vertex set
    std::unordered_map<std::string, Vertex*> idToVertex;

  public:
    Graph();

    [[nodiscard]] Vertex *findVertex(const std::string &id) const;

    bool addVertex(const std::string &id);

    [[nodiscard]] bool
    addBidirectionalEdge(const std::string &source, const std::string &dest, unsigned int c, Service service);

    [[nodiscard]] unsigned int getNumVertex() const;

    [[nodiscard]] std::vector<Vertex *> getVertexSet() const;

    unsigned int incomingFlux(const std::string &station, Graph &residualGraph);

    std::pair<std::list<std::pair<std::string, std::string>>, unsigned int> calculateNetworkMaxFlow(Graph &residualGraph);

    [[nodiscard]] unsigned int getNumEdges() const;

    static Edge *getCorrespondingEdge(const Edge *e, const Graph &graph);

    void augmentPath(const std::string &target, const unsigned int &value, Graph &regularGraph) const;

    [[nodiscard]] unsigned int findBottleneck(const std::string &target) const;

    unsigned int edmondsKarp(const std::list<std::string> &source, const std::string &target, Graph &residualGraph);

    void visitedDFS(Vertex *source);

    [[nodiscard]] std::list<std::string> findEndOfLines(const std::string &stationId) const;

    bool path(const std::list<std::string> &source, const std::string &target) const;

    std::vector<std::pair<Vertex *, Vertex *>> findVerticesWithMaxFlow(Graph &residualGraph);

    std::pair<Edge *, Edge *>
    addAndGetBidirectionalEdge(const std::string &source, const std::string &dest, unsigned int c, Service service);

    bool dijkstraPath(const std::list<std::string> &source, const std::string &target) const;

    bool dijkstraPath(const std::string &source, const std::string &target) const;
};


#endif //RAILWAYMANAGEMENT_GRAPH_H
