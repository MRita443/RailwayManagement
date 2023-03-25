//
// Created by rita on 12-03-2023.
//

#ifndef RAILWAYMANAGEMENT_EDGE_H
#define RAILWAYMANAGEMENT_EDGE_H

class Vertex;

#include "vertex.h"

#define INF std::numeric_limits<double>::max()

class Edge {
public:
    Edge(Vertex *orig, Vertex *dest, double w);

    [[nodiscard]] Vertex *getDest() const;

    [[nodiscard]] double getCapacity() const;

    [[nodiscard]] bool isSelected() const;

    [[nodiscard]] Vertex *getOrig() const;

    [[nodiscard]] Edge *getReverse() const;

    [[nodiscard]] double* getFlow() const;

    void setSelected(bool selected);

    void setReverse(Edge *reverse);

    void setFlow(double* flow);

protected:
    Vertex *dest; // destination vertex
    double capacity; // edge capacity

    // auxiliary fields
    bool selected = true;

    // used for bidirectional edges
    Vertex *orig;
    Edge *reverse = nullptr;

    double* flow; // for flow-related problems
};


#endif //RAILWAYMANAGEMENT_EDGE_H
