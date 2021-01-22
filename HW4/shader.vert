#version 410

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
// Data from CPU 
uniform mat4 MVP; // ModelViewProjection Matrix
uniform mat4 MV; // ModelView idMVPMatrix
uniform vec4 cameraPosition;
uniform float heightFactor;

// Texture-related data
uniform sampler2D rgbTexture;
uniform int widthTexture;
uniform int heightTexture;


// Output to Fragment Shader
out vec2 textureCoordinate; // For texture-color
out vec3 vertexNormal; // For Lighting computation
out vec3 ToLightVector; // Vector from Vertex to Light;
out vec3 ToCameraVector; // Vector from Vertex to Camera;

// Our added data 
uniform vec3 lightPos;




void main()
{

    // get texture value, compute height
    // compute normal vector using also the heights of neighbor vertices

    // compute toLight vector vertex coordinate in VCS
    vertexNormal = normalize(normal);
    // set gl_Position variable correctly to give the transformed vertex position
//    gl_Position = vec4(0,0,0,0); // this is a placeholder. It does not correctly set the position 
    ToCameraVector = normalize(cameraPosition.xyz - position);
    ToLightVector = normalize(lightPos - position);
    gl_Position = MVP * vec4(position.xyz, 1.0f); // this is a placeholder. It does not correctly set the position 
    // gl_Position = vec4(1.0,2.0,3.0,1.0);
}
