#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <string.h>
#include <vector>


// other includes and definitions

/* YOUR CODE HERE */
#include <algorithm>
using namespace std;

namespace {

vector<int> recordClique;

int countLinksToPool(int** mat, int u, const vector<int>& pool) {
    int links = 0;
    for (int w : pool) {
        if (mat[u][w]) {
            ++links;
        }
    }
    return links;
}

int pickPivot(int** mat, const vector<int>& pool, const vector<int>& excluded) {
    int pivot = pool.empty() ? -1 : pool[0];
    int bestLinks = -1;

    auto consider = [&](int u) {
        int links = countLinksToPool(mat, u, pool);
        if (links > bestLinks) {
            bestLinks = links;
            pivot = u;
        }
    };

    for (int u : pool) consider(u);
    for (int u : excluded) consider(u);
    return pivot;
}

bool lexWinsDescending(const vector<int>& left, const vector<int>& right) {
    const int n = static_cast<int>(left.size());
    for (int i = 0; i < n; ++i) {
        if (left[i] != right[i]) {
            return left[i] > right[i];
        }
    }
    return false;
}

void storeIfBest(const vector<int>& candidate) {
    if (candidate.size() > recordClique.size()) {
        recordClique = candidate;
        return;
    }
    if (candidate.size() < recordClique.size() || recordClique.empty()) {
        return;
    }
    vector<int> a = candidate, b = recordClique;
    sort(a.rbegin(), a.rend());
    sort(b.rbegin(), b.rend());
    if (lexWinsDescending(a, b)) {
        recordClique = candidate;
    }
}

void bronKerbosch(int** mat, vector<int> built, vector<int> pool, vector<int> excluded) {
    if (static_cast<int>(built.size() + pool.size()) < static_cast<int>(recordClique.size())) {
        return;
    }

    if (pool.empty()) {
        if (excluded.empty()) {
            storeIfBest(built);
        }
        return;
    }

    const int pivot = pickPivot(mat, pool, excluded);

    for (size_t idx = 0; idx < pool.size(); ) {
        const int v = pool[idx];
        if (mat[pivot][v]) {
            ++idx;
            continue;
        }

        vector<int> nextPool, nextExcluded;
        for (int u : pool) {
            if (mat[v][u]) nextPool.push_back(u);
        }
        for (int u : excluded) {
            if (mat[v][u]) nextExcluded.push_back(u);
        }

        built.push_back(v);
        bronKerbosch(mat, built, nextPool, nextExcluded);
        built.pop_back();

        pool.erase(pool.begin() + static_cast<long>(idx));
        excluded.push_back(v);
    }
}

}  // namespace


using namespace std;


// throw the following class if function is not implemented
class NotImplementedException {
};

// final result
int result;

// output
vector<int> output;




void hamiltonianCycle(int **input, int size){
    // result: set to size if hamiltonian cycle exists, and to 0 otherwise
    // output: node indices (starting at 0) in the hamiltonian cycle;
    // if many cycles are present, output should be such that it represents
    // the largest multidigital number among those possible
    // heuristic: the longer the cycle, the better
    /* YOUR CODE HERE */
    throw NotImplementedException();
}
void largestClique(int **input, int size){
    // result: set to the size of the largest clique
    // output: node indices (starting at 0) in the largest clique in a descending order;
    // if many such cliques are present, output should be such that it represents
    // the largest multidigital number among those possible
    // heuristic: the larger the clique, the better
    /* YOUR CODE HERE */
    recordClique.clear();
    output.clear();

    vector<int> built, pool, excluded;
    pool.reserve(size);
    for (int i = 0; i < size; ++i) {
        pool.push_back(i);
    }

    bronKerbosch(input, built, pool, excluded);

    result = static_cast<int>(recordClique.size());
    output = recordClique;
    sort(output.rbegin(), output.rend());
}
void dominatingSet(int **input, int size){
    // result: set to the size of the minimum dominating set
    // output: node indices (starting at 0) in the minimum dominating set in a descending order;
    // if many such sets are present, output should be such that it represents
    // the largest multidigital number among those possible
    // heuristic: the smaller the set, the better
    /* YOUR CODE HERE */
    throw NotImplementedException();
}
void independentSet(int **input, int size){
    // result: set to the size of the maximum independent set
    // output: node indices (starting at 0) in the maximum independent set in a descending order;
    // if many such sets are present, output should be such that it represents
    // the largest multidigital number among those possible
    // heuristic: the larger the set, the better
    /* YOUR CODE HERE */
    throw NotImplementedException();
}
void vertexCover(int **input, int size){
    // result: set to the size of the minimum vertex cover
    // output: node indices (starting at 0) in the minimum vertex cover in a descending order;
    // if many such covers are present, output should be such that it represents
    // the largest multidigital number among those possible
    // heuristic: the smaller the cover, the better
    /* YOUR CODE HERE */
    throw NotImplementedException();
}
void longestPath(int **input, int size){
    // result: set to the length of the longest path
    // output: node indices (starting at 0) in the longest path;
    // if many such paths are present, output should be such that it represents
    // the largest multidigital number among those possible
    // heuristic: the longer the path, the better
    /* YOUR CODE HERE */
    throw NotImplementedException();
}
void intersectionNumber(int **input, int size){
    // result: set to the intersection number (edge clique cover number)
    // output: clique sizes that cover the graph, in a descending order
    // heuristic: the smaller the number, the better
    /* YOUR CODE HERE */
    throw NotImplementedException();
}
void vertexColoring(int **input, int size){
    // result: set to the chromatic number
    // output: numbers of vertices colored in each color, in a descending order;
    // if many such colorings are possible, output should be such that it represents
    // the largest multidigital number among those possible
    // heuristic: the smaller the number, the better
    /* YOUR CODE HERE */
    throw NotImplementedException();
}
void edgeColoring(int **input, int size){
    // result: set to the chromatic index
    // output: numbers of edges colored in each color, in a descending order;
    // if many such colorings are possible, output should be such that it represents
    // the largest multidigital number among those possible
    // heuristic: the smaller the index, the better
    /* YOUR CODE HERE */
    throw NotImplementedException();
}


/* DO NOT MODIFY CODE BELOW */

int main(){
    try {

        int x = -1;
        while(1){
            cin >> x;
            if (x == 0){
                break;
            } else {
                int n;
                cin >> n;
                int **input = new int*[n];
                for(int i = 0; i < n; ++i){
                    input[i] = new int [n];
                }
                for(int i = 0; i < n; ++i){
                    for(int j = 0; j < n; ++j){
                        cin >> input[i][j];
                    }
                }
                switch(x){
                    case 1:
                        hamiltonianCycle(input, n);
                        break;
                    case 2:
                        largestClique(input, n);
                        break;
                    case 3:
                        dominatingSet(input, n);
                        break;
                    case 4:
                        independentSet(input, n);
                        break;
                    case 5:
                        vertexCover(input, n);
                        break;
                    case 6:
                        longestPath(input, n);
                        break;
                    case 7:
                        intersectionNumber(input, n);
                        break;
                    case 8:
                        vertexColoring(input, n);
                        break;
                    case 9:
                        edgeColoring(input, n);
                        break;
                }
                cout << result << endl;
                for (unsigned int i = 0; i < output.size(); ++i){
                    cout << output[i] << " ";
                }
                cout << endl;


                for(int i = 0; i < n; ++i){
                    delete [] input[i];
                }
                delete [] input;

            }
        }
    } catch (NotImplementedException &e){
        return 2018;
    } catch (...){
        return -1;
    }
    return 0;
}
