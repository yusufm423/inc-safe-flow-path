#include <bits/stdc++.h>
using namespace std;

struct pathinfo
{
    vector<vector<int>> safety;
};

struct TrieNode *getNode();

struct TrieNode
{
    vector<TrieNode *> children;
    int data;
    int rank;
    bool isEndOfWord;
} *root = getNode();

struct TrieNode *getNode(void)
{
    struct TrieNode *pNode = new TrieNode;

    pNode->isEndOfWord = false;
    pNode->data = -1;
    pNode->rank = -1;

    return pNode;
}

void trie_insert(struct TrieNode *root, vector<int> key, int rank)
{
    struct TrieNode *pCrawl = root;

    for (int i = 0; i < key.size(); i++)
    {
        int flag = 0;
        for (int j = 0; j < pCrawl->children.size(); j++)
        {
            if (pCrawl->children[j]->data == key[i])
            {
                flag = 1;
                pCrawl = pCrawl->children[j];
                break;
            }
        }
        if (flag == 0)
        {
            struct TrieNode *temp = getNode();
            pCrawl->children.push_back(temp);
            pCrawl->children[pCrawl->children.size() - 1]->data = key[i];
            pCrawl = temp;
        }
    }

    // mark last node as leaf
    pCrawl->isEndOfWord = true;
    pCrawl->rank = rank;
}

int trie_search(struct TrieNode *root, vector<int> key)
{
    struct TrieNode *pCrawl = root;

    for (int i = 0; i < key.size(); i++)
    {
        int flag = 0;
        for (int j = 0; j < pCrawl->children.size(); j++)
        {
            if (pCrawl->children[j]->data == key[i])
            {
                flag = 1;
                pCrawl = pCrawl->children[j];
                break;
            }
        }
        if (flag == 0)
        {
            return -1;
        }
    }
    if (pCrawl->isEndOfWord != true)
    {
        return -1;
    }
    return pCrawl->rank;
}

void dfs(vector<vector<int>> &g, vector<vector<int>> &flows, vector<int> &path, map<pair<int, int>, int> &w, int u, int min)
{
    while (1)
    {
        if (u == g.size() - 1)
        {
            path.push_back(u);
            trie_insert(root, path, flows.size());
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

void flowDecomp(vector<vector<int>> &g, vector<vector<int>> &flows, vector<int> &path, map<pair<int, int>, int> w, int u)
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

int binarySearch(vector<int> &nums, int target)
{
    if (nums.empty())
        return -1;
    int l = 0, r = nums.size() - 1;
    while (l < r)
    {
        int mid = l + (r - l) / 2;
        if (nums[mid] == target)
            return mid + 1;
        else if (nums[mid] < target)
            l = mid + 1;
        else
            r = mid - 1;
    }
    if (nums[l] > target)
        return l;
    return r + 1;
}

void calculate_safe_static(vector<int> &path, map<pair<int, int>, int> &w, int outdegree[], pathinfo *p)
{
    int l = 0, r = 0;
    int fp = 0;
    fp = w[{path[l], path[l + 1]}];
    r++;
    while (r < path.size())
    {
        while (fp > 0 && r < path.size())
        {
            fp = fp - (outdegree[path[r]] - w[{path[r], path[r + 1]}]);
            r++;
        }
        {
            int t = r - 1;
            p->safety.push_back(vector<int>{l, t, fp});
        }

        while (fp <= 0 && r <= path.size() - 1)
        {
            l++;
            fp = fp - (w[{path[l - 1], path[l]}] - (outdegree[path[l]] - w[{path[l], path[l + 1]}])) + w[{path[l], path[l + 1]}];
        }
    }
}

void check_for_new_path(vector<int> &path, map<pair<int, int>, int> &w, int outdegree[], pathinfo *p, int safepathno)
{
    int l = p->safety[safepathno][0];
    int r = p->safety[safepathno][1] + 1;
    int fp = p->safety[safepathno][2];
    while (r < path.size() && fp <= 0)
    {
        if (l == r - 1)
            return;

        while (fp <= 0 && l < path.size() - 1)
        {
            l++;
            fp = fp - (w[{path[l - 1], path[l]}] - (outdegree[path[l]] - w[{path[l], path[l + 1]}])) + w[{path[l], path[l + 1]}];
        }
        fp = fp - (outdegree[path[r]] - w[{path[r], path[r + 1]}]);
        r++;
        while (fp > 0 && r < path.size())
        {
            fp = fp - (outdegree[path[r]] - w[{path[r], path[r + 1]}]);
            r++;
        }
        // if <l,r-1> doesn't exist in safety report it, else no new paths emerged
        bool flag = 0;
        for (auto x : p->safety)
        {
            if (x[0] == l && x[1] == r - 1)
            {
                flag = 1;
                break;
            }
        }

        if (!flag)
        {
            p->safety.push_back(vector<int>{l, r - 1, fp});
            continue;
        }
        return;
    }
}

void right_extension(vector<int> &path, map<pair<int, int>, int> &w, int outdegree[], pathinfo *p, int safepathno, int b)
{
    int l = p->safety[safepathno][0];
    int r = p->safety[safepathno][1] + 1;
    int fp = p->safety[safepathno][2];

    fp += b;
    while (fp > 0 && r < path.size())
    {
        fp = fp - (outdegree[path[r]] - w[{path[r], path[r + 1]}]);
        r++;
    }

    p->safety[safepathno][1] = r - 1;
    p->safety[safepathno][2] = fp;
}

bool check_redundancy(pathinfo *p, int safepathno)
{
    int l = p->safety[safepathno][0];
    int r = p->safety[safepathno][1] + 1;
    int fp = p->safety[safepathno][2];

    int temp = p->safety.size() - 1;
    while (temp > safepathno)
    {
        if (p->safety[temp][1] == r - 1)
        {
            if (p->safety[temp][0] < l)
            {
                p->safety.erase(p->safety.begin() + safepathno);
                return 1;
            }
            else if (p->safety[temp][0] >= l)
            {
                p->safety.erase(p->safety.begin() + temp);
            }
        }
        temp--;
    }
    return 0;
}

void divergence(vector<int> &diverge, vector<int> currpath, map<int, pair<int, int>> &present)
{
    int t, flag = 0;
    for (int i = 1, t = 1; i < currpath.size() - 1; i++)
    {
        if (present[currpath[i]].first == 1)
        {
            if (present[currpath[i]].second == currpath[i + 1])
            {
                flag = 1;
            }
            else
            {
                // diverges here
                flag = 0;
                diverge.push_back(i);
            }
        }
    }
}

void new_subpath(vector<int> &diverge, vector<int> path, map<pair<int, int>, int> &w, int outdegree[], pathinfo *p, int loc, int &l, int &r, int &fp)
{
    while (fp <= 0 && r <= loc)
    {
        if (l == r - 1)
            return;

        while (fp <= 0 && l < r)
        {
            l++;
            fp = fp - (w[{path[l - 1], path[l]}] - (outdegree[path[l]] - w[{path[l], path[l + 1]}])) + w[{path[l], path[l + 1]}];
        }
        fp = fp - (outdegree[path[r]] - w[{path[r], path[r + 1]}]);
        r++;

        while (fp > 0 && r < path.size())
        {
            fp = fp - (outdegree[path[r]] - w[{path[r], path[r + 1]}]);
            r++;
        }

        {
            int t = r - 1;
            p->safety.push_back(vector<int>{l, t, fp});
        }
    }
    check_for_new_path(path, w, outdegree, p, p->safety.size() - 1);
}

int handle_intersectingpath(vector<int> &diverge, vector<int> path, map<int, pair<int, int>> &present, map<pair<int, int>, int> &w, int outdegree[], pathinfo *p, int safepathno, int b)
{
    int l = p->safety[safepathno][0];
    int r = p->safety[safepathno][1] + 1;
    int fp = p->safety[safepathno][2];

    if (diverge.empty())
    {
        if (present[path[l]].first == 1 && present[path[l + 1]].first == 1 && present[path[r]].first == 1)
        {
            // right extension(merging of safepaths)
            fp += b;
            while (fp > 0 && r < path.size())
            {
                fp = fp - (outdegree[path[r]] - w[{path[r], path[r + 1]}]);
                r++;
            }
            p->safety[safepathno][0] = l;
            p->safety[safepathno][1] = r - 1;
            p->safety[safepathno][2] = fp;
        }
        // else no change in safe path (completely diff route)
        return 0;
    }
    int l_ = binarySearch(diverge, l);
    int r_ = binarySearch(diverge, r - 1);

    if (present[path[l + 1]].first == 1 && present[path[l]].first == 1)
    {
        if (r_ - l_ == 1)
        {
            return 0;
        }
        fp += b;
    }
    fp = fp - (r_ - l_) * b;
    if (r_ == l_ && present[path[l + 1]].first != 1)
    {
        return 0;
    }

    if ((fp > 0 && p->safety[safepathno][2] > 0) || (fp <= 0 && p->safety[safepathno][2] < 0))
    {
        if (fp <= 0 && p->safety[safepathno][2] <= 0)
        {
            if (fp + (outdegree[path[r - 1]] - w[{path[r - 1], path[r]}]) <= 0)
            {
                // path has shrunk
                int temp = r;
                int flag = 0;

                while (fp <= 0)
                {
                    fp = fp + (outdegree[path[temp - 1]] - w[{path[temp - 1], path[temp]}]);
                    temp--;
                }
                temp++;
                fp = fp - (outdegree[path[temp - 1]] - w[{path[temp - 1], path[temp]}]);

                if (r != temp)
                {
                    p->safety.erase(p->safety.begin() + safepathno);
                    flag = 1;
                }

                // new path <l,temp-1>
                {
                    int t = temp - 1;
                    p->safety.push_back(vector<int>{l, t, fp});
                }

                new_subpath(diverge, path, w, outdegree, p, diverge[r_ - 1], l, temp, fp);
                return flag;
            }
            p->safety[safepathno][2] = fp;
            check_for_new_path(path, w, outdegree, p, safepathno);
        }
        else
        {
            p->safety[safepathno][2] = fp;
            return 0;
        }
    }
    else
    {
        if (fp > 0)
        {
            p->safety[safepathno][2] = fp;
            right_extension(path, w, outdegree, p, safepathno, 0);
        }
        else
        {
            // path has shrunk
            int temp = r;
            int flag = 0;

            while (fp <= 0)
            {
                fp = fp + (outdegree[path[temp - 1]] - w[{path[temp - 1], path[temp]}]);
                temp--;
            }
            temp++;
            fp = fp - (outdegree[path[temp - 1]] - w[{path[temp - 1], path[temp]}]);

            if (r != temp)
            {
                p->safety.erase(p->safety.begin() + safepathno);
                flag = 1;
            }

            // new path <l,temp-1>
            {
                int t = temp - 1;
                p->safety.push_back(vector<int>{l, t, fp});
            }

            new_subpath(diverge, path, w, outdegree, p, diverge[r_ - 1], l, temp, fp);
            return flag;
        }
    }
}

void print_safeflows(vector<vector<int>> &flows, vector<pathinfo> &p)
{
    for (int k = 0; k < flows.size(); k++)
    {
        cout << "\n\nk = " << k;
        for (auto x : p[k].safety)
        {
            cout << "\n<" << x[0] << " , " << x[1] << ">";
            cout << "\n"
                 << x[2];
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
    }
    cout << endl;

    vector<vector<int>> flows;
    vector<int> path;

    flowDecomp(g, flows, path, w, 0);

    vector<pathinfo> pathno(flows.size());

    cout << "---------Calculating safe paths using Static Algo---------\n";
    for (int i = 0; i < flows.size(); i++)
    {
        calculate_safe_static(flows[i], w, outdegree, &pathno[i]);
    }

    print_safeflows(flows, pathno);

    // Update
    while (true)
    {
        int exitstatus, b, rank;
        cout << "Select one of the following:\n";
        cout << "0 -> Terminate program\n";
        cout << "1 -> Incrementally compute on a Flow path.\n";

        cin >> exitstatus;
        if (!exitstatus)
        {
            exit(0);
        }

        if (exitstatus == 1)
        {
            // how the input is taken totally depends on the user. Here we assume the user types it in, user may read input from file as well.
            int vsize,temp;
            path.clear();
            cout << "Enter the number of nodes in flow = ";
            cin>>vsize;
            cout << "Enter the flow = ";
            for(int i=0;i<vsize;i++){
                cin>>temp;
                path.push_back(temp);
            }
            
            rank = trie_search(root, path);
            if (rank == -1)
            {
                cout << "This is a new path\n";
                rank = flows.size();
                cout << rank << endl;
                trie_insert(root, path, rank);
                flows.push_back(path);
                pathno.resize(flows.size());
                exitstatus = 2;
            }
            else
            {
                cout << "This is an existing path\n";
                path = flows[rank];
            }
        }
        else
        {
            cout << "Invalid selection\n";
            exit(0);
        }

        cout << "Enter flow increment value (b) = ";
        cin >> b;

        if (rank >= flows.size())
        {
            cout << "Invalid flow selection\n";
            continue;
        }
        if (b < 0)
        {
            cout << "Increment value should be +ve\n";
            continue;
        }

        map<int, pair<int, int>> present;
        int size;
        for (int i = 0; i < path.size() - 1; i++)
        {
            present[path[i]] = {1, path[i + 1]};
            w[{path[i], path[i + 1]}] += b;
            outdegree[path[i]] += b;
        }
        present[path[path.size() - 1]] = {1, -1};

        if (exitstatus == 1)
        {
            size = pathno[rank].safety.size();
            for (int i = 0; i < size; i++)
            {
                if (pathno[rank].safety[i][2] <= 0 && pathno[rank].safety[i][2] > -b)
                {
                    right_extension(flows[rank], w, outdegree, &pathno[rank], i, b);
                }
                else
                {
                    pathno[rank].safety[i][2] += b;
                }
            }
            for (int i = 0; i < pathno[rank].safety.size(); i++)
            {
                check_for_new_path(flows[rank], w, outdegree, &pathno[rank], i);
            }

            for (int i = 0; i < pathno[rank].safety.size(); i++)
            {
                bool flag = check_redundancy(&pathno[rank], i);
                if (flag == 1)
                    i--;
            }
        }
        else if (exitstatus == 2)
        {
            calculate_safe_static(flows[rank], w, outdegree, &pathno[rank]);
        }

        // divergence handling
        for (int k = 0; k < flows.size(); k++)
        {
            if (k == rank)
                continue;
            vector<int> diverge;
            divergence(diverge, flows[k], present);

            size = pathno[k].safety.size();
            for (int i = 0; i < size; ++i)
            {
                int check = handle_intersectingpath(diverge, flows[k], present, w, outdegree, &pathno[k], i, b);
                if (check == 1)
                {
                    i--;
                    size--;
                }
            }
            for (int i = 0; i < pathno[k].safety.size(); ++i)
            {
                bool flag = check_redundancy(&pathno[k], i);
                if (flag == 1)
                    i--;
            }
        }

        print_safeflows(flows, pathno);
    }
}