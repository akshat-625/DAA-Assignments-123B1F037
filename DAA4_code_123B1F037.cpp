
//Name : Akshat Jhanwar
//PRN  : 123B1F037

#include<bits/stdc++.h>
using namespace std;

struct Edge {
    int to, weight;
    Edge(int t, int w) : to(t), weight(w) {}
};

void dijkstra(int source, vector<vector<Edge>>& graph, vector<int>& dist, vector<int>& parent) {
    int V = graph.size();
    dist.assign(V, INT_MAX);
    parent.assign(V, -1);
    dist[source] = 0;

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({0, source});

    while (!pq.empty()) {
        int u = pq.top().second;
        int d = pq.top().first;
        pq.pop();

        if (d > dist[u]) continue;

        for (Edge& edge : graph[u]) {
            int v = edge.to;
            int w = edge.weight;

            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
}

void printPath(int node, vector<int>& parent) {
    if (node == -1) return;
    vector<int> path;
    int current = node;
    while (current != -1) {
        path.push_back(current);
        current = parent[current];
    }
    reverse(path.begin(), path.end());

    cout << "Optimal Path: ";
    for (int i = 0; i < path.size(); i++) {
        cout << path[i];
        if (i != path.size() - 1) cout << " -> ";
    }
    cout << endl;
}

int main() {
    int V, E;
    cout << "Enter number of intersections (vertices):\n";
    cin >> V;
    cout << "Enter number of roads (edges):\n";
    cin >> E;

    vector<vector<Edge>> graph(V);

    cout << "Enter edges (u v w):" << endl;
    for (int i = 0; i < E; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        graph[u].push_back(Edge(v, w));
        graph[v].push_back(Edge(u, w)); // undirected
    }

    int source;
    cout << "Enter ambulance start location (source):\n";
    cin >> source;

    int H;
    cout << "Enter number of hospitals:\n";
    cin >> H;
    vector<int> hospitals(H);
    cout << "Enter hospital nodes:" << endl;
    for (int i = 0; i < H; i++) {
        cin >> hospitals[i];
    }

    vector<int> dist(V);
    vector<int> parent(V);
    dijkstra(source, graph, dist, parent);

    int minTime = INT_MAX, nearestHospital = -1;
    for (int h : hospitals) {
        if (dist[h] < minTime) {
            minTime = dist[h];
            nearestHospital = h;
        }
    }

    if (nearestHospital == -1) {
        cout << "No hospital reachable." << endl;
    } else {
        cout << "\nNearest hospital is at node " << nearestHospital 
             << " with travel time " << minTime << " minutes." << endl;
        printPath(nearestHospital, parent);
    }

    cout << "\nDo you want to update traffic conditions? (y/n): ";
    char choice;
    cin >> choice;

    while (choice == 'y' || choice == 'Y') {
        cout << "Enter road (u v) and new travel time: ";
        int u, v, newW;
        cin >> u >> v >> newW;

        // Update edge weights
        for (Edge& edge : graph[u]) {
            if (edge.to == v) edge.weight = newW;
        }
        for (Edge& edge : graph[v]) {
            if (edge.to == u) edge.weight = newW;
        }

        // Re-run Dijkstra
        dijkstra(source, graph, dist, parent);

        minTime = INT_MAX;
        nearestHospital = -1;
        for (int h : hospitals) {
            if (dist[h] < minTime) {
                minTime = dist[h];
                nearestHospital = h;
            }
        }

        if (nearestHospital == -1) {
            cout << "No hospital reachable after update." << endl;
        } else {
            cout << "\nAfter traffic update, nearest hospital is at node " << nearestHospital 
                 << " with travel time " << minTime << " minutes." << endl;
            printPath(nearestHospital, parent);
        }

        cout << "\nDo you want to update traffic again? (y/n):\n";
        cin >> choice;
    }

    return 0;
}


/*
Sample Output:

Enter number of intersections (vertices):
6
Enter number of roads (edges):
8
Enter edges (u v w):
0 1 7
0 2 9
0 5 14
1 2 10
1 3 15
2 3 11
2 5 2
3 4 6
Enter ambulance start location (source):
0
Enter number of hospitals:
2
Enter hospital nodes:
3 4

Nearest hospital is at node 3 with travel time 20 minutes.
Optimal Path: 0 -> 2 -> 3

Do you want to update traffic conditions? (y/n): y
Enter road (u v) and new travel time: 2 3 20

After traffic update, nearest hospital is at node 4 with travel time 26 minutes.
Optimal Path: 0 -> 2 -> 3 -> 4

Do you want to update traffic again? (y/n):
n


*/