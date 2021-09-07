#version 330 core
layout (location = 0) in vec3 aPos;
layout(location = 1) in vec3 aCol;
layout(location = 2) in vec2 aTex;
layout(location = 3) in vec3 aNorm;

out vec3 color;
out vec2 texCoord;
out vec3 Normal;
out vec3 crntPos;
//
//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 proj;

uniform mat4 camMatrix;
uniform mat4 model;

void main()
{
//    gl_Position = vec4(aPos.x + aPos.x * scale, aPos.y + aPos.y * scale, aPos.z + aPos.z * scale, 1.0);
//    gl_Position = proj * view * model * vec4(aPos, 1.0);
    crntPos = vec3(model * vec4(aPos, 1.0f));

    gl_Position = camMatrix * vec4(crntPos,1.0);
    color = aCol;
    texCoord = aTex;
    Normal = aNorm;
}