
varying vec3 normal;
uniform vec3 sun_dir;

void main()
{
  gl_FragColor = vec4(0.3, 1.0, 0.5, 1.0) * max(dot(normalize(normal), sun_dir), 0.0);
}