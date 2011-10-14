
varying vec3 normal;

uniform vec3 sun_dir;
uniform sampler2D texture;
uniform vec4 uv_offset;
uniform vec2 repeat;

void main()
{
  vec2 uv = gl_TexCoord[0].xy * repeat;
  uv = uv_offset.xy + ((uv - floor(uv)) * uv_offset.zw);
  vec4 color = texture2D(texture, uv);
  gl_FragColor = color * max(dot(normalize(normal), sun_dir), 0.0);
}
