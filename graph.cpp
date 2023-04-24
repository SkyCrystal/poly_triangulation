#include <stack>
#include <algorithm>
#include <cassert>
#include "graph.h"
#include "io.h"
#include <cassert>

double cross(const Node &v1, const Node &v2);

bool intersection(const Cut &a, const Cut &b);

Map::Map(FILE *input) {
	fscanf(input, "%u", &polygon_count);
	for (int i = 0; i < polygon_count; ++i) {
		polygons.emplace_back(new Polygon(input));
		polygons[i]->set_clockwise(i != 0);
		polygons[i]->set_id(i);
		polygons[i]->load_nodes(nodes);
	}
	standardization();
	IO::init("debug.svg", width, height);
}

Map::~Map() {
	IO::close();
	for (auto &i: polygons) {
		delete i;
	}
}

void Map::merge_hole() {
	Node *now = *nodes.begin();
	for (auto &i: nodes) {
		if (polygons[i->id]->is_clockwise) {
			Link *pX = now->ln;
			Link *pY = i->ln;
			Link *newX = new Link(*pX);
			newX->pLink[1]->pLink[0] = newX;
			Link *newY = new Link(*pY);
			newY->pLink[1]->pLink[0] = newY;
//			polygons[1]->debug();
			pX->pLink[1] = newY;
			newY->pLink[0] = pX;
			pY->pLink[1] = newX;
			newX->pLink[0] = pY;
			polygons[i->id]->is_clockwise = false;
//			polygons[0]->node_count += polygons[i->id]->node_count + 2;
//			polygons[i->id]->node_count = 0;
			polygons[i->id]->p = nullptr;
			polygon_count--;
//			polygons[0]->debug();
		}
		now = i;
	}
	for (int i = 1; i < polygons.size(); ++i) {
		delete polygons[i];
	}
	polygons.resize(1);
}

void Map::standardization() {
	double dlt;
	std::sort(nodes.begin(), nodes.end(), [](const Node *l, const Node *r) {
		if (l->x == r->x) return l->y < r->y;
		return l->x < r->x;
	});
	dlt = 10 - nodes[0]->x;
	for (auto &i: nodes) {
		i->x += dlt;
	}
	width = std::ceil((*nodes.rbegin())->x + 10);

	std::sort(nodes.begin(), nodes.end(), [](auto l, auto r) {
		if (l->y == r->y) return l->x < r->x;
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

void Map::cut() {
	std::vector<Polygon *> t;
	polygons[0]->cut_into(t);
	polygons = std::move(t);
}

Polygon::Polygon(FILE *input) {
	fscanf(input, "%d", &node_count);
	double posX, posY;
	fscanf(input, "%lf %lf", &posX, &posY);
	p = new Link(Node(posX, posY), nullptr);
	p->node.ln = p;
	Link *now, *last = p;
	for (int i = 0; i < node_count - 1; ++i) {
		fscanf(input, "%lf %lf", &posX, &posY);
		now = new Link(Node(posX, posY), last);
		now->node.ln = now;
		last->pLink[1] = now;
		last = now;
	}

	p->pLink[0] = now;
	now->pLink[1] = p;
	init();

//#ifdef DEBUG
//	Link *t = p;
//	do {
//		fprintf(stderr, "%f %f\n", t->node.x, t->node.y);
//		t = t->pLink[1];
//	} while (t != p);
//#endif
}

Polygon::~Polygon() {
//	if (node_count == 0)return;
	Link *now = p;
	if (!now)return;
	do {
		delete now->pLink[0];
		now = now->pLink[1];
	} while (now != p);
}

void Polygon::init() {
	double area = 0;
	Link *now = p;
	do {
		area += cross(now->node, now->pLink[1]->node);
		now = now->pLink[1];
	} while (p != now);
	is_clockwise = (area < 0);
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
		p->node.id = id;
		p = p->pLink[1];
	} while (now != p);
}

void Polygon::load_nodes(std::vector<Node *> &nodes) const {
	Link *now = p;
	do {
		nodes.push_back(&now->node);
		now = now->pLink[1];
	} while (now != p);
}

void Polygon::cut_into(std::vector<Polygon *> &output) {
//	this->debug();
//	fprintf(stderr, "cut:%p\n", this);
	Link *first = p;
	int cnt = 0;
	first->dis = 0;
	first->node.id = 0;
	int ang=angle(first)<0;
	first = first->pLink[1];
	while (first != p) {
		first->dis = first->pLink[0]->dis + Node(first->pLink[0]->node, first->node).get_len();
		assert(first->pLink[0]->pLink[1] == first);
		first->node.id = ++cnt;
		if(angle(first)<0)ang++;
//		fprintf(stderr, "at %.2f %.2f,dis=%.2f\n", first->node.x, first->node.y, first->dis);
		first = first->pLink[1];
	}
	if(ang==0){
		output.push_back(this);
		return;
	}
	fprintf(stderr, "\n");
	this->circle = p->pLink[0]->dis + Node(p->pLink[0]->node, p->node).get_len();
	std::vector<Cut> cuts;
	do {
		Link *second = first->pLink[1];
		do {
			if (angle(first->node - first->pLink[0]->node, second->node - first->node) > angle(first) &&
			    angle(second->node - second->pLink[0]->node, first->node - second->node) > angle(second) &&
			    (angle(first) < 0 || angle(second) < 0)) {
				if ((first->node - second->node).get_len() == 0 ||
				    (first->pLink[0]->node - second->node).get_len() == 0) {
					goto next;
				}
				Link *lnk = first;
				do {
					if (intersection(first->node, second->node, lnk->node, lnk->pLink[1]->node))break;
					lnk = lnk->pLink[1];
				} while (lnk != first);

				if (lnk == first) {
					double score1 = std::min(second->dis - first->dis, this->circle - second->dis + first->dis) /
					                Node(first->node, second->node).get_len();
					if (angle(second) < 0)score1 *= 10;
					if (angle(first) < 0)score1 *= 10;
					if(score1<10)goto next;
					cuts.push_back({&first->node, &second->node, score1});
					assert(first->node.id<second->node.id);
//					fprintf(stderr, "get cut:%.2f %.2f-%.2f %.2f:%.2f\n", first->node.x, first->node.y,
//					        second->node.x, second->node.y, score1);
//					fprintf(stderr, "score:%.2f %.2f /%.2f\n",
//					        second->dis - first->dis, this->circle - second->dis + first->dis,
//					        Node(first->node, second->node).get_len());
				}
			}
			next:
			second = second->pLink[1];
		} while (second != p);
		first = first->pLink[1];
	} while (first->pLink[1] != p);
	std::sort(cuts.begin(), cuts.end());

	std::vector<Cut> pass2;
	for (auto &i: cuts) {
		int flag = 0;
		for (auto &j: pass2) {
			if (intersection(i, j)) {
				flag = 1;
				break;
			}
		}
		if (flag == 0)pass2.push_back(i);
		if(pass2.size()>ang)break;
	}
	std::vector<Polygon *> temp;
	std::sort(pass2.begin(), pass2.end(), [](const Cut &l, const Cut &r) -> bool {
		if (l.a->id == r.a->id) {
			return l.b->id > r.b->id;
		}
		return l.a->id < r.a->id;
	});
	std::stack<Node *> stk;
//	Link *last= nullptr;
	for (auto &i: pass2) {
		Link *pX = i.a->ln;
//		if(pX==last)continue;
//		last=pX;
		Link *pY = i.b->ln;
		if(pX->dis==-1||pY->dis==-1)continue;
		pX->dis=-1;
		pY->dis=-1;

//		fprintf(stderr, "cut:%.0f.%.0f %.0f.%.0f,id:%d %d\n", i.a->x, i.a->y, i.b->x, i.b->y, i.a->id, i.b->id);
//		while (!stk.empty() && pY->node.id > stk.top()->id)
//			stk.pop();
//		if (!stk.empty() && pY->node.id == stk.top()->id){
//			pY = stk.top()->ln;
//			stk.pop();
//		}
		Link *newX = new Link(*pX);
		Link *newY = new Link(*pY);
		newX->pLink[1]->pLink[0] = newX;

		newX->pLink[0] = newY;
		pY->pLink[0] = pX;
		pX->pLink[1] = pY;
		newY->pLink[1] = newX;

		newY->pLink[0]->pLink[1] = newY;
//		stk.push(&newX->node);
//		if(!temp.empty()&&temp[temp.size()-1]->p==pX){
//			temp[temp.size()-1]->p=newX;
//		}
		temp.push_back(new Polygon(newX));
//		if(p==i.a->ln){
//			p=newX;
//		}
	}
	for (auto &i: temp) {
		i->debug();
		IO::print(*i);
	}
	for (auto &i: temp) {
		i->cut_into(output);
	}
	this->cut_into(output);
	first = p;
//	do{
//		if(angle(first)<0){
//
//		}
//	}while(first!=p)
//	output.push_back(this);
}

Polygon::Polygon(Link *start) {
	p = start;
//	debug();
}

void Polygon::debug() {
	Link *now = p;
	do {
		assert(now->pLink[0]->pLink[1] == now);
		fprintf(stderr, "%.0f.%.0f ", now->node.x, now->node.y);
		now = now->pLink[1];
	} while (p != now);
	fprintf(stderr, "\n");
}



//[[maybe_unused]]void Polygon::fake_cut_into(std::vector<Polygon *> &output) {
//	IO::print(*this);
//	Link *first = p;
//	do {
//		fprintf(stderr, "check at:%f %f\n", first->node.x, first->node.y);
//		if (angle(first) < 0) {
//			fprintf(stderr, "cut at:%f %f\n", first->node.x, first->node.y);
//			std::stack<Link *> stk;
//			Link *second = first->pLink[1];
//			stk.push(second);
//			second = second->pLink[1];
//
//			double prev, rote, comp;
//			int is_inc = 1, is_covered = 0;
//			while (second != first) {
//				prev = angle(second->pLink[0]->node - first->node, second->node - first->node);
//				rote = angle(second->pLink[0]);
//				comp = angle(stk.top()->node - first->node, second->node - first->node);
//				if (prev > 0) {
//					if (is_covered == 1) {
//						if (comp > 0) {
//							is_covered = 0;
//							stk.push(second);
//						}
//					} else {
//						if (is_inc == 0)
//							stk.push(second->pLink[0]);
//						stk.push(second);
//					}
//					is_inc = 1;
//				}
//				if (prev == 0) {
//					if (is_covered == 0) {
//						if (second->node.get_len() < stk.top()->node.get_len()) {
//							stk.pop();
//							stk.push(second);
//						}
//					}
//				}
//				if (prev < 0) {
//					if (is_inc == 1) {
//						if (rote < 0) {
//							is_covered = 1;
//						}
//					}
//					if (is_covered == 0) {
//						while (angle(stk.top()->node - first->node, second->node - first->node) < 0) {
//							stk.pop();
//							rote = angle(stk.top()->node - first->node, second->node - first->node);
//							if (rote < 0) {
//								is_covered = 1;
//								break;
//							}
//						}
//					}
//					is_inc = 0;
//				}
//				second = second->pLink[1];
//			}
//			while (!stk.empty()) {
//				IO::print(first->node, stk.top()->node);
//				fprintf(stderr, "link:%f %f-%f %f\n", first->node.x, first->node.y, stk.top()->node.x,
//				        stk.top()->node.y);
//				stk.pop();
//			}
//			IO::flush();
//		}
//		first = first->pLink[1];
//	} while (first != p);
//}

Link::Link(Node p, Link *fr, Link *ne) : node(p), dis(0) {
	pLink[0] = fr;
	pLink[1] = ne;
}

Link::Link(const Link &p) : node(p.node) {
//	this->node=Node(p.node);
	node.ln = this;
	pLink[0] = p.pLink[0];
	pLink[1] = p.pLink[1];
}


bool Node::operator<(const Node &rhs) const {
	if (fabs(y - rhs.y) < 1e-6)return x < rhs.x;
	return y < rhs.y;
}

Node::Node(const Node &x, const Node &y) {
	this->x = y.x - x.x;
	this->y = y.y - x.y;
}

Node Node::operator-(const Node &rhs) const {
	return {x - rhs.x, y - rhs.y, id};
}

bool Node::operator==(const Node &rhs) const {
	return x == rhs.x &&
	       y == rhs.y;
}


double point(const Node &a, const Node &b) {
	return a.x * b.x + a.y * b.y;
}

/**
 * calculate the angle of edge p->a->n
 * @return [-pi,pi]
 */
double angle(Link *const &a) {
	Node v1(a->pLink[0]->node, a->node);
	Node v2(a->node, a->pLink[1]->node);
	double c = acos(point(v1, v2) / v1.get_len() / v2.get_len());
	if (cross(v1, v2) < 0)c = -c;
	return c;
}

/**
 * calculate the angle from vector a to vector b
 * @param a first vector
 * @param b second vector
 * @return [-pi,pi]
 */
double angle(Node a, Node b) {
	double c = acos(point(a, b) / a.get_len() / b.get_len());
	if (cross(a, b) < 0)c = -c;
	return c;
}

double cross(const Node &v1, const Node &v2) {
	return v1.x * v2.y - v2.x * v1.y;
}

bool intersection(const Cut &a, const Cut &b) {
	return intersection(*a.a, *a.b, *b.a, *b.b);
}

bool intersection(const Node &a, const Node &b, const Node &c, const Node &d) {  // seg(a, b) and seg(c, d)
	using std::min;
	using std::max;
	if (
			a == c ||
			a == d ||
			b == c ||
			b == d
			)
		return false;
	if (
			min(a.x, b.x) > max(c.x, d.x) ||
			min(a.y, b.y) > max(c.y, d.y) ||
			min(c.x, d.x) > max(a.x, b.x) ||
			min(c.y, d.y) > max(a.y, b.y)
			)
		return false;
	if (cross(b - a, c - a) * cross(b - a, d - a) <= 0)return true;
	return false;
}

bool Cut::operator<(const Cut &rhs) const {

	return score > rhs.score;
}
