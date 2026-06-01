#pragma once
#include "Vector3.h"

struct HEHalfEdge;

struct EdgeCollapse {

	HEHalfEdge* edge; // 要折叠的半边
	double cost; // 折叠成本
	Point3 p; // 最优折叠位置
	bool valid; // 是否有效

	EdgeCollapse(){}

	EdgeCollapse(HEHalfEdge* edge, double cost, Point3 p, bool valid = true) : edge(edge), cost(cost), p(p), valid(valid){}
	//EdgeCollapse(HEHalfEdge* edge, double cost, Point3 p) : edge(edge), cost(cost), p(p){}

	bool operator> (const EdgeCollapse& other) const {
		return cost > other.cost;
	}

};
