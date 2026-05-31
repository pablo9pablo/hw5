#include "graph.h"

Graph::Graph(int vertexCount) : liveVertices(vertexCount), numEdges(0) {
    edges.assign(vertexCount, std::vector<bool>(vertexCount, false));
    isLive.assign(vertexCount, true);
}

void Graph::addVertex() {
    const int oldN = static_cast<int>(edges.size());
    for (auto& row : edges) {
        row.push_back(false);
    }
    edges.emplace_back(oldN + 1, false);
    isLive.push_back(true);
    ++liveVertices;
}

void Graph::removeVertex(int v) {
    const int n = static_cast<int>(edges.size());
    if (v < 0 || v >= n || !isLive[v]) {
        return;
    }
    for (int u = 0; u < n; ++u) {
        if (isLive[u] && edges[v][u]) {
            edges[v][u] = false;
            edges[u][v] = false;
            --numEdges;
        }
    }
    isLive[v] = false;
    --liveVertices;
}

void Graph::addEdge(int u, int v) {
    if (u == v || !isLive[u] || !isLive[v] || edges[u][v]) {
        return;
    }
    edges[u][v] = edges[v][u] = true;
    ++numEdges;
}

void Graph::removeEdge(int u, int v) {
    const int n = static_cast<int>(edges.size());
    if (u < 0 || v < 0 || u >= n || v >= n || !edges[u][v]) {
        return;
    }
    edges[u][v] = edges[v][u] = false;
    --numEdges;
}

int Graph::size() const { return numEdges; }
int Graph::order() const { return liveVertices; }

int Graph::degree(int v) const {
    if (!isLive[v]) {
        return 0;
    }
    int deg = 0;
    const int n = static_cast<int>(edges.size());
    for (int u = 0; u < n; ++u) {
        if (isLive[u] && edges[v][u]) {
            ++deg;
        }
    }
    return deg;
}

std::vector<int> Graph::neighbors(int v) const {
    std::vector<int> nbrs;
    if (!isLive[v]) {
        return nbrs;
    }
    const int n = static_cast<int>(edges.size());
    for (int u = 0; u < n; ++u) {
        if (isLive[u] && edges[v][u]) {
            nbrs.push_back(u);
        }
    }
    return nbrs;
}

bool Graph::hasEdge(int u, int v) const {
    return isLive[u] && isLive[v] && edges[u][v];
}
