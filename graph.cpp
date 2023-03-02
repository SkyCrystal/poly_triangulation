#include "graph.h"

//
// Created by pb on 2023/2/19.
//
Map::Map(FILE *input) {
	fscanf(input, "%d", polygon_count);
	for (int i = 0; i < polygon_count; ++i) {
		polygons.emplace_back(new Polygon(input));
	}
}

Map::~Map() {

}


Polygon::Polygon(FILE *input) {
	fscanf(input, "%d", node_count);
	p=new Link(new Node(0,0,0),nullptr);
	Link *now,*last=p;
	double posx, posy;
	for (int i = 0; i < node_count-1; ++i) {
		fscanf(input, "%f %f", posx, posy);
		now = new Link(new Node(posx, posy, -1),last);
		last->pLink[1]=now;
		last=now;
	}
	fscanf(input,"%f %f",posx,posy);
	
}

Link::Link(Node *p, Link *fr, Link *ne) : node(p) {
	pLink[0] = fr;
	pLink[1] = ne;
}