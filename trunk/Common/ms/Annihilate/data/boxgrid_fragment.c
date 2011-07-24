
precision lowp float;

uniform vec4 color;
uniform ivec4 lineBorder;
uniform ivec4 cornerBorder;

uniform sampler2D borderExternalLineTex;
uniform sampler2D borderInternalLineTex;
uniform sampler2D borderCornerTex;

varying vec2 _borderTexelLeft;
varying vec2 _borderTexelBottom;
varying vec2 _borderTexelRight;
varying vec2 _borderTexelTop;

void main()
{
    vec4 c = color;

    if(lineBorder[0] == 1)
        c *= texture2D(borderExternalLineTex, _borderTexelLeft);
    else
        c *= texture2D(borderInternalLineTex, _borderTexelLeft);
    
    if(lineBorder[1] == 1)
        c *= texture2D(borderExternalLineTex, _borderTexelTop);
    else
        c *= texture2D(borderInternalLineTex, _borderTexelTop);
    
    if(lineBorder[2] == 1)
        c *= texture2D(borderExternalLineTex, _borderTexelRight);
    else
        c *= texture2D(borderInternalLineTex, _borderTexelRight);

    if(lineBorder[3] == 1)
        c *= texture2D(borderExternalLineTex, _borderTexelBottom);
    else
        c *= texture2D(borderInternalLineTex, _borderTexelBottom);
       
    if(cornerBorder[0] == 1)
        c *= texture2D(borderCornerTex, _borderTexelLeft);
    
    if(cornerBorder[1] == 1)
        c *= texture2D(borderCornerTex, _borderTexelTop);
    
    if(cornerBorder[2] == 1)
        c *= texture2D(borderCornerTex, _borderTexelRight);

    if(cornerBorder[3] == 1)
        c *= texture2D(borderCornerTex, _borderTexelBottom);
        
    gl_FragColor = c;
}
