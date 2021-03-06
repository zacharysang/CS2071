#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <time.h>

using namespace std;

struct Edge {
  string v1, v2;
  int weight;

  bool operator <(const Edge &otherEdge) const {
    return this->weight < otherEdge.weight;
  }

  Edge(string inV1, string inV2, int inWeight): v1(inV1), v2(inV2), weight(inWeight) {}
  Edge() {}
};

class Graph {
private:
  vector<string> vertices;
  vector<Edge> edges;

  /**
   * These data structures exist so we
   * can form disjoint sets for Kruskal's
   * algorithm.
   */
  unordered_map<string, string> parentMap;
  unordered_map<string, int> depthMap;

  /**
   * Find method is designed to take in
   * some vertex, and find the root of the
   * disjoint set the vertex is in.
   */
  string findRoot(string vertex) {
    string returnRoot = vertex;

    // Keep climbing up ancestral chain until we find the root
    while (parentMap[returnRoot] != returnRoot) {
      returnRoot = parentMap[returnRoot];
    }

    return returnRoot;
  }

  /**
   * Union method is designed to merge
   * two disjoint sets. How we merge two
   * sets/trees/graphs will affect our perf
   * we so we want to make the more shallow
   * tree a child of the deeper tree.
   */
  void mergeTrees(string root1, string root2) {
    if (depthMap[root1] > depthMap[root2]) {
      parentMap[root2] = root1;
    } else if (depthMap[root1] < depthMap[root2]) {
      parentMap[root1] = root2;
    } else {
      parentMap[root1] = root2;
      depthMap[root2]++;
    }
  }

  void createDisjointSets() {
    for (int i = 0; i < vertices.size(); ++i) {
      parentMap[vertices[i]] = vertices[i];
      depthMap[vertices[i]] = 0;
    }
  }

public:
  Graph(vector<string>& inVertices, vector<Edge>& inEdges): vertices(inVertices), edges(inEdges) {}

  void sortEdges() {
    sort(this->edges.begin(), this->edges.end());
  }

  void printEdges() {
    for (int i = 0; i < this->edges.size(); ++i) {
      cout << this->edges[i].weight << " " << this->edges[i].v1 << " " << this->edges[i].v2 << endl;
    }
  }

  /**
   * Kruskal's algorithm to find the minimum
   * cost spanning tree of a graph utilizes a
   * sorting algorithm and disjoint sets to compute
   * the minimum cost spanning tree.
   * Time complexity: O(e*log(e))
   * Space complexity: O(v + e)
   */
  vector<Edge> kruskal() {
    vector<Edge> minimumCostSpanningTree;
    this->sortEdges();
    this->createDisjointSets();

    string root1, root2;
    for (int i = 0; i < edges.size(); ++i) {
      root1 = findRoot(edges[i].v1);
      root2 = findRoot(edges[i].v2);

      /**
       * If root1 and root2 are not in the same
       * set, join them. If they are we don't need
       * to push this edge to the minimum cost spanning
       * tree. Regardless we know that the edge linking
       * root1's root with root2's root we come across first
       * is the shortest, since we sorted mate!
       */
      if (root1 != root2) {
        minimumCostSpanningTree.push_back(edges[i]);
        mergeTrees(root1, root2);
      }
    }

    return minimumCostSpanningTree;
  }

  /**
   * This version of Kruskal's algorithm assumes
   * the edges presented to it are in sorted order
   */
  vector<Edge> kruskalNoSort() {
    vector<Edge> minimumCostSpanningTree;
    this->createDisjointSets();

    string root1, root2;
    for (int i = 0; i < edges.size(); ++i) {
      root1 = findRoot(edges[i].v1);
      root2 = findRoot(edges[i].v2);

      /**
       * If root1 and root2 are not in the same
       * set, join them. If they are we don't need
       * to push this edge to the minimum cost spanning
       * tree. Regardless we know that the edge linking
       * root1's root with root2's root we come across first
       * is the shortest, since we sorted mate!
       */
      if (root1 != root2) {
        minimumCostSpanningTree.push_back(edges[i]);
        mergeTrees(root1, root2);
      }
    }

    return minimumCostSpanningTree;
  }

    vector<Edge> prim(int vertex) {
        vector<Edge> E1;
        vector<string> V1;

        //add a vertex to V1 to start the process
        V1.push_back(this->vertices[vertex]);

        for(int n = 0; n < this->edges.size()-1; n++){
            //find an edge in this->edges of min cost that connects vertex in V1 to a vertex not in V1

            vector<Edge> bridgeEdges;

            //of all edges in graph, select ones with one vertex in V1 and one not in V1
            for(int i = 0; i < this->edges.size(); i++){
                    //if the edge has got one foot in and one foot out
                if((find(V1.begin(),V1.end(),this->edges[i].v1) != V1.end() && find(V1.begin(),V1.end(),this->edges[i].v2)==V1.end()) || (find(V1.begin(),V1.end(),this->edges[i].v1) == V1.end() && find(V1.begin(),V1.end(),this->edges[i].v2)!= V1.end())){
                    //add it to bridge edges (shake it all about)
                    bridgeEdges.push_back(this->edges[i]);
                }
            }

            if(bridgeEdges.size()>0){

                //get min cost edge
                int minLoc = 0;
                for(int i = 0; i < bridgeEdges.size(); i++){
                    minLoc = bridgeEdges[i] < bridgeEdges[minLoc] ? i : minLoc;
                }
                E1.push_back(bridgeEdges[minLoc]);

                //add the vertex that is not in V1 already
                if(find(V1.begin(),V1.end(),bridgeEdges[minLoc].v1) != V1.end()){
                    V1.push_back(bridgeEdges[minLoc].v2);
                }else{
                    V1.push_back(bridgeEdges[minLoc].v1);
                    }
            }
        }
        return E1;

    }

};

int main() {
  const int NUM_RUNS = 10000;
  int numVertices, numEdges;
  cout<< "num vertices: ";
  cin >> numVertices;
  cout << "num edges: ";
  cin >> numEdges;
  vector<string> vertices(numVertices);
  vector<Edge> edges(numEdges);

  /**
   * Process the input of vertices
   */
  for (int i = 0; i < numVertices; ++i) {
    cout<<"input vertices: ";
    cin >> vertices[i];
  }

  /**
   * Process the input of edges
   */
  for (int i = 0; i < numEdges; ++i) {
    cout<<"input edge "<<i+1<<" start vertex: ";
    cin >> edges[i].v1;
    cout<<"input edge "<<i+1<<" end vertex: ";
    cin>> edges[i].v2;
    cout<<"input edge " << i+1<<" weight: ";
    cin >> edges[i].weight;
  }

  /**
   * Talk to Graph class to run and time Kruskal
   */
  Graph G(vertices, edges);
  vector<Edge> minimumCostSpanningTree;
  clock_t clock_kruskal_start =  clock();
  for (int i = 0; i < NUM_RUNS; i++) {
     minimumCostSpanningTree = G.kruskal();
  }
  clock_t clock_kruskal_end = clock();
  clock_t clock_kruskal_result = (clock_kruskal_end - clock_kruskal_start);

  G.sortEdges();
  clock_t clock_kruskalNoSort_start =  clock();
  for (int i = 0; i < NUM_RUNS; i++) {
     minimumCostSpanningTree = G.kruskalNoSort();
  }
  clock_t clock_kruskalNoSort_end = clock();
  clock_t clock_kruskalNoSort_result = (clock_kruskalNoSort_end - clock_kruskalNoSort_start);

  // Print minimum spanning tree A
  cout<<endl<<"printing out kruskal result..."<<endl;
  for (int i = 0; i < minimumCostSpanningTree.size(); ++i) {
    cout << minimumCostSpanningTree[i].v1 << " -- " << minimumCostSpanningTree[i].v2 << " " << minimumCostSpanningTree[i].weight << endl;
  }

  /**
   * Talk to Graph Class to run and time Prim
   */
  clock_t clock_prim_sum;
  for (int i = 0; i <numVertices; i++) {
    clock_t clock_prim_start = clock();
    for (int j = 0; j < NUM_RUNS; j++) {
      minimumCostSpanningTree = G.prim(i);
    }
    clock_t clock_prim_end = clock();
    clock_prim_sum += clock_prim_end - clock_prim_start;
  }
  clock_t clock_prim_result = clock_prim_sum / numVertices;

  // Print minimum spanning tree B
  cout<<endl<<"printing out prim result..."<<endl;
  for (int i = 0; i < minimumCostSpanningTree.size(); ++i) {
    cout << minimumCostSpanningTree[i].v1 << " -- " << minimumCostSpanningTree[i].v2 << " " << minimumCostSpanningTree[i].weight << endl;
  }


  long double timeInSeconds = (clock_kruskal_result / (long double) CLOCKS_PER_SEC);
  cout << "---------------------------------------------------------" << endl;
  cout << "Kruskal Time in Seconds for " << NUM_RUNS << " runs: " << timeInSeconds << endl;
  cout << "---------------------------------------------------------" << endl;

  timeInSeconds = (clock_kruskalNoSort_result / (long double) CLOCKS_PER_SEC);
  cout << "---------------------------------------------------------" << endl;
  cout << "Kruskal No Sort Time in Seconds for " << NUM_RUNS << " runs: " << timeInSeconds << endl;
  cout << "---------------------------------------------------------" << endl;

  timeInSeconds = (clock_prim_result / (long double) CLOCKS_PER_SEC);
  cout << "---------------------------------------------------------" << endl;
  cout << "Prim Time in Seconds for " << NUM_RUNS << " runs averaged: " << timeInSeconds << endl;
  cout << "---------------------------------------------------------" << endl;

  return 0;
}


/******************************************************************************************************************
 Analyis - Dom Farolino, Kurt Lewis, Zachary Sang

Analysis for the different algorithms over their respective scenarios was done over the course of 10,000 runs of the algorithms. This accounts for 
random interefernce, and gives a more clear distinction between the different algorithms.

---------------------------------------------------------     
Kruskal Time in Seconds for 10000 runs: 0.315945              
---------------------------------------------------------     
---------------------------------------------------------     
Kruskal No Sort Time in Seconds for 10000 runs: 0.29184       
---------------------------------------------------------     
---------------------------------------------------------     
Prim Time in Seconds for 10000 runs averaged: 1.93304         
---------------------------------------------------------  

The Kruskal algorithm is considerably faster than the prim algorithm, even with its sort. It averages being 6 times vaster

Kruskal's algorithm runs in O(n*log(n)) time. Kruskal's algorithm includes a quicksort, which runs in O(n*log(n)) time. With sorting Kruskal's algorithm is O(2*(n*log(n))), but the constant is ignored in calculation of time complexity.
 Prim's algorithm runs in O(n^2) time. In both cases, n is the number of edges.

*********************************************************************************************************************/