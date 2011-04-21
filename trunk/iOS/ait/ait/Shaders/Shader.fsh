//
//  Shader.fsh
//  ait
//
//  Created by Mikalai Silivonik on 11-04-20.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

varying lowp vec4 colorVarying;

void main()
{
    gl_FragColor = colorVarying;
}
