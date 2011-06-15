uniform mat4 mvp;

attribute vec4 position;
attribute vec4 color;

attribute vec4 borderLineTexelLeft;
attribute vec4 borderLineTexelBottom;
attribute vec4 borderLineTexelRight;
attribute vec4 borderLineTexeltop;

attribute vec4 borderCornerTexelLeft;
attribute vec4 borderCornerTexelBottom;
attribute vec4 borderCornerTexelRight;
attribute vec4 borderCornerTexeltop;

varying vec4 outcolor;

varying vec4 border_line_texel_left;
varying vec4 border_line_texel_bottom;
varying vec4 border_line_texel_right;
varying vec4 border_line_texel_top;

varying vec4 border_corner_texel_left;
varying vec4 border_corner_texel_bottom;
varying vec4 border_corner_texel_right;
varying vec4 border_corner_texel_top;


void main()
{
	vec4 p = position;

	// invert y and center
	p.x = p.x * 2.0 - 1.0;	
	p.y = 1.0 - p.y * 2.0;
	p.z = 1.0;

    gl_Position = mvp * p;
    
    outcolor = color;

	border_line_texel_left = borderLineTexelLeft;
	border_line_texel_bottom = borderLineTexelBottom;
	border_line_texel_right = borderLineTexelRight;
	border_line_texel_top = borderLineTexeltop;

	border_corner_texel_left = borderCornerTexelLeft;
	border_corner_texel_bottom = borderCornerTexelBottom;
	border_corner_texel_right = borderCornerTexelRight;
	border_corner_texel_top = borderCornerTexeltop;
}
