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

    [[nodiscard]] int getCost() const;

    Edge *getCorrespondingEdge() const;

    void setSelected(bool s);

    void setReverse(Edge *r);

    void setService(Service s);

    void setFlow(unsigned int f);

    void setCapacity(unsigned int c);

    void setCorrespondingEdge(Edge *correspondingEdge);

    void setCost(int cost);

    void print() const;

    void initializeCost();

  private:
    Vertex *orig;
    Vertex *dest; // destination vertex
    unsigned int capacity; // edge capacity
    Service service;

    // auxiliary fields
    bool selected = true;
    Edge *reverse = nullptr;

    //corresponding edge in the residual/regular graph
    Edge *correspondingEdge = nullptr;

    unsigned int flow = 0; // for flow-related problems
    int cost;

};

#endif //RAILWAYMANAGEMENT_EDGE_H
