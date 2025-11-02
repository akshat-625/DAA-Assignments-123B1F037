
//Name : Akshat Jhanwar
//PRN  : 123B1F037

#include <bits/stdc++.h>
using namespace std;

struct Route {
    int dest;
    double baseCost;
    double liveCost;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int stages;
    cout << "Enter total number of stages: " << flush;
    cin >> stages;

    vector<int> nodesInStage(stages);
    cout << "Enter node count in each stage (" << stages << " values): " << flush;
    for (int i = 0; i < stages; ++i) cin >> nodesInStage[i];

    int totalNodes = 0;
    vector<int> startIndex(stages);
    for (int i = 0; i < stages; ++i) {
        startIndex[i] = totalNodes;
        totalNodes += nodesInStage[i];
    }

    int edgeCount;
    cout << "Enter number of edges: " << flush;
    cin >> edgeCount;

    vector<vector<Route>> graph(totalNodes);
    vector<vector<int>> revGraph(totalNodes);

    cout << "Enter each edge as: source destination cost\n" << flush;
    for (int i = 0; i < edgeCount; ++i) {
        int from, to;
        double cost;
        cin >> from >> to >> cost;
        if (from < 0 || from >= totalNodes || to < 0 || to >= totalNodes) {
            cerr << "Invalid edge input.\n";
            return 0;
        }
        graph[from].push_back({to, cost, cost});
        revGraph[to].push_back(from);
    }

    const double INF = 1e18;
    vector<double> minCost(totalNodes, INF);
    vector<int> nextHop(totalNodes, -1);

    for (int k = 0; k < nodesInStage.back(); ++k) {
        int node = startIndex.back() + k;
        minCost[node] = 0.0;
    }

    for (int st = stages - 2; st >= 0; --st) {
        for (int j = 0; j < nodesInStage[st]; ++j) {
            int u = startIndex[st] + j;
            double best = INF;
            int bestNext = -1;
            for (auto &r : graph[u]) {
                double newCost = r.liveCost + minCost[r.dest];
                if (newCost < best) {
                    best = newCost;
                    bestNext = r.dest;
                }
            }
            minCost[u] = best;
            nextHop[u] = bestNext;
        }
    }

    cout << fixed << setprecision(6);
    cout << "\nOptimal costs from Stage 0 nodes:\n";
    for (int j = 0; j < nodesInStage[0]; ++j) {
        int node = startIndex[0] + j;
        if (minCost[node] >= INF / 2)
            cout << "Node " << node << ": Unreachable\n";
        else
            cout << "Node " << node << ": Cost = " << minCost[node] << "\n";
    }

    cout << "\nEnter a source node (stage 0) to display path, or -1 to skip: " << flush;
    int src;
    cin >> src;

    if (src >= 0 && src < totalNodes) {
        if (minCost[src] >= INF / 2)
            cout << "No route from " << src << "\n";
        else {
            cout << "Best path from " << src << " : ";
            double total = 0;
            int cur = src;
            while (cur != -1) {
                cout << cur;
                int nxt = nextHop[cur];
                if (nxt == -1) break;
                cout << " -> ";
                double cst = 0;
                for (auto &r : graph[cur]) {
                    if (r.dest == nxt) {
                        cst = r.liveCost;
                        break;
                    }
                }
                total += cst;
                cur = nxt;
            }
            cout << "\nTotal route cost: " << total << "\n";
        }
    }

    cout << "\nEnter number of live edge cost updates (0 to finish): " << flush;
    int updates;
    cin >> updates;

    while (updates-- > 0) {
        int u, v;
        double factor;
        cout << "Enter update (u v multiplier): " << flush;
        cin >> u >> v >> factor;

        for (auto &r : graph[u])
            if (r.dest == v)
                r.liveCost = r.baseCost * factor;

        const double INF = 1e18;
        auto updateNode = [&](int node, const vector<vector<Route>> &graph, const vector<double> &minCost, vector<int> &nextHop) {
            double best = INF;
            int bestNext = -1;
            for (auto &r : graph[node]) {
                if (minCost[r.dest] >= INF / 2) continue;
                double candidate = r.liveCost + minCost[r.dest];
                if (candidate < best) {
                    best = candidate;
                    bestNext = r.dest;
                }
            }
            nextHop[node] = bestNext;
            return best;
        };

        queue<int> q;
        double newCost = updateNode(u, graph, minCost, nextHop);
        if (fabs(newCost - minCost[u]) > 1e-9) {
            minCost[u] = newCost;
            q.push(u);
        }

        while (!q.empty()) {
            int node = q.front(); q.pop();
            for (int pred : revGraph[node]) {
                double nc = updateNode(pred, graph, minCost, nextHop);
                if (fabs(nc - minCost[pred]) > 1e-9) {
                    minCost[pred] = nc;
                    q.push(pred);
                }
            }
        }
    }

    cout << "\nAfter updates, best costs from Stage 0 nodes:\n";
    for (int j = 0; j < nodesInStage[0]; ++j) {
        int node = startIndex[0] + j;
        if (minCost[node] >= INF / 2)
            cout << "Node " << node << ": Unreachable\n";
        else
            cout << "Node " << node << ": Cost = " << minCost[node] << "\n";
    }

    cout << "\nEnter a Stage 0 source node to view updated path (-1 to exit): " << flush;
    cin >> src;
    if (src >= 0 && src < totalNodes) {
        if (minCost[src] >= INF / 2)
            cout << "No route from " << src << "\n";
        else {
            cout << "Updated path from " << src << " : ";
            double total = 0;
            int cur = src;
            while (cur != -1) {
                cout << cur;
                int nxt = nextHop[cur];
                if (nxt == -1) break;
                cout << " -> ";
                double cst = 0;
                for (auto &r : graph[cur]) {
                    if (r.dest == nxt) { cst = r.liveCost; break; }
                }
                total += cst;
                cur = nxt;
            }
            cout << "\nUpdated route cost: " << total << "\n";
        }
    }

    return 0;
}


// sample input :

// 3
// 2 2 1
// 4
// 0 2 4
// 0 3 6
// 1 2 3
// 1 3 5
// 0

/* Sample output :

Enter total number of stages: Enter node count in each stage (3 values): Enter number of edges: Enter each edge as: source destination cost

Optimal costs from Stage 0 nodes:
Node 0: Cost = 4.000000
Node 1: Cost = 3.000000

Enter a source node (stage 0) to display path, or -1 to skip: -1

Enter number of live edge cost updates (0 to finish): 
After updates, best costs from Stage 0 nodes:
Node 0: Cost = 4.000000
Node 1: Cost = 3.000000

Enter a Stage 0 source node to view updated path (-1 to exit): -1
*/
