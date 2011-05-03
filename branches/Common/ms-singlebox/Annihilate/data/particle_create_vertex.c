uniform float u_size;
     
attribute vec3 a_position;
attribute float a_size;
attribute vec4 a_color;
varying vec4 v_color;

void main()
{
    gl_Position.xy = a_position.xy;
    gl_Position.x = gl_Position.x - 1.0;
    gl_Position.y = 1.0 - gl_Position.y;
	gl_Position.z = 1.0;
	gl_Position.w = 1.0;
    gl_PointSize = a_position.z;
	v_color = a_color;
}