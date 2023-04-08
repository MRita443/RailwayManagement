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
#include <list>

#include "vertex.h"
#include "station.h"

class Graph {
  private:
    Graph(Graph &g);

    unsigned int numEdges;
    std::vector<Vertex *> vertexSet;    // vertex set
    std::unordered_map<std::string, Vertex*> idToVertex;

    [[nodiscard]] unsigned int findVertexIdx(const std::string &id) const;

    bool path(const std::list<std::string> &source, const std::string &target);

    void visitedDFS(Vertex *source);

    [[nodiscard]] unsigned int findBottleneck(const std::string &target) const;

    void augmentPath(const std::string &target, const unsigned int &value) const;

    [[nodiscard]] std::list<std::string> findEndOfLines(const std::string& stationId) const;

public:
    Graph();

    [[nodiscard]] Vertex *findVertex(const std::string &id) const;

    bool addVertex(const std::string &id);

    [[nodiscard]] bool
    addBidirectionalEdge(const std::string &source, const std::string &dest, unsigned int c, Service service);

    [[nodiscard]] unsigned int getNumVertex() const;

    [[nodiscard]] std::vector<Vertex *> getVertexSet() const;

    std::vector<Edge*> deactivateEdges(int numEdges);

    std::vector<Edge*> deactivateEdges(std::vector<Edge*> Edges);

    void activateEdges(std::vector<Edge*> Edges);

    unsigned int maxFlowDeactivatedEdgesRandom(const int &numEdges, const std::list<std::string> &source, const std::string &target, Graph &residualGraph);

    unsigned int maxFlowDeactivatedEdgesSelected(std::vector<Edge*> selectedEdges, const std::list<std::string> &source, const std::string &target, Graph &residualGraph);

    std::pair<std::string, std::pair<unsigned int, unsigned int>> maxFlowDifference(std::string vertexID, std::vector<Edge*> edges, Graph &residualGraph);

    std::list<std::string> superSourceCreator(std::string vertexId);

    [[nodiscard]] unsigned int incomingFlux(const std::string &station, Graph &residualGraph);

    unsigned int edmondsKarp(const std::list<std::string> &source, const std::string &target, Graph &residualGraph);

    std::pair<std::list<std::pair<std::string, std::string>>, unsigned int> calculateNetworkMaxFlow(Graph &residualGraph);

    [[nodiscard]] unsigned int getNumEdges() const;
    
    static Edge *getCorrespondingEdge(const Edge *e, const Graph &graph);

    void augmentPath(const std::string &target, const unsigned int &value, Graph &regularGraph) const;

    bool path(const std::list<std::string> &source, const std::string &target) const;

    std::vector<std::pair<Vertex *, Vertex *>> findVerticesWithMaxFlow(Graph &residualGraph);

    std::pair<Edge *, Edge *>
    addAndGetBidirectionalEdge(const std::string &source, const std::string &dest, unsigned int c, Service service);

    std::list<std::pair<std::string, double>> topGroupings(const std::unordered_map<std::string, std::list<Station>> &group, Graph &residualGraph);
    double getAverageIncomingFlux(const std::list<Station>& stations, Graph &residualGraph);
};


#endif //RAILWAYMANAGEMENT_GRAPH_H
