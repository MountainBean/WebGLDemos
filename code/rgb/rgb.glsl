@ctype mat4 glm::mat4
@ctype vec3 glm::vec3

@vs vs
in vec3 aPos;
in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

layout(binding = 0) uniform vs_params {
    mat4 model;
    mat4 view;
    mat4 projection;
};

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
}
@end

@fs fs
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

layout(binding = 1) uniform fs_params {
    vec3 viewPos;
};

layout(binding = 2) uniform fs_material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
} material;

layout(binding = 3) uniform fs_light_red {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} light_red;
layout(binding = 4) uniform fs_light_green {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} light_green;
layout(binding = 5) uniform fs_light_blue {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} light_blue;


void main()
{
    // ambient
    vec3 ambient =  vec3(0.4) * material.ambient;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir_red = normalize(light_red.position - FragPos);
    float diff_red = max(dot(norm, lightDir_red), 0.0);
    vec3 diffuse_red = light_red.diffuse * (diff_red * material.diffuse);
    vec3 lightDir_green = normalize(light_green.position - FragPos);
    float diff_green = max(dot(norm, lightDir_green), 0.0);
    vec3 diffuse_green = light_green.diffuse * (diff_green * material.diffuse);
    vec3 lightDir_blue = normalize(light_blue.position - FragPos);
    float diff_blue = max(dot(norm, lightDir_blue), 0.0);
    vec3 diffuse_blue = light_blue.diffuse * (diff_blue * material.diffuse);
    vec3 diffuse = diffuse_red + diffuse_green + diffuse_blue;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir_red = reflect(-lightDir_red, norm);
    float spec_red = pow(max(dot(viewDir, reflectDir_red), 0.0), material.shininess);
    vec3 specular_red = light_red.specular * (spec_red * material.specular);
    vec3 reflectDir_green = reflect(-lightDir_green, norm);
    float spec_green = pow(max(dot(viewDir, reflectDir_green), 0.0), material.shininess);
    vec3 specular_green = light_green.specular * (spec_green * material.specular);
    vec3 reflectDir_blue = reflect(-lightDir_blue, norm);
    float spec_blue = pow(max(dot(viewDir, reflectDir_blue), 0.0), material.shininess);
    vec3 specular_blue = light_blue.specular * (spec_blue * material.specular);
    vec3 specular = specular_red + specular_green + specular_blue;

    vec3 result = (ambient + diffuse + specular) * vec3(0.8);
    FragColor = vec4(result, 1.0);
}
@end

@vs light_sphere_vs
in vec3 aPos;

layout(binding = 0) uniform vs_params {
    mat4 model;
    mat4 view;
    mat4 projection;
};

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
@end

@fs light_sphere_fs
out vec4 FragColor;

layout(binding = 1) uniform light_sphere_fs_params {
    vec3 lightColour;
};

void main() {
    FragColor = vec4(lightColour, 1.0);
}
@end

@program rgb vs fs
@program light_sphere light_sphere_vs light_sphere_fs
