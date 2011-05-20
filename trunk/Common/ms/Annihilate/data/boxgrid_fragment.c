
precision mediump float;

uniform sampler2D border_line_tex;
uniform sampler2D border_corner_tex;

varying vec4 border_line_texel_left;
varying vec4 border_line_texel_bottom;
varying vec4 border_line_texel_right;
varying vec4 border_line_texel_top;

varying vec4 border_corner_texel_left;
varying vec4 border_corner_texel_bottom;
varying vec4 border_corner_texel_right;
varying vec4 border_corner_texel_top;

varying vec4 outcolor;

varying vec4 border_corner_texel;

void main()
{
	vec4 c = outcolor;

	if(border_line_texel_left.w != -1.0)
		c *= texture2D(border_line_tex, border_line_texel_left.xy);
	
	if(border_line_texel_bottom.w != -1.0)
		c *= texture2D(border_line_tex, border_line_texel_bottom.xy);

	if(border_line_texel_right.w != -1.0)
		c *= texture2D(border_line_tex, border_line_texel_right.xy);

	if(border_line_texel_top.w != -1.0)
		c *= texture2D(border_line_tex, border_line_texel_top.xy);

	if(border_corner_texel_left.z != -1.0)
		c *= texture2D(border_corner_tex, border_corner_texel_left.xy);

	if(border_corner_texel_bottom.z != -1.0)
		c *= texture2D(border_corner_tex, border_corner_texel_bottom.xy);

	if(border_corner_texel_right.z != -1.0)
		c *= texture2D(border_corner_tex, border_corner_texel_right.xy);

	if(border_corner_texel_top.z != -1.0)
		c *= texture2D(border_corner_tex, border_corner_texel_top.xy);


	gl_FragColor = c;
}
