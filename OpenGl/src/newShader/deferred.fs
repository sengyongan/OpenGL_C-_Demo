#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

//�����
uniform float shininess;

//ƽ�й�
struct DirLight {
    vec3 direction;//λ��

	//ƽ�й��3��Ч����ǿ��Ӱ��
    vec3 ambient;//�����ⷴ��
    vec3 diffuse;//������
    vec3 specular;//���淴��
};
//���
struct PointLight {
    vec3 position;
    vec3 color;
	
    //����3��Ч����ǿ��Ӱ��
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float Radius;
};
//�۹�
struct SpotLight {
    //�۹ⷶΧ
    float cutOff;
    float outerCutOff;

    //�۹��3��Ч����ǿ��Ӱ��
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};
struct ConstVal{
    //camera
    vec3 camera_Position;
    vec3 camera_Direction;
    //��Ե��Attenuation
    float constant;
    float linear;
    float quadratic;
};
//
uniform DirLight dirLight;
uniform PointLight pointLights[10];
uniform SpotLight spotLight;
uniform ConstVal constVal;


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

vec3 FragPos ;
vec3 Normal ;
vec3 Diffuse ;
float Specular;

void main()
{             
    // retrieve data from gbuffer
     FragPos = texture(gPosition, TexCoords).rgb;
     Normal = texture(gNormal, TexCoords).rgb;
     Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
     Specular = texture(gAlbedoSpec, TexCoords).a;
    
    //
    vec3 viewPos = normalize(constVal.camera_Position - FragPos);
    //
    vec3 result = CalcDirLight(dirLight, Normal, viewPos);

    for(int i = 0; i < 10; i++){

        float distance = length(pointLights[i].position - FragPos);
        if(distance < pointLights[i].Radius){
            result += CalcPointLight(pointLights[i], Normal, FragPos, viewPos);
        }
    }

    result += CalcSpotLight(spotLight, Normal, FragPos, viewPos);   
    
    FragColor = vec4(result, 1.0);//����Ƭ����ɫ
}

//
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);//ƽ�й�����   
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);//������
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    //����Ӱ��
    vec3 ambient = light.ambient * Diffuse;
    vec3 diffuse = light.diffuse * diff * Diffuse;
    vec3 specular = light.specular * spec * Specular;

    return (ambient + diffuse + specular);
    }
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);//��ǰ������� 
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // attenuation˥��
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (constVal.constant + constVal.linear * distance + constVal.quadratic * (distance * distance));    
    //����Ӱ��
    vec3 ambient = light.ambient * Diffuse;
    vec3 diffuse = light.color * light.diffuse * diff * Diffuse;
    vec3 specular = light.color * light.specular * spec * Specular;
    ambient *= attenuation;
    diffuse *= attenuation ;
    specular *= attenuation ;
    return (ambient + diffuse + specular);
}
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(constVal.camera_Position - fragPos);//���������
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // attenuation
    float distance = length(constVal.camera_Position - fragPos);
    float attenuation = 1.0 / (constVal.constant + constVal.linear * distance + constVal.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-constVal.camera_Direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    //����Ӱ��
    vec3 ambient = light.ambient * Diffuse;
    vec3 diffuse = light.diffuse * diff * Diffuse;
    vec3 specular = light.specular * spec * Specular;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}
