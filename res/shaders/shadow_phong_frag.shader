#version 330 core
out vec4 FragColor;

in VS_OUT{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform samplerCube depthMap;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float far_plane;
uniform bool shadows;


// array of offset direction for sampling
vec3 randomVecs[20] = vec3[]
(
    vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
    vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
    vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
    vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1),
    vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1)
    );

float ShadowCalculation(vec3 fragPos)
{
    // Basic hard shadow code
    vec3 fragToLight = fragPos - lightPos;
    float currentDepth = length(fragToLight);
    float sampled_depth = texture(depthMap, fragToLight).r * far_plane;
    
    // if without bias, there would be acne depending on
    // the angle the view
   /* float bias = 0.02;
    float shadow = currentDepth - bias > sampled_depth ? 1.0 : 0.0;*/

    // PCF
    /*float bias = 0.1;
    float sample_size = 2.0;
    float offset = 0.05;
    float shadow = 0.0;
    vec3 curDir;
    for (float x = -offset; x < offset; x += (offset / sample_size * 2.0)) {
        for (float y = -offset; y < offset; y += (offset / sample_size * 2.0)) {
            for (float z = -offset; z < offset; z += (offset / sample_size * 2.0)) {
                curDir = fragToLight + vec3(x, y, z);
                currentDepth = length(curDir);
                sampled_depth = texture(depthMap, curDir).r * far_plane;
                shadow += currentDepth - bias > sampled_depth ? 1.0 : 0.0;
            }
        }
    }
    shadow /= (sample_size * sample_size * sample_size);*/
   

    //Disk, we can use this trick since pcf sometimes
    // is very expensive
    
    // we take roughly evenly separated vectors like what we have
    // at the top of the file

    float shadow = 0.0;

    // another trick is that we can make shadow further away from us softer
    // and close to us sharper through adding the view distance as another factor
    float viewDis = length(fragPos - viewPos);
    viewDis = viewDis > far_plane ? far_plane : viewDis;
    float radius = (1.0 + (viewDis / far_plane)) / 60.0;
    float bias = 0.2;
    float sample_size = 20.0;
    vec3 curDir;
    for (int i = 0; i < 20; ++i) {
        curDir = fragToLight + randomVecs[i] * radius;
        currentDepth = length(curDir);
        sampled_depth = texture(depthMap, curDir).r * far_plane;
        shadow += currentDepth - bias > sampled_depth ? 1.0 : 0.0;
    }
    shadow /= sample_size;
     /*display closestDepth as debug (to visualize depth cubemap)
     FragColor = vec4(vec3(closestDepth / far_plane), 1.0);   */ 

    return shadow;
}

void main()
{
    vec3 color = vec3(0.3, 0.1, 0.8);
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(1.0);
    // ambient
    vec3 ambient = 0.3 * color;
    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;
    // calculate shadow
    float shadow = ShadowCalculation(fs_in.FragPos);
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

    FragColor = vec4(lighting, 1.0);
   //FragColor = vec4(vec3(shadow), 1.0);
}