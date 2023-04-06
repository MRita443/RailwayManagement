//
// Created by rita on 12-03-2023.
//

#ifndef RAILWAYMANAGEMENT_EDGE_H
#define RAILWAYMANAGEMENT_EDGE_H

#include <memory>
#include "vertex.h"

class Vertex;

enum class Service : unsigned int {
    STANDARD = 0,
    ALFA_PENDULAR = 1
};

class Edge {
public:
    Edge(Vertex *orig, Vertex *dest, unsigned int w, Service s);

    [[nodiscard]] Vertex *getDest() const;

    [[nodiscard]] unsigned int getCapacity() const;

    [[nodiscard]] bool isSelected() const; //isOpen

    [[nodiscard]] Vertex *getOrig() const;

    [[nodiscard]] Edge *getReverse() const;

    [[nodiscard]] Service getService() const;

    [[nodiscard]] unsigned int getFlow() const;

    void setSelected(bool s);

    void setReverse(Edge *r);

    void setService(Service s);

    void setFlow(unsigned int f);

    void setCapacity(unsigned int c);

    void print() const;

private:
    Vertex *orig;
    Vertex *dest; // destination vertex
    unsigned int capacity; // edge capacity
    Service service;

    // auxiliary fields
    bool selected = true;
    Edge *reverse = nullptr;

    unsigned int flow = 0; // for flow-related problems
};

#endif //RAILWAYMANAGEMENT_EDGE_H
