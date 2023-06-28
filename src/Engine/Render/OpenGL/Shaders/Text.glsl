// #version 330 core

#ifdef VERTEX_SHADER
layout(location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 texCoords;

// Reference: https://learnopengl.com/Advanced-OpenGL/Advanced-GLSL
layout(std140) uniform FontMatrices {
                   // Base alignment // Aligned offset (Starting byte, always multiple of 16)
  mat4 projection; // 16             // 0   (col1)
                   // 16             // 16  (col2)
                   // 16             // 32  (col3)
                   // 16             // 48  (col4)
};

void main() {
  gl_Position = projection * vec4(vertex.xy, 10.0, 1.0);
  texCoords = vertex.zw;
}
#endif

#ifdef FRAGMENT_SHADER
in vec2 texCoords;
out vec4 color;

uniform sampler2D text;

void main() {
  // color = vec4(1.0, 1.0, 1.0, texture(text, texCoords).r);
  // color = vec4(1.0, 1.0, 1.0, 1.0);
  color = applyOutlineGlow(text, texCoords);

}
#endif