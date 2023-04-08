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


unsigned int Graph::getTotalEdges() const {
    return totalEdges;
}

/**
 * Finds the vertex with a given id
 * Time Complexity: O(1) (average case) | O(|V|) (worst case)
 * @param id - Id of the vertex to be found
 * @return Pointer to the found Vertex, or nullptr if none was found
 */
Vertex *Graph::findVertex(const std::string &id) const {
    auto it = idToVertex.find(id);
    if (it == idToVertex.end()) { return nullptr; }
    return it->second;
}

/**
 * Adds a vertex with a given id to the Graph, representing a given station
 * Time Complexity: O(1) (average case) | O(|V|) (worst case)
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
 * Time Complexity: O(1) (average case) | O(|V|) (worst case)
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

    totalEdges++;
    return true;
}

/**
 * Adds and returns a bidirectional edge to the Graph between the vertices with id source and dest, with a capacity of c, representing a Service s
 * Time Complexity: O(1) (average case) | O(|V|) (worst case)
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

    totalEdges++;
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
        if (currentVertex->getId() == "Leça do Balio") {
            int a = 1;
        }
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

/**
 * Bellman-Ford algorithm variation that returns a list of edges belonging to a negative cycle that was found
 * Time Complexity: O(|VE|)
 * @param source - Id of source Vertex, to which distances will be relative to
 * @return List of pointers to Edges that belong to a negative cycle, or an empty list if no negative cycle was found
 */
std::list<Edge *> Graph::bellmanFord(const std::string &source) {
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

                        if (e->getDest()->getId() == "Figueira da Foz") {
                            int a = 1;
                        }

                        if (i == vertexSet.size()) { //Edge being relaxed on Nth iteration - Negative cycle!
                            Vertex *currentVertex = v;
                            std::list<Edge *> negativeCycle;

                            //Get beginning of cycle
                            for (int i = 0; i < vertexSet.size(); i++) {
                                currentVertex = currentVertex->getPath()->getOrig();
                            }

                            for (Vertex *temp = currentVertex;; temp = temp->getPath()->getOrig()) {
                                negativeCycle.push_back(currentVertex->getPath());
                                if (temp == currentVertex && negativeCycle.size() > 1) return negativeCycle;
                            }
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

/**
 * Cycle-cancelling algorithm for finding the minimum cost for the maximum flow of this Graph's network
 * @param source - Id of the source Vertex
 * @param target - Id of the target Vertex
 * @param residualGraph - Graph object representing this Graph's residual network
 * @return A pair of unsigned ints representing the value of the max flow and its min cost
 */
std::pair<unsigned int, unsigned int>
Graph::minCostMaxFlow(const std::string &source, const std::string &target, Graph &residualGraph) {
    std::pair<unsigned int, unsigned int> result;
    result.first = edmondsKarp({source}, target, residualGraph);

    Graph minCostResidual;
    makeMinCostResidual(minCostResidual);

    std::list<Edge *> negativeCycle = minCostResidual.bellmanFord(source);
    while (!negativeCycle.empty()) {
        unsigned int bottleneckCapacity = findListBottleneck(negativeCycle);
        augmentMinCostPath(negativeCycle, bottleneckCapacity);
        negativeCycle = minCostResidual.bellmanFord(source);
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
 * @return Bottleneck (minimum) capacity of the path connecting source to target
 */
unsigned int Graph::findBottleneck(const std::string &target) const {
    Vertex const *currentVertex = findVertex(target);
    unsigned int currBottleneck;
    unsigned int bottleneck = UINT32_MAX;

    while (currentVertex->getPath() != nullptr) {

        currBottleneck = currentVertex->getPath()->getCapacity();
        currentVertex = currentVertex->getPath()->getOrig();

        if (currBottleneck < bottleneck) {
            bottleneck = currBottleneck;
        }
    }
    return bottleneck;
}

/**
 * Finds the minimum available capacity value of the edges passed as parameter
 * Time Complexity: O(|E|)
 * @param edges - List of pointers to the Edges to evaluate
 * @return Bottleneck (minimum) capacity of the list of Edges
 */
unsigned int Graph::findListBottleneck(const std::list<Edge *> &edges) {
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
 * Augments or reduces the flow in the regular Graph path connecting source to target by value units, and updates the residual network. Indicated for use on residual graphs
 * Time Complexity: O(|E|)
 * @param target - Id of the target Vertex
 * @param value - Number of units to alter the flow by
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

/**
 * Augments or reduces the flow in the regular graph edges by value units, and updates the residual network. Indicated for use on residual graphs auxiliary to the min cost max flow algorithm
 * Time Complexity: O(|E|)
 * @param edges - List of pointers to the Edges whose flow will be altered
 * @param value - Number of units to alter the flow by
 */
void Graph::augmentMinCostPath(const std::list<Edge *> &edges, const unsigned int &value) {

    for (Edge *residualEdge: edges) {
        Edge *reverseResidualEdge = residualEdge->getReverse(); //For each minCostResidual edge, its reverse is the corresponding negative cost edge, and vice-versa
        Edge *regularEdge = residualEdge->getCorrespondingEdge();

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
 * Takes a number and sets the bool "selected" of that amount of edges, their reverse edges and their corresponding edges in the residual graph to false
 * Time Complexity: O(|E|)
 * @param numEdges - Number of edges to be deactivated
 * @return A vector of pointers for all the edges that were deactivated
 */
std::vector<Edge *> Graph::deactivateEdges(unsigned int numEdges) {
    unsigned int stationNum;
    Vertex *currentVertex;
    unsigned int choice;
    std::vector<Edge *> deactivatedEdges = {};
    while (deactivatedEdges.size() < numEdges) {
        stationNum = rand() % vertexSet.size();
        currentVertex = vertexSet[stationNum];
        choice = rand() % currentVertex->getAdj().size();
        if (currentVertex->getAdj()[choice]->isSelected()) {
            currentVertex->getAdj()[choice]->setSelected(false);
            currentVertex->getAdj()[choice]->getCorrespondingEdge()->setSelected(false);
            currentVertex->getAdj()[choice]->getReverse()->setSelected(false);
            currentVertex->getAdj()[choice]->getReverse()->getCorrespondingEdge()->setSelected(false);
            deactivatedEdges.push_back(currentVertex->getAdj()[choice]);
        }
    }
    return deactivatedEdges;
}

/**
 * Takes a vector of edge pointers and sets the selected state of those edges, their reverses and their corresponding edges in the residual graph to false
 * Time Complexity: O(size(edges))
 * @param edges - Vector of edge pointers to be deactivated
 * @return A vector of pointers for all edges that were deactivated
 */
std::vector<Edge *> Graph::deactivateEdges(std::vector<Edge *> edges) {
    for (Edge *edge: edges) {
        edge->setSelected(false);
        edge->getCorrespondingEdge()->setSelected(false);
        edge->getReverse()->setSelected(false);
        edge->getReverse()->getCorrespondingEdge()->setSelected(false);
    }
    return edges;
}

/**
 * Takes a vector of edge pointers and sets the selected state of those edges, their reverses and their corresponding edges in the residual graph to true
 * Time Complexity: O(size(edges))
 * @param edges - Vector of edge pointers to be activated
 */
void Graph::activateEdges(const std::vector<Edge *> &edges) {
    for (Edge *edge: edges) {
        edge->setSelected(true);
        edge->getCorrespondingEdge()->setSelected(true);
        edge->getReverse()->setSelected(true);
        edge->getReverse()->getCorrespondingEdge()->setSelected(true);
    }
}

/**
 * Calculates the maximum flow between a source vertex and a target vertex with (totalEdges) number of random edges being deactivated and reactivated after calculating the maximum flow
 * Time Complexity: O(|VE²|)
 * @param numEdges - Number of edges to be deactivated and later reactivated
 * @param source - List of Ids of source vertexes
 * @param target - Id of the target Vertex
 * @param residualGraph - Graph object representing this Graph's residual network
 * @return The value of the Max Flow with the interrupted lines
 */
unsigned int Graph::maxFlowDeactivatedEdgesRandom(const int &numEdges, const std::list<std::string> &source,
                                                  const std::string &target, Graph &residualGraph) {
    std::vector<Edge *> deactivatedEdges = deactivateEdges(numEdges);
    unsigned int maxFlowInterrupted = edmondsKarp(source, target, residualGraph);
    activateEdges(deactivatedEdges);
    return maxFlowInterrupted;
}


/**
 *Calculates the maximum flow between a source vertex and a target vertex with the edges inputted to the function being deactivated and reactivated after calculating the maximum flow
 * Time Complexity: O(|VE²|)
 * @param selectedEdges - Vector of edges to be deactivated and later reactivated
 * @param source - List of Ids of source vertexes
 * @param target - Id of the target Vertex
 * @param residualGraph - Graph object representing this Graph's residual network
 * @return The value of the Max Flow with the interrupted lines
 */
unsigned int
Graph::maxFlowDeactivatedEdgesSelected(const std::vector<Edge *> &selectedEdges, const std::list<std::string> &source,
                                       const std::string &target, Graph &residualGraph) {
    std::vector<Edge *> deactivatedEdges = deactivateEdges(selectedEdges);
    unsigned int maxFlowInterrupted = edmondsKarp(source, target, residualGraph);
    activateEdges(deactivatedEdges);
    return maxFlowInterrupted;
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
 * Calculates the max flow of a station normally, and once again with certain edges deactivated, and returns a pair with the name of the station and the difference between normal flow and reduced flow
 * Time Complexity: O(|VE²|)
 * @param vertexID - ID of the station to have it's flow measured
 * @param edges - Edges to be deactivated
 * @return A Pair with the name of the station first, and a pair the original max flow and the reduced max flow
 */
std::pair<std::string, std::pair<unsigned int, unsigned int>>
Graph::maxFlowDifference(const std::string &vertexID, const std::vector<Edge *> &edges, Graph &residualGraph) {
    std::pair<std::string, std::pair<unsigned int, unsigned int>> stationResults;
    std::list<std::string> superSource = superSourceCreator(vertexID);
    unsigned int baseFlow = edmondsKarp(superSource, vertexID, residualGraph);
    unsigned int reducedFlow = maxFlowDeactivatedEdgesSelected(edges, superSource, vertexID, residualGraph);
    stationResults = {vertexID, {baseFlow, reducedFlow}};
    return stationResults;
}

/**
 * Creates a list with every vertex except the one selected in the function
 * Time Complexity: O(V)
 * @param vertexId - Vertex to be excluded from the list
 * @return A list with every vertex except the selected one
 */
std::list<std::string> Graph::superSourceCreator(const std::string &vertexId) {
    std::list<std::string> superSource = {};
    for (Vertex *vertex: vertexSet) {
        if (vertexId != vertex->getId()) { superSource.push_back(vertex->getId()); }
    }
    return superSource;
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

    return edmondsKarp(super_source, station, residualGraph);
}


bool
sort_pair_decreasing_second(const std::pair<std::string, double> &left, const std::pair<std::string, double> &right) {
    return left.second > right.second;
}

/**
 * Creates an ordered vector with incoming fluxes of previously grouped stations
 * @param group - Map that identifies a group of stations
 * @param residualGraph - Graph object representing the graph's residual network
 * @return An ordered vector of pairs with decreasing average flow (second element), identified by its grouping name (first element)
 */
std::vector<std::pair<std::string, double>>
Graph::topGroupings(const std::unordered_map<std::string, std::list<Station>> &group, Graph &residualGraph) {
    std::vector<std::pair<std::string, double>> result;
    for (const auto &it: group) {
        double average = getAverageIncomingFlux(it.second, residualGraph);
        result.emplace_back(it.first, average);
    }
    std::sort(result.begin(), result.end(), sort_pair_decreasing_second);
    return result;
}

/**
 * Finds the average incoming flux for every station in a list (normally, representing a township, etc.)
 * Time Complexity: O(n|VE²|), n being the size of stations
 * @param stations - List with the stations' id
 * @param residualGraph - Graph object representing the graph's residual network
 */
double Graph::getAverageIncomingFlux(const std::list<Station> &stations, Graph &residualGraph) {
    double flux_sum = 0;
    for (const Station &s: stations) {
        const std::string &sid = s.getName();
        flux_sum += incomingFlux(sid, residualGraph);
    }
    return flux_sum / (double) stations.size();
}

/**
 * Based on this Graph, builds an auxiliary min cost max flow residual graph in minCostResidual, with edges representing removing flow having negative cost
 * Time Complexity: O(|V|+|E|)
 * @param minCostResidual - Graph object in which to construct the residual network
 */
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