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
    unsigned int totalEdges = 0;
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

    std::vector<Edge*> deactivateEdges(unsigned int numEdges);

    static std::vector<Edge*> deactivateEdges(std::vector<Edge*> Edges);

    static void activateEdges(const std::vector<Edge*>& Edges);

    unsigned int maxFlowDeactivatedEdgesRandom(const int &numEdges, const std::list<std::string> &source, const std::string &target, Graph &residualGraph);

    std::pair<std::string, std::pair<unsigned int, unsigned int>> maxFlowDifference(const std::string& vertexID, const std::vector<Edge*>& edges, Graph &residualGraph);

    std::list<std::string> superSourceCreator(const std::string& vertexId);

    [[nodiscard]] unsigned int incomingFlux(const std::string &station, Graph &residualGraph);

    unsigned int edmondsKarp(const std::list<std::string> &source, const std::string &target, Graph &residualGraph);

    std::pair<std::list<std::pair<std::string, std::string>>, unsigned int> calculateNetworkMaxFlow(Graph &residualGraph);

    [[nodiscard]] unsigned int getTotalEdges() const;
    
    static Edge *getCorrespondingEdge(const Edge *e, const Graph &graph);

    void augmentPath(const std::string &target, const unsigned int &value) const;

    bool path(const std::list<std::string> &source, const std::string &target) const;

    std::pair<Edge *, Edge *>
    addAndGetBidirectionalEdge(const std::string &source, const std::string &dest, unsigned int c, Service service);

    std::pair<unsigned int, unsigned int>
    
    minCostMaxFlow(const std::string &source, const std::string &target, Graph &residualGraph);

    static unsigned int findListBottleneck(const std::list<Edge *>& edges) ;

    void makeMinCostResidual(Graph &minCostResidual);

    static void augmentMinCostPath(const std::list<Edge *>& edges, const unsigned int &value) ;

    std::vector<std::pair<std::string, double>>
    topGroupings(const std::unordered_map<std::string, std::list<Station>> &group, Graph &residualGraph);
    
    double getAverageIncomingFlux(const std::list<Station>& stations, Graph &residualGraph);

    std::list<Edge *> bellmanFord(const std::string &source);

    void visitedDFS(Vertex *source);

    [[nodiscard]] unsigned int findBottleneck(const std::string &target) const;

    [[nodiscard]] std::list<std::string> findEndOfLines(const std::string& stationId) const;

    unsigned int
    maxFlowDeactivatedEdgesSelected(const std::vector<Edge *> &selectedEdges, const std::list<std::string> &source,
                                    const std::string &target, Graph &residualGraph);
};


#endif //RAILWAYMANAGEMENT_GRAPH_H
