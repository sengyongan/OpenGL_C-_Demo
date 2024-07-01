#version 330 core

layout(location = 0) out vec4 FragColor;
//in
in vec3 v_Position;
in vec2 v_TexCoord;
in vec3 v_Normal;


//����
struct Material {
    //��ͼ
    sampler2D texture1;
    sampler2D texture2;
    //specular��ͼ
    sampler2D specular_Texture;
    samplerCube depthMap;
    //�����
    float shininess;
}; 
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
    bool blinn;//Blinn-Phong
    bool gamma;
    //
    vec3 lightPos;
    bool shadows;
    float far_plane;
};
//
uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[10];
uniform SpotLight spotLight;
uniform ConstVal constVal;

// function prototypes�����ڴ˴���main������
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);

float closestDepth;
//Shadow
float ShadowCalculation(vec3 fragPos,vec3 viewPos)
{

   // �⵽Ƭ�εķ���
    vec3 fragToLight = fragPos - constVal.lightPos;
    // �������������ͼ���������ϵ�  ���ֵ   
    closestDepth = texture(material.depthMap, fragToLight).r;
    //�任0��far_plane�ķ�Χ
    closestDepth *= constVal.far_plane;
    // ��ȡƬ�ε���ȣ����ȣ�
    float currentDepth = length(fragToLight);
    //�����Ƿ�����Ӱ
    float bias = 0.05; 
    float viewDistance = length(viewPos - fragPos);
    float diskRadius = (1.0 + (viewDistance / constVal.far_plane)) / 25.0;
    //float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;
    float shadow = 0.0;
    float samples = 20;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(material.depthMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
        closestDepth *= constVal.far_plane;   // Undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }

    shadow /= float(samples);


    return shadow;
}


////2Linear
float near = 0.1; 
float far  = 1000.0; 

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));    
}


//
void main()
{

    vec3 viewPos = normalize(constVal.camera_Position - v_Position);//����
    vec3 norm = normalize(v_Normal);//Ƭ��modle��ķ���

    vec3 result1 = CalcDirLight(dirLight, norm, viewPos);

    vec3 result2;
    for(int i = 0; i < 10; i++){
        result2 += CalcPointLight(pointLights[i], norm, v_Position, viewPos);
    }
    vec3 result3 = CalcSpotLight(spotLight, norm, v_Position, viewPos);  

    //Shadow
    //vec3 lightDir = normalize(constVal.lightPos - v_Position);
    //float bias = max(0.02 * (1.0 - dot(norm, lightDir)), 0.005);

    //float bias = 0.005;
    float shadow = constVal.shadows ? ShadowCalculation(v_Position,viewPos) : 0.0;     
    vec3 result = (result1 + (1.0 - shadow) * (result2 + result3));
    
    
    //FragColor = vec4(vec3(closestDepth / constVal.far_plane), 1.0);
    FragColor = vec4(result, 1.0) * vec4(0.8,0.9,1.0,1.0);//����Ƭ����ɫ * ƫ��ɫ��
      
}

//
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{


    vec3 lightDir = normalize(-light.direction);//ƽ�й�����   
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);//������
    // specular shading
    float spec = 0.0;
    if(constVal.blinn)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
    }

    //����Ӱ��
    vec3 ambient = light.ambient * texture(material.texture1, v_TexCoord).rgb;
    vec3 diffuse = light.diffuse * diff * texture(material.texture1, v_TexCoord).rgb;
    vec3 specular = light.specular * spec * vec3(texture(material.specular_Texture, v_TexCoord));

    return (ambient + diffuse + specular);
    }
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);//��ǰ������� 
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    float spec = 0.0;
    if(constVal.blinn)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
    }

    // attenuation˥��
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (constVal.constant + constVal.linear * distance + constVal.quadratic * (distance * distance));   
    //float attenuation = 1.0 / (constVal.gamma ? distance * distance : distance);
    //����Ӱ��
    vec3 ambient = light.ambient * texture(material.texture1, v_TexCoord).rgb;
    vec3 diffuse = light.color * light.diffuse * diff * texture(material.texture1, v_TexCoord).rgb;
    vec3 specular = light.color * light.specular * spec * vec3(texture(material.specular_Texture, v_TexCoord));
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
    float spec = 0.0;
    if(constVal.blinn)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
    }

    // attenuation
    float distance = length(constVal.camera_Position - fragPos);
    float attenuation = 1.0 / (constVal.constant + constVal.linear * distance + constVal.quadratic * (distance * distance));    
    //float attenuation = 1.0 / (constVal.gamma ? distance * distance : distance);
    // spotlight intensity
    float theta = dot(lightDir, normalize(-constVal.camera_Direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    //����Ӱ��
    vec3 ambient = light.ambient * texture(material.texture1, v_TexCoord).rgb;
    vec3 diffuse = light.diffuse * diff * texture(material.texture1, v_TexCoord).rgb;
    vec3 specular = light.specular * spec * vec3(texture(material.specular_Texture, v_TexCoord));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}
