#version 330 compatibility

uniform float	uTime;		// "Time", from Animate( )

//out vec2 vST;
out vec3 vN;
out vec3 vL;
out vec3 vE;

const vec3 LIGHTPOSITION = vec3(0.,5.,5.);

out vec2  	vST;		// texture coords
uniform bool uAnimate;

const float PI = 	3.14159265;
const float AMP = 	0.2;		// amplitude
const float W = 	2.;		// frequency
//uniform float uRand;
void
main( )
{ 
	/* lighting stuff */
	vec3 vert = gl_Vertex.xyz;
	vec4 ECposition = gl_ModelViewMatrix * gl_Vertex ;
	vN = normalize( gl_NormalMatrix * gl_Normal );	// normal vector
	vL = LIGHTPOSITION - ECposition.xyz;		// vector from the point
							// to the light position
	vE = vec3( 0., 0., 0. ) - ECposition.xyz;	// vector from the point
							// to the eye position 
	//gl_Position = gl_ModelViewProjectionMatrix * vec4( vert, 1. );
	/* */


	vST = gl_MultiTexCoord0.st;
	//vec3 vert = gl_Vertex.xyz;
	//vert.x = uTime*vert.x;
	//vert.y = uTime*vert.y;
	//vert.z = uTime*vert.z;
	if(uAnimate){
		//vec3 vert = gl_Vertex.xyz;
		//vert.x = uTime*vert.x;
		vert.x =  (1 + 0.5* sin(2*PI*W*uTime))* vert.x;
		//vert.x = uTime*(vert.x)*tan(vert.x); //??? something fun of your own design
		vert.y = uTime*sin(vert.y); //??? something fun of your own design
		vert.z = uTime*(vert.z)*cos(vert.z); //??? something fun of your own design
	}
	gl_Position = gl_ModelViewProjectionMatrix * vec4( vert, 1. );
}
