#version 330 compatibility

uniform float	uTime;		// "Time", from Animate( )

//out vec2 vST;
out vec3 vN;
out vec3 vL;
out vec3 vE;

const vec3 LIGHTPOSITION = vec3(5.,5.,0.);

out vec2  	vST;		// texture coords
uniform bool uAnimate;

const float PI = 	3.14159265;
const float AMP = 	0.2;		// amplitude
const float W = 	2.;		// frequency
//uniform float uRand;
void
main( )
{ 
	

	vST = gl_MultiTexCoord0.st;
	vec3 vert = gl_Vertex.xyz;
	//vert.x = uTime*vert.x;
	//vert.y = uTime*vert.y;
	//vert.z = uTime*vert.z;
	if(uAnimate){
	vert.x = uTime*(vert.x)*tan(vert.z); //??? something fun of your own design
	vert.y = uTime*(vert.y)*sin(vert.x); //??? something fun of your own design
	vert.z = uTime*(vert.z)*cos(vert.y); //??? something fun of your own design
	}
	gl_Position = gl_ModelViewProjectionMatrix * vec4( vert, 1. );
}
