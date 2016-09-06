precision mediump float;

uniform sampler2D tex;
uniform sampler2D ns0;

uniform vec2 location;
uniform float radius;
uniform float power; // make radius less in course of time

varying vec4 texcoord0;

void main()
{
    vec4 color;

    vec3 shockParams = vec3(10.0, 0.2, 0.1 * power);    
    vec2 uv = texcoord0.xy;    
    vec2 texCoord = uv;
    
    float distance = distance(uv, location);
    float diff = (distance - radius);
    if (diff <= shockParams.z && diff >= -shockParams.z)
    {
        float powDiff = 1.0 - pow(abs(diff * shockParams.x), shockParams.y);
        float diffRadius = diff  * powDiff;
        vec2 diffUV = normalize(uv - location);
        texCoord = uv + (diffUV * diffRadius);
         
        color = texture2D(tex, texCoord);
        color += (diffRadius * 2.5);
    }
    else
    {
        color = texture2D(tex, texCoord);
    }    
    
    gl_FragColor = color;
}
