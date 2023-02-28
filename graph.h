#ifndef GRAPH_H
#define GRAPH_H
#include<cstdlib>
#include<vector>
#include<iostream>
struct Node{
    double x,y;
	Node(double x,double y):x(x),y(y){};
};
struct link{
    Node *p;
    link *ne[2];
    bool is_clockwise;
};
struct Map{
    std::vector<Node*> nodes;
    const size_t node_count;

    std::vector<link*> polygon_hulls;
    size_t polygon_count;
    Map(const size_t nodec, const size_t polyc, std::basic_istream<char> &input=std::cin)
    :node_count(nodec),polygon_count(polyc){
        nodes.reserve(node_count);
        for(int i=0;i<nodec;++i)
        {

        }
    }
    ~Map(){
        
    }
};
#endif