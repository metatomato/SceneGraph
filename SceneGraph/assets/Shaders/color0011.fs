precision highp float;

uniform vec3 albedoColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;

uniform sampler2D albedoMap;
uniform sampler2D receiveShadow;

varying highp vec3 frag_normal;
varying highp vec2 frag_texCoord;
varying highp vec3 eye_vec;
varying highp vec3 light_vec;



float LinearizeDepth(vec2 uv)
{
    float n = 1.0; // camera z near
    float f = 1000.0; // camera z far
    float z = texture2D(receiveShadow, uv).x;
    return (2.0 * n) / (f + n - z * (f - n));
}


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

    float depthValue = LinearizeDepth(frag_texCoord);
    
    
    vec3 albedoMapComponent = texture2D(albedoMap,frag_texCoord).xyz;
    
    vec3 shadowMapComponent = vec3(depthValue,depthValue,depthValue);
    
    
    float alpha = texture2D(albedoMap,frag_texCoord).a;

    vec3 colorMix = mix(diffuseColor, albedoMapComponent, alpha);
    
     
    
    vec3 diffuseComponent = colorMix * intensity;

  
    gl_FragColor = vec4(0.5 * albedoMapComponent,1.0) + vec4(1.7 * diffuseComponent,1.0)  + vec4(1.3 * specularColor * shininess,1.0);
    
   // gl_FragColor = vec4(shadowMapComponent,1.0) ;
}