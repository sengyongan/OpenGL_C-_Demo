#version 330 core
out vec4 FragColor;

in vec2 v_TexCoord;

uniform sampler2D screenTexture;
uniform int screenWidth_mid;
uniform int screenHeight_mid;

const float offset = 1.0 / 300.0;  

void main()
{
 vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // ����
        vec2( 0.0f,    offset), // ����
        vec2( offset,  offset), // ����
        vec2(-offset,  0.0f),   // ��
        vec2( 0.0f,    0.0f),   // ��
        vec2( offset,  0.0f),   // ��
        vec2(-offset, -offset), // ����
        vec2( 0.0f,   -offset), // ����
        vec2( offset, -offset)  // ����
    );

    float kernel[9] = float[](
        1, 1, 1,
        1, -8, 1,
        1, 1, 1
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, v_TexCoord.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    FragColor = texture(screenTexture, v_TexCoord);

    //if (gl_FragCoord.x < (screenWidth_mid / 2)) {
    //    if (gl_FragCoord.y < (screenHeight_mid / 2)) {
    //        FragColor = texture(screenTexture, v_TexCoord) * vec4(1.0, 0.0, 0.0, 1.0); // ��ɫ
    //    } else {
    //        FragColor = texture(screenTexture, v_TexCoord) * vec4(0.0, 0.0, 1.0, 1.0); // ��ɫ
    //    }
    //} else {
    //    if (gl_FragCoord.y < (screenHeight_mid / 2)) {
    //        FragColor = texture(screenTexture, v_TexCoord) * vec4(0.0, 1.0, 0.0, 1.0); // ��ɫ
    //    } else {
    //        FragColor = texture(screenTexture, v_TexCoord) * vec4(1.0, 1.0, 0.0, 1.0); // ��ɫ
    //    }
    //}
}