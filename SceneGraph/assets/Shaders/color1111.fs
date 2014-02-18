precision highp float;

uniform mat3 MV_I;
uniform mat4 MV;
uniform mat4 VIEW;

uniform vec3 albedoColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;

uniform sampler2D albedoMap;
uniform sampler2D diffuseMap;
uniform sampler2D receiveShadow;

uniform mat4 light_0;
uniform mat4 light_1;

varying highp vec3 frag_vertex;
varying highp vec3 frag_normal;
varying highp vec3 frag_tangent;
varying highp vec2 frag_texCoord;


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

void changeBase(inout vec3 result , in vec3 t , in vec3 b , in vec3 n){
    vec3 v;
    v.x = dot (result, t);
    v.y = dot (result, b);
    v.z = dot (result, n);
    result = normalize (v);
}


void main() {
    
    //vec3 n = normalize( MV_I * frag_normal  );
    //vec3 t = normalize( MV_I * frag_tangent );
	vec3 n = normalize( MV * vec4(frag_normal,0.0)).xyz;
	vec3 t = normalize( MV * vec4(frag_tangent,0.0)).xyz;
  	vec3 b = cross (n, t);
    
    vec3 eye =  vec3( MV *  vec4(frag_vertex,1.0) ) ;
  	
    vec4 light_pos_0 = VIEW * light_0[0];
    vec4 light_pos_1 = VIEW * light_1[0];
    
    vec3 dist_0   = normalize(light_pos_0.xyz - eye);
    vec3 dist_1   = normalize(light_pos_1.xyz  - eye);

    changeBase( dist_0   , t , b , n );
    changeBase( dist_1   , t , b , n );
    changeBase( eye      , t , b , n );
    
    vec4 tex = texture2D( diffuseMap , frag_texCoord ) ;
    vec3 normalBump =  2.0 * tex.rgb - vec3( 1.0 , 1.0 , 1.0 ) ;
    vec3 normalBump_n = normalize(normalBump) ;
    
    float lamberFactor = 0.0;
    computeDiffuse( lamberFactor , dist_0   , normalBump_n);
    computeDiffuse( lamberFactor , dist_1 , normalBump_n);
    
    
 
    float shininess = 0.0 ;
    float shadowVal = 1.0 ;
    
    if( lamberFactor > 0.0 ) {
        computeSpecular( shininess , normalBump_n , dist_0   , eye);
        computeSpecular( shininess , normalBump_n , dist_1 , eye);
        computeShadow( shadowVal , shadow_proj , receiveShadow , lamberFactor);
    }


    vec3 albedoMapComponent = texture2D(albedoMap,frag_texCoord).xyz;
    
    float alpha = texture2D(albedoMap,frag_texCoord).a;
    
    vec3 diffuseComponent = mix(diffuseColor, albedoMapComponent, alpha);
    
    
    vec3 finalColor = 1.7 * lamberFactor * albedoMapComponent
                    + 1.3 * shininess    * albedoMapComponent
                    + 0.1                * albedoMapComponent;
    
    finalColor *= shadowVal;
    
    gl_FragColor = vec4(finalColor,1.0);



}