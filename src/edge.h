//
// Created by rita on 12-03-2023.
//

#ifndef RAILWAYMANAGEMENT_EDGE_H
#define RAILWAYMANAGEMENT_EDGE_H

class Vertex;

#include "vertex.h"

#define INF std::numeric_limits<double>::max()

enum Service {
    STANDARD,
    ALFA_PENDULAR
};

class Edge {
public:
    Edge(Vertex *orig, Vertex *dest, double w, Service s);

    [[nodiscard]] Vertex *getDest() const;

    [[nodiscard]] double getCapacity() const;

    [[nodiscard]] bool isSelected() const; //isOpen

    [[nodiscard]] Vertex *getOrig() const;

    [[nodiscard]] Edge *getReverse() const;

    [[nodiscard]] Service getService() const;

    [[nodiscard]] unsigned int* getFlow() const;

    void setSelected(bool selected);

    void setReverse(Edge *reverse);

    void setService(Service service);

    void setFlow(unsigned int *flow);
    
    void setFlowValue(unsigned int flow);

protected:
    Vertex *dest; // destination vertex
    double capacity; // edge capacity
    Service service;

    // auxiliary fields
    bool selected = true;

    // used for bidirectional edges
    Vertex *orig;
    Edge *reverse = nullptr;

    unsigned int *flow; // for flow-related problems
};


#endif //RAILWAYMANAGEMENT_EDGE_H
