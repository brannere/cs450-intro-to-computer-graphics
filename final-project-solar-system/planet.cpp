#include <iostream> 
#include <cstdlib> 
using std::cout; 

#include "./planet.h"



Planet::Planet(){
	return; 
}


Planet::Planet(	string name, double diameter, double rotation_period, 
								double day_len, double dist_from_sun, double orbital_period)
{
	this->name = name;
	this->diameter = diameter;
	this->rotation_period = rotation_period;
	this->day_len = day_len;
	this->dist_from_sun = dist_from_sun;
	this->orbital_period = orbital_period;
	return; 
}
// Planet::~Planet(){

// 	return;
// }
// int main(){

// 	Planet p; 
// 	return 0;
// }