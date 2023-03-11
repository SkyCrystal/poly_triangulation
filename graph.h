#ifndef GRAPH_H
#define GRAPH_H
#define DEBUG

#include<cstdlib>
#include<vector>
#include<iostream>

struct Node {
	double x, y;
	int id;

	Node(double x, double y, int id) : x(x), y(y), id(id) {};

	Node(double x, double y) : x(x), y(y), id(-1) {};
};

struct Link {
	Node *node;
	Link *pLink[2];

	Link(Node *p, Link *fr, Link *ne = nullptr);

	[[maybe_unused]]explicit operator Node() const {
		return *node;
	}
};

struct Polygon {
	bool is_clockwise = 0;
	int node_count;
	Link *p;

	Polygon() : Polygon(stdin) {};

	~Polygon();

	explicit Polygon(FILE *input);

	void update();

	void set_clockwise(bool b);


};

/**
 * The whole map containing all nodes
 */
struct Map {
	std::vector<Node *> nodes;
	unsigned node_count;

	std::vector<Polygon *> polygons;
	unsigned polygon_count;
	int height, width;

	Map() : Map(stdin) {};

	explicit Map(FILE *input);

	~Map();
};

#endif