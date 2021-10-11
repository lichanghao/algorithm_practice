#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

vector<vector<int> > read_data(string fname)
{
    fstream myfile;
    myfile.open(fname, ios::in);

    int n;
    myfile >> n;
    vector<vector<int> > mygraph(2*n, vector<int>());

    for (int i = 0; i < n; i++)
    {
        int a, b;
        myfile >> a; myfile >> b;
        int aflag = (a > 0) ? 1 : 0;
        int bflag = (b > 0) ? 1 : 0;
        mygraph[2*abs(a)-2+aflag].push_back(2*abs(b)-2+!(bflag));
        mygraph[2*abs(b)-2+bflag].push_back(2*abs(a)-2+!(aflag));
    }
    myfile.close();
    return mygraph;
}

vector<vector<int> > reverse_graph(vector<vector<int> > g)
{
    vector<vector<int> > mygraph(g.size(), vector<int>());
    for (int i = 0; i < g.size(); i++)
    {
        for (int j = 0; j < g[i].size(); j++)
        {
            mygraph[g[i][j]].push_back(i);
        }
    }
    return mygraph;
}

int n;
vector<vector<int> > g, gt;
vector<bool> used;
vector<int> order, comp;
vector<bool> assignment;

void dfs1(int v) {
    used[v] = true;
    for (int u : g[v]) {
        if (!used[u])
            dfs1(u);
    }
    order.push_back(v);
}

void dfs2(int v, int cl) {
    comp[v] = cl;
    for (int u : gt[v]) {
        if (comp[u] == -1)
            dfs2(u, cl);
    }
}

bool solve_2SAT() {
    order.clear();
    used.assign(n, false);
    for (int i = 0; i < n; ++i) {
        if (!used[i])
            dfs1(i);
    }

    comp.assign(n, -1);
    for (int i = 0, j = 0; i < n; ++i) {
        int v = order[n - i - 1];
        if (comp[v] == -1)
            dfs2(v, j++);
    }

    assignment.assign(n / 2, false);
    for (int i = 0; i < n; i += 2) {
        if (comp[i] == comp[i + 1])
            return false;
        assignment[i / 2] = comp[i] > comp[i + 1];
    }
    return true;
}

int main()
{
    g = read_data("test6.txt");
    gt = reverse_graph(g);

    n = 2000000;

    bool result = solve_2SAT();
    cout << result << endl;
}