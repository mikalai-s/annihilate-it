
precision lowp float;

uniform sampler2D u2_texture;

varying vec4 texcoord;

const float k = 1.0;

vec4 getTextureColor()
{
    vec4 c = texture2D(u2_texture, texcoord.st);
    c.a = (c.r + c.b + c.g);// / k;

    return c;
}

void main()
{  
    gl_FragColor = getTextureColor();
}
