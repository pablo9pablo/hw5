// Compara clique máximo exacto (Bron-Kerbosch) frente a heurística voraz.

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include "graph.h"

using clk = std::chrono::high_resolution_clock;

int** toAdjMatrix(const Graph& g, int n) {
    int** m = new int*[n];
    for (int i = 0; i < n; ++i) {
        m[i] = new int[n]();
        for (int j = 0; j < n; ++j) {
            m[i][j] = g.hasEdge(i, j) ? 1 : 0;
        }
    }
    return m;
}

void releaseMatrix(int** m, int n) {
    for (int i = 0; i < n; ++i) {
        delete[] m[i];
    }
    delete[] m;
}

static std::vector<int> bestExact;

static bool betterLex(std::vector<int> a, std::vector<int> b) {
    std::sort(a.begin(), a.end(), std::greater<int>());
    std::sort(b.begin(), b.end(), std::greater<int>());
    return std::lexicographical_compare(b.begin(), b.end(), a.begin(), a.end());
}

static int pivotInUnion(int** mat, const std::vector<int>& pool, const std::vector<int>& ban) {
    int p = pool[0], hits = -1;
    auto score = [&](int u) {
        int c = 0;
        for (int v : pool) {
            if (mat[u][v]) ++c;
        }
        if (c > hits) {
            hits = c;
            p = u;
        }
    };
    for (int u : pool) score(u);
    for (int u : ban) score(u);
    return p;
}

static void searchExact(int** mat, std::vector<int> cur, std::vector<int> pool, std::vector<int> ban) {
    if (cur.size() + pool.size() < bestExact.size()) {
        return;
    }
    if (pool.empty() && ban.empty()) {
        if (cur.size() > bestExact.size() || (cur.size() == bestExact.size() && betterLex(cur, bestExact))) {
            bestExact = cur;
        }
        return;
    }
    if (pool.empty()) {
        return;
    }

    int pv = pivotInUnion(mat, pool, ban);
    for (size_t k = 0; k < pool.size(); ) {
        int v = pool[k];
        if (mat[pv][v]) {
            ++k;
            continue;
        }
        std::vector<int> p2, b2;
        for (int u : pool) {
            if (mat[v][u]) p2.push_back(u);
        }
        for (int u : ban) {
            if (mat[v][u]) b2.push_back(u);
        }
        cur.push_back(v);
        searchExact(mat, cur, p2, b2);
        cur.pop_back();
        pool.erase(pool.begin() + static_cast<long>(k));
        ban.push_back(v);
    }
}

std::vector<int> runExactSolver(int** mat, int n) {
    bestExact.clear();
    std::vector<int> cur, pool(n), ban;
    for (int i = 0; i < n; ++i) pool[i] = i;
    searchExact(mat, cur, pool, ban);
    std::sort(bestExact.rbegin(), bestExact.rend());
    return bestExact;
}

static bool beats(const std::vector<int>& a, const std::vector<int>& b) {
    if (a.size() != b.size()) return a.size() > b.size();
    return betterLex(a, b);
}

std::vector<int> runGreedySolver(int** mat, int n) {
    std::vector<int> top;
    for (int s = 0; s < n; ++s) {
        std::vector<int> K = {s}, opts;
        for (int i = 0; i < n; ++i) {
            if (mat[s][i]) opts.push_back(i);
        }
        while (!opts.empty()) {
            int v = opts[0], deg = -1;
            for (int x : opts) {
                int d = 0;
                for (int y : opts) {
                    if (x != y && mat[x][y]) ++d;
                }
                if (d > deg) {
                    deg = d;
                    v = x;
                }
            }
            K.push_back(v);
            std::vector<int> nxt;
            for (int x : opts) {
                if (x != v && mat[v][x]) nxt.push_back(x);
            }
            opts = nxt;
        }
        if (beats(K, top)) top = K;
    }
    std::sort(top.rbegin(), top.rend());
    return top;
}

Graph completeGraph(int n) {
    Graph g(n);
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            g.addEdge(i, j);
        }
    }
    return g;
}

Graph petersenGraph() {
    Graph g(10);
    for (int i = 0; i < 5; ++i) g.addEdge(i, (i + 1) % 5);
    for (int i = 0; i < 5; ++i) g.addEdge(5 + i, 5 + (i + 2) % 5);
    for (int i = 0; i < 5; ++i) g.addEdge(i, 5 + i);
    return g;
}

Graph cycleGraph(int n) {
    Graph g(n);
    for (int i = 0; i < n; ++i) g.addEdge(i, (i + 1) % n);
    return g;
}

Graph embeddedK4Graph() {
    Graph g(12);
    for (int i = 8; i <= 11; ++i) {
        for (int j = i + 1; j <= 11; ++j) g.addEdge(i, j);
    }
    for (int i = 0; i < 5; ++i) {
        for (int j = i + 1; j < 5; ++j) {
            if (!(i == 0 && j == 4)) g.addEdge(i, j);
        }
    }
    g.addEdge(0, 8); g.addEdge(1, 9); g.addEdge(2, 10); g.addEdge(3, 11);
    g.addEdge(5, 8); g.addEdge(6, 9); g.addEdge(7, 10);
    return g;
}

Graph overlappingK4Graph() {
    Graph g(6);
    for (int i = 0; i < 4; ++i) {
        for (int j = i + 1; j < 4; ++j) g.addEdge(i, j);
    }
    for (int i = 2; i < 6; ++i) {
        for (int j = i + 1; j < 6; ++j) g.addEdge(i, j);
    }
    return g;
}

struct TimedClique {
    int k;
    std::vector<int> verts;
    double millis;
};

TimedClique timeExact(int** mat, int n) {
    auto t0 = clk::now();
    auto c = runExactSolver(mat, n);
    auto t1 = clk::now();
    double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
    return {static_cast<int>(c.size()), c, ms};
}

TimedClique timeGreedy(int** mat, int n) {
    auto t0 = clk::now();
    auto c = runGreedySolver(mat, n);
    auto t1 = clk::now();
    double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
    return {static_cast<int>(c.size()), c, ms};
}

void dumpSet(const std::vector<int>& v) {
    std::cout << "{";
    for (size_t i = 0; i < v.size(); ++i) {
        if (i) std::cout << ",";
        std::cout << v[i];
    }
    std::cout << "}";
}

void bench(const std::string& label, const Graph& g, int n) {
    int** mat = toAdjMatrix(g, n);
    TimedClique ex = timeExact(mat, n);
    TimedClique gr = timeGreedy(mat, n);
    releaseMatrix(mat, n);

    const char* tag = (gr.k == ex.k) ? "optimal" : "suboptimal";
    std::cout << std::left << std::setw(16) << label
              << "  |  exact=" << std::setw(2) << ex.k
              << " " << std::fixed << std::setprecision(4) << std::setw(9) << ex.millis << " ms"
              << "  |  heur=" << std::setw(2) << gr.k
              << " " << std::fixed << std::setprecision(4) << std::setw(9) << gr.millis << " ms"
              << "  (" << tag << ")\n";
    std::cout << "                    exact: ";
    dumpSet(ex.verts);
    std::cout << "\n                    heur:  ";
    dumpSet(gr.verts);
    std::cout << "\n\n";
}

int main() {
    std::cout << "=== Maximum Clique: Exact (Bron-Kerbosch) vs Greedy Heuristic ===\n\n";
    std::cout << std::string(80, '-') << "\n";

    bench("K5", completeGraph(5), 5);
    bench("K6", completeGraph(6), 6);
    bench("Petersen", petersenGraph(), 10);
    bench("Cycle C8", cycleGraph(8), 8);
    bench("TwoK4", overlappingK4Graph(), 6);
    bench("Embedded K4", embeddedK4Graph(), 12);

    std::cout << std::string(80, '-') << "\n";
    std::cout << "Note: heuristic may find suboptimal cliques on adversarial graphs;\n"
              << "      it is significantly faster for large inputs.\n";
    return 0;
}
