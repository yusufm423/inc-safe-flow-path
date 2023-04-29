#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;

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

void insert(struct TrieNode *root, vector<int> key, int rank)
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
    // cout<<pCrawl->rank<<"\n";
}

void display(struct TrieNode *root)
{
    struct TrieNode *pCrawl = root;

    while (true)
    {
        cout << pCrawl->data << "\t";
        if (pCrawl->isEndOfWord)
        {
            break;
        }
        pCrawl = pCrawl->children[pCrawl->children.size() - 1];
    }
}

int search(struct TrieNode *root, vector<int> key)
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

void calculate_safe_static(vector<int> &path, map<pair<int, int>, int> &w, int outdegree[], int indegree[], pathinfo *p)
{
    int l = 0, r = 0;
    int sR = w[{path[l], path[l + 1]}];
    int sL = 0;
    r++;
    while (r < path.size())
    {
        while (sR > 0 && r < path.size())
        {
            sR = sR - (outdegree[path[r]] - w[{path[r], path[r + 1]}]);
            if (sR > 0 && r != path.size() - 1)
            {
                sL = sL - (indegree[path[r]] - w[{path[r - 1], path[r]}]);
            }
            r++;
        }
        sL = sL + w[{path[r - 2], path[r - 1]}];

        {
            int t = r - 1;
            p->safety.push_back(vector<int>{l, t, sR, sL});
            // cout << "<" << l << "," << t << ">\n";
            // cout << "sR=" << sR << ", sL=" << sL << endl;
        }
        sL = sL - w[{path[r - 2], path[r - 1]}] - (indegree[path[r - 1]] - w[{path[r - 2], path[r - 1]}]);

        while (sR <= 0 && r <= path.size() - 1)
        {
            l++;
            sR = sR - (w[{path[l - 1], path[l]}] - (outdegree[path[l]] - w[{path[l], path[l + 1]}])) + w[{path[l], path[l + 1]}];
            if (l - 2 >= 0)
            {
                sL = sL + (indegree[path[l - 1]] - w[{path[l - 2], path[l - 1]}]);
            }
        }
    }
}

void right_extension(vector<int> &path, map<pair<int, int>, int> &w, int outdegree[], pathinfo *p, int safepathno, int b)
{
    int l = p->safety[safepathno][0];
    int r = p->safety[safepathno][1] + 1;
    int sR = p->safety[safepathno][2];

    sR += b;
    while (sR > 0 && r < path.size())
    {
        sR = sR - (outdegree[path[r]] - w[{path[r], path[r + 1]}]);
        r++;
    }

    p->safety[safepathno][1] = r - 1;
    p->safety[safepathno][2] = sR;
}

void left_extension(vector<int> &path, map<pair<int, int>, int> &w, int indegree[], pathinfo *p, int safepathno, int b)
{
    int l = p->safety[safepathno][0];
    int r = p->safety[safepathno][1] + 1;
    int sL = p->safety[safepathno][3];
    sL += b;
    while (sL > 0 && l > 0)
    {
        l--;
        if (l == 0)
        {
            break;
        }
        sL = sL - (indegree[path[l]] - w[{path[l - 1], path[l]}]);
    }
    p->safety[safepathno][0] = l;
    p->safety[safepathno][3] = sL;
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

void handle_intersectingpath(vector<int> &diverge, vector<int> path, map<int, pair<int, int>> &present, map<pair<int, int>, int> &w, int outdegree[], pathinfo *p, int safepathno, int b)
{
    int l = p->safety[safepathno][0];
    int r = p->safety[safepathno][1] + 1;
    int sR = p->safety[safepathno][2];
    int sL = p->safety[safepathno][3];


    if (diverge.empty())
    {
        if (present[path[l]].first == 1 && present[path[l + 1]].first == 1 && present[path[r-1]].first == 1)
        {
            // right extension(merging of safepaths)
            cout << "<" << l << " , " << r - 1 << "> is has become a trivial safe path(right@ extension)\n";
        }
        else if (present[path[r-1]].first == 1 && present[path[r- 2]].first == 1 && present[path[l]].first == 1 && sL<=0)
        {
            // left extension(merging of safepaths)
            cout << "<" << l << " , " << r - 1 << "> is has become a trivial safe path(left@ extension)\n";
        }
        // else no change in safe path (completely diff route)
        return;
    }
    int l_ = binarySearch(diverge, l);
    int r_ = binarySearch(diverge, r - 1);

    if (present[path[l + 1]].first == 1 && present[path[l]].first == 1 && present[path[l]].second == path[l + 1]) // updated edge or not?
    {
        if (r_ - l_ == 1)
        {
            return;
        }
        sR += b;
    }
    sR = sR - (r_ - l_) * b;
    if (r_ == l_ && present[path[l + 1]].first != 1)
    {
        return;
    }

    if ((sR > 0 && p->safety[safepathno][2] > 0) || (sR <= 0 && p->safety[safepathno][2] < 0))
    {
        if (sR <= 0 && p->safety[safepathno][2] <= 0)
        {
            if (sR + (outdegree[path[r - 1]] - w[{path[r - 1], path[r]}]) <= 0)
            {
                // re calculate safe path
                cout << "<" << l << " , " << r - 1 << "> it4ts not a safe path\n";
            }
        }
        else
        {
            p->safety[safepathno][2] = sR;
            return;
        }
    }
    else
    {
        if (sR > 0)
        {
            cout << "<" << l << " , " << r - 1 << "> is has become a trivial safe path(right!! extension1)\n";
        }
        else if (sL > 0)
        {
            cout << "<" << l << " , " << r - 1 << "> is has become a trivial safe path(left!! extension2)\n";
        }
        else
        {
            // path has shrunk
            // re calculate safe path
            cout << "<" << l << " , " << r - 1 << "> is not safe path\n";
        }
    }
}

int main()
{
    int n;
    ifstream myfile("99.graph");
    // ifstream myfile(to_string(fileno).append(".graph"));
    myfile >> n;
    int outdegree[n] = {0};
    int indegree[n] = {0};

    map<pair<int, int>, int> w;
    // cout<<"j";
    if (myfile.is_open())
    {
        while (!myfile.eof())
        {
            // cout<<"k";
            int u, v;
            float weight;
            myfile >> u;
            myfile >> v;
            myfile >> weight;
            // cout<<u<<" "<<v<<" "<<weight<<endl;
            w[pair<int, int>(u, v)] = (int)weight;
        }
        myfile.close();
    }

    // calculate outdegree and indegree
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            outdegree[i] += w[pair<int, int>(i, j)];
            indegree[i] += w[pair<int, int>(j, i)];
        }
        // cout << outdegree[i] << endl;
    }

    vector<vector<int>> flows;
    vector<int> path;

    ifstream file("99.truth");
    // ifstream file(to_string(fileno).append(".truth"));

    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {                     // read each line of the file
            vector<int> flow; // declare a vector to store the values of each line
            stringstream ss(line);
            int num;
            ss >> num;
            while (ss >> num)
            {                        // read each number in the line
                flow.push_back(num); // add the number to the vector
            }
            insert(root, flow, flows.size());
            flows.push_back(flow); // add the vector to the vector of vectors
        }
        file.close(); // close the file
    }
    else
    {
        // cout << "Error: Unable to open file." << endl;
    }

    vector<pathinfo> pathno(flows.size());

    cout << "---------Calculating safe paths using Static Algo---------\n";
    for (int i = 0; i < flows.size(); i++)
    {
        // cout << "k=" << i + 1 << endl;
        calculate_safe_static(flows[i], w, outdegree, indegree, &pathno[i]);
    }

    // Update

    int exitstatus = 1, b, rank;

    ifstream ufile("99.txt"); // Open the file in read mode
    // ifstream ufile(to_string(fileno).append(".txt")); // Open the file in read mode

    if (!ufile.is_open())
    {
        // std::cout << "Failed to open file\n";
        return 1;
    }

    string line;
    getline(ufile, line);  // Read the first line into a string
    stringstream ss(line); // Create a stringstream object to parse the string
    path.clear();

    int num;
    while (ss >> num)
    { // Extract integers from the stringstream and store in a vector
        path.push_back(num);
    }

    ufile >> b; // Read the second line into an integer variable

    ufile.close(); // Close the file

    rank = search(root, path);
    // cout << rank << endl;

    if (rank >= flows.size())
    {
        // cout << "Invalid flow selection\n";
        exit(0);
    }
    if (b < 0)
    {
        // cout << "Increment value should be +ve\n";
        exit(0);
    }

    if (rank == -1)
    {
        // cout << "This is a new path\n";
        rank = flows.size();
        cout << rank << endl;
        insert(root, path, rank);
        flows.push_back(path);
        pathno.resize(flows.size());
        exitstatus = 2;
    }
    else
    {
        // cout << "This is an existing path\n";
        path = flows[rank];
    }

    map<int, pair<int, int>> present;
    int size;
    for (int i = 0; i < path.size() - 1; i++)
    {
        present[path[i]] = {1, path[i + 1]};
        w[{path[i], path[i + 1]}] += b;
        outdegree[path[i]] += b;
        indegree[path[i + 1]] += b;
    }
    present[path[path.size() - 1]] = {1, -1};

    if (exitstatus == 1)
    {
        cout << "k =" << rank + 1 << "\n";

        size = pathno[rank].safety.size();
        for (int i = 0; i < size; i++)
        {
            if (pathno[rank].safety[i][2] <= 0 && pathno[rank].safety[i][2] > -b)
            {
                // trivial safe path
                cout << "<" << pathno[rank].safety[i][0] << " , " << pathno[rank].safety[i][1] << "> is became trivial safe path(right extension)\n";
            }

            else if (pathno[rank].safety[i][3] <= 0 && pathno[rank].safety[i][3] > -b)
            {
                cout << "<" << pathno[rank].safety[i][0] << " , " << pathno[rank].safety[i][1] << "> is became trivial safe path(left extension)\n";
            }
        }
    }

    // divergence handling
    for (int k = 0; k < flows.size(); k++)
    {
        cout << "k =" << k + 1 << "\n";
        if (k == rank)
            continue;
        vector<int> diverge;
        divergence(diverge, flows[k], present);

        size = pathno[k].safety.size();
        for (int i = 0; i < size; ++i)
        {
            handle_intersectingpath(diverge, flows[k], present, w, outdegree, &pathno[k], i, b);
        }
    }
}