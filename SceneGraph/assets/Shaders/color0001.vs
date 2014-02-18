uniform mat4 MODEL;
uniform mat4 VIEW;
uniform mat4 PROJ;

uniform mat4 MV;
uniform mat3 MV_I;
uniform mat4 MVP;

attribute vec3 vertex;
attribute vec3 normal;

varying highp vec3 frag_normal;
varying highp vec3 eye_vec;
varying highp vec3 light_vec;

void main()
{
    vec4 lightPos =  VIEW * vec4( 0.0 , 300000.0 , 0.0 , 1.0 ) ;

    frag_normal = normalize ( MV_I *  normal );
    
    eye_vec      = vec3( MV   *  vec4( vertex , 1.0 )  );
  
    light_vec = normalize( vec3( lightPos ) );
  
     gl_Position = MVP * vec4(vertex,1.0);
}
