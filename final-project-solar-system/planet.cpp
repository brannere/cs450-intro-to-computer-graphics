#include <iostream> 
#include <cstdlib> 
using std::cout; 

#include "./planet.h"



Planet::Planet(){
	return; 
}


Planet::Planet(	string name, double diameter, double rotation_period, 
								double day_len, double dist_from_sun, double orbital_period,
								int scaler)
{
	this->scaler 					= scaler;
	this->name 						= name;
	this->diameter 				= (this->scaler)*(diameter);
	this->rotation_period = (this->scaler)*(rotation_period);
	this->day_len 				= (this->scaler)*(day_len);
	this->dist_from_sun 	= (this->scaler)*(dist_from_sun);
	this->orbital_period 	= (this->scaler)*(orbital_period);
	return; 
}
// Planet::~Planet(){

// 	return;
// }
// int main(){

// 	Planet p; 
// 	return 0;
// }