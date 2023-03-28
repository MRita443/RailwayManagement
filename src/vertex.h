//
// Created by rita on 12-03-2023.
//

#ifndef RAILWAYMANAGEMENT_VERTEX_H
#define RAILWAYMANAGEMENT_VERTEX_H

class Edge;

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include "edge.h"

#define INF std::numeric_limits<double>::max()

class Vertex {
public:
    explicit Vertex(std::string id);

    bool operator<(Vertex &vertex) const; // // required by MutablePriorityQueue

    [[nodiscard]] std::string getId() const;

    [[nodiscard]] std::vector<Edge *> getAdj() const;

    [[nodiscard]] bool isVisited() const;

    [[nodiscard]] bool isProcessing() const;

    [[nodiscard]] unsigned int getIndegree() const;

    [[nodiscard]] double getDist() const;

    [[nodiscard]] Edge *getPath() const;

    [[nodiscard]] std::vector<Edge *> getIncoming() const;

    void setId(std::string info);

    void setVisited(bool visited);

    void setProcesssing(bool processing);

    void setIndegree(unsigned int indegree);

    void setDist(double dist);

    void setPath(Edge *path);

    Edge *addEdge(Vertex *dest, double w, Service s);

    bool removeEdge(const std::string& destID);

    //friend class MutablePriorityQueue<Vertex>;

protected:
    std::string id;                // identifier
    std::vector<Edge *> adj;  // outgoing edges

    // auxiliary fields
    bool visited = false; // used by DFS, BFS, Prim ...
    bool processing = false; // used by isDAG (in addition to the visited attribute)
    unsigned int indegree; // used by topsort
    double dist = 0;
    Edge *path = nullptr;

    std::vector<Edge *> incoming; // incoming edges

    //int queueIndex = 0;        // required by MutablePriorityQueue and UFDS
};


#endif //RAILWAYMANAGEMENT_VERTEX_H
