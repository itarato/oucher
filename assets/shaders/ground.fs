#version 330

in vec2 fragTexCoord; 

out vec4 finalColor;

void main()
{
    vec4 color = vec4(0.8, fragTexCoord.y / 500.0, fragTexCoord.x / 500.0, 1.0);
    finalColor = vec4(color.rgb, 1.0);
}
