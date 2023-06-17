// #version 330 core

#ifdef VERTEX_SHADER
layout(location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 projection;

void main() {
  gl_Position = projection * vec4(vertex.xy, 10.0, 1.0);
  TexCoords = vertex.zw;
}
#endif

#ifdef FRAGMENT_SHADER
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;

void main() {
  color = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
  // color = vec4(1.0, 1.0, 1.0, 1.0);

}
#endif