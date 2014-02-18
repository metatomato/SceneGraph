uniform mat3 MV_I;
uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 MVP_SHADOW;

attribute vec3 vertex;
attribute vec3 normal;
attribute vec2 texCoord;

varying highp vec3 frag_normal;
varying highp vec2 frag_texCoord;
varying highp vec3 vert;

varying highp vec4 shadow_proj;

void main()
{
    frag_texCoord = texCoord;
    
    //frag_normal = normalize ( MV_I *  normal );
    frag_normal = normalize (MV *  vec4(normal,0.0)).xyz;
    
    vert = vertex;
    
    shadow_proj = MVP_SHADOW * vec4(vertex,1.0);
    
    gl_Position = MVP * vec4(vertex,1.0);
}