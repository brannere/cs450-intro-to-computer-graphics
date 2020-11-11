#version 330 compatibility

uniform float uS0;
uniform float uT0;
uniform float uDs;
uniform float uDt;
uniform float	uTime;		// "Time", from Animate( )
in vec2  	vST;		// texture coords

void
main( )
{
	vec3 myColor = vec3( 0,0,1 );
	if
	( 
		uS0 - uDs/2. <= vST.s && vST.s <= uS0 + uDs/2. &&
		uT0 - uDt/2. <= vST.t && vST.t <= uT0 + uDt/2. 
	)
	{
		myColor = vec3( 1,0,0 );
	}
	gl_FragColor = vec4( myColor,  1. );
}
