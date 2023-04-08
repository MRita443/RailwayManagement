//
// Created by rita on 28-02-2023.
//

#include "graph.h"

Graph::Graph() = default;

unsigned int Graph::getNumVertex() const {
    return (unsigned int) vertexSet.size();
}

std::vector<Vertex *> Graph::getVertexSet() const {
    return vertexSet;
}

unsigned int Graph::getNumEdges() const {
    return numEdges;
}

/**
 * Finds the vertex with a given id
 * Time Complexity: O(1) (average case) | O(|V|) (worst case)
 * @param id - Id of the vertex to be found
 * @return Pointer to the found Vertex, or nullptr if none was found
 */
Vertex *Graph::findVertex(const std::string &id) const {
    auto it = idToVertex.find(id);
    if (it == idToVertex.end()) return nullptr;
    return it->second;
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
    idToVertex[id] = vertexSet.back();
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
bool Graph::addBidirectionalEdge(const std::string &source, const std::string &dest, unsigned int c, Service service) {
    auto v1 = findVertex(source);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;

    auto e1 = v1->addEdge(v2, c, service);
    auto e2 = v2->addEdge(v1, c, service);
    e1->setReverse(e2);
    e2->setReverse(e1);

    numEdges++;
    return true;
}

/**
 * Adds and returns a bidirectional edge to the Graph between the vertices with id source and dest, with a capacity of c, representing a Service s
 * Time Complexity: O(|V|)
 * @param source - Id of the source Vertex
 * @param dest - Id of the destination Vertex
 * @param c - Capacity of the Edge to be added
 * @param service - Service of the Edge to be added
 * @return Pair containing a pointer to the created Edge and to its reverse
 */
std::pair<Edge *, Edge *>
Graph::addAndGetBidirectionalEdge(const std::string &source, const std::string &dest, unsigned int c, Service service) {
    auto v1 = findVertex(source);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return {nullptr, nullptr};

    auto e1 = v1->addEdge(v2, c, service);
    auto e2 = v2->addEdge(v1, c, service);
    e1->setReverse(e2);
    e2->setReverse(e1);

    numEdges++;
    return {e1, e2};
}

/**
 * Single-source or Multi-source Edmonds-Karp algorithm to find the the network's max flow
 * Time Complexity: O(|VE²|)
 * @param source - List of ids of the source Vertex(es)
 * @param target - Id of the target Vertex
 * @param residualGraph - Graph object representing this Graph's residual network
 * @return unsigned int representing computed value of max flow
 */
unsigned int Graph::edmondsKarp(const std::list<std::string> &source, const std::string &target, Graph &residualGraph) {


    for (Vertex const *v: vertexSet) {
        for (Edge *e: v->getAdj()) {
            e->setFlow(0);
            e->getCorrespondingEdge()->setCapacity(e->getCapacity()); //Reset residual edge capacity
        }
    }

    // Initialize the maximum flow to 0
    unsigned int maxFlow = 0;

    while (residualGraph.path(source, target)) {

        // Find the bottleneck capacity of the path
        unsigned int bottleneckCapacity = residualGraph.findBottleneck(target);

        // Augment the flow by the bottleneck capacity
        residualGraph.augmentPath(target, bottleneckCapacity);

        // Update the maximum flow with the bottleneck capacity
        maxFlow += bottleneckCapacity;
    }
    return maxFlow;
}

/**
 * Adapted BFS that checks if there is a valid path connecting the source and target vertices. Indicated for use on residual graphs
 * Time Complexity: O(|V| + |E|)
 * @param source - List of ids of the source Vertex(es)
 * @param target - Id of the target Vertex
 * @return True if a path was found, false if not
 */
bool Graph::path(const std::list<std::string> &source, const std::string &target) const {

    for (Vertex *v: vertexSet) {
        v->setVisited(false);
        v->setPath(nullptr);
    }

    std::queue<std::string> q;
    for (const auto &it: source) {
        q.push(it);
        findVertex(it)->setVisited(true);
    }

    while (!q.empty()) {
        Vertex const *currentVertex = findVertex(q.front());
        q.pop();
        for (Edge *e: currentVertex->getAdj()) {
            if (!e->getDest()->isVisited() && e->getCapacity() > 0 && e->isSelected()) {
                q.push(e->getDest()->getId());
                e->getDest()->setVisited(true);
                e->getDest()->setPath(e);
                if (e->getDest()->getId() == target) return true;
            }
        }
    }
    return false;
}

std::list<Edge *> Graph::bellmanFord(const std::string &source, const std::string &target) {
    for (Vertex *v: vertexSet) {
        v->setCost(UINT32_MAX);
        v->setPath(nullptr);
    }
    findVertex(source)->setCost(0);

    for (int i = 1; i <= vertexSet.size(); i++) { //V times
        for (Vertex *v: vertexSet) { //Relax every Edge
            for (Edge *e: v->getIncoming()) {
                if (e->getCapacity() > 0) {
                    int tempCost = e->getOrig()->getCost() + e->getCost();
                    if (tempCost < v->getCost()) {
                        if (i == vertexSet.size()) { //Edge being relaxed on Nth iteration - Negative cycle!
                            Vertex *currentVertex = v;
                            std::list<Edge *> negativeCycle;
                            do {
                                negativeCycle.push_back(currentVertex->getPath());
                                currentVertex = currentVertex->getPath()->getOrig();
                            } while (currentVertex != v);
                            return negativeCycle;
                        }
                        v->setCost(tempCost);
                        v->setPath(e);
                    }
                }
            }
        }
    }
    return {};
}

std::pair<unsigned int, unsigned int>
Graph::minCostMaxFlow(const std::string &source, const std::string &target, Graph &residualGraph) {
    std::pair<unsigned int, unsigned int> result;
    result.first = edmondsKarp({source}, target, residualGraph);

    Graph minCostResidual;
    makeMinCostResidual(minCostResidual);

    std::list<Edge *> negativeCycle = minCostResidual.bellmanFord(source, target);
    while (!negativeCycle.empty()) {
        unsigned int bottleneckCapacity = minCostResidual.findListBottleneck(negativeCycle);
        minCostResidual.augmentMinCostPath(negativeCycle, bottleneckCapacity);
        negativeCycle = minCostResidual.bellmanFord(source, target);
    }

    unsigned int cost = 0;
    for (Vertex *v: vertexSet) {
        for (Edge *e: v->getAdj()) {
            cost += e->getCost() * e->getFlow();
        }
    }
    result.second = cost;

    return result;
}

/**
 * Finds the minimum available capacity value in the path connecting source and target vertices
 * Time Complexity: O(|E|)
 * @param target - Id of the target Vertex
 * @return Bottleneck of the path connecting source to target
 */
unsigned int Graph::findBottleneck(const std::string &target) const {
    Vertex const *currentVertex = findVertex(target);
    unsigned int currBottleneck;
    unsigned int bottleneck = UINT32_MAX;

    while (currentVertex->getPath() != nullptr) {

        currBottleneck = currentVertex->getPath()->getCapacity();
        currentVertex = currentVertex->getPath()->getOrig();

        if (currBottleneck < bottleneck)
            bottleneck = currBottleneck;

    }
    return bottleneck;
}


unsigned int Graph::findListBottleneck(std::list<Edge *> edges) const {
    unsigned int currBottleneck;
    unsigned int bottleneck = UINT32_MAX;

    for (Edge *e: edges) {
        currBottleneck = e->getCapacity();
        if (currBottleneck < bottleneck)
            bottleneck = currBottleneck;
    }
    return bottleneck;
}


/**
 * Augments the flow in the regularGraph path connecting source to target by value units, and updates the residual network. Indicated for use on residual graphs
 * Time Complexity: O(|E|)
 * @param target - Id of the target Vertex
 * @param value - Number of units to augment the flow by
 */
void Graph::augmentPath(const std::string &target, const unsigned int &value) const {
    Vertex const *currentVertex = findVertex(target);

    while (currentVertex->getPath() != nullptr) {
        Edge *residualEdge = currentVertex->getPath();
        Edge *reverseResidualEdge = residualEdge->getReverse();
        Edge *regularEdge = residualEdge->getCorrespondingEdge();
        Edge *reverseRegularEdge = regularEdge->getReverse();


        if (regularEdge->getFlow() + value > regularEdge->getCapacity()) { //Intended value exceeds capacity

            //Augment flow in regular graph
            unsigned int intendedValue = regularEdge->getFlow() + value;
            regularEdge->setFlow(regularEdge->getCapacity());
            reverseRegularEdge->setFlow(reverseRegularEdge->getFlow() - (intendedValue -
                                                                         regularEdge->getCapacity())); //Reduce reverse edge flow by exceeding amount
            //Update residual Graph edges
            residualEdge->setCapacity(0);
            reverseResidualEdge->setCapacity(reverseResidualEdge->getCapacity() + (intendedValue -
                                                                                   regularEdge->getCapacity()));

        } else { //Intended value is within capacity

            //Augment flow in regular graph
            regularEdge->setFlow(regularEdge->getFlow() + value);

            //Update residual Graph edges
            residualEdge->setCapacity(regularEdge->getCapacity() - regularEdge->getFlow());
            reverseResidualEdge->setCapacity(reverseResidualEdge->getCapacity() + value);

        }
        currentVertex = currentVertex->getPath()->getOrig();
    }
}

void Graph::augmentMinCostPath(std::list<Edge *> edges, const unsigned int &value) const {

    for (Edge *residualEdge: edges) {
        Edge *reverseResidualEdge = residualEdge->getReverse(); //For each minCostResidual edge, its reverse is the corresponding negative cost edge, and vice-versa
        Edge *regularEdge = residualEdge->getCorrespondingEdge();
        Edge *reverseRegularEdge = regularEdge->getReverse();

        if (residualEdge->getCost() < 0) {
            //Reduce flow in regular graph
            regularEdge->setFlow(regularEdge->getFlow() - value);

            //Update residual Graph edges
            residualEdge->setCapacity(residualEdge->getCapacity() + value);
            reverseResidualEdge->setCapacity(reverseResidualEdge->getCapacity() - value);
        } else {
            //Augment flow in regular graph
            regularEdge->setFlow(regularEdge->getFlow() + value);

            //Update residual Graph edges
            residualEdge->setCapacity(residualEdge->getCapacity() - value);
            reverseResidualEdge->setCapacity(reverseResidualEdge->getCapacity() + value);
        }
    }
}


/**
* Finds the stations that are at the end of the indicated station's line (i.e have only connection to one other station)
 * Time Complexity: O(|V+E|)
 * @param stationId - Id of the starting station
*/
std::list<std::string> Graph::findEndOfLines(const std::string &stationId) const {
    std::list<std::string> eol_stations;
    std::queue<Vertex *> q;

    for (Vertex *v: vertexSet) v->setVisited(false);
    q.push(findVertex(stationId));

    while (!q.empty()) {
        Vertex *curr = q.front();
        q.pop();
        curr->setVisited(true);
        if (curr->getAdj().size() == 1) eol_stations.push_back(curr->getId());
        for (Edge const *e: curr->getAdj()) {
            if (!e->getDest()->isVisited()) {
                q.push(e->getDest());
            }
        }
    }

    return eol_stations;
}

/**
 * DFS traversal variation that sets the visited attribute to true of the nodes the DFS traverses to
 * Time Complexity: O(|V|+|E|)
 * @param source - Vertex where the DFS starts
 */
void Graph::visitedDFS(Vertex *source) {
    source->setVisited(true);
    for (Edge const *e: source->getAdj()) {
        if (!e->getDest()->isVisited()) {
            visitedDFS(e->getDest());
        }
    }
}

/**
 * Finds the pairs of stations with max Max-Flow
 * Time Complexity: O(|V³|*|E²|)
 * @return A pair consisting of a list of pairs, the stations with max-flow, and an unsigned int of the value of the max flow between them
 */
std::pair<std::list<std::pair<std::string, std::string>>, unsigned int>
Graph::calculateNetworkMaxFlow(Graph &residualGraph) {
    unsigned int max = 0;
    std::list<std::pair<std::string, std::string>> stationList;
    for (auto itV1 = vertexSet.begin(); itV1 < vertexSet.end(); itV1++) {
        Vertex *v1 = *itV1;
        for (Vertex *aux: vertexSet) aux->setVisited(false);
        visitedDFS(*itV1);
        for (auto itV2 = itV1 + 1; itV2 < vertexSet.end(); itV2++) {
            Vertex *v2 = *itV2;
            if (v2->isVisited()) {
                unsigned int itFlow = edmondsKarp({v1->getId()}, v2->getId(), residualGraph);
                if (itFlow == max) stationList.emplace_back(v1->getId(), v2->getId());
                if (itFlow > max) {
                    max = itFlow;
                    stationList = {{v1->getId(), v2->getId()}};
                }
            }
        }
    }
    return {stationList, max};
}

/**
 * Finds the incoming flux that a certain station can receive (i.e the amount of trains that can arrive there at the same time)
 * Time Complexity: O(|VE²|)
 * @param station - Vertex's ID
 * @return Max flow that can arrive at the given vertex from all the network
 */
unsigned int Graph::incomingFlux(const std::string &station, Graph &residualGraph) {
    std::list<std::string> super_source = findEndOfLines(station);
    for (auto it = super_source.begin(); it != super_source.end(); it++)
        if (*it == station) {
            super_source.erase(it);
            break;
        }

    return edmondsKarp(findEndOfLines(station), station, residualGraph);
}

/**
 * Gets the corresponding Edge to e in the passed graph, i.e. the Edge with the same origin and destiny
 * Time Complexity: O(|Vg| + |Eg|), where Vg is the number of vertices and Eg is the number of edges in the parameter graph
 * @param e - Pointer to the Edge whose correspondent is to be found
 * @param graph - Graph in which the corresponding Edge should be found
 * @return Pointer to the found Edge, or nullptr if no such Ede was found in the parameter graph
 */
Edge *Graph::getCorrespondingEdge(const Edge *e, const Graph &graph) {
    auto adjList = graph.findVertex(e->getOrig()->getId())->getAdj();
    for (Edge *r: adjList) {
        if (r->getDest()->getId() == e->getDest()->getId())
            return r;
    }
    return nullptr;
}


void Graph::makeMinCostResidual(Graph &minCostResidual) {
    for (Vertex *v: vertexSet) {
        minCostResidual.addVertex(v->getId());
    }
    for (Vertex *v: vertexSet) {
        for (Edge *e: v->getAdj()) {
            auto [edge, negativeCostEdge] = minCostResidual.addAndGetBidirectionalEdge(
                    e->getOrig()->getId(), e->getDest()->getId(), e->getCapacity(), e->getService());

            negativeCostEdge->setCost(-edge->getCost());

            edge->setCapacity(e->getCapacity() - e->getFlow());
            negativeCostEdge->setCapacity(e->getFlow());

            edge->setCorrespondingEdge(e);
            negativeCostEdge->setCorrespondingEdge(e);
        }
    }

}


