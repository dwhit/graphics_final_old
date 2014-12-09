#version 330 core

in vec3 position_worldSpace; // position of vertex in world space
in vec3 position_cameraSpace; // position of vertex in camera space
in vec3 normal_cameraSpace; // normal of vertex in camera space
in vec3 eyeDirection_cameraSpace; // vector from vertex to eye
in vec3 lightPosition_worldSpace;
in vec2 uv; // uv coordinate for frag position

uniform mat4 mvp; // Modelview Projection matrix. This maps the vertices in model (object) space to film coordinates (world->film)
uniform mat4 v; // viewing matrix (world -> camera)
uniform mat4 m; // model matrix (object -> world)
uniform vec3 diffuse_color;
uniform vec3 ambient_color;
uniform vec3 color;
uniform vec3 lightPosition_cameraSpace;
uniform sampler2D textureSampler;

out vec4 fragColor; //output color

void main(){
    //Convenience code for setting a single color
    vec3 diffuseColor;
    vec3 ambientColor;
    if (dot(color, vec3(1,1,1)) > 0){
        diffuseColor = color;
        ambientColor = vec3(.05,.05,.05);
    } else{
        diffuseColor = diffuse_color;
        ambientColor = ambient_color;
    }

    //Diffuse computation
    vec3 normal = normal_cameraSpace;
    vec3 vertex_to_light_cameraSpace = lightPosition_cameraSpace - position_cameraSpace;
    float lam = max(dot(vertex_to_light_cameraSpace, normal), 0.0);
    float distance = length(vertex_to_light_cameraSpace);
    float light_power = 25;

    float alpha = .3f; // Blend factor

    vec3 Idiff = clamp(light_power*lam / (distance*distance)* texture2D(textureSampler, uv).rgb, 0, 1);

    fragColor =  vec4(clamp(ambientColor + Idiff, 0, 1),1);

    // Uncomment this line to visualize the normals where rgb maps to xyz of the normal.
//    fragColor = vec4(normal, 1);
}
