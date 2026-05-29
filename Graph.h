//
// Created by a1191 on 2026/5/27.
//
#ifndef _GRAPH_H
#define _GRAPH_H

#include<string>
#include<vector>

#define MAXSIZE     15//一共15个地点

const  int INF=1e9;//表示无穷

//每个adjList[i]的出边
struct Edge
{
    int to;//与哪个结点相连
    int weight;//边的权重，表示两个地点之间距离
};

//校园地图的图类
class map_graph
{
private:
    int vertex_num;//顶点数量
    int edge_num;//边数量
    std::string placeNames[MAXSIZE];//地点名称数组
    std::vector<std::vector<int>>adjMatrix;//邻接矩阵
    std::vector<std::vector<Edge>>adjList;//无向邻接表
    bool visited[MAXSIZE];//表示该顶点是否访问过，下标对应每一个顶点;flase:未被访问  true:被访问过
public:
    map_graph();

    bool load_map_graph(const std::string& fileName);

    void printAdjMatrix()const;//加const确保只读

    void printAdjList()const;

    void dfs(int vertex,bool reset);

    void bfs(int vertex,bool reset);
};

#endif //_GRAPH_H