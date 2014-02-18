precision highp float;

uniform mat4 VIEW;
uniform mat4 MV;

uniform vec3 albedoColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;

uniform sampler2D albedoMap;
uniform sampler2D receiveShadow;

uniform mat4 light_0;
uniform mat4 light_1;

varying highp vec3 frag_normal;
varying highp vec2 frag_texCoord;
varying highp vec3 vert;

varying highp vec4 shadow_proj;


void computeDiffuse(inout float result, in vec3 norm, in vec3 light){
    float value = dot(norm,light);
    result += max(value,0.0);
}

void computeSpecular(inout float result , in vec3 normal, in vec3 light, in vec3 eye){
    vec3 reflection = reflect( -light , normal );
    float dot_prod = dot( normalize(reflection) , -eye );
    result += pow(max( dot_prod , 0.0 ), 50.0 );
}

void computeShadow(inout float result , in vec4 proj , in sampler2D shadow_map , in float angle){
    //float bias = 0.00003*tan(acos(angle));
    //float bias = clamp(biass, 0.0 ,0.00003);
    float bias = 0.00003;
    
    float fAmbient = 0.6;
    float comp = (proj.z / proj.w) -  bias;
    float depth = texture2DProj(shadow_map, proj).r;
    result = comp <= depth ? 1.0 : fAmbient;
    
}

void main() {
    
	vec4 light_pos_0 = VIEW * light_0[0];
	vec4 light_pos_1 = VIEW * light_1[0];
    vec4 eye = MV * vec4(vert,1.0);
    
    vec3 dist_0   = normalize(light_pos_0.xyz - eye.xyz);
    vec3 dist_1   = normalize(light_pos_1.xyz  - eye.xyz);
     
       
    vec3 frag_normal_n = normalize( frag_normal );
    vec3 eye_vec_n     = normalize( eye.xyz );
    
    float intensity = 0.0;
    
    computeDiffuse( intensity , frag_normal_n , dist_0 );
    computeDiffuse( intensity , frag_normal_n , dist_1 );
    
    
    float shadowVal = 1.0;
    float shininess = 0.0 ;
    
    if( intensity > 0.0 ) {
        computeSpecular( shininess , frag_normal_n , dist_0 , eye_vec_n );
        computeSpecular( shininess , frag_normal_n , dist_1 , eye_vec_n );
        computeShadow( shadowVal , shadow_proj , receiveShadow , intensity);
    }
    
    
    vec3 albedoMapComponent = texture2D(albedoMap,frag_texCoord).xyz;
    
    float alpha = texture2D(albedoMap,frag_texCoord).a;

    vec3 diffuseComponent = mix(diffuseColor, albedoMapComponent, alpha);
    
    
    vec3 finalColor = intensity * albedoMapComponent
                    + shininess * specularColor
                    + 0.1       * albedoMapComponent;
    
    finalColor *= shadowVal;
    
    gl_FragColor = vec4(finalColor,1.0);
}





