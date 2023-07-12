// #version 330 core

#ifdef VERTEX_SHADER
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in ivec2 translation; // New attribute for per-instance translation
layout(location = 3) in vec2 uvOffset;     // New attribute for per-instance UV offsets

out VS_OUT {
  vec2 TexCoords;
} vs_out;

// Reference: https://learnopengl.com/Advanced-OpenGL/Advanced-GLSL
layout(std140) uniform Matrices {
                   // Base alignment // Aligned offset (Starting byte, always multiple of 16)
  mat4 projection; // 16             // 0   (col1)
                   // 16             // 16  (col2)
                   // 16             // 32  (col3)
                   // 16             // 48  (col4)
                   //
  mat4 view;       // 16             // 64  (col1)
                   // 16             // 80  (col2)
                   // 16             // 96  (col3)
                   // 16             // 128 (col4)
};

void main() {
  gl_Position = projection * view * model * (vec4(position, 1.0) + vec4(translation, 0, 0));
  vs_out.TexCoords = aTexCoord + uvOffset - vec2(0.001, 0.001); // Need to subtract by a little less to avoid texture bleed
}
#endif

#ifdef FRAGMENT_SHADER
in VS_OUT {
  vec2 TexCoords;
} fs_in;

out vec4 fragColor;

uniform sampler2D texture1;
uniform vec3 textColor;

void main() {
  fragColor = vec4(1.0, 1.0, 1.0, texture(texture1, fs_in.TexCoords).r);
}
#endif
