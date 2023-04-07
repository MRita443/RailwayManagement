//
// Created by rita on 28-02-2023.
//

#include "graph.h"

/*
unsigned int Graph::getNumVertex() const {
    return vertexSet.size();
}
*/
/*
std::vector<Vertex *> Graph::getVertexSet() const {
    return vertexSet;
}
 */

unsigned int Graph::getNumEdges() const{
    return numEdges;
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
/*
unsigned int Graph::findVertexIdx(const std::string &id) const {
    for (unsigned i = 0; i < vertexSet.size(); i++)
        if (vertexSet[i]->getId() == id)
            return i;
    return -1;
}
*/

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
bool Graph::addBidirectionalEdge(const std::string &source, const std::string &dest, double c, Service service) {
    auto v1 = findVertex(source);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;

    //Sets each Edge and its reverse to share flow attribute
    unsigned int sharedFlow = 0;

    auto e1 = v1->addEdge(v2, c, service);
    auto e2 = v2->addEdge(v1, c, service);
    e1->setReverse(e2);
    e2->setReverse(e1);
    e1->setFlow(&sharedFlow);
    e2->setFlow(&sharedFlow);

    numEdges++;
    return true;
}

/**
 * Single-source or Multi-source Edmonds-Karp algorithm to find the the network's max flow
 * Time Complexity: O(|VE²|)
 * @param source - List of ids of the source Vertex(es)
 * @param target - Id of the target Vertex
 */
unsigned int Graph::edmondsKarp(const std::list<std::string> &source, const std::string &target) {
    unsigned int maxFlow = 0;
    for (Vertex *v: vertexSet) {
        for (Edge *e: v->getAdj()) {
            e->setFlowValue(0);
        }
    }

    while (path(source, target)) {
        augmentPath(target, findBottleneck(target));
    }

    for(Edge* edge : findVertex(target)->getIncoming()){
        unsigned int *currentFlow = edge->getFlow();
        maxFlow += *currentFlow;
    }

    return maxFlow;
}

/**
 * Adapted BFS (to use residual graph edges) that checks if there is a valid path connecting the source and target vertices
 * Time Complexity: O(|E|)
 * @param source - List of ids of the source Vertex(es)
 * @param target - Id of the target Vertex
 * @return True if a path was found, false if not
 */
bool Graph::path(const std::list<std::string> &source, const std::string &target) {

    for (Vertex *v: vertexSet) {
        v->setVisited(false);
        v->setPath(nullptr);
    }

    std::queue<std::string> q;
    for (const auto & it : source) {
        q.push(it);
        findVertex(it)->setVisited(true);
    }

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
unsigned int Graph::findBottleneck(const std::string &target) const {
    Vertex *currentVertex = findVertex(target);
    unsigned int currBottleneck;
    unsigned int bottleneck = INF;

    while (currentVertex->getPath() != nullptr) {

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

        if (currBottleneck < bottleneck) {
            bottleneck = currBottleneck;
        }
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
void Graph::augmentPath(const std::string &target, const unsigned int &value) const {
    Vertex *currentVertex = findVertex(target);

    while (currentVertex->getPath() != nullptr) {
        Edge *currentPath = currentVertex->getPath();

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
//change to return void

/**
 * Takes a number and sets the bool "selected" of that amount of edges and the corresponding reverses to false
 * Time Complexity: O(1)
 * @param numEdges - Number of edges to be deactivated
 * @return A vector of pointers for all the edges that were deactivated
 */
std::vector<Edge*> Graph::deactivateEdges(int numEdges) {
    int stationNum;
    Vertex* currentVertex;
    int choice;
    std::vector<Edge*> deactivatedEdges = {};
    while(deactivatedEdges.size() < numEdges){
        stationNum = rand() % vertexSet.size();
        currentVertex = vertexSet[stationNum];
        choice = rand() % currentVertex->getAdj().size();
        if(currentVertex->getAdj()[choice]->getCorrespondingEdge()->isSelected()){
           currentVertex->getAdj()[choice]->getCorrespondingEdge()->setSelected(false);
           currentVertex->getAdj()[choice]->getReverse()->getCorrespondingEdge()->setSelected(false);
           deactivatedEdges.push_back(currentVertex->getAdj()[choice]->getCorrespondingEdge());
           }
    }
    return deactivatedEdges;
}

/**
 * Takes a vector of edge pointers and sets the selected state of those edges and the corresponding reverses to false
 * Time Complexity: O(size(edges))
 * @param edges - Vector of edge pointers to be deactivated
 * @return A vector of pointers for all edges that were deactivated
 */
std::vector<Edge*> Graph::deactivateEdges(std::vector<Edge *> edges) {
    for(Edge* edge : edges){
        edge->setSelected(false);
        edge->getReverse()->setSelected(false);
    }
    return edges;
}

/**
 * Takes a vector of edge pointers and sets the selected state of those edges and the corresponding reverses to true
 * Time Complexity: O(size(edges))
 * @param edges - Vector of edge pointers to be activated
 */
void Graph::activateEdges(std::vector<Edge *> edges) {
    for(Edge* edge : edges){
        edge->setSelected(true);
        edge->getReverse()->setSelected(true);
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
unsigned int Graph::maxFlowDeactivatedEdgesRandom(const int &numEdges, const std::list<std::string> &source, const std::string &target) {
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
unsigned int Graph::maxFlowDeactivatedEdgesSelected(std::vector<Edge*> selectedEdges, const std::list<std::string> &source, const std::string &target) {
    std::vector<Edge*> deactivatedEdges = deactivateEdges(selectedEdges);
    unsigned int maxFlowInterrupted = edmondsKarp(source, target);
    activateEdges(deactivatedEdges);
    return maxFlowInterrupted;
}

/**
* Finds the stations that are at the end of the indicated station's line (i.e have only connection to one other station)
 * Time Complexity: O(|V+E|)
 * @param stationId - Id of the starting station
*/
std::list<std::string> Graph::findEndOfLines(const std::string& stationId) const {
    std::list<std::string> eol_stations;
    std::queue<Vertex *> q;

    for (Vertex *v: vertexSet) v->setVisited(false);
    q.push(findVertex(stationId));

    while (!q.empty()) {
        Vertex *curr = q.front();
        q.pop();
        curr->setVisited(true);
        if (curr->getAdj().size() == 1) eol_stations.push_back(curr->getId());
        for (Edge *e : curr->getAdj()){
            if(!e->getDest()->isVisited()){
                q.push(e->getDest());
            }
        }
    }
    return eol_stations;
}

/**
 * Calculates the max flow of a station normally, and once again with certain edges deactivated, and returns a pair with the name of the station and the difference between normal flow and reduced flow
 * Time Complexity: O(|VE²|)
 * @param vertexID - ID of the station to have it's flow measured
 * @param edges - Edges to be deactivated
 * @return A Pair with the name of the station first, and a pair with the absolute and relative difference second
 */
std::pair<std::string, std::pair<unsigned int, unsigned int>> Graph::maxFlowDifference(std::string vertexID, std::vector<Edge*> edges){
    std::pair<std::string, std::pair<unsigned int, unsigned int>> stationResults;
    std::list<std::string> superSource = superSourceCreator(vertexID);
    unsigned int baseFlow = edmondsKarp(superSource, vertexID);
    unsigned int reducedFlow = maxFlowDeactivatedEdgesSelected(edges, superSource, vertexID);
    stationResults = {vertexID, {baseFlow, reducedFlow}};
    return stationResults;
}

/**
 * Creates a list with every vertex except the one selected in the function
 * Time Complexity: O(V)
 * @param vertexId - Vertex to be excluded from the list
 * @return A list with every vertex except the selected one
 */
std::list<std::string> Graph::superSourceCreator(std::string vertexId) {
    std::list<std::string> superSource = {};
    for(Vertex* vertex : vertexSet){
        if(vertexId != vertex->getId()){superSource.push_back(vertex->getId());}
    }
    return superSource;
}
/*
 * DFS traversal variation that sets the visited attribute to true of the nodes the DFS traverses to
 * Time Complexity: O(|V|+|E|)
 * @param source - Vertex where the DFS starts
 */
void Graph::visitedDFS(Vertex *source) {
    source->setVisited(true);
    for (Edge *e : source->getAdj()){
        if (!e->getDest()->isVisited()){
            visitedDFS(e->getDest());
        }
    }
}

/**
 * Finds the pairs of stations with max Max-Flow
 * Time Complexity: O(|V^3|*|E^2|)
 * @return
 */
std::pair<std::list<std::pair<std::string,std::string>>,unsigned int> Graph::calculateNetworkMaxFlow() {
    unsigned int max = -1;
    std::list<std::pair<std::string,std::string>> stationList;
    for (auto itV1 = vertexSet.begin(); itV1 < vertexSet.end(); itV1++){
        for (auto itV2 = (itV1+1); itV2 < vertexSet.end(); itV2++){
            Vertex * v1 = *itV1;
            Vertex * v2 = *itV2;
            for (Vertex *aux : vertexSet) aux->setVisited(false);
            visitedDFS(v1);
            if (v2->isVisited()){
                unsigned int itFlow = edmondsKarp({v1->getId()}, v2->getId());
                if (itFlow == max) stationList.push_back({v1->getId(),v2->getId()});
                if (itFlow > max) {
                    max = itFlow;
                    stationList = {{v1->getId(),v2->getId()}};
                }
            }
        }
    }
    return {stationList, max};
}

/**
 * Finds the incoming flux that a certain station can receive (i.e the amount of trains that can arrive there at the same time)
 * Time Complexity: O(|VE^2|)
 * @param station - Station's ID
 */
unsigned int Graph::incomingFlux(const std::string &station) {
    std::list<std::string> super_source = findEndOfLines(station);
    for (auto it = super_source.begin(); it != super_source.end(); it++)
        if (*it == station) {
            super_source.erase(it);
            break;
        }

    return edmondsKarp(findEndOfLines(station),station);
}

/*
bool Graph::minCostPath(const std::string &source, const std::string &target) {

    for (Vertex *v: vertexSet) {
        v->setVisited(false);
        v->setPath(nullptr);
        v->setCost(std::numeric_limits<double>::max());
    }

    Vertex* sourceVertex = findVertex(source);

    std::priority_queue<std::pair<double, Vertex *>, std::vector<std::pair<double, Vertex *>>, std::greater<>> pq;
    sourceVertex->setVisited(true);
    sourceVertex->setCost(0);
    pq.push(std::make_pair(0, sourceVertex));

    while (!pq.empty()) {
        Vertex *currentVertex = pq.top().second;
        pq.pop();

        if (currentVertex == findVertex(target)) {
            return true;
        }

        for (Edge *e: currentVertex->getAdj()) {
            if (!e->getDest()->isVisited() && *e->getFlow() < e->getCapacity() && e->isSelected()) {
                double newCost = currentVertex->getCost() + e->getCost();
                if (newCost < e->getDest()->getCost()) {
                    e->getDest()->setVisited(true);
                    e->getDest()->setCost(newCost);
                    e->getDest()->setPath(e);
                    pq.push(std::make_pair(newCost, e->getDest()));
                }
            }
        }

        for (Edge *e: currentVertex->getIncoming()) {
            if (!e->getOrig()->isVisited() && *e->getFlow() > 0 && e->isSelected()) {
                double newCost = currentVertex->getCost() - e->getCost();
                if (newCost < e->getOrig()->getCost()) {
                    e->getOrig()->setVisited(true);
                    e->getOrig()->setCost(newCost);
                    e->getOrig()->setPath(e);
                    pq.push(std::make_pair(newCost, e->getOrig()));
                }
            }
        }
    }

    return false;
}*/
