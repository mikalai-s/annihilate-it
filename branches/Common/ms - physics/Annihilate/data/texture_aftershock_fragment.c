
precision mediump float;

uniform sampler2D tex;
uniform sampler2D ns0;

uniform vec2 ep;
varying float power0;

varying vec4 texcoord0;
varying vec4 outcolor;

varying float radius0;

varying vec4 fake_FragCoord1;

varying vec2 buffersize1;

float l = 50.0;
float t = 0.1;

vec4 testTexture()
{
	vec4 c = texture2D(tex, texcoord0.st);
	return c;
}

vec4 testColorVarying()
{
   return outcolor;
}


float rb, ra, rd, r;

float curx()
{
    return gl_FragCoord.x;
}


float cury()
{
    return gl_FragCoord.y;
}

float radius2()
{
    float x = curx() - ep.x;
    float y = cury() - ep.y;
    return r = sqrt(x * x  + y * y);
}

bool condition1()
{
	if(radius0 == -1.0)
		return false;
		
    return r <= ra && r >= rb;
}

bool condition2()
{
	if(radius0 == -1.0)
		return false;
		
    return r >= ra && r <= rd;
}



void main()
{    
	vec3 noiseVec;
	vec2 displacement;
	float scaledradius;
	
	l *= power0;


    ra = radius0 / 2.0;
    rb = ra - l;
    rd = ra + l;
    r = radius2();  


	displacement = texcoord0.st;
	
	
    if(condition1())    
    {
        scaledradius = t * 0.1;

		displacement.x += scaledradius;
		displacement.y -= scaledradius;

		noiseVec = normalize(texture2D(ns0, displacement.xy).xyz);
		noiseVec = (noiseVec * 2.0 - 1.0) * 0.035 * (1.0 - (ra - r) / l) * power0;
		
		gl_FragColor = texture2D(tex, texcoord0.st + noiseVec.xy);
    }
    else if(condition2())
	{
		scaledradius = t * 0.1;

		displacement.x += scaledradius;
		displacement.y -= scaledradius;

		noiseVec = normalize(texture2D(ns0, displacement.xy).xyz);
		noiseVec = (noiseVec * 2.0 - 1.0) * 0.035 * (1.0 - (r - ra) / l) * power0;
		
		gl_FragColor = texture2D(tex, texcoord0.st + noiseVec.xy);
	}
	else
	{
		gl_FragColor = testTexture();
	}
}
