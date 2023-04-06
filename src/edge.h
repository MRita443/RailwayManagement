//
// Created by rita on 12-03-2023.
//

#ifndef RAILWAYMANAGEMENT_EDGE_H
#define RAILWAYMANAGEMENT_EDGE_H

class Vertex;

#include <memory>
#include "vertex.h"

#define INF std::numeric_limits<double>::max()

enum Service : unsigned int {
    STANDARD = 0,
    ALFA_PENDULAR = 1
};

class Edge {
public:
    Edge(Vertex *orig, Vertex *dest, double w, Service s);

    [[nodiscard]] Vertex *getDest() const;

    [[nodiscard]] unsigned int getCapacity() const;

    [[nodiscard]] bool isSelected() const; //isOpen

    [[nodiscard]] Vertex *getOrig() const;

    [[nodiscard]] Edge *getReverse() const;

    [[nodiscard]] Service getService() const;

    [[nodiscard]] unsigned int getFlow() const;

    void setSelected(bool selected);

    void setReverse(Edge *reverse);

    void setService(Service service);

    void setFlow(unsigned int flow);

    void setCapacity(unsigned int capacity);

    void print() const;

protected:
    Vertex *dest; // destination vertex
    unsigned int capacity; // edge capacity
    Service service;

    // auxiliary fields
    bool selected = true;

    // used for bidirectional edges
    Vertex *orig;
    Edge *reverse = nullptr;

    unsigned int flow; // for flow-related problems
};


#endif //RAILWAYMANAGEMENT_EDGE_H
