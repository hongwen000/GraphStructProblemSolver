#ifndef GRAPH_H
#define GRAPH_H

#include <QString>
#include <QTextStream>
#include <QDataStream>
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <climits>
#include <list>
#include <utility>
#include <queue>
#define MATRIX_GRAPH false
#define ADJLIST_GRAPH	true
using namespace std;
//Adjacency Matrix
#define MAX_V_NUM 200
struct MatrixGraph{
    bool has_direct;
    bool has_weight;
    int 	vex_num = 0;
    bool    M[MAX_V_NUM][MAX_V_NUM];
    long 	W[MAX_V_NUM][MAX_V_NUM];
    map<int, QString> node_name;
    friend QDataStream& operator << (QDataStream& out, MatrixGraph& g);
    friend QDataStream& operator >> (QDataStream& in, MatrixGraph& g);
};

QDataStream& operator << (QDataStream& out, MatrixGraph& g);

QDataStream& operator >> (QDataStream& in, MatrixGraph& g);

using AdjGraph = map<int, list<pair<int, long>>>;

int dijkstra(MatrixGraph* graph, int start, int dest);

int dijkstra(AdjGraph* graph, int start, int dest);

/*
template<typename F>
void BFS(MatrixGraph* g, int s, F f) ;

template<typename F>
void BFS(AdjGraph* g, int s, F f);
*/

void DFS(MatrixGraph *g, int s);

void BFS(MatrixGraph *g, int s);

void floyd(MatrixGraph* graph);

void getGraphInfo(MatrixGraph* graph);

MatrixGraph *prim(MatrixGraph& g);

MatrixGraph* kruskal(MatrixGraph& g);

class Graph
{
public:
    Graph();
};

    union graph_t{
        MatrixGraph* matrixG;
    };
#endif // GRAPH_H
