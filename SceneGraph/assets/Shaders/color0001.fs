precision highp float;

uniform vec3 albedoColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;

varying highp vec3 frag_normal;
varying highp vec3 eye_vec;
varying highp vec3 light_vec;

void main() {
    
    
    vec3 frag_normal_n = normalize( frag_normal );
    vec3 eye_vec_n     = normalize( eye_vec );
    vec3 light_vec_n   = normalize( light_vec );

  
    
    float dot_prod = dot(frag_normal_n, light_vec_n);
    
    float intensity = max(dot_prod, 0.0);
    
    
    
    vec3 reflec_vec = reflect(-light_vec_n, frag_normal_n);

    dot_prod = dot( normalize(reflec_vec), -eye_vec_n );
    
    float shininess = 0.0 ;
    if( intensity > 0.0 ) {
         shininess = pow(max( dot_prod , 0.0 ), 50.0 );
    }
   
    
    gl_FragColor =  vec4(1.0 * albedoColor , 1.0 ) + vec4(0.7 * diffuseColor * intensity,1.0) + vec4(0.3 * specularColor * shininess,1.0);
}