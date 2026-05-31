#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

class Graph {
    std::vector<std::vector<bool>> edges;
    std::vector<bool> isLive;
    int liveVertices;
    int numEdges;

public:
    explicit Graph(int vertexCount = 0);

    void addVertex();
    void removeVertex(int v);

    void addEdge(int u, int v);
    void removeEdge(int u, int v);

    int size() const;
    int order() const;

    int degree(int v) const;
    std::vector<int> neighbors(int v) const;

    bool hasEdge(int u, int v) const;
};

#endif
