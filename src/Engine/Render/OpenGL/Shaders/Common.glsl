#ifdef GL_ES
precision mediump float;
#endif

#ifdef VERTEX_SHADER
#endif

#ifdef FRAGMENT_SHADER

const float offset = 1.0 / 384.0;
const int width = 3;

vec4 applyOutlineGlow(sampler2D text, vec2 texCoords) {
  // vec4 col = texture2D(text, texCoords);
  // if(col.r > 0.5) {
  //   return vec4(1.0, 1.0, 1.0, col.r);
  // }
  // float a = 0;
  // for(int i = 0; i < width; ++i) {
  //   float currOffset = offset * i;
  //   a +=
  //     texture2D(text, vec2(texCoords.x, texCoords.y)).r +
  //     texture2D(text, vec2(texCoords.x + currOffset, texCoords.y)).r +
  //     texture2D(text, vec2(texCoords.x - currOffset, texCoords.y)).r +
  //     texture2D(text, vec2(texCoords.x, texCoords.y + currOffset)).r +
  //     texture2D(text, vec2(texCoords.x, texCoords.y - currOffset)).r +
  //     texture2D(text, vec2(texCoords.x + currOffset, texCoords.y + currOffset)).r +
  //     texture2D(text, vec2(texCoords.x + currOffset, texCoords.y - currOffset)).r +
  //     texture2D(text, vec2(texCoords.x - currOffset, texCoords.y + currOffset)).r +
  //     texture2D(text, vec2(texCoords.x - currOffset, texCoords.y - currOffset)).r;
  // }
  // if(a > 0.0) {
  //   return vec4(0.0, 0.0, 0.0, 1.0);
  // }
  // return vec4(col.r);

  // return vec4( 1.0 - sobel.rgb, texture(text, texCoords).r );
  return vec4(1.0, 1.0, 1.0, texture(text, texCoords).r);
}

#endif