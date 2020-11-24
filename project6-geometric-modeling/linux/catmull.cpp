#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>


/* More of Erick's stuff */

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

// Curve curves[NUM_CURVES];		// if you are creating a pattern of curves


// call in loop of array statically allocated curves
void makePoints(struct curve* c, int num_points){

	// struct curve* new_curve = NULL;

	if(num_points <= 0){
		return;
	}
	
	// new_curve = new curve; 
	c->points = new point[num_points];
	for(int i = 0; i < num_points; i++){
		c->points[i].x0 = -1; 
		c->points[i].y0 = -1; 
		c->points[i].z0 = -1; 
	}
	c->count = num_points;
	return;
}

// do a cat mull for EACH point (x,y,z)
float catmull(float p0, float p1, float p2, float p3, float t){
	
	float res = 0;

	res = 0.5 * ((2*p1 + t * ((-1*p0) + p2) ) + 
							((t*t) * ( (2 * p0) - (5 * p1) + (4*p2) -p3 + 
							( (t*t*t) *( (-1*p0)+ (3*p1) - (3*p2) + p3) ) ) ) ) ;

	return res;
}

void genCurve(struct curve* c, float r, float g, float b){
	float x,y,z;
	glColor3f(r,g,b);
	/* 
	for group:
		glbegin()
		for t in some increment: t += 0.5
			catmull(x0,x1,x2,x3,t)
			catmull(y0,y1,y2,y3,t)
			catmull(z0,z1,z2,z3,t)
			//glvertex(x,y,z)
	*/
	for(int i = 0 ; i < c->count-4; i++){
		glBegin(GL_LINE_STRIP);
		for(float t = 0; t <= 1; t+=0.05){
			// std::cout << "i: " << i << "\n";
			// std::cout <<"t: " << t << "\n";
			// std::cout << "i+0:\t" << c->points[i].x0 << "\n";
			// std::cout << "i+1:\t" << c->points[i+1].x0 << "\n";
			// std::cout << "i+2:\t" << c->points[i+2].x0 << "\n";
			// std::cout << "i+3:\t" << c->points[i+3].x0 << "\n";
			// std::cout << "[" << i << "].y0: " << c->points[i].y0 << "\n";
			// std::cout << "[" << i << "].z0: " << c->points[i].z0 << "\n\n";
			// // std::cout << catmull(c->points[i].x0, c->points[i+1].x0, c->points[i+2].x0, c->points[i+3].x0,t) << "\n";
			// std::cout << catmull(c->points[i].y0, c->points[i+1].y0, c->points[i+2].y0, c->points[i+3].y0,t) << "\n";
			// std::cout << catmull(c->points[i].z0, c->points[i+1].z0, c->points[i+2].z0, c->points[i+3].z0,t) << "\n";
			// std::cout << "\n";
			x =catmull(c->points[i].x0, c->points[i+1].x0, c->points[i+2].x0, c->points[i+3].x0,t);
			y =catmull(c->points[i].y0, c->points[i+1].y0, c->points[i+2].y0, c->points[i+3].y0,t);
			z =catmull(c->points[i].z0, c->points[i+1].z0, c->points[i+2].z0, c->points[i+3].z0,t);
			

			std::cout << "x: " << x <<"\ny: " << y << "\nz: "<< z << "\n\n";
			glVertex3f(x,y,z);
		}
		glEnd();
	}

	return;
}


// void
// RotateX(point* p, float deg, float xc, float yc, float zc)
// {
// 	float rad = deg * (M_PI / 180.f);         // radians
// 	float x = p->x0 - xc;
// 	float y = p->y0 - yc;
// 	float z = p->z0 - zc;

// 	float xp = x;
// 	float yp = y * cos(rad) - z * sin(rad);
// 	float zp = y * sin(rad) + z * cos(rad);

// 	p->x = xp + xc;
// 	p->y = yp + yc;
// 	p->z = zp + zc;
// }

// void
// RotateY(point* p, float deg, float xc, float yc, float zc)
// {
// 	float rad = deg * (M_PI / 180.f);         // radians
// 	float x = p->x0 - xc;
// 	float y = p->y0 - yc;
// 	float z = p->z0 - zc;

// 	float xp = x * cos(rad) + z * sin(rad);
// 	float yp = y;
// 	float zp = -x * sin(rad) + z * cos(rad);

// 	p->x = xp + xc;
// 	p->y = yp + yc;
// 	p->z = zp + zc;
// }

// void
// RotateZ(point* p, float deg, float xc, float yc, float zc)
// {
// 	float rad = deg * (M_PI / 180.f);         // radians
// 	float x = p->x0 - xc;
// 	float y = p->y0 - yc;
// 	float z = p->z0 - zc;

// 	float xp = x * cos(rad) - y * sin(rad);
// 	float yp = x * sin(rad) + y * cos(rad);
// 	float zp = z;

// 	p->x = xp + xc;
// 	p->y = yp + yc;
// 	p->z = zp + zc;
// }



// int main(){

// 	struct curve c; 
// 	makePoints(&c, 5);
// 	c.points[0].x0 = 0;
// 	c.points[0].y0 = 0;
// 	c.points[0].z0 = 0;
	
// 	c.points[1].x0 = 1;
// 	c.points[1].y0 = 1;
// 	c.points[1].z0 = 1;
	
	
// 	c.points[2].x0 = 2;
// 	c.points[2].y0 = 2;
// 	c.points[2].z0 = 2;

// 	c.points[3].x0 = 6;
// 	c.points[3].y0 = 6;
// 	c.points[3].z0 = 6;

	
// 	// for each point
// 		//catmull x
// 		//catmull y 
// 		//catmull z

// 	genCurve(&c, 1,1,1);

// 	return 0;
// }