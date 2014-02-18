uniform mat4 MVP;

attribute vec3 vertex;



void main()
{
    gl_PointSize = 10.0;
    
    gl_Position = MVP * vec4(vertex,1.0);
}
