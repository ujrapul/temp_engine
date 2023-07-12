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
  gl_Position = projection * model * vec4(vertex.xy, 10.0, 1.0);
  texCoords = vertex.zw;
}
#endif

#ifdef FRAGMENT_SHADER
precision mediump float;

in vec2 texCoords;
out vec4 color;

uniform sampler2D text;

uniform bool u_useOutline = false;
uniform vec4 u_color = vec4(1.0);
uniform vec4 u_outline_color = vec4(1.0f, 0.77f, 0.0f, 1.0f);
uniform float u_thickness = 0.5;
uniform float u_softness = 0.1;
uniform float u_outline_thickness = 0.5;
uniform float u_outline_softness = 0.1;

void main() {
  // color = vec4(1.0, 1.0, 1.0, texture(text, texCoords).r);
  // color = vec4(1.0, 1.0, 1.0, 1.0);
  // color = applyOutlineGlow(text, texCoords);

  float alpha = texture(text, texCoords).r;

  float outline = 0;
  if(u_useOutline) {
    outline = 1.0 - smoothstep(u_outline_thickness - u_outline_softness, u_outline_thickness + u_outline_softness, alpha);
  }
  alpha = smoothstep(1.0 - u_thickness - u_softness, 1.0 - u_thickness + u_softness, alpha);
  // Discarding transparent fragments
  if(alpha <= 0 || alpha > 1.0) {
    discard;
  }
  color = vec4(mix(applyOutlineGlow(text, texCoords).rgb, u_outline_color.rgb, outline), alpha);
}
#endif
