#include <iostream> 
#include <cstdlib> 
using std::cout; 

#include "./planet.h"



Planet::Planet(){
	return; 
}


Planet::Planet(	string name, double diameter, double rotation_period, 
								double day_len, double dist_from_sun, double orbital_period,
								double scaler)
{
	this->scaler 					= scaler;
	this->name 						= name;
	this->diameter 				= (this->scaler)*(diameter);
	this->rotation_period = (rotation_period);
	this->day_len 				= (this->scaler)*(day_len);
	this->dist_from_sun 	= 0.000001*((this->scaler)*(dist_from_sun)*(10*10*10*10*10*10));
	this->orbital_period 	= (this->scaler)*(orbital_period);
	this->x = 0;
	this->y = 0;
	this->z = 0;
	return; 
}


void Planet::set_cords(float x, float y, float z){
	this->x = x;
	this->y = y;
	this->z = z; 
	return;
}
// Planet::~Planet(){

// 	return;
// }
// int main(){

// 	Planet p; 
// 	return 0;
// }