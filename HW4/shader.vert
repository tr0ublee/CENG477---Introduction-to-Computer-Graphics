#version 410

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 txtCoords;

// Data from CPU 
uniform mat4 MVP; // ModelViewProjection Matrix
uniform mat4 MV; // ModelView idMVPMatrix
uniform vec3 cameraPosition;
uniform float heightFactor;

// Texture-related data
// uniform sampler2D rgbTexture;
uniform sampler2D heightTextureData; //????
uniform int widthTexture;
uniform int heightTexture;


// Output to Fragment Shader
out vec2 textureCoordinate; // For texture-color
out vec3 vertexNormal; // For Lighting computation
out vec3 ToLightVector; // Vector from Vertex to Light;
out vec3 ToCameraVector; // Vector from Vertex to Camera;

// Our added data 
uniform vec3 lightPos;
uniform int textureDelta;

float getHeight(vec2 textCoords) {
    vec4 heightVec = texture(heightTextureData, textCoords); //get RGBA from the texture
    return heightVec.r * heightFactor; // use the red channel
}

float getNeighborHeight(vec3 pos) {
    float u = 1.0 - (float(pos.x) / widthTexture);
    float v = 1.0 - (float(pos.z) / heightTexture);
    u += float(textureDelta)/widthTexture;
    vec2 uv = vec2(u,v);
    return getHeight(uv);
}
/*
____v1__v2
| / |0/1|
v0--p---v3
|1/0|+/ |
v5--v4---
*/
vec3 getNormal(vec3 p) {
    // (v1-v0) x (v2-v0)
    vec3 v0,v1,v2,v3,v4,v5;
    v0 = v1 = v2 = v3 = v4 = v5 = vec3(0.0,0.0,0.0);
    float d = 1.0f;
    v0 = vec3(p.x - d   , getNeighborHeight(vec3(p.x- d,    p.y,    p.z  ))   , p.z);
    v1 = vec3(p.x       , getNeighborHeight(vec3(p.x   ,    p.y,    p.z-d))   , p.z- d);
    v2 = vec3(p.x+d     , getNeighborHeight(vec3(p.x+d ,    p.y,    p.z-d))   , p.z-d);
    v3 = vec3(p.x+d     , getNeighborHeight(vec3(p.x+d ,    p.y,    p.z  ))   , p.z);
    v4 = vec3(p.x       , getNeighborHeight(vec3(p.x   ,    p.y,    p.z+d))   , p.z+d);
    v5 = vec3(p.x-d     , getNeighborHeight(vec3(p.x-d ,    p.y,    p.z+d))   , p.z+d);
    vec3 triangleNW = cross(v1-p, v0-p);
    vec3 triangleNE0 = cross(v2-p, v1-p);
    vec3 triangleNE1 = cross(v3-p, v2-p);
    vec3 triangleSW0 = cross(v5-p, v4-p);
    vec3 triangleSW1 = cross(v0-p, v5-p);
    vec3 triangleSE = cross(v4-p, v3-p);
    vec3 result = triangleNW + triangleNE0 + triangleNE1 + triangleSW0 + triangleSW1 + triangleSE;
    return normalize(result);
}


void main()
{
    // get texture value, compute height
    // compute normal vector using also the heights of neighbor vertices

    // compute toLight vector vertex coordinate in VCS
    // vertexNormal = normalize(normal);
    // set gl_Position variable correctly to give the transformed vertex position
    // gl_Position = vec4(0,0,0,0); // this is a placeholder. It does not correctly set the position 
    textureCoordinate = txtCoords;
    textureCoordinate.x += float(textureDelta)/widthTexture;
    mat4 invTMp = transpose(inverse(MV));
    vec3 posCopy = vec3(position.x, getHeight(textureCoordinate), position.z);
    vertexNormal =  vec3(invTMp * vec4(getNormal(posCopy),0.0f));
    ToCameraVector = vec3(MV * vec4(normalize(cameraPosition.xyz - posCopy),0.0f));
    ToLightVector = vec3(MV * vec4(normalize(lightPos - posCopy),0.0f));
    gl_Position = MVP * vec4(posCopy, 1.0f);
}
