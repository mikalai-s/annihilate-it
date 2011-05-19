
precision mediump float;

uniform sampler2D border_line_tex;
uniform sampler2D border_corner_tex;

uniform int border_line;
uniform int border_corner;

varying vec4 outcolor;
varying vec4 border_line_texel_left;
varying vec4 border_line_texel_bottom;
varying vec4 border_line_texel_right;
varying vec4 border_line_texel_top;
varying vec4 border_corner_texel;

#define MS_BORDER_LEFT 1
#define MS_BORDER_TOP 2
#define MS_BORDER_RIGTH 4
#define MS_BORDER_BOTTOM 8

void main()
{
	vec4 c = outcolor;
	
	if((border_line & MS_BORDER_LEFT) == MS_BORDER_LEFT)
		c *= texture2D(border_line_tex, border_line_texel_left.xy);
	
	if((border_line & MS_BORDER_BOTTOM) == MS_BORDER_BOTTOM)
		c *= texture2D(border_line_tex, border_line_texel_bottom.xy);

	if((border_line & MS_BORDER_RIGTH) == MS_BORDER_RIGTH)
		c *= texture2D(border_line_tex, border_line_texel_right.xy);

	if((border_line & MS_BORDER_TOP) == MS_BORDER_TOP)
		c *= texture2D(border_line_tex, border_line_texel_top.xy);

	gl_FragColor = c;
}
