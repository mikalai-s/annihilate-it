attribute vec4 position;
attribute vec4 texcoord;

varying vec4 texcoord0;

void main()
{
    gl_Position = position;
    
    texcoord0 = texcoord;
}