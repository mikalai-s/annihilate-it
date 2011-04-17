attribute vec4 position;
attribute vec4 color;

varying vec4 outcolor;

void main()
{
    gl_Position = position;
    
    outcolor = color;
}
