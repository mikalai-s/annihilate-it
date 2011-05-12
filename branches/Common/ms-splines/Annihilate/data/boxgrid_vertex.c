attribute vec4 position;
attribute vec4 color;

varying vec4 outcolor;

void main()
{
	vec4 p = position;

	// invert y and center
	p.x = p.x - 1.0;	
	p.y = 1.0 - p.y;

    gl_Position = p;
    
    outcolor = color;
}
