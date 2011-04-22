attribute vec4 position;
attribute vec4 color;
attribute vec4 texcoord;

attribute float radius;
attribute float power;

uniform mat4 mvp;
varying vec4 texcoord0;
varying vec4 outcolor;

varying float radius0;
varying float power0;



void main()
{
    radius0 = radius;
	power0 = power;

  vec4 p = position;

	// invert y and center
	//p.x = p.x - 1.0;	
	//p.y = 1.0 - p.y;

    gl_Position = p;
    
    
    outcolor = color;
    texcoord0 = texcoord;
}
