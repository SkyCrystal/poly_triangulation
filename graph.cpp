#include "graph.h"


Map::Map(FILE *input) {
	fscanf(input, "%u", &polygon_count);
	for (int i = 0; i < polygon_count; ++i) {
		polygons.emplace_back(new Polygon(input));
		polygons[i]->set_clockwise(i == 0);
		polygons[i]->set_id(i);
		polygons[i]->load_nodes(nodes);
	}
	standardization();

	fprintf(stderr, "map inited!\n");
}

Map::~Map() {
	for (auto &i: polygons) {
		delete i;
	}
}

void Map::merge_hole() {
	Node *now = *nodes.begin();
	for (auto &i: nodes) {
		if (!polygons[i->id]->is_clockwise) {
			Link *pX = now->ln;
			Link *pY = i->ln;
			Link *newX = new Link(*pX);
			Link *newY = new Link(*pY);
			pX->pLink[1] = newY;
			newX->pLink[0] = pY;
			pY->pLink[1] = newX;
			newY->pLink[0] = pX;
			polygons[i->id]->is_clockwise = true;
			polygons[0]->node_count += polygons[i->id]->node_count + 2;
			polygons[i->id]->node_count = 0;
		}
		now = i;
	}
	//FIXME: error at complex polygon holes
//	for (int i = 1; i < polygon_count; ++i) {
//		Link *nowX = polygons[0]->p;
//		Link *nowY = polygons[i]->p;
//		Link *px = nowX, *py = nowY;
//		double dis = dist(*px->node, *py->node);
//		do {
//			do {
//				if (dis > dist(*nowX->node, *nowY->node)) {
//					dis = dist(*nowX->node, *nowY->node);
//					px = nowX;
//					py = nowY;
//				}
//				nowY = nowY->pLink[1];
//			} while (nowY != polygons[i]->p);
//			nowX = nowX->pLink[1];
//		} while (nowX != polygons[0]->p);
//		Link *newX = new Link(*px);
//		Link *newY = new Link(*py);
//		px->pLink[1] = newY;
//		newX->pLink[0] = py;
//		py->pLink[1] = newX;
//		newY->pLink[0] = px;
//		polygons[0]->node_count += polygons[i]->node_count + 2;
//		polygons[i]->node_count = 0;
//	}
//
//	polygons.resize(1);
}

void Map::standardization() {
	double dlt;
	std::sort(nodes.begin(), nodes.end(), [](auto l, auto r) {
		if (std::fabs(l->y - r->y) < 1e-6) return l->y < r->y;
		return l->x < r->x;
	});
	dlt = 10 - nodes[0]->x;
	for (auto &i: nodes) {
		i->x += dlt;
	}
	width = std::ceil((*nodes.rbegin())->x + 10);

	std::sort(nodes.begin(), nodes.end(), [](auto l, auto r) {
		if (std::fabs(l->x - r->x) < 1e-6) return l->x < r->x;
		return l->y < r->y;
	});
	dlt = 10 - nodes[0]->y;
	for (auto &i: nodes) {
		i->y += dlt;
	}
	height = std::ceil((*nodes.rbegin())->y + 10);

	int multi = 1000 / std::max(height, width);
	if (multi > 1) {
		for (auto i: nodes) {
			i->x *= multi;
			i->y *= multi;
		}
		width *= multi;
		height *= multi;
	}
}

Polygon::Polygon(FILE *input) {
	fscanf(input, "%d", &node_count);
	double posX, posY;
	fscanf(input, "%lf %lf", &posX, &posY);
	p = new Link(new Node(posX, posY), nullptr);
	p->node->ln = p;
	Link *now, *last = p;
	for (int i = 0; i < node_count - 1; ++i) {
		fscanf(input, "%lf %lf", &posX, &posY);
		now = new Link(new Node(posX, posY), last);
		now->node->ln = now;
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

void Polygon::update() {
	double area = 0;
	Link *now = p;
	do {
		area += cross(*now, *now->pLink[1]);
		now = now->pLink[1];
	} while (p != now);
	is_clockwise = (area > 0);
	//maybe do some check here
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

void Polygon::set_id(int id) {
	Link *now = p;
	do {
		p->node->id = id;
		p = p->pLink[1];
	} while (now != p);
}

void Polygon::load_nodes(std::vector<Node *> &nodes) const {
	Link *now = p;
	do {
		nodes.push_back(now->node);
		now = now->pLink[1];
	} while (now != p);
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

bool Node::operator<(const Node &rhs) const {
	if (fabs(y - rhs.y) < 1e-6)return x < rhs.x;
	return y < rhs.y;
}

double dist(Node px, Node py) {
	return sqrt((px.x - py.x) * (px.x - py.x) + (px.y - py.y) * (px.y - py.y));
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

