#ifndef PLANET_H
#define PLANET_H
#include <cstdlib>
#include <string>
using std::string;
/* https://nssdc.gsfc.nasa.gov/planetary/factsheet/ */
/* textures are named planetname.bmp --> will append .bmp to get texture */

class Planet{

	private: 
		string name = ""; 

	public:
		float x = 0;
		float y = 0; 
		float z = 0;
		double  diameter = 0; //km
		double  rotation_period = 0; //hours
		double  day_len = 0; //hours 
		double  dist_from_sun = 0; //10^6 km
		double  orbital_period = 0; //days
		//int orbital_velocity; // km/s
		double scaler;
		Planet();
		Planet(	string name, double diameter, double rotation_period, double day_len, 
						double dist_from_sun, double orbital_period, double scaler );

		void set_cords(float x, float y, float z);
		// ~Planet();

};

#endif