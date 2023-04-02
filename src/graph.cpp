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
Vertex *Graph::findVertex(const std::string &id) const {
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
unsigned int Graph::findVertexIdx(const std::string &id) const {
    for (unsigned i = 0; i < vertexSet.size(); i++)
        if (vertexSet[i]->getId() == id)
            return i;
    return -1;
}

/**
 * Adds a vertex with a given id to the Graph, representing a given station
 * Time Complexity: O(|V|)
 * @param id - Id of the Vertex to add
 * @return True if successful, and false if a vertex with the given id already exists
 */
bool Graph::addVertex(const std::string &id) {
    if (findVertex(id) != nullptr)
        return false;
    vertexSet.push_back(new Vertex(id));
    return true;
}


/**
 * Adds a bidirectional edge to the Graph between the vertices with id source and dest, with a capacity of c, representing a Service s
 * Time Complexity: O(|V|)
 * @param source - Id of the source Vertex
 * @param dest - Id of the destination Vertex
 * @param c - Capacity of the Edge to be added
 * @param service - Service of the Edge to be added
 * @return True if successful, and false if the source or destination vertices do not exist
 */
bool Graph::addBidirectionalEdge(const std::string &source, const std::string &dest, double c, Service service) const {
    auto v1 = findVertex(source);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;

    //Sets each Edge and its reverse to share flow attribute
    double sharedFlow = 0;

    auto e1 = v1->addEdge(v2, c, service);
    auto e2 = v2->addEdge(v1, c, service);
    e1->setReverse(e2);
    e2->setReverse(e1);
    e1->setFlow(&sharedFlow);
    e2->setFlow(&sharedFlow);
    return true;
}

/**
 * Standard Edmonds-Karp algorithm to find the the network's max flow
 * Time Complexity: O(|VE²|)
 * @param source - Id of the source Vertex
 * @param target - Id of the target Vertex
 */
void Graph::edmondsKarp(const std::string &source, const std::string &target) {
    for (Vertex *v: vertexSet) {
        for (Edge *e: v->getAdj()) {
            e->setFlow(0);
        }
    }

    while (path(source, target)) {
        augmentPath(source, target, findBottleneck(source, target));
    }
}

/**
 * Adapted BFS (to use residual graph edges) that checks if there is a valid path connecting the source and target vertices
 * Time Complexity: O(|E|)
 * @param source - Id of the source Vertex
 * @param target - Id of the target Vertex
 * @return True if a path was found, false if not
 */
bool Graph::path(const std::string &source, const std::string &target) {

    for (Vertex *v: vertexSet) {
        v->setVisited(false);
        v->setPath(nullptr);
    }

    std::queue<std::string> q;
    q.push(source);
    findVertex(source)->setVisited(true);

    while (!q.empty()) {
        Vertex *currentVertex = findVertex(q.front());
        q.pop();


        for (Edge *e: currentVertex->getAdj()) {
            if (!e->getDest()->isVisited() && *e->getFlow() < e->getCapacity() && e->isSelected()) {
                q.push(e->getDest()->getId());
                e->getDest()->setVisited(true);
                e->getDest()->setPath(e);
                if (e->getDest()->getId() == target) return true;
            }
        }

        for (Edge *e: currentVertex->getIncoming()) {
            if (!e->getOrig()->isVisited() && *e->getFlow() > 0 && e->isSelected()) {
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
double Graph::findBottleneck(const std::string &source, const std::string &target) const {
    Vertex *currentVertex = findVertex(target);
    double currBottleneck;
    double bottleneck = INF;

    while (currentVertex->getId() != source) {

        if (currentVertex->getPath()->getDest() == currentVertex) //Regular Edge
        {
            currBottleneck = currentVertex->getPath()->getCapacity() -
                             *currentVertex->getPath()->getFlow();
            currentVertex = currentVertex->getPath()->getOrig();
        } else //Residual Edge (was traversed backwards)
        {
            currBottleneck = *currentVertex->getPath()->getFlow();
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
void Graph::augmentPath(const std::string &source, const std::string &target, const double &value) const {
    Vertex *currentVertex = findVertex(target);

    while (currentVertex->getId() != source) {
        Edge *currentPath = currentVertex->getPath();

        if (currentPath != nullptr) {

            if (currentPath->getDest() == currentVertex) //Regular Edge
            {
                currentPath->setFlowValue(*(currentPath->getFlow()) + value);
                currentVertex = currentVertex->getPath()->getOrig();

            } else { //Residual Edge (was traversed backwards)
                currentPath->setFlowValue(*(currentPath->getFlow()) - value);
                currentVertex = currentVertex->getPath()->getDest();
            }
        }
    }
}


/**
* Finds the stations that are at the end of the indicated station's line (i.e have only connection to one other station)
 * Time Complexity: O(|V|+|E|)
 * @param stationId - Id of the starting station
*/
std::vector<Vertex *> Graph::findEndOfLines(const std::string stationId) const {
    std::vector<Vertex *> eol_stations;
    std::queue<Vertex *> q;

    for (Vertex * v: vertexSet) v->setVisited(false);
    q.push(findVertex(stationId));

    while(!q.empty()){
        Vertex *curr = q.front();
        q.pop();
        curr->setVisited(true);
        if (curr->getAdj().size() == 1) eol_stations.push_back(curr);
        for (Edge *e : curr->getAdj()){
            if(!e->getDest()->isVisited()){
                q.push(e->getDest());
            }
        }
    }

    return eol_stations;
}

std::pair<std::list<std::pair<Vertex *,Vertex *>>,unsigned int> Graph::calculateNetworkMaxFlow() {
    unsigned int max = -1;
    std::list<std::pair<Vertex *,Vertex *>> stationList;
    for (auto itV1 = vertexSet.begin(); itV1 < vertexSet.end(); itV1++){
        for (auto itV2 = itV1; itV2 < vertexSet.end(); itV2++){
            Vertex * v1 = *itV1++;
            Vertex * v2 = *itV2;
            if (path(v1->getId(), v2->getId() )){
                unsigned int itFlow = edmondsKarp(v1->getId(), v2->getId());
                if (itFlow == max) stationList.push_back({v1,v2});
                if (itFlow > max) {
                    max = itFlow;
                    stationList = {{v1, v2}};
                }
            }
        }
    }
    return {stationList, max};
}
