/*
Mohammad Yusuf
*/

#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;

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
        // cout << "Rank=(" << pCrawl->rank << ")\n";
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

        // cout << "<" << l << " , " << r - 1 << ">\n";
        // cout << sR << "\n";

        while (sR <= 0 && r <= path.size() - 1)
        {
            l++;
            sR = sR - (w[{path[l - 1], path[l]}] - (outdegree[path[l]] - w[{path[l], path[l + 1]}])) + w[{path[l], path[l + 1]}];
            // cout<<"l = "<<l<<"sR = "<<sR<<endl;
        }
    }
    // cout << endl;
}

int main()
{
    vector<vector<int>> master;
    int t = 7;
    while (t--)
    {
        vector<int> time;
        for (int fileno = 0; fileno < 100; fileno++)
        {
            int n;

            ifstream myfile(to_string(fileno).append(".graph"));
            // ifstream myfile("0.graph");
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

            ifstream file(to_string(fileno).append(".truth"));
            // ifstream file("0.truth");
            if (file.is_open())
            {
                string line;
                while (getline(file, line))
                {                     // read each line of the file
                    vector<int> flow; // declare a vector to store the values of each line
                    stringstream ss(line);
                    int num;
                    ss >> num; // ignoring the first number
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

            // for (auto& flow : flows) {
            //     for (auto& num : flow) {
            //         cout << num << " ";
            //     }
            //     cout << endl;
            // }

            // cout << "---------Calculating safe paths using Static Algo---------\n";
            for (int i = 0; i < flows.size(); i++)
            {
                // cout << "k = " << i + 1 << endl;
                calculate_safe(flows[i], w, outdegree);
            }

            // display(root);

            // Update
            // cout << "Update\n";

            ifstream ufile(to_string(fileno).append(".txt")); // Open the file in read mode
            // std::ifstream ufile("0.txt"); // Open the file in read mode
            if (!ufile.is_open())
            {
                // std::cout << "Failed to open file\n";
                return 1;
            }

            string line;
            getline(ufile, line);  // Read the first line into a string
            stringstream ss(line); // Create a stringstream object to parse the string
            // vector<int> path;

            int num;
            while (ss >> num)
            { // Extract integers from the stringstream and store in a vector
                path.push_back(num);
            }

            int b;
            ufile >> b;    // Read the second line into an integer variable
                           // cout<<b<<endl;
            ufile.close(); // Close the file

            /// trie datastructure to find if it exists in flow decomposition

            int rank_ = search(root, path);
            // cout << rank_ << endl;
            // for (auto i : path)
            // {
            //     cout << i << " ";
            // }
            // cout << endl;
            if (rank_ == -1)
            {
                // cout << "This is a new path\n";
                rank_ = flows.size();
                insert(root, path, rank_);
                // cout << rank_ << endl;
                flows.push_back(path);
            }
            else
            {
                // cout << "This is an existing path\n";
                path = flows[rank_];
            }

            path = flows[rank_];

            for (int i = 0; i < path.size() - 1; i++)
            {
                w[{path[i], path[i + 1]}] += b;
                outdegree[path[i]] += b;
            }
            auto start = high_resolution_clock::now();
            // cout << "---------Calculating safe paths using Static Algo---------\n";
            for (int i = 0; i < flows.size(); i++)
            {
                // cout << "\nk = " << i + 1 << endl;
                calculate_safe(flows[i], w, outdegree);
            }
            auto stop = high_resolution_clock::now();

            auto duration = duration_cast<microseconds>(stop - start);

            time.push_back(duration.count());

            // Paths printed;
            // cout << "S.no. \t Flow\n";

            // for (int x = 0; x < flows.size(); x++)
            // {
            //     cout << x+1 << "\t";
            //     for (auto i : flows[x])
            //     {
            //         cout << i << " ";
            //     }
            //     cout << endl;
            // }
            /**/
        }

        master.push_back(time);
    }
    fstream f;
    f.open("sta6.csv", ios::out | ios::app);
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