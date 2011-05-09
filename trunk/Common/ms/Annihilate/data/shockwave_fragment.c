precision mediump float;

uniform sampler2D tex;
uniform sampler2D ns0;

uniform vec2 location;
uniform float radius;
uniform float power;

varying vec4 texcoord0;

void main()
{
	vec3 shockParams = vec3(10.0, 0.2, 0.1 * power);	
	vec2 uv = texcoord0.xy;
	vec2 texCoord = uv;
	float distance = distance(uv, location);
	if ( (distance <= (radius + shockParams.z)) && (distance >= (radius - shockParams.z)) )
	{
		float diff = (distance - radius);
		float powDiff = 1.0 - pow(abs(diff * shockParams.x), shockParams.y);
		float diffRadius = diff  * powDiff;
		vec2 diffUV = normalize(uv - location);
		texCoord = uv + (diffUV * diffRadius);
	}

	gl_FragColor = texture2D(tex, texCoord);
}
