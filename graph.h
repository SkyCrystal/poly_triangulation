#ifndef GRAPH_H
#define GRAPH_H

#include<cstdlib>
#include<vector>
#include<iostream>

struct Node {
	double x, y;
	int id;

	Node(double x, double y, int id) : x(x), y(y), id(id) {};
};

struct Link {
	Node *node;
	Link *pLink[2];

	Link(Node *p, Link *fr, Link *ne = nullptr);
};

struct Polygon {
	bool is_clockwise, is_hole;
	int node_count;
	Link *p;

	Polygon() : Polygon(stdin) {};

	explicit Polygon(FILE *input);
};

/**
 * The whole map containing all nodes
 */
struct Map {
	std::vector<Node *> nodes;
	size_t node_count;

	std::vector<Polygon *> polygons;
	size_t polygon_count;

	Map() : Map(stdin) {};

	explicit Map(FILE *input);

	~Map();
};

#endif