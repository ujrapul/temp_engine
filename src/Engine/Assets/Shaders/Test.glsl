// #version 330 core

#ifdef VERTEX_SHADER
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;
  
uniform mat4 view;
uniform mat4 projection;

void main()
{
  gl_Position = projection * view * model * vec4(aPos, 1.0);
  TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
#endif

#ifdef FRAGMENT_SHADER
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
  // linearly interpolate between both textures (80% container, 20% awesomeface)
  //FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
  vec4 sampled = vec4(1.0, 1.0, 1.0, texture(texture2, TexCoord).r);
  vec4 color = vec4(1.0, 1.0, 1.0, 1.0) * sampled;
  FragColor = mix(texture(texture1, TexCoord), color, 0.5);
}
#endif