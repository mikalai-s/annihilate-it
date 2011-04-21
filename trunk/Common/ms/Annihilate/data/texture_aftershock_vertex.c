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

    gl_Position = /*mvp * */position;
    
    outcolor = color;
    texcoord0 = texcoord;
}
