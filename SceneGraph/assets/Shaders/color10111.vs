uniform mat4 MODEL;
uniform mat4 VIEW;
uniform mat4 PROJ;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat3 MV_I;

attribute vec3 vertex;
attribute vec3 normal;
attribute vec3 tangent;
attribute vec2 texCoord;

varying highp vec3 frag_normal;
varying highp vec2 frag_texCoord;

varying highp vec3 light_vec;
varying highp vec3 light_vec_ntb;
varying highp vec3 eye_vec;
varying highp vec3 half_vec;

void main()
{
      gl_PointSize = 5.0;
    
    vec3 n = normalize( MV_I * normal  );
    vec3 t = normalize( MV_I * tangent );
  	vec3 b = cross (n, t);
    
    vec4 lightPos =  VIEW * vec4( 0.0 , 0.0 , 100.0 , 1.0 ) ;

    vec3 vertexPosition = normalize( vec3( MV *  vec4(vertex,1.0) ) );
  	vec3 lightDir = normalize( vec3( lightPos ) );
  	
  	 vec3 v;
		  v.x = dot (lightDir, t);
		  v.y = dot (lightDir, b);
		  v.z = dot (lightDir, n);
		  light_vec_ntb = normalize (v);
		
		  
		  v.x = dot (vertexPosition, t);
		  v.y = dot (vertexPosition, b);
		  v.z = dot (vertexPosition, n);
		  eye_vec = normalize (v);

 
		  vec3 halfVector = normalize( (vertexPosition + lightDir) / 2.0 );
		  v.x = dot (halfVector, t);
		  v.y = dot (halfVector, b);
		  v.z = dot (halfVector, n);
		  half_vec = normalize (v);

    light_vec = lightDir ;

    frag_normal = n ;
    
    frag_texCoord = texCoord;
    
   
    gl_Position = MVP * vec4(vertex,1.0);
}