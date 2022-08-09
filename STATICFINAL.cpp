#include <bits/stdc++.h>
using namespace std;

void dfs(vector<vector<int>> g, vector<vector<int>> &flows, vector<int> &path, map<pair<int, int>, int> &w, int u, int min)
{
    while (1)
    {
        if (u == g.size() - 1)
        {
            path.push_back(u);
            flows.push_back(path);
            for (int i = 0; i < path.size() - 1; i++)
            {
                w[{path[i], path[i + 1]}] -= min;
            }
            path.clear();
            break;
        }
        for (auto i : g[u])
        {
            if (w[{u, i}] > 0)
            {
                path.push_back(u);
                if (min > w[{u, i}])
                    min = w[{u, i}];
                u = i;
                break;
            }
        }
    }
}

void flowDecomp(vector<vector<int>> g, vector<vector<int>> &flows, vector<int> &path, map<pair<int, int>, int> w, int u)
{
    int i = 0;
    while (i < g[0].size())
    {
        int min = INT_MAX;
        dfs(g, flows, path, w, 0, min);
        // cout<< g[0][i]<<endl;
        if (w[{0, g[0][i]}] <= 0)
        {
            i++;
        }
    }
    cout << endl;
}

void calculate_safe(vector<int> path, map<pair<int, int>, int> &w, int outdegree[])
{

    int l = 0, r = 0;
    int sR = 0;
    sR = w[{path[l], path[l + 1]}];
    r++;
    while (r < path.size())
    {
        while (sR > 0 && r < path.size())
        {
            sR = sR - (outdegree[path[r]] - w[{path[r], path[r + 1]}]);
            r++;
        }

        cout << "<" << l << " , " << r - 1 << ">\n";
        cout << sR << "\n";

        while (sR <= 0 && r <= path.size() - 1)
        {
            l++;
            sR = sR - (w[{path[l - 1], path[l]}] - (outdegree[path[l]] - w[{path[l], path[l + 1]}])) + w[{path[l], path[l + 1]}];
            // cout<<"l = "<<l<<"sR = "<<sR<<endl;
        }
    }
    cout << endl;
}

int main()
{
    int n, m;
    ifstream myfile("sampleinput.txt");
    myfile >> n;
    myfile >> m;
    int outdegree[n] = {0};
    map<pair<int, int>, int> w;
    vector<vector<int>> g(n);

    if (myfile.is_open())
    {
        while (!myfile.eof())
        {
            int u, v, weight;
            myfile >> u;
            myfile >> v;
            myfile >> weight;
            g[u].push_back(v);
            w[pair<int, int>(u, v)] = weight;
        }

        myfile.close();
    }
    // calculate outdegree and indegree
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            outdegree[i] += w[pair<int, int>(i, j)];
        }
        // cout << outdegree[i] << endl;
    }
    cout << endl;

    vector<vector<int>> flows;
    vector<int> path;

    flowDecomp(g, flows, path, w, 0);

    cout << "---------Calculating safe paths using Static Algo---------\n";
    for (int i = 0; i < flows.size(); i++)
    {
        cout << "k = " << i << endl;
        calculate_safe(flows[i], w, outdegree);
    }

    // Update
    while (true)
    {
        int exitstatus, b, rank;
        cout << "Select one of the following:\n";
        cout << "0 -> Terminate program\n";
        cout << "1 -> Incrementally compute on an existing Flow path.\n";

        cin >> exitstatus;
        if (!exitstatus)
        {
            exit(0);
        }
        if (exitstatus != 1)
        {
            cout << "Invalid selection\n";
            exit(0);
        }

        // Paths printed;
        cout << "S.no. \t Flow\n";

        for (int x = 0; x < flows.size(); x++)
        {
            cout << x << "\t";
            for (auto i : flows[x])
            {
                cout << i << " ";
            }
            cout << endl;
        }

        cout << "Select an existing path from given flow decompositions = ";
        cin >> rank;
        cout << endl;

        if (rank >= flows.size())
        {
            cout << "Invalid flow selection\n";
            continue;
        }

        cout << "Enter flow increment value (b) = ";
        cin >> b;

        if (b < 0)
        {
            cout << "Increment value should be +ve\n";
            continue;
        }

        path = flows[rank];

        for (int i = 0; i < path.size() - 1; i++)
        {
            w[{path[i], path[i + 1]}] += b;
            outdegree[path[i]] += b;
        }

        cout << "---------Calculating safe paths using Static Algo---------\n";
        for (int i = 0; i < flows.size(); i++)
        {
            cout << "k = " << i << endl;
            calculate_safe(flows[i], w, outdegree);
        }
    }
}