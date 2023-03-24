//
// Created by rita on 12-03-2023.
//

#include "edge.h"

Edge::Edge(Vertex *orig, Vertex *dest, double w) : orig(orig), dest(dest), capacity(w) {}

Vertex *Edge::getDest() const {
    return this->dest;
}

double Edge::getCapacity() const {
    return this->capacity;
}

Vertex *Edge::getOrig() const {
    return this->orig;
}

Edge *Edge::getReverse() const {
    return this->reverse;
}

bool Edge::isSelected() const {
    return this->selected;
}

unsigned int* Edge::getFlow() const {
    return flow;
}

void Edge::setSelected(bool selected) {
    this->selected = selected;
}

void Edge::setReverse(Edge *reverse) {
    this->reverse = reverse;
}

void Edge::setFlow(unsigned int *flow) {
    this->flow = flow;
}

void Edge::setFlowValue(unsigned int flow) {
    *(this->flow) = flow;
}
