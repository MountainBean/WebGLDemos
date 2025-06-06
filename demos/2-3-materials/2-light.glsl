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
    vec3 lightColour;
    vec3 lightPos;
    vec3 viewPos;
};

layout(binding = 2) uniform fs_material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
} material;

layout(binding = 3) uniform fs_light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} light;

void main() {
    // ambient
    vec3 ambient =  light.ambient * material.ambient;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
@end

@vs light_cube_vs
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

@fs light_cube_fs
out vec4 FragColor;

layout(binding = 1) uniform light_cube_fs_params {
    vec3 lightColour;
};

void main() {
    FragColor = vec4(lightColour, 1.0);
}
@end

@program phong vs fs
@program light_cube light_cube_vs light_cube_fs
