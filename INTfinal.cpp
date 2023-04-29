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

void calculate_safe_static(vector<int> &path, map<pair<int, int>, int> &w, int outdegree[], pathinfo *p)
{
    int l = 0, r = 0;
    int sR = w[{path[l], path[l + 1]}];
    r++;
    while (r < path.size())
    {
        while (sR > 0 && r < path.size())
        {
            sR = sR - (outdegree[path[r]] - w[{path[r], path[r + 1]}]);
            r++;
        }
        {
            int t = r - 1;
            p->safety.push_back(vector<int>{l, t, sR});
        }

        while (sR <= 0 && r <= path.size() - 1)
        {
            l++;
            sR = sR - (w[{path[l - 1], path[l]}] - (outdegree[path[l]] - w[{path[l], path[l + 1]}])) + w[{path[l], path[l + 1]}];
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

    if (diverge.empty())
    {
        if (present[path[l]].first == 1 && present[path[l + 1]].first == 1 && present[path[r]].first == 1)
        {
            // right extension(merging of safepaths)
            sR += b;
            while (sR > 0 && r < path.size())
            {
                sR = sR - (outdegree[path[r]] - w[{path[r], path[r + 1]}]);
                r++;
            }
            p->safety[safepathno][0] = l;
            p->safety[safepathno][1] = r - 1;
            p->safety[safepathno][2] = sR;
        }
        // else no change in safe path (completely diff route)
        return;
    }
    int l_ = binarySearch(diverge, l);
    int r_ = binarySearch(diverge, r - 1);

    if (present[path[l + 1]].first == 1 && present[path[l]].first == 1 && present[path[l]].second==path[l+1]) //updated edge or not?
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
            // re calculate safe path
            p->safety.clear();
            calculate_safe_static(path, w, outdegree, p);
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
            p->safety[safepathno][2] = sR;
            right_extension(path, w, outdegree, p, safepathno, 0);
        }
        else
        {
            // path has shrunk
            // re calculate safe path
            p->safety.clear();
            calculate_safe_static(path, w, outdegree, p);
        }
    }
}

void print_safeflows(vector<vector<int>> &flows, vector<pathinfo> &p)
{
    for (int k = 0; k < flows.size(); k++)
    {
        cout << "\n\nk = " << k + 1;
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
    vector<vector<int>> master;
    int t = 5;
    while (t--)
    {
        vector<int> time;
        for (int fileno = 0; fileno < 100; fileno++)
        {
            int n;
            // ifstream myfile("0.graph");
            ifstream myfile(to_string(fileno).append(".graph"));
            myfile >> n;
            // cout<<n<<" "<<m<<endl;
            int outdegree[n] = {0};
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
            // cout<<"l";
            // calculate outdegree and indegree
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    outdegree[i] += w[pair<int, int>(i, j)];
                }
                // cout << outdegree[i] << endl;
            }

            vector<vector<int>> flows;
            vector<int> path;

            // ifstream file("0.truth");
            ifstream file(to_string(fileno).append(".truth"));

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

            // cout << "---------Calculating safe paths using Static Algo---------\n";
            for (int i = 0; i < flows.size(); i++)
            {
                calculate_safe_static(flows[i], w, outdegree, &pathno[i]);
            }

            // print_safeflows(flows, pathno);

            // Update

            int exitstatus = 1, b, rank;

            // // Paths printed;
            // cout << "S.no. \tFlow\n";
            // for (int x = 0; x < flows.size(); x++)
            // {
            //     cout << x << "\t";
            //     for (auto i : flows[x])
            //     {
            //         cout << i << " ";
            //     }
            //     cout << endl;
            // }

            // path = {0, 1, 7, 16, 9, 51};

            // cout << "Update\n";

            // ifstream ufile("0.txt"); // Open the file in read mode
            ifstream ufile(to_string(fileno).append(".txt")); // Open the file in read mode

            if (!ufile.is_open())
            {
                // std::cout << "Failed to open file\n";
                return 1;
            }

            string line;
            getline(ufile, line);  // Read the first line into a string
            stringstream ss(line); // Create a stringstream object to parse the string
            // vector<int> path;
            path.clear();

            int num;
            while (ss >> num)
            { // Extract integers from the stringstream and store in a vector
                path.push_back(num);
            }

            ufile >> b; // Read the second line into an integer variable
            // cout << b << endl;

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
            }
            present[path[path.size() - 1]] = {1, -1};
            auto start = high_resolution_clock::now();

            if (exitstatus == 1)
            {
                size = pathno[rank].safety.size();
                for (int i = 0; i < size; i++)
                {
                    if (pathno[rank].safety[i][2] <= 0 && pathno[rank].safety[i][2] > -b)
                    {
                        // right_extension(flows[rank], w, outdegree, &pathno[rank], i, b);
                        pathno[rank].safety.clear();
                        calculate_safe_static(path,w,outdegree,&pathno[rank]);
                        break;
                    }

                    else
                    {
                        pathno[rank].safety[i][2] += b;
                    }
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
                    handle_intersectingpath(diverge, flows[k], present, w, outdegree, &pathno[k], i, b);
                }
            }


            auto stop = high_resolution_clock::now();

            auto duration = duration_cast<microseconds>(stop - start);

            time.push_back(duration.count());
            // cout<<duration.count();

            // redundant removal
            for (int k = 0; k < flows.size(); k++)
            {
                vector<int> check;
                size = pathno[k].safety.size();
                for (int i = 0; i < size; ++i)
                {
                    if (count(check.begin(), check.end(), pathno[k].safety[i][1]))
                    {
                        // remove trivial safe path
                        pathno[k].safety.erase(pathno[k].safety.begin() + i);
                        i--;
                        size--;
                    }
                    else
                    {
                        // new right extension
                        check.push_back(pathno[k].safety[i][1]);
                    }
                }
            }

            // print_safeflows(flows, pathno);
        }
        master.push_back(time);
    }
    fstream f;
    f.open("inter6.csv", ios::out | ios::app);
    for (auto i : master)
    {
        for (auto k : i)
        {
            f << k << ", ";
        }
        f << endl;
    }
    f.close();
}