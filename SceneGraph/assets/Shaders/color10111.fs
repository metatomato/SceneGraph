precision highp float;

uniform vec3 albedoColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;

uniform sampler2D albedoMap;
uniform sampler2D diffuseMap;

varying highp vec3 frag_normal;
varying highp vec2 frag_texCoord;

varying highp vec3 light_vec;
varying highp vec3 light_vec_ntb;
varying highp vec3 eye_vec;
varying highp vec3 half_vec;

void main() {
    
  
    
    vec3 frag_normal_n      = normalize( frag_normal );
    vec3 light_vec_n        = normalize( light_vec );
    vec3 light_vec_ntb_n    = normalize( light_vec_ntb );
    vec3 eye_vec_n          = normalize( eye_vec );
    vec3 half_vec_n         = normalize( half_vec );
    

    vec4 tex = texture2D(diffuseMap,frag_texCoord);
    
    vec3 normalBump =  2.0 * tex.rgb - vec3( 1.0 , 1.0 , 1.0 ) ;
    
    vec3 normalBump_n = normalize(normalBump);
    
    float lamberFactor = max( dot ( light_vec_ntb_n , normalBump_n ) , 0.0 ) ;
    
 
    
    float dot_prod = dot(frag_normal_n, light_vec_n);
    
    float intensity = max(dot_prod, 0.0);
    
    float shininess = 0.0 ;
    if( lamberFactor > 0.0 ) {
        
    }
    shininess = pow (max (dot (half_vec_n, normalBump_n), 0.0), 50.0);
    
    float alpha = texture2D(albedoMap,frag_texCoord).a;
    
    vec3 colorMix = mix(diffuseColor, texture2D(albedoMap,frag_texCoord).xyz, alpha);
        
    vec3 diffuseComponent = colorMix * lamberFactor ;
    
    


    gl_FragColor = vec4(0.2 * texture2D(albedoMap,frag_texCoord).rgb,1.0) + vec4(1.7 * diffuseComponent,1.0)  + vec4(1.3 * vec3(1.0,1.0,1.0) * shininess,1.0);
}