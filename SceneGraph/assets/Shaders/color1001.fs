precision highp float;

uniform mat4 VIEW;
uniform mat4 MV;

uniform vec3 albedoColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;


uniform highp sampler2D receiveShadow;

uniform mat4 lights[4];


varying highp vec3 frag_normal;
varying highp vec2 frag_texCoord;

varying highp vec4 shadow_proj;

varying highp vec4 vert;


const int LIGHT_NUM_MAX = 1;

bool computeDiffuse(inout vec3 result, in vec3 norm, in vec3 light, in vec3 light_color){
    float value = dot(norm,light);
    result += light_color * max(value,0.0);
    if(value > 0.0){
    	return true;
    }else return false;
}

void computeSpecular(inout vec3 result , in vec3 normal, in vec3 light, in vec3 eye, in vec3 light_color){
    vec3 reflection = reflect( -light , normal );
    float dot_prod = dot( normalize(reflection) , -eye );
    result += light_color * pow(max( dot_prod , 0.0 ), 50.0 );
}

void computeShadow(inout float result , in vec4 proj , in sampler2D shadow_map){
    //float bias = 0.00003*tan(acos(angle));
    //float bias = clamp(biass, 0.0 ,0.00003);
    float bias = 0.00003;
    
    float fAmbient = 0.6;
    float comp = (proj.z / proj.w) -  bias;
    float depth = texture2DProj(shadow_map, proj).r;
    result = comp <= depth ? 1.0 : fAmbient;
    
}

void setupLight(inout vec3 light_v,inout vec3 color, in vec3 eye, in mat4 light_packed,const in mat4 VIEW){
	int type = int(light_packed[3].x);
	if(type == 0){
		color = vec3(0.0,0.8,1.0);
		light_v =  normalize((VIEW * light_packed[1]).xyz);
	}
	if(type == 1){
		color = vec3(1.0,0.8,0.0);
		light_v = normalize((VIEW * light_packed[0]).xyz - eye);
	}
	//light_v =  normalize((VIEW * light_packed[1]).xyz);
	//light_v = normalize((VIEW * light_packed[0]).xyz - eye);
}

void main() {

	vec3 eye 			= vert.xyz;
	vec3 frag_normal_n 	= normalize( frag_normal );
	vec3 eye_vec_n    	= normalize( eye );
	vec3 intensity 		= vec3(0.0);
	vec3 shininess 		= vec3(0.0);
	float shadowVal 	= 1.0;
	
	
	vec3 dist[LIGHT_NUM_MAX];
	vec3 color[LIGHT_NUM_MAX];
	
	computeShadow( shadowVal , shadow_proj , receiveShadow);
	
	for(int i = 0 ; i < LIGHT_NUM_MAX ; i++){
		setupLight(dist[i], color[i], eye, lights[i], VIEW);
		bool lighted = computeDiffuse( intensity , frag_normal_n , dist[i], color[i] );
		if(lighted && shadowVal >= 1.0){
			computeSpecular( shininess , frag_normal_n , dist[i]   , eye_vec_n, color[i] );
		}
	}
    
	
	
    /*
    float radius = 64.0 ;
    
    float cst[3];
    cst[0]= 1.0;
    cst[1]= 2.0/radius;
    cst[2]= 1.0/(radius*radius);
    
    float att_factor = 1.0;
    vec3 att_dis ;
    att_dis.x = 1.0;
    att_dis.z = dot(dist,dist);
    att_dis.y = sqrt(att_dis.z);
    att_factor = 1.0 / dot(att_dis, vec3( cst[0],cst[1],cst[2] ) );
    //att_factor = clamp(att_factor,att_factor,1.0);
    //lightdir = normalize(lightdir);
    */
    
    
    vec3 finalColor = intensity * diffuseColor
                    + shininess * specularColor
                    + 0.5       * albedoColor ;
    
    finalColor *= shadowVal;
    
    finalColor = clamp(finalColor, 0.0 , 1.0);
    
    gl_FragColor = vec4(finalColor,1.0);
   
}


/*
 * uniform vec3 diffuseColor;
uniform vec3 specularColor;


uniform highp sampler2D receiveShadow;

uniform mat4 light_0;
uniform mat4 light_1;
uniform mat4 light_2;
uniform mat4 light_3;

varying highp vec3 frag_normal;
varying highp vec2 frag_texCoord;

varying highp vec4 shadow_proj;

varying highp vec4 vert;


const int LIGHT_NUM_MAX = 4;

void computeDiffuse(inout vec3 result, in vec3 norm, in vec3 light, in vec3 light_color){
    float value = dot(norm,light);
    result += light_color * max(value,0.0);
}

void computeSpecular(inout vec3 result , in vec3 normal, in vec3 light, in vec3 eye, in vec3 light_color){
    vec3 reflection = reflect( -light , normal );
    float dot_prod = dot( normalize(reflection) , -eye );
    result += light_color * pow(max( dot_prod , 0.0 ), 50.0 );
}

void computeShadow(inout float result , in vec4 proj , in sampler2D shadow_map){
    //float bias = 0.00003*tan(acos(angle));
    //float bias = clamp(biass, 0.0 ,0.00003);
    float bias = 0.00003;
    
    float fAmbient = 0.6;
    float comp = (proj.z / proj.w) -  bias;
    float depth = texture2DProj(shadow_map, proj).r;
    result = comp <= depth ? 1.0 : fAmbient;
    
}

void setupLight(inout vec3 light_v,inout vec3 color, in vec3 eye, in mat4 light_packed, in mat4 VIEW){
	int type = int(light_packed[3].x);
	if(type == 0){
		color = vec3(0.0,0.8,1.0);
	}
	if(type == 1){
			color = vec3(1.0,0.8,0.0);
		}
	
	
	light_v = normalize((VIEW * light_packed[0]).xyz - eye);
	
}

void main() {

	vec3 dist_0;
	vec3 dist_1;
	vec3 dist_2;
	vec3 dist_3;
	
	vec3 color_0;
	vec3 color_1;
	vec3 color_2;
	vec3 color_3;
	
    //vec4 light_pos_0 = VIEW * light_0[0];
    //vec4 light_pos_1 = VIEW * light_1[0];
    vec3 eye = vert.xyz;
    
    setupLight(dist_0, color_0, eye, light_0, VIEW);
    setupLight(dist_1, color_1, eye, light_1, VIEW);
    setupLight(dist_2, color_2, eye, light_2, VIEW);
    setupLight(dist_3, color_3, eye, light_3, VIEW);
    
    // vec3 dist_0   = normalize(light_pos_0.xyz - eye);
     //vec3 dist_1   = normalize(light_pos_1.xyz  - eye);
    
 
    
    vec3 frag_normal_n = normalize( frag_normal );
    vec3 eye_vec_n     = normalize( eye );
    
    vec3 intensity = vec3(1.0);
   
    computeDiffuse( intensity , frag_normal_n , dist_0, color_0 );
    computeDiffuse( intensity , frag_normal_n , dist_1, color_1 );
    computeDiffuse( intensity , frag_normal_n , dist_2, color_2 );
    computeDiffuse( intensity , frag_normal_n , dist_3, color_3 );
    

    float shadowVal = 1.0;
    vec3 shininess = vec3(1.0);
    
    if( intensity.x > 0.0) {
        computeSpecular( shininess , frag_normal_n , dist_0   , eye_vec_n, color_0 );
        computeSpecular( shininess , frag_normal_n , dist_1   , eye_vec_n, color_1 );
        computeSpecular( shininess , frag_normal_n , dist_2   , eye_vec_n, color_2 );
        computeSpecular( shininess , frag_normal_n , dist_3   , eye_vec_n, color_3 );
        computeShadow( shadowVal , shadow_proj , receiveShadow);
    }
    
    
    vec3 finalColor = intensity * diffuseColor;
                    + shininess * specularColor
                    + 0.1       * albedoColor ;
    
    finalColor *= shadowVal;
    
    finalColor = clamp(finalColor, 0.0 , 1.0);
    
    gl_FragColor = vec4(finalColor,1.0);
   
}
 */