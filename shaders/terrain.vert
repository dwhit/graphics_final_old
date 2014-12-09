#version 330 core

in vec3 position; // Position of the vertex
in vec3 normal; // normal of the vertex
in vec2 texCoord; // UV texture coordinates of the vertex

out vec3 position_worldSpace; // position of vertex in world space
out vec3 position_cameraSpace; // position of vertex in camera space
out vec3 normal_cameraSpace; // normal of vertex in camera space
out vec3 eyeDirection_cameraSpace; // vector from vertex to eye
out vec3 lightPosition_worldSpace;
out vec2 uv; //UV texture coordinates of the vertex

uniform mat4 m; // model matrix (object -> world)
uniform mat4 v; // viewing matrix (world -> camera)
uniform mat4 p; // projection matrix
uniform mat4 mvp; // Modelview Projection matrix. (object -> film)
uniform vec3 lightPosition_cameraSpace;

void main(){
        // Vertex data
        position_worldSpace = (m * vec4(position, 1.0)).xyz;
        position_cameraSpace = ( v*m * vec4(position, 1.0)).xyz;
        uv = texCoord;
        mat4 normalMatrix = transpose(inverse(v*m));
        normal_cameraSpace = (normalMatrix * vec4(normal,0)).xyz;

        //Camera data
        eyeDirection_cameraSpace = vec3(0,0,0) - position_cameraSpace;

        lightPosition_worldSpace = (inverse(v) * vec4(lightPosition_cameraSpace,1.0)).xyz;

        gl_Position =  mvp*vec4(position, 1.0);
}
