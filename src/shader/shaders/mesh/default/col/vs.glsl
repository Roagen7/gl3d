#version 330 core
layout (location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNorm;
layout(location = 2) in vec3 aCol;
layout(location = 3) in vec2 aTex;



out vec3 crntPos;
out vec3 Normal;
out vec3 color;
out vec2 texCoord;
//
//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 proj;

uniform mat4 camMatrix;
uniform mat4 model;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;


void main()
{
    //    gl_Position = vec4(aPos.x + aPos.x * scale, aPos.y + aPos.y * scale, aPos.z + aPos.z * scale, 1.0);
    //    gl_Position = proj * view * model * vec4(aPos, 1.0);


//    vec4 rotNorm = mat3(MVI) * aNorm;
    crntPos = vec3(model * translation * -rotation * scale * vec4(aPos, 1.0f));
//    Normal = vec3(rotNorm.x,rotNorm.y, rotNorm.z);
    Normal = mat3(transpose(inverse(model))) * aNorm;
//    Normal = aNorm;
    color = aCol;
    texCoord = mat2(0.0,-1.0, 1.0, 0.0) * aTex;

    gl_Position = camMatrix * vec4(crntPos,1.0);

}