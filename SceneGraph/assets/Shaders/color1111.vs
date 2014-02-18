uniform mat4 MVP;
uniform mat4 MVP_SHADOW;

attribute vec3 vertex;
attribute vec3 normal;
attribute vec3 tangent;
attribute vec2 texCoord;

varying highp vec3 frag_vertex;
varying highp vec3 frag_normal;
varying highp vec3 frag_tangent;
varying highp vec2 frag_texCoord;


varying highp vec4 shadow_proj;

void main()
{
   
    frag_vertex = vertex ;
    
    frag_normal = normal ;
    
    frag_tangent = tangent ;
    
    frag_texCoord = texCoord;
    
    shadow_proj = MVP_SHADOW * vec4(vertex,1.0);
   
    gl_Position = MVP * vec4(vertex,1.0);
}