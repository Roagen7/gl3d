#version 330 core
in vec3 color;
in vec2 texCoord;
in vec3 crntPos;
in vec3 Normal;

//out vec4 FragColor;

uniform sampler2D tex0; // ??

uniform vec4 lightColor;
uniform vec3 lightPos;

void main()
{
//   FragColor = vec4(color, 1.0f);
   vec3 normal = normalize(Normal);
   vec3 lightDirection = normalize(lightPos - crntPos);

   float diffuse = max(dot(normal, lightDirection), 0.0f);

   gl_FragColor = texture(tex0, texCoord) * lightColor * diffuse;
}