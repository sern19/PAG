#version 330

layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

in Data
{
    vec3 position;
    vec3 normal;
    vec3 tangent;
    vec3 bitangent;
    mat4 mvp;
} vdata[];

out vec4 color;
uniform mat3 normalMatrix;

void main()
{
    const vec4 green = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    const vec4 red = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    const vec4 blue = vec4(0.0f, 0.0f, 1.0f, 1.0f);

    for (int i = 0; i < gl_in.length(); i++)
    {
        gl_Position = vdata[i].mvp * vec4(vdata[i].position,1);
        color = blue;
        EmitVertex();

        gl_Position = vdata[i].mvp * vec4(vdata[i].position + vdata[i].normal*0.1,1);
        color = blue;
        EmitVertex();

        EndPrimitive();
        
        gl_Position = vdata[i].mvp * vec4(vdata[i].position,1);
        color = red;
        EmitVertex();
        
        gl_Position = vdata[i].mvp * vec4(vdata[i].position + vdata[i].tangent*0.1,1);
        color = red;
        EmitVertex();
        
        EndPrimitive();
        
        gl_Position = vdata[i].mvp * vec4(vdata[i].position,1);
        color = green;
        EmitVertex();
        
        gl_Position = vdata[i].mvp * vec4(vdata[i].position + vdata[i].bitangent*0.1,1);
        color = green;
        EmitVertex();
        
        EndPrimitive();
    }
}
