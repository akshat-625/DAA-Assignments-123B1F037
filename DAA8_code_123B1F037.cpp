//Name : Akshat Jhanwar
//PRN  : 123B1F037

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void addEdge(vector<vector<int>>& graph, int u, int v) {
    graph[u].push_back(v);
    graph[v].push_back(u);
}

void greedyColoring(vector<vector<int>>& graph, int numCourses) {
    vector<int> result(numCourses, -1);
    result[0] = 0;
   
    vector<bool> available(numCourses);
    
    for (int u = 1; u < numCourses; u++) {
        fill(available.begin(), available.end(), true);
        
        for (int adj : graph[u]) {
            if (result[adj] != -1)
                available[result[adj]] = false;
        }
      
        int color;
        for (color = 0; color < numCourses; color++) {
            if (available[color]) break;
        }
        result[u] = color;
    }
  
    cout << "Exam Slot Assignment (Greedy Coloring):" << endl;
    for (int u = 0; u < numCourses; u++) {
        cout << "Course " << u << " → Slot " << result[u] << endl;
    }
    
    int maxColor = *max_element(result.begin(), result.end());
    cout << "\nTotal Exam Slots Used: " << (maxColor + 1) << endl;
}

int main() {
    int numCourses = 6;
    vector<vector<int>> graph(numCourses);
  
    addEdge(graph, 0, 1);
    addEdge(graph, 1, 2);
    addEdge(graph, 2, 3);
    addEdge(graph, 3, 4);
    addEdge(graph, 3, 5);
    addEdge(graph, 0, 5);
   
    greedyColoring(graph, numCourses);
    
    return 0;
}

/*
Sample Input:
4
9999999 10 15 20
10 9999999 35 25
15 35 9999999 30
20 25 30 9999999

Sample Output:
Enter number of cities: 
Enter cost matrix (use large number for no direct route):

Optimal Delivery Route (SwiftShip): 0 1 3 2 0
Minimum Total Delivery Cost: 80
*/
