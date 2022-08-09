// A C++ Program to generate test cases for
// an unweighted directed graph
#include <bits/stdc++.h>
using namespace std;

// Define the number of runs for the test data
// generated
#define RUN 1

// Define the maximum number of vertices of the graph
#define MAX_VERTICES 1000

// Define the maximum number of edges
#define MAX_EDGES 1800

#define MAX_WEIGHT 100

class Graph
{
public:
    int V; // No. of vertices

    // Pointer to an array containing adjacency lists
    list<int> *adj;

    Graph(int); // Constructor

    void addEdge(int, int);

    vector<int> BFS(int, int, int[]);
};

Graph::Graph(int V)
{
    this->V = V;
    adj = new list<int>[V + 1];
}

void Graph::addEdge(int u, int v)
{
    adj[u].push_back(v); // Add w to v’s list.
    // adj[v].push_back(u); // Add v to w’s list.
}

vector<int> Graph::BFS(int componentNum, int src, int visited[])
{
    // Mark all the vertices as not visited
    // Create a queue for BFS
    queue<int> queue;

    queue.push(src);

    // Assign Component Number
    visited[src] = componentNum;

    // Vector to store all the reachable nodes from 'src'
    vector<int> reachableNodes;

    while (!queue.empty())
    {
        // Dequeue a vertex from queue
        int u = queue.front();
        queue.pop();

        reachableNodes.push_back(u);

        // Get all adjacent vertices of the dequeued
        // vertex u. If a adjacent has not been visited,
        // then mark it visited and enqueue it
        for (auto itr = adj[u].begin();
             itr != adj[u].end(); itr++)
        {
            if (!visited[*itr])
            {
                // Assign Component Number to all the
                // reachable nodes
                visited[*itr] = componentNum;
                queue.push(*itr);
            }
        }
    }
    return reachableNodes;
}

// Display all the Reachable Nodes from a node 'n'
void displayReachableNodes(int n, unordered_map<int, vector<int>> m)
{
    vector<int> temp = m[n];
    for (int i = 0; i < temp.size(); i++)
        cout << temp[i] << " ";

    cout << endl;
}

// Find all the reachable nodes for every element in the arr
bool findReachableNodes(Graph g, int a, int b)
{
    // Get the number of nodes in the graph
    int V = g.V;
    int flag = 0;

    // Take a integer visited array and initialize
    // all the elements with 0
    int visited[V + 1];
    memset(visited, 0, sizeof(visited));

    // Map to store list of reachable Nodes for a
    // given node.
    unordered_map<int, vector<int>> m;

    // Initialize component Number with 0
    int componentNum = 0;

    // For each node in arr[] find reachable
    // Nodes

    // Visit all the nodes of the component
    if (!visited[b])
    {
        componentNum++;

        // Store the reachable Nodes corresponding to
        // the node 'i'
        m[visited[b]] = g.BFS(componentNum, b, visited);
    }

    // At this point, we have all reachable nodes
    // from b, print them by doing a look up in map m.
    // cout << "Reachable Nodes from " << b << " are\n";
    // displayReachableNodes(visited[b], m);
    // vector<int> temp = m[visited[b]];
    for (int i = 0; i < m[visited[b]].size(); i++)
    {
        if (m[visited[b]][i] == a)
        {
            flag = 1;
        }
    }
    return flag;
}

void printpath(vector<int> &path)
{
    int size = path.size();
    for (int i = 0; i < size; i++)
        cout << path[i] << " ";
    cout << endl;
}

// utility function to check if current
// vertex is already present in path
int isNotVisited(int x, vector<int> &path)
{
    int size = path.size();
    for (int i = 0; i < size; i++)
        if (path[i] == x)
            return 0;
    return 1;
}

// utility function for finding paths in graph
// from source to destination
void findpaths(Graph &g, map<pair<int, int>, int> &w, int src, int dst)
{
    // create a queue which stores
    // the paths
    queue<vector<int>> q;
    int count = 0;

    // path vector to store the current path
    vector<int> path;
    path.push_back(src);
    q.push(path);
    while (!q.empty() )
    {
        path = q.front();
        q.pop();
        int last = path[path.size() - 1];

        // if last vertex is the desired destination
        // then print the path
        if (last == dst)
        {
            int flag = 1;
            // printpath(path);
            for (auto it : w)
            {
                if (it.second == 0)
                {
                    flag = 0;
                    break;
                }
            }

            if (flag == 0)
            {
                int wt = 1 + rand() % MAX_WEIGHT;
                for (int i = 0; i < path.size() - 1; i++)
                {
                    w[{path[i], path[i + 1]}] += wt;
                }
                // cout << "\t" << wt << endl;
            }

            for (int i = 0; i < path.size() - 1; i++)
            {
                path.pop_back();
            }
        }

        // traverse to all the nodes connected to
        // current vertex and push new path to queue

        for (auto it : g.adj[last])
        {
            if (isNotVisited(it, path))
            {
                if (w[{last, it}] == 0)
                {
                    vector<int> newpath(path);
                    newpath.push_back(it);
                    q.push(newpath);
                }
            }
        }
        count++;
        
    }
}

void BFS(Graph &g, int s, vector<int> &bfs)
{
    // Mark all the vertices as not visited
    vector<bool> visited;
    visited.resize(g.V, false);

    // Create a queue for BFS
    list<int> queue;

    // Mark the current node as visited and enqueue it
    visited[s] = true;
    queue.push_back(s);

    while (!queue.empty())
    {
        // Dequeue a vertex from queue and print it
        s = queue.front();

        // cout << "\n"<< s;
        bfs.push_back(s);

        queue.pop_front();

        // Get all adjacent vertices of the dequeued
        // vertex s. If a adjacent has not been visited,
        // then mark it visited and enqueue it
        for (auto adjecent : g.adj[s])
        {
            if (!visited[adjecent])
            {
                visited[adjecent] = true;
                queue.push_back(adjecent);
            }
        }
    }
}

void print_weights(map<pair<int, int>, int> w, int n)
{
    cout << "\t";
    for (int i = 0; i < n; i++)
        cout << i << "\t";
    cout << endl;
    for (int i = 0; i < n; i++)
    {
        cout << i << "\t";
        for (int j = 0; j < n; j++)
        {
            cout << w[pair<int, int>(i, j)] << "\t";
        }
        cout << endl;
    }
    cout << endl;
}

int main()
{
    set<pair<int, int>> container;
    set<pair<int, int>>::iterator it;
    std::ofstream ofs;
    ofs.open("DAGraph.txt", std::ofstream::out | std::ofstream::trunc);
    ofs.close();

    // Uncomment the below line to store
    // the test data in a file
    freopen("DAGraph.txt", "w", stdout);

    // For random values every time
    srand(time(NULL));

    int NUM;     // Number of Vertices
    int NUMEDGE; // Number of Edges

    // NUM = 1 + rand() % MAX_VERTICES;
    NUM = MAX_VERTICES;
    NUMEDGE = MAX_EDGES;

    vector<bool> sources(NUM, true);
    vector<bool> sink(NUM, true);
    map<pair<int, int>, int> w;
    vector<int> path;

    Graph g(NUM);
    // Define the maximum number of edges of the graph
    // Since the most dense graph can have N*(N-1)/2 edges
    // where N = number of vertices in the graph
    // First print the number of vertices and edges

    // Then print the edges of the form (a b)
    // where 'a' is connected to 'b'
    for (int j = 1; j <= NUMEDGE; j++)
    {
        int a = 1 + rand() % (NUM - 2);
        int b = 1 + rand() % (NUM - 2);
        pair<int, int> p = make_pair(a, b);
        //    cout << a << " " << b << endl;

        // Search for a random "new" edge everytime
        // Note - In a tree the edge (a, b) is same
        // as the edge (b, a)
        while (container.find(p) != container.end() || findReachableNodes(g, a, b) || a == b)
        {
            a = 1 + rand() % (NUM - 2);
            b = 1 + rand() % (NUM - 2);
            p = make_pair(a, b);
        }

        container.insert(p);
        g.addEdge(a, b);
        w[{a, b}] = 0;
    }


    // now check for all sinks and all sources
    for (it = container.begin(); it != container.end(); it++)
    {
        sources[it->second] = false;
        sink[it->first] = false;
    }

    // cout << "\nSources\n";
    for (int i = 1; i < NUM - 1; i++)
    {
        if (sources[i])
        {
            // printf("\n%d %d", 0, i);
            pair<int, int> p = make_pair(0, i);
            g.addEdge(0, i);
            container.insert(p);
        }
    }


    // cout << "\nSink\n";
    for (int i = 1; i < NUM - 1; i++)
    {
        if (sink[i])
        {
            // printf("\n%d %d", i, NUM - 1);
            pair<int, int> p = make_pair(i, NUM - 1);
            g.addEdge(i, NUM - 1);
            container.insert(p);
        }
    }
    printf("%d %d", NUM, container.size());

    findpaths(g, w, 0, NUM - 1);
   
    for (it = container.begin(); it != container.end(); ++it)
    {
        printf("\n%d %d %d", it->first, it->second, w[{it->first, it->second}]);
    }

    // Uncomment the below line to store
    // the test data in a file
    container.clear();
    fclose(stdout);

    return (0);
}
