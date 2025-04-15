@ctype mat4 glm::mat4
@ctype vec3 glm::vec3
@ctype vec4 glm::vec4

@vs vs
in vec3 aPos;
in vec3 aNormal;
in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

layout(binding = 0) uniform vs_params {
    mat4 model;
    mat4 view;
    mat4 projection;
};

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal; 
    TexCoords = aTexCoords;
}
@end

@fs fs
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

layout(binding = 1) uniform fs_params {
    vec3 viewPos;
};

layout(binding = 2) uniform fs_material {
    float shininess;
} material;

layout(binding = 3) uniform fs_dir_light {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} dirLight;
struct dir_light_t {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

const int NR_POINT_LIGHTS = 4;
layout(binding = 4) uniform fs_point_lights {
    vec4 position[NR_POINT_LIGHTS];

    vec4 ambient[NR_POINT_LIGHTS];
    vec4 diffuse[NR_POINT_LIGHTS];
    vec4 specular[NR_POINT_LIGHTS];

    vec4 attenuation[NR_POINT_LIGHTS];
} pointLights;
struct point_light_t {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

layout(binding = 5) uniform fs_spot_light {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
} spotLight;
struct spot_light_t {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

dir_light_t get_dir_light();
point_light_t get_point_light(int i);
spot_light_t get_spot_light();

vec3 calcDirLight(dir_light_t light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(point_light_t light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 calcSpotLight(spot_light_t light, vec3 normal, vec3 fragPos, vec3 viewDir);

layout(binding = 0) uniform texture2D _diffuse_texture;
layout(binding = 0) uniform sampler diffuse_texture_smp;
#define diffuse_texture sampler2D(_diffuse_texture, diffuse_texture_smp)
layout(binding = 1) uniform texture2D _specular_texture;
layout(binding = 1) uniform sampler specular_texture_smp;
#define specular_texture sampler2D(_specular_texture, specular_texture_smp)



void main() {

    // Properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Directional lighting
    vec3 result = calcDirLight(get_dir_light(), norm, viewDir);

    // Point Lights
    for (int i = 0; i < NR_POINT_LIGHTS; i++) {
        result += calcPointLight(get_point_light(i), norm, FragPos, viewDir);
    }

    // Spot Light
    result += calcSpotLight(get_spot_light(), norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}

dir_light_t get_dir_light() {
    return dir_light_t(
        dirLight.direction,
        dirLight.ambient,
        dirLight.diffuse,
        dirLight.specular
    );
}

point_light_t get_point_light(int index) {
    int i = index;
    return point_light_t(
        pointLights.position[i].xyz,
        pointLights.ambient[i].rgb,
        pointLights.diffuse[i].rgb,
        pointLights.specular[i].rgb,
        pointLights.attenuation[i].x,
        pointLights.attenuation[i].y,
        pointLights.attenuation[i].z
    );
}

spot_light_t get_spot_light() {
    return spot_light_t(
        spotLight.position,
        spotLight.direction,
        spotLight.cutOff,
        spotLight.outerCutOff,
        spotLight.ambient,
        spotLight.diffuse,
        spotLight.specular,
        spotLight.constant,
        spotLight.linear,
        spotLight.quadratic
    );
}

vec3 calcDirLight(dir_light_t light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // combine results
    vec3 ambient = light.ambient * vec3(texture(diffuse_texture, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(diffuse_texture, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(specular_texture, TexCoords));
    return (ambient + diffuse + specular);

}

vec3 calcPointLight(point_light_t light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant
        + light.linear * distance
        + light.quadratic * (distance * distance));

    // combine results
    vec3 ambient = light.ambient * vec3(texture(diffuse_texture, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(diffuse_texture, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(specular_texture, TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);

}

vec3 calcSpotLight(spot_light_t light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(diffuse_texture, TexCoords).rgb;

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(specular_texture, TexCoords).rgb;

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return (diffuse + specular);

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
