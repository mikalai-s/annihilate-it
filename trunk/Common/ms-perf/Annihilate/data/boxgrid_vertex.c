uniform mat4 mvp;

attribute vec4 position;

attribute vec2 borderTexelLeft;
attribute vec2 borderTexelBottom;
attribute vec2 borderTexelRight;
attribute vec2 borderTexelTop;

varying vec2 _borderTexelLeft;
varying vec2 _borderTexelBottom;
varying vec2 _borderTexelRight;
varying vec2 _borderTexelTop;

void main()
{
    vec4 p = mvp * position;

	gl_Position = p;

	_borderTexelLeft = borderTexelLeft;
	_borderTexelBottom = borderTexelBottom;
	_borderTexelRight = borderTexelRight;
	_borderTexelTop = borderTexelTop;
}
