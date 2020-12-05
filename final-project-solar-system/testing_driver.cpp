
#include <iostream> 
#include <cstdlib>
using std::cout; 
#include "./planet.h"


int main(){

	Planet mercury("mercury",	4879,		1407.6,		4222.6,	57.9,		88		);
	Planet venus	("venus",		12104,	-58325.5,	2802,		108.2,	224.7	);
	Planet earth	("earth",		12756,	23.9,			24,			149.6,	365.2	);
	Planet mars		("mars",		6792,		24.6,			24.7,		227.9,	687		);
	Planet jupiter("jupiter",	142984,	9.9,			9.9,		778.6,	4331	);
	Planet saturn	("saturn",	120536,	10.7,			10.7,		1433.5,	10747	);
	Planet uranus	("uranus",	51118,	-17.2,		17.2,		2872.5,	30589	);
	Planet neptune("neptune",	49528,	16.1,			16.1,		4495.1,	59800	);

	return 0;
}