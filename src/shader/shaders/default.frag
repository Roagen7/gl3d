#version 330 core
in vec3 color;
in vec2 texCoord;
in vec3 crntPos;
in vec3 Normal;

//out vec4 FragColor;

uniform sampler2D tex0;
uniform sampler2D tex1;

uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;


vec4 spotLight(){
   //                cos(angle)
   float outerCone = 0.90f;
   float innerCone = 0.95f;


   float ambient = 0.1f;
   float specularLight = 0.5f;

   //diffuse
   vec3 normal = normalize(Normal);
   vec3 lightDirection = normalize(lightPos - crntPos);
   float diffuse = max(dot(normal, lightDirection), 0.0f);

   //specular
   vec3 viewDirection = normalize(camPos - crntPos);
   vec3 reflectionDirection = reflect(-lightDirection, normal);
   float specular = pow(max(dot(viewDirection, reflectionDirection),0.0f),16) * specularLight;

   float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
   float i = clamp((angle - outerCone) / (innerCone - outerCone),0.0f, 1.0f);

   return  texture(tex0, texCoord) * lightColor * (diffuse * i  + ambient)  +  i * specular * texture(tex1, texCoord).r ;

}


vec4 direcLight(){
   float ambient = 0.1f;
   float specularLight = 0.5f;

   //diffuse
   vec3 normal = normalize(Normal);
   vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));
   float diffuse = max(dot(normal, lightDirection), 0.0f);

   //specular
   vec3 viewDirection = normalize(camPos - crntPos);
   vec3 reflectionDirection = reflect(-lightDirection, normal);
   float specular = pow(max(dot(viewDirection, reflectionDirection),0.0f),16) * specularLight;


   return  texture(tex0, texCoord) * lightColor * (diffuse  + ambient)  +  specular * texture(tex1, texCoord).r ;


}


vec4 pointLight(){

   //intensity calculation

   vec3 d = lightPos - crntPos;
   float dist = length(d);
   float a = 1.0f;
   float b = 0.04f;
   float i = 1.0f /  (a * dist * dist + b * dist + 1.0f);
//   float i = 1.0f;
   //PHONG
   float ambient = 0.1f;
   float specularLight = 0.5f;

   //diffuse
   vec3 normal = normalize(Normal);
   vec3 lightDirection = normalize(d);
   float diffuse = max(dot(normal, lightDirection), 0.0f);

   //specular
   vec3 viewDirection = normalize(camPos - crntPos);
   vec3 reflectionDirection = reflect(-lightDirection, normal);
   float specular = pow(max(dot(viewDirection, reflectionDirection),0.0f),16) * specularLight;


   return  texture(tex0, texCoord) * lightColor * (diffuse * i + ambient)  + i * specular * texture(tex1, texCoord).r ;
}


void main()
{




   gl_FragColor = spotLight();
}