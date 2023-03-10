#include "graph.h"

//
// Created by pb on 2023/2/19.
//
Map::Map(FILE *input) {
	fscanf(input, "%d", &polygon_count);
	for (int i = 0; i < polygon_count; ++i) {
		polygons.emplace_back(new Polygon(input));
		polygons[i]->set_clockwise(i == 0);
		node_count += polygons[i]->node_count;
	}
}

Map::~Map() {
	for (auto &i: polygons) {
		delete i;
	}
}

Polygon::Polygon(FILE *input) {
	fscanf(input, "%d", &node_count);
	Link *now, *last = p;
	double posX, posY;
	fscanf(input, "%f %f", &posX, &posY);
	p = new Link(new Node(posX, posY), nullptr);

	for (int i = 0; i < node_count - 1; ++i) {
		fscanf(input, "%f %f", &posX, &posY);
		now = new Link(new Node(posX, posY), last);
		last->pLink[1] = now;
		last = now;
	}

	p->pLink[0] = now;
	now->pLink[1] = p;
	update();

#ifdef DEBUG
	Link *t = p;
	do {
		fprintf(stderr, "%d %d\n", t->node->x, t->node->y);
		t = t->pLink[1];
	} while (t != p);
#endif
}

Polygon::~Polygon() {
	Link *now = p;
	do {
		delete now->pLink[0];
		now = now->pLink[1];
	} while (now != p);
}

double cross(const Node &o, const Node &a, const Node &b) {
	return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
}

double cross(const Node &a, const Node &b) {
	return a.x * b.y - a.y * b.x;
}

double cross(const Link &a, const Link &b) {
	return cross(*a.node, *b.node);
}

void Polygon::update() {
	double area = 0;
	Link *now = p;
	do {
		area += cross(*now, *now->pLink[1]);
		now = now->pLink[1];
	} while (p != now);
	is_clockwise = (area > 0);
//TODO
}

void Polygon::set_clockwise(bool b) {
	if (b == is_clockwise)return;
	is_clockwise = b;
	Link *now = p;
	do {
		std::swap(now->pLink[1], now->pLink[0]);
		now = now->pLink[1];
	} while (p != now);
}

Link::Link(Node *p, Link *fr, Link *ne) : node(p) {
	pLink[0] = fr;
	pLink[1] = ne;
}