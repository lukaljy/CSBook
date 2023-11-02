#include <iostream>
#include <queue>
#include <string.h>
#include <random>
using namespace std;
#define MAX_NODES 2100
#define MAX_EDGE 5000000
const long long INF = 1000000000;
struct Edge
{
    int src;
    int dst;
    long long weight;
    int next;//下一条连接相同起点的边
    Edge() {}
    Edge(int s, int d, int w, int n) : src(s), dst(d), weight(w), next(n) { }
};
int cnt = 1;
int n, m, s, t;

int head[MAX_NODES] = { 0 };//每个点连接的第一条边
int edge_cur[MAX_NODES] = { 0 };//dfs遍历时每个点所遍历的边
int depth[MAX_NODES] = { 0 };
Edge edges[MAX_EDGE];

void AddEdge(int src, int dst, int weight)
{
    edges[cnt] = Edge(src, dst, weight, head[src]);
    head[src] = cnt++;
    edges[cnt] = Edge(dst, src, 0, head[dst]);
    head[dst] = cnt++;
}


long long dfs(int u, long long flow)
{
    if (u == t) {
        return flow;
    }
    long long flow_used = 0;
    int i = edge_cur[u];
    while (i != 0)
    {
        edge_cur[u] = i;
        int next = edges[i].dst;
        if (edges[i].weight && depth[next] == (depth[u] + 1)) {
            long long flow_next = dfs(next, std::min(flow - flow_used, edges[i].weight));
            if (flow_next) {
                flow_used += flow_next;
                edges[i].weight -= flow_next;
                edges[i ^ 1].weight += flow_next;
                if (flow_used == flow) {
                    break;
                }
            }
            else {
                depth[next] = INF;
            }
        }
        i = edges[i].next;
    }
    return flow_used;
}

bool bfs()
{
    for (int i = 0; i < n + 1; i++) {
        depth[i] = INF;
        edge_cur[i] = head[i];
    }
    std::queue<int> q;
    q.push(s);
    depth[s] = 0;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int i = head[u]; i != 0; i = edges[i].next) {
            int next = edges[i].dst;
            if (edges[i].weight && depth[next] == INF) {
                depth[next] = depth[u] + 1;
                q.push(next);
                if (next == t) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

long long Dinic()
{
    long long result = 0;
    while (bfs()) {
        result += dfs(s, INF);
    }
    return result;
}

void max_flow_random(int size, int max_weight, int min_weight, int times = 100)
{
    s = 1;
    t = size;
    n = size + 1;
    for (int k = 0; k < times; k++) {
        cnt = 1;
        memset(head, 0, sizeof(head));
        srand((unsigned)time(NULL) + k);
        for (int i = 1; i <= size; i++) {
            for (int j = i + 1; j <= size; j++) {
                int weight = min_weight + ((double)rand() / RAND_MAX) * ((double)rand() / RAND_MAX) * max_weight;
                AddEdge(i, j, weight);
            }
        }
        printf("%lld\n", Dinic());
    }
}

void max_flow()
{
    n = 5;
    m = 6;
    s = 1;
    t = 5;
    AddEdge(1, 2, 20);
    AddEdge(1, 3, 30);
    AddEdge(2, 3, 20);
    AddEdge(3, 5, 40);
    AddEdge(4, 5, 20);
    AddEdge(1, 4, 40);
    cout << Dinic() << endl;
}
void bipartite_random(int size, double p, int times = 100)
{
    n = 2 * (size + 1);
    s = 0;
    t = size * 2 + 1;
    for (int k = 0; k < times; k++) {
        cnt = 2;
        memset(head, 0, sizeof(head));
        srand((unsigned)time(NULL) + k);
        for (int i = 1; i <= size; i++) {
            for (int j = 1; j <= size; j++) {
                if ((double)rand() / RAND_MAX < p) {
                    AddEdge(i, j + size, 1);
                }
            }
        }
        //转换为最大流问题
        for (int i = 1; i <= size; i++) {
            AddEdge(0, i, 1);
        }
        for (int i = 1; i <= size; i++) {
            AddEdge(i + size, size * 2 + 1, 1);
        }
        printf("%lld\n", Dinic());
    }
}
void bipartite()
{
    int left = 4;
    int right = 3;
    n = left + right + 2;
    s = 0;
    t = left + right + 1;
    AddEdge(1, 6, 1);
    AddEdge(2, 5, 1);
    AddEdge(2, 7, 1);
    AddEdge(3, 7, 1);
    AddEdge(4, 7, 1);
    //转换为最大流问题
    for (int i = 1; i <= left; i++) {
        AddEdge(s, i, 1);
    }
    for (int i = 1; i <= right; i++) {
        AddEdge(i + left, t, 1);
    }

    printf("%lld\n", Dinic());
}
int main()
{
    max_flow();
    bipartite();
    bipartite_random(1000, 0.001);
    max_flow_random(1000, 1000, 10);
    return 0;
}




