//
// Created by rita on 12-03-2023.
//

#ifndef RAILWAYMANAGEMENT_VERTEX_H
#define RAILWAYMANAGEMENT_VERTEX_H

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include "edge.h"

#define INF std::numeric_limits<double>::max()

class Edge;
enum class Service: unsigned int;

class Vertex {
public:
    explicit Vertex(std::string id);

    [[nodiscard]] std::string getId() const;

    [[nodiscard]] std::vector<Edge *> getAdj() const;

    [[nodiscard]] bool isVisited() const;

    [[nodiscard]] bool isProcessing() const;

    [[nodiscard]] unsigned int getIndegree() const;

    [[nodiscard]] int getCost() const;

    [[nodiscard]] Edge *getPath() const;

    [[nodiscard]] std::vector<Edge *> getIncoming() const;

    void setId(std::string info);

    void setVisited(bool visited);

    void setProcesssing(bool processing);

    void setIndegree(unsigned int indegree);

    void setCost(int dist);

    void setPath(Edge *path);

    Edge *addEdge(Vertex *dest, unsigned int w, Service s);

    bool removeEdge(const std::string& destID);

private:
    std::string id;                // identifier
    std::vector<Edge *> adj;  // outgoing edges

    // auxiliary fields
    bool visited = false; // used by DFS, BFS, Prim ...
    bool processing = false; // used by isDAG (in addition to the visited attribute)
    unsigned int indegree; // used by topsort
    int cost;
    Edge *path = nullptr;
    std::vector<Edge *> incoming; // incoming edges

};


#endif //RAILWAYMANAGEMENT_VERTEX_H
