#include "graph.h"
#include<cmath>

//
// Created by pb on 2023/2/19.
//
Map::Map(FILE *input) {
	fscanf(input, "%u %d %d", &polygon_count, &height, &width);
	for (int i = 0; i < polygon_count; ++i) {
		polygons.emplace_back(new Polygon(input));
		polygons[i]->set_clockwise(i == 0);
		node_count += polygons[i]->node_count;
	}
	fprintf(stderr, "map inited!\n");
}

Map::~Map() {
	for (auto &i: polygons) {
		delete i;
	}
}

void Map::merge_hole() {
	//FIXME: error at complex polygon holes
	for (int i = 1; i < polygon_count; ++i) {
		Link *nowX = polygons[0]->p;
		Link *nowY = polygons[i]->p;
		Link *px = nowX, *py = nowY;
		double dis = dist(*px->node, *py->node);
		do {
			do {
				if (dis > dist(*nowX->node, *nowY->node)) {
					dis = dist(*nowX->node, *nowY->node);
					px = nowX;
					py = nowY;
				}
				nowY = nowY->pLink[1];
			} while (nowY != polygons[i]->p);
			nowX = nowX->pLink[1];
		} while (nowX != polygons[0]->p);
		Link *newX = new Link(*px);
		Link *newY = new Link(*py);
		px->pLink[1] = newY;
		newX->pLink[0] = py;
		py->pLink[1] = newX;
		newY->pLink[0] = px;
		polygons[0]->node_count += polygons[i]->node_count + 2;
		polygons[i]->node_count = 0;
	}

	polygons.resize(1);
}

Polygon::Polygon(FILE *input) {
	fscanf(input, "%d", &node_count);
	double posX, posY;
	fscanf(input, "%lf %lf", &posX, &posY);
	p = new Link(new Node(posX, posY), nullptr);
	Link *now, *last = p;
	for (int i = 0; i < node_count - 1; ++i) {
		fscanf(input, "%lf %lf", &posX, &posY);
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
		fprintf(stderr, "%f %f\n", t->node->x, t->node->y);
		t = t->pLink[1];
	} while (t != p);
#endif
}

Polygon::~Polygon() {
	if (node_count == 0)return;
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

Link::Link(const Link &l) {
	pLink[0] = l.pLink[0];
	pLink[1] = l.pLink[1];
	node = new Node(*l.node);
}

double dist(Node px, Node py) {
	return sqrt((px.x - py.x) * (px.x - py.x) + (px.y - py.y) * (px.y - py.y));
}
