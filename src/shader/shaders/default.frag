#version 330 core
in vec3 color;
in vec2 texCoord;
in vec3 crntPos;
in vec3 Normal;

//out vec4 FragColor;

uniform sampler2D tex0; // ??

uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

void main()
{
//   FragColor = vec4(color, 1.0f);
   //PHONG
   float ambient = 0.2f;
   float specularLight = 0.5f;

   //diffuse
   vec3 normal = normalize(Normal);
   vec3 lightDirection = normalize(lightPos - crntPos);
   float diffuse = max(dot(normal, lightDirection), 0.0f);

   //specular
   vec3 viewDirection = normalize(camPos - crntPos);
   vec3 reflectionDirection = reflect(-lightDirection, normal);
   float specular = pow(max(dot(viewDirection, reflectionDirection),0.0f),8) * specularLight;



   gl_FragColor = texture(tex0, texCoord) * lightColor * (diffuse + ambient + specular);
}