#include "graph.h"

//
// Created by pb on 2023/2/19.
//
Map::Map(FILE *input) {
	fscanf(input, "%d", &polygon_count);
	for (int i = 0; i < polygon_count; ++i) {
		polygons.emplace_back(new Polygon(input));
	}
}

Map::~Map() {
//TODO
}


Polygon::Polygon(FILE *input) {
	fscanf(input, "%d", &node_count);
	Link *now,*last=p;
	double posx, posy;
	fscanf(input,"%f %f",&posx,&posy);
	p=new Link(new Node(posx,posy),nullptr);

	for (int i = 0; i < node_count-1; ++i) {
		fscanf(input, "%f %f", &posx, &posy);
		now = new Link(new Node(posx, posy),last);
		last->pLink[1]=now;
		last=now;
	}

	p->pLink[0]=now;
	now->pLink[1]=p;

#ifdef DEBUG
	Link* t=p;
	do {
		fprintf(stderr, "%d %d\n",t->node->x,t->node->y);
	} while (t!=p);
#endif
}

void Polygon::update(){
//TODO
}

Link::Link(Node *p, Link *fr, Link *ne) : node(p) {
	pLink[0] = fr;
	pLink[1] = ne;
}