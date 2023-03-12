//
// Created by rita on 28-02-2023.
//

#include "graph.h"

unsigned int Graph::getNumVertex() const {
    return vertexSet.size();
}

std::vector<Vertex *> Graph::getVertexSet() const {
    return vertexSet;
}

/*
 * Auxiliary function to find a vertex with a given content.
 */
Vertex *Graph::findVertex(const int &id) const {
    for (auto v: vertexSet)
        if (v->getId() == id)
            return v;
    return nullptr;
}

/*
 * Finds the index of the vertex with a given content.
 */
unsigned int Graph::findVertexIdx(const int &id) const {
    for (unsigned i = 0; i < vertexSet.size(); i++)
        if (vertexSet[i]->getId() == id)
            return i;
    return -1;
}

/*
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
bool Graph::addVertex(const int &id) {
    if (findVertex(id) != nullptr)
        return false;
    vertexSet.push_back(new Vertex(id));
    return true;
}

/*
 * Adds an edge to a graph (this), given the contents of the source and
 * destination vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
bool Graph::addEdge(const int &source, const int &dest, double w) const {
    auto v1 = findVertex(source);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    v1->addEdge(v2, w);
    return true;
}

bool Graph::addBidirectionalEdge(const int &source, const int &dest, double w) const {
    auto v1 = findVertex(source);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    auto e1 = v1->addEdge(v2, w);
    auto e2 = v2->addEdge(v1, w);
    e1->setReverse(e2);
    e2->setReverse(e1);
    return true;
}

void Graph::edmondsKarp(const int &source, const int &target) {
    for (Vertex *v: vertexSet) {
        for (Edge *e: v->getAdj()) {
            e->setFlow(0);
        }
    }

    while (path(source, target)) {
        augmentPath(source, target, findBottleneck(source, target));
    }
}

bool Graph::path(const int &source, const int &target) {

    for (Vertex *v: vertexSet) {
        v->setVisited(false);
        v->setPath(nullptr);
    }

    std::queue<int> q;
    q.push(source);
    findVertex(source)->setVisited(true);

    while (!q.empty()) {
        Vertex *currentVertex = findVertex(q.front());
        q.pop();


        for (Edge *e: currentVertex->getAdj()) {
            if (!e->getDest()->isVisited() && e->getFlow() < e->getCapacity()) {
                q.push(e->getDest()->getId());
                e->getDest()->setVisited(true);
                e->getDest()->setPath(e);
                if (e->getDest()->getId() == target) return true;
            }
        }

        for (Edge *e: currentVertex->getIncoming()) {
            if (!e->getOrig()->isVisited() && e->getFlow() > 0) {
                q.push(e->getOrig()->getId());
                e->getOrig()->setVisited(true);
                e->getOrig()->setPath(e);
                if (e->getOrig()->getId() == target) return true;
            }
        }
    }

    return false;
}

double Graph::findBottleneck(const int &source, const int &target) const {
    Vertex *currentVertex = findVertex(target);
    double currBottleneck;
    double bottleneck = INF;

    while (currentVertex->getId() != source) {

        if (currentVertex->getPath()->getDest() == currentVertex) //Regular Edge
        {
            currBottleneck = currentVertex->getPath()->getCapacity() -
                             currentVertex->getPath()->getFlow();
            currentVertex = currentVertex->getPath()->getOrig();
        } else //Residual Edge (was traversed backwards)
        {
            currBottleneck = currentVertex->getPath()->getFlow();
            currentVertex = currentVertex->getPath()->getDest();
        }

        if (currBottleneck < bottleneck)
            bottleneck = currBottleneck;

    }
    return bottleneck;
}

void Graph::augmentPath(const int &source, const int &target, const double &value) const {
    Vertex *currentVertex = findVertex(target);

    while (currentVertex->getId() != source) {
        Edge *currentPath = currentVertex->getPath();

        if (currentPath != nullptr) {

            if (currentPath->getDest() == currentVertex) //Regular Edge
            {
                currentPath->setFlow(currentPath->getFlow() + value);
                currentVertex = currentVertex->getPath()->getOrig();

            } else { //Residual Edge (was traversed backwards)
                currentPath->setFlow(currentPath->getFlow() - value);
                currentVertex = currentVertex->getPath()->getDest();
            }
        }
    }
}