#ifndef GRAPH_H
#define GRAPH_H
#define DEBUG

#include<cstdlib>
#include<vector>
#include<iostream>
#include<cmath>
struct Link;
struct Node {
	double x, y;
	int id;
	Link* ln= nullptr;

	Node(double x, double y, int id) : x(x), y(y), id(id) {};

	Node(double x, double y) : x(x), y(y), id(-1) {};

	Node(const Node &x)=default;

	bool operator < (const Node &rhs) const;
};

struct Link {
	Node *node;
	Link *pLink[2];

	Link(Node *p, Link *fr, Link *ne = nullptr);

	[[maybe_unused]]explicit operator Node() const {
		return *node;
	}
	Link(const Link &l);
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

	void set_id(int id);

	void load_nodes(std::vector<Node *> &vector1) const;
};

/**
 * The whole map containing all nodes
 */
struct Map {
	std::vector<Node *> nodes;

	std::vector<Polygon *> polygons;
	unsigned polygon_count{};
	int height{}, width{};

	Map() : Map(stdin) {};

	explicit Map(FILE *input);
	void merge_hole();
	~Map();

	void standardization();
};

double dist(Node px,Node py);

double cross(const Link &a, const Link &b);

double cross(const Node &a, const Node &b);

double cross(const Node &o, const Node &a, const Node &b);

#endif