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

/**
 * Finds the vertex with a given id
 * Time Complexity: O(|V|)
 * @param id - Id of the vertex to be found
 * @return Pointer to the found Vertex, or nullptr if none was found
 */
Vertex *Graph::findVertex(const int &id) const {
    for (auto v: vertexSet)
        if (v->getId() == id)
            return v;
    return nullptr;
}

/**
 * Finds the index of the vertex with a given id
 * Time Complexity: O(|V|)
 * @param id - Id of the vertex to be found
 * @return Index of the found Vertex, or -1 if none was found
 */
unsigned int Graph::findVertexIdx(const int &id) const {
    for (unsigned i = 0; i < vertexSet.size(); i++)
        if (vertexSet[i]->getId() == id)
            return i;
    return -1;
}

/**
 * Adds a vertex with a given id to the Graph
 * Time Complexity: O(|V|)
 * @param id - Id of the vertex to be added
 * @return True if successful, and false if a vertex with the given id already exists
 */
bool Graph::addVertex(const int &id) {
    if (findVertex(id) != nullptr)
        return false;
    vertexSet.push_back(new Vertex(id));
    return true;
}

/**
 * Adds a unidirectional edge to the Graph between the vertices with id source and dest, with a capacity of c.
 * Time Complexity: O(|V|)
 * @param source - Id of the source Vertex
 * @param dest - Id of the destination Vertex
 * @param c - Capacity of the Edge to be added
 * @return True if successful, and false if the source or destination vertices do not exist
 */
bool Graph::addEdge(const int &source, const int &dest, double c) const {
    auto v1 = findVertex(source);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    v1->addEdge(v2, c);
    return true;
}

/**
 * Adds a bidirectional edge to the Graph between the vertices with id source and dest, with a capacity of c.
 * Time Complexity: O(|V|)
 * @param source - Id of the source Vertex
 * @param dest - Id of the destination Vertex
 * @param c - Capacity of the Edge to be added
 * @return True if successful, and false if the source or destination vertices do not exist
 */
bool Graph::addBidirectionalEdge(const int &source, const int &dest, double c) const {
    auto v1 = findVertex(source);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    auto e1 = v1->addEdge(v2, c);
    auto e2 = v2->addEdge(v1, c);
    e1->setReverse(e2);
    e2->setReverse(e1);
    return true;
}

/**
 * Standard Edmonds-Karp algorithm to find the the network's max flow
 * Time Complexity: O(|VE²|)
 * @param source - Id of the source Vertex
 * @param target - Id of the target Vertex
 */
unsigned int Graph::edmondsKarp(const int &source, const int &target) {
    int maxFlow = 0;
    for (Vertex *v: vertexSet) {
        for (Edge *e: v->getAdj()) {
            e->setFlow(0);
        }
    }

    while (path(source, target)) {
        augmentPath(source, target, findBottleneck(source, target));
    }

    for(Edge* edge : vertexSet[target]->getIncoming()){
        maxFlow += edge->getFlow();
    }

    return maxFlow;
}

/**
 * Adapted BFS (to use residual graph edges) that checks if there is a valid path connecting the source and target vertices
 * Time Complexity: O(|E|)
 * @param source - Id of the source Vertex
 * @param target - Id of the target Vertex
 * @return True if a path was found, false if not
 */
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

/**
 * Finds the minimum available flow value in the path connecting source and target vertices
 * Time Complexity: O(|E|)
 * @param source - Id of the source Vertex
 * @param target - Id of the target Vertex
 * @return Bottleneck of the path connecting source to target
 */
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

/**
 * Augments the flow in the path connecting source to target by value units
 * Time Complexity: O(|E|)
 * @param source - Id of the source Vertex
 * @param target - Id of the target Vertex
 * @param value - Number of units to augment the flow by
 */
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

//change to return void

/**
 * Takes a number and sets the bool "selected" of that amount of edges to false
 * Time Complexity: O(VE)
 * @param numEdges - Number of edges to be deactivated
 * @return A vector of pointers for all the edges that were deactivated
 */
std::vector<Edge*> Graph::deactivateEdges(int numEdges) {
    int stationNum = 0;
    Vertex* currentVertex;
    int choice = 0;
    std::vector<Edge*> deactivatedEdges = {};
    while(deactivatedEdges.size() < numEdges){
        stationNum = rand() % vertexSet.size();
        currentVertex = findVertex(stationNum);
        choice = rand() % currentVertex->getAdj().size();
        if(currentVertex->getAdj()[choice]->isSelected()){
           currentVertex->getAdj()[choice]->setSelected(false);
           deactivatedEdges.push_back(currentVertex->getAdj()[choice]);
        }
    }
    return deactivatedEdges;
}

/**
 * Takes a vector of edge pointers and sets the selected state of those edges to false
 * Time Complexity: O(N)
 * @param edges - Vector of edge pointers to be deactivated
 * @return A vector of pointers for all edges that were deactivated
 */
std::vector<Edge*> Graph::deactivateEdges(std::vector<Edge *> edges) {
    for(Edge* edge : edges){
        edge->setSelected(false);
    }
    return edges;
}

/**
 * Takes a vector of edge pointers and sets the selected state of those edges to true
 * Time Complexity: O(N)
 * @param edges - Vector of edge pointers to be activated
 */
void Graph::activateEdges(std::vector<Edge *> edges) {
    for(Edge* edge : edges){
        edge->setSelected(true);
    }
}

/**
 * Calculates the maximum flow between a source vertex and a target vertex with (numEdges) number of random edges being deactivated and reactivated after calculating the maximum flow
 * Time Complexity: O(|VE²|)
 * @param numEdges - Number of edges to be deactivated and later reactivated
 * @param source - Id of the source Vertex
 * @param target - Id of the target Vertex
 * @return The value of the Max Flow with the interrupted lines
 */
unsigned int Graph::maxFlowDeactivatedEdgesRandom(const int &numEdges, const int &source, const int &target) {
    std::vector<Edge*> deactivatedEdges = deactivateEdges(numEdges);
    unsigned int maxFlowInterrupted = edmondsKarp(source, target);
    activateEdges(deactivatedEdges);
    return maxFlowInterrupted;
}

/**
 *Calculates the maximum flow between a source vertex and a target vertex with the edges inputted to the function being deactivated and reactivated after calculating the maximum flow
 * Time Complexity: O(|VE²|)
 * @param selectedEdges - Vector of edges to be deactivated and later reactivated
 * @param source - Id of the source Vertex
 * @param target - Id of the target Vertex
 * @return The value of the Max Flow with the interrupted lines
 */
unsigned int Graph::maxFlowDeactivatedEdgesSelected(std::vector<Edge*> selectedEdges, const int &source, const int &target) {
    std::vector<Edge*> deactivatedEdges = deactivateEdges(selectedEdges);
    unsigned int maxFlowInterrupted = edmondsKarp(source, target);
    activateEdges(deactivatedEdges);
    return maxFlowInterrupted;
}

