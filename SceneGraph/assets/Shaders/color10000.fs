precision highp float;

uniform vec3 albedoColor;
uniform vec3 diffuseColor;
uniform int MODE;

void main() {
    vec3 color = vec3(0.0,0.0,1.0);
    
    if(MODE == 0){ color = vec3(1.0,0.5,0.0);}
    if(MODE == 1){ color = vec3(0.0,1.0,1.0);}
    if(MODE == 2){ color = diffuseColor;     }
    if(MODE == 3){ color = vec3(0.0,1.0,1.0);}
    if(MODE == 4){ color = vec3(0.0,0.0,1.0);}
    if(MODE == 5){ color = vec3(1.0,0.0,0.0);}
    
    gl_FragColor =  vec4(color , 1.0 ) ;

}

//enum DEBUG_MODE {POINT_MODE,LINE_MODE,TRI_MODE,BOX_MODE, NORMAL_MODE, TANGENT_MODE};
