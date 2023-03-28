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
	Link *ln = nullptr;

	Node(double x, double y, int id) : x(x), y(y), id(id) {};

	Node(double x, double y) : x(x), y(y), id(-1) {};

	Node(const Node &x) = default;

	bool operator<(const Node &rhs) const;

	Node operator-(const Node &rhs) const;

	Node(const Node &x, const Node &y);

	Node();

	[[nodiscard]] inline double get_len() const {
		return std::sqrt(x * x + y * y);
	}

	bool operator==(const Node &rhs) const;
};

struct Link {
	Node node;
	Link *pLink[2];
	double dis;

	Link(Node p, Link *fr, Link *ne = nullptr);

	explicit Link(const Link &p);

};

struct Polygon {
	bool is_clockwise = false;
	int node_count;//only used for io
	Link *p;
	double circle;

	Polygon() : Polygon(stdin) {};

	~Polygon();

	explicit Polygon(FILE *input);

	explicit Polygon(Link *start);

	void init();

	void set_clockwise(bool b);

	void set_id(int id);

	void load_nodes(std::vector<Node *> &vector1) const;

	void cut_into(std::vector<Polygon *> &output);

	void fake_cut_into(std::vector<Polygon *> &output);

	void debug();
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

	void cut();

	void standardization();
};


struct Cut {
	Node *a, *b;
	double score;

	bool operator<(const Cut &rhs) const;
};

double point(const Node &a, const Node &b);

double angle(Link *const &a);

double angle(Node a, Node b);


bool intersection(const Node &a, const Node &b, const Node &c,
                  const Node &d);

#endif