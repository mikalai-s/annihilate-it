attribute vec4 position;
attribute vec4 color;
attribute vec4 texcoord;
attribute vec4 borderLineTexelLeft;
attribute vec4 borderLineTexelBottom;
attribute vec4 borderLineTexelRight;
attribute vec4 borderLineTexeltop;
attribute vec4 borderCornerTexel;

varying vec4 outcolor;

varying vec4 border_line_texel_left;
varying vec4 border_line_texel_bottom;
varying vec4 border_line_texel_right;
varying vec4 border_line_texel_top;
varying vec4 border_corner_texel;

void main()
{
	vec4 p = position;

	// invert y and center
	p.x = p.x - 1.0;	
	p.y = 1.0 - p.y;

    gl_Position = p;
    
    outcolor = color;
	border_line_texel_left = borderLineTexelLeft;
	border_line_texel_bottom = borderLineTexelBottom;
	border_line_texel_right = borderLineTexelRight;
	border_line_texel_top = borderLineTexeltop;
	border_corner_texel = borderCornerTexel;
}
