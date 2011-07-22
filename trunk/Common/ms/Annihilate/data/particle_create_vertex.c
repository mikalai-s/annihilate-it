uniform float u_size;
     
attribute vec2 a_position;
attribute float a_size;
attribute vec4 a_color;

varying vec4 v_color;


void main()
{
    gl_Position.xy = a_position;
    gl_Position.x = gl_Position.x * 2.0 - 1.0;
    gl_Position.y = 1.0 - gl_Position.y * 2.0;
	gl_Position.z = 1.0;
	gl_Position.w = 1.0;
    gl_PointSize = a_size;
	v_color = a_color;
}