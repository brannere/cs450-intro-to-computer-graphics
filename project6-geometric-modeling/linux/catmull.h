#ifndef CATMULL_H
#define CATMULL_H

struct point
{
	float x0, y0, z0;       // initial coordinates
	float x, y, z;        // animated coordinates
};

struct curve
{
	float r, g, b;
	point* points;
	int count;
};

void makePoints(struct curve* c, int num_points);
float catmull(float p0, float p1, float p2, float p3, float t);
void genCurve(struct curve* c, float r, float g, float b);

#endif