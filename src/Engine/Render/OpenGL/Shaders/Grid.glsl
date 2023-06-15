// #version 330 core

#ifdef VERTEX_SHADER
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in ivec2 translation; // New attribute for per-instance translation
layout (location = 3) in vec2 uvOffset;     // New attribute for per-instance UV offsets

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  gl_Position = projection * view * model * (vec4(position, 1.0) + vec4(translation, 0, 0));
  TexCoords = aTexCoord + uvOffset - vec2(0.001, 0.001); // Need to subtract by a little less to avoid texture bleed
}
#endif

#ifdef FRAGMENT_SHADER
in vec2 TexCoords;

out vec4 fragColor;

uniform sampler2D texture1;
uniform vec3 textColor;

void main()
{
  fragColor = vec4(1.0, 1.0, 1.0, texture(texture1, TexCoords).r);
}
#endif