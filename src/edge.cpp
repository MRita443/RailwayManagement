//
// Created by rita on 12-03-2023.
//

#include "edge.h"

Edge::Edge(Vertex *orig, Vertex *dest, unsigned int capacity, Service service) {
    this->orig = orig;
    this->dest = dest;
    this->capacity = capacity;
    this->service = service;
    initializeCost();
}

Vertex *Edge::getDest() const {
    return this->dest;
}

unsigned int Edge::getCapacity() const {
    return this->capacity;
}

Vertex *Edge::getOrig() const {
    return this->orig;
}

Edge *Edge::getReverse() const {
    return this->reverse;
}

Service Edge::getService() const {
    return service;
}

bool Edge::isSelected() const {
    return this->selected;
}

unsigned int Edge::getFlow() const {
    return flow;
}

void Edge::initializeCost() {
    cost = service == Service::STANDARD ? 2 : 4;
}

void Edge::setSelected(bool s) {
    this->selected = s;
}

void Edge::setReverse(Edge *r) {
    this->reverse = r;
}

void Edge::setService(Service s) {
    Edge::service = s;
}

void Edge::setFlow(unsigned int f) {
    this->flow = f;
}

void Edge::print() const {
    std::cout << orig->getId() << " <-> " << dest->getId() << std::endl;
}

void Edge::setCapacity(unsigned int c) {
    this->capacity = c;
}

Edge *Edge::getCorrespondingEdge() const {
    return correspondingEdge;
}

void Edge::setCorrespondingEdge(Edge *correspondingEdge) {
    Edge::correspondingEdge = correspondingEdge;
}

void Edge::setCost(int cost) {
    Edge::cost = cost;
}

int Edge::getCost() const {
    return cost;
}


