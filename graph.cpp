#include "graph.h"
#include "draw.h"



Graph::Graph()
{

}

int dijkstra(MatrixGraph *graph, int start, int dest) {
    start -= 1; dest -= 1;
    vector<int> L;
    for(int i = 0; i < graph->vex_num; ++i) L.push_back(INT_MAX);
    L.push_back(INT_MAX);
    L[start] = 0;
    set<int> S;
    while(!S.count(dest)) {
        int u = graph->vex_num;
        for(int i = 0; i < graph->vex_num; ++i)
            if(!S.count(i) && L[i] < L[u]) u = i;
        S.insert(u);
        for(int v = 0; v < graph->vex_num; ++v)
            if(!S.count(v) && L[u] + graph->W[u][v] < L[v]) L[v] = L[u] + graph->W[u][v];
    }
    return L[dest];
}

void floyd(MatrixGraph* graph) {
    long V[graph->vex_num][graph->vex_num];
    for(int i = 0; i < graph->vex_num; ++i)
        for(int j = 0; j < graph->vex_num; ++j)
            V[i][j] = graph->W[i][j];
    for(int k = 0; k < graph->vex_num; ++k) {
        for(int i = 0; i < graph->vex_num; ++i) {
            for(int j = 0; j < graph->vex_num; ++j) {
                V[i][j] = std::min(V[i][j], graph->W[i][k] + graph->W[k][j]);
            }
        }
    }
    for(int i = 0; i < graph->vex_num; ++i) {
        for(int j = 0; j < graph->vex_num; ++j) {
            cout << (V[i][j] != INT_MAX ? to_string(V[i][j]) : "∞") << '\t';
        }
        cout << endl;
    }
}

template<typename F>
void BFS(MatrixGraph* g, int s, F f) {
    s -= 1;
    if(g->vex_num == 0) return;
    vector<bool> flags(g->vex_num);
    for(int i = 0; i < g->vex_num; ++i) flags[i] = false;
    queue<int> Q;
    Q.push(s);
    flags[s] = true;
    while(!Q.empty()) {
        int v = Q.front();
        Q.pop();
        f(g->node_name[v]);
        for(int i = 0; i < g->vex_num; ++i)
            if(g->M[v][i] && flags[i] == false) {
                Q.push(i);
                flags[i] = true;
            }
    }
}



template<typename F>
void DFS(MatrixGraph* g, int s, F f, vector<bool>& flags) {
    f(g->node_name[s]);
    flags[s] = true;
    for(int i = 0; i < g->vex_num; ++i) {
        if(g->M[s][i] && flags[i] == false) {
            DFS(g, i, f, flags);
        }
    }
}

void DFS(MatrixGraph *g, int s) {
    s -= 1;
    if(g->vex_num == 0) return;
    vector<bool> flags(g->vex_num);
    for(int i = 0; i < g->vex_num; ++i) flags[i] = false;
    DFS(g, s, [](const QString& str) {
        std::cout << str.toStdString() << std::endl;
        }, flags);
}

void BFS(MatrixGraph *g, int s) {
    BFS(g, s, [](QString arg){ cout << arg.toStdString() << ' '; });
    cout << endl;
}

template<typename F>
void BFS(AdjGraph *g, int s, F f)
{
    s -= 1;
    if(g->size() == 0) return;
    vector<bool> flags(g->size());
    for(size_t i = 0; i < g->size(); ++i) flags[i] = false;
    queue<int> Q;
    Q.push(s);
    flags[s] = true;
    while (!Q.empty()) {
        int v = Q.front();
        Q.pop();
        f(v + 1);
        for(size_t i = 0; i < g->size(); ++i) {
            if(g[v].find(i) != g[v].end() && flags[i] == false) {
                Q.push(i);
                flags[i] = true;
            }
        }
    }
}

void getGraphInfo(MatrixGraph *graph)
{
        cout << "邻接矩阵：" << std::endl;
        if(graph->has_weight) {
            for(int i = 0; i < graph->vex_num; ++i) {
                for(int j = 0; j < graph->vex_num; ++j) {
                    cout << (graph->W[i][j] != INT_MAX ? QString::number(graph->W[i][j]).toStdString() : QString("∞").toStdString()) << ' ';
                }
                cout << std::endl;
            }
            cout << std::endl;
        } else {
            for(int i = 0; i < graph->vex_num; ++i) {
                for(int j = 0; j < graph->vex_num; ++j) {
                    cout << graph->M[i][j] << ' ';
                }
                cout << std::endl;
            }
            cout << std::endl;
        }
        QString* s_outedge = new QString();
        QString* s_inedge = new QString();
        QTextStream ss1(s_outedge);
        QTextStream ss2(s_inedge);
        for(int i = 0; i < graph->vex_num; ++i) {
            int in_degree = 0;
            int out_degree = 0;
            list<QString> outedge;
            list<QString> inedge;
            for(int j = 0; j < graph->vex_num; ++j) {
                for(int k = 0; k < graph->vex_num; ++k) {
                    if(graph->M[j][k] && k == i) {
                        in_degree++;
                        inedge.push_back(graph->node_name[j]);
                    }
                    if(graph->M[j][k] && j == i) {
                        out_degree++;
                        outedge.push_back(graph->node_name[k]);
                    }
                }
            }
            if(graph->has_direct)
                cout << graph->node_name[i].toStdString() << " 入度：" << in_degree << " 出度：" << out_degree << std::endl;
            else
                cout << graph->node_name[i].toStdString() << " 度：" << in_degree << std::endl;
            ss1 << graph->node_name[i] << "->";
            for(auto n : outedge) {
                ss1 << n << "->";
            }
            ss1 << "^" << ::endl;
            ss2 << graph->node_name[i] << "->";
            for(auto n : inedge) {
                ss2 << n << "->";
            }
            ss2 << "^" << ::endl;
        }
        if(graph->has_direct) {
            cout << "正邻接链表：" << std::endl;
            cout << s_outedge->toStdString() << std::endl;
            cout << "逆邻接链表：" << std::endl;
            cout << s_inedge->toStdString() << std::endl;
        } else {
            cout << "邻接表：" << std::endl;
            cout << s_outedge->toStdString() << std::endl;
        }
}

QDataStream &operator <<(QDataStream &out, MatrixGraph &g) {
    out << g.vex_num << g.has_direct << g.has_weight;
    for(int i = 0; i < MAX_V_NUM; ++i) {
        for(int j = 0; j < MAX_V_NUM; ++j) {
            out << g.M[i][j];
        }
    }

    for(int i = 0; i < MAX_V_NUM; ++i) {
        for(int j = 0; j < MAX_V_NUM; ++j) {
            out << static_cast<qlonglong>(g.W[i][j]);
        }
    }
    for(int i = 0; i < g.vex_num; ++i) {
        out << g.node_name[i];
    }
    return out;
}

QDataStream &operator >>(QDataStream &in, MatrixGraph &g) {
    in >> g.vex_num >> g.has_direct >> g.has_weight;
    for(int i = 0; i < MAX_V_NUM; ++i) {
        for(int j = 0; j < MAX_V_NUM; ++j) {
            in >> g.M[i][j];
        }
    }

    for(int i = 0; i < MAX_V_NUM; ++i) {
        for(int j = 0; j < MAX_V_NUM; ++j) {
            qlonglong w;
            in >> w;
            g.W[i][j] = w;
        }
    }
    for(int i = 0; i < g.vex_num; ++i) {
        QString name;
        in >> name;
        g.node_name[i] = name;
    }
    return in;
}

MatrixGraph* prim(MatrixGraph& g){
    //初始化
    MatrixGraph* ret = new MatrixGraph;
    if(g.vex_num == 0)
        return ret;
    ret->has_direct = false;
    ret->has_weight = true;
    ret->node_name = g.node_name;
    ret->vex_num = g.vex_num;
    long long total_cost = 0;
    long lowcost[g.vex_num];
    int towhom[g.vex_num];
    set<int> U;
    U.insert(0);
    cout << "Added " << g.node_name[0].toStdString() << endl;
    int v = 0;
    lowcost[0] = 0;
    towhom[0] = -1;
    for(int i = 1; i < g.vex_num; ++i) {
        lowcost[i] = INT_MAX;
        towhom[i] = 0;
    }
    //!更新两个辅助数组
    while ((int)U.size() != g.vex_num) {
        //遍历每个顶点
        for(int i = 0; i < g.vex_num; ++i) {
            //如果已经加入生成树则跳过
            if(U.count(i))
                continue;
            //对于这个顶点中的每一条边
               if(g.M[i][v] && i != v) {
                if(g.W[i][v] < lowcost[i]) {
                    lowcost[i] = g.W[i][v];
                    towhom[i] = v;
                }
            }
        }

       //!选择要新加入的顶点，依据是这一次加进去的cost最小
        int min = INT_MAX;

        for(int i = 0; i < g.vex_num; ++i) {
            //跳过已经加入的点
            if(!U.count(i)) {
                if(lowcost[i] < min) {
                    v = i;
                    min = lowcost[v];
                }
            }
        }

        //加入新边
        ret->M[v][towhom[v]] = true;
        ret->M[towhom[v]][v] = true;
        ret->W[v][towhom[v]] = lowcost[v];
        ret->W[towhom[v]][v] = lowcost[v];
        draw* pDraw = new draw();
        graph_t graph;
        graph.matrixG = ret;
        pDraw->drawGraph(graph, MATRIX_GRAPH, ret->node_name, ret->has_direct, ret->has_weight);
        pDraw->show();
        U.insert(v);
        total_cost += lowcost[v];
        cout << "Added " << g.node_name[v].toStdString() << " Cost " << lowcost[v] << endl;
        towhom[v] = -1;
    }
    cout << "Total cost: " << total_cost << endl;
    return ret;
}

MatrixGraph* kruskal(MatrixGraph& g) {
    MatrixGraph* ret = new MatrixGraph;
    if(g.vex_num == 0)
        return ret;
    ret->has_direct = false;
    ret->has_weight = true;
    ret->node_name = g.node_name;
    ret->vex_num = g.vex_num;
    long long total_cost = 0;
    vector<int> dis_f(g.vex_num);
    for(int i = 0; i < g.vex_num; ++i) {
        dis_f[i] = i;
    }
    std::function<int(int)> dis_find = [&](int t) {
        if(dis_f[t] != t)
            dis_f[t] = dis_find(dis_f[t]);
        return dis_f[t];
    };
    auto dis_union = [&](int t1, int t2) {
        auto t1Root = dis_find(t1);
        auto t2Root = dis_find(t2);
        if(t1Root == t2Root)
            return;
        else
            dis_f[t1Root] = t2Root;
    };
    struct Edge {
        int n1;
        int n2;
        long w;
        bool operator <(const Edge& rhs) const {
            return w < rhs.w;
        }
    };
    vector<Edge> E;
    for(int i = 0; i < g.vex_num; ++i) {
        for(int j = 0; j < g.vex_num; ++j) {
            if(g.M[i][j] && i != j) {
                E.push_back({i,j,g.W[i][j]});
            }
        }
    }
    std::sort(E.begin(), E.end());
    int idx = 0;
    for(auto it = E.begin(); it != E.end(); ++it) {
        Edge e = *it;
        if(dis_find(e.n1) != dis_find(e.n2)) {
            ret->M[e.n1][e.n2] = true;
            ret->M[e.n2][e.n1] = true;
            ret->W[e.n1][e.n2] = e.w;
            ret->W[e.n2][e.n2] = e.w;
            draw* pDraw = new draw();
            graph_t graph;
            graph.matrixG = ret;
            pDraw->drawGraph(graph, MATRIX_GRAPH, ret->node_name, ret->has_direct, ret->has_weight);
            pDraw->show();
            total_cost += e.w;
            dis_union(e.n1, e.n2);
            std::cout << "第" << ++idx << "步：" << "插入边" << g.node_name[e.n1].toStdString() << "---" <<g.node_name[e.n2].toStdString() << "，权重：" << e.w << std::endl;
        }
    }
    return ret;
}

