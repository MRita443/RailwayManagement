//
// Created by rita on 12-03-2023.
//

#include "edge.h"

Edge::Edge(Vertex *orig, Vertex *dest, double w): orig(orig), dest(dest), capacity(w) {}

Vertex * Edge::getDest() const {
    return this->dest;
}

double Edge::getCapacity() const {
    return this->capacity;
}

Vertex * Edge::getOrig() const {
    return this->orig;
}

Edge *Edge::getReverse() const {
    return this->reverse;
}

bool Edge::isSelected() const {
    return this->selected;
}

double Edge::getFlow() const {
    return flow;
}

void Edge::setSelected(bool selected) {
    this->selected = selected;
}

void Edge::setReverse(Edge *reverse) {
    this->reverse = reverse;
}

void Edge::setFlow(double flow) {
    this->flow = flow;
}
