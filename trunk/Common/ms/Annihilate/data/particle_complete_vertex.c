attribute vec4 a2_position;
attribute vec4 a2_texcoord;

varying vec4 texcoord;

void main()
{
	gl_Position = a2_position;
	
	texcoord = a2_texcoord;
}
