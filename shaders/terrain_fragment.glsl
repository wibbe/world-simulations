
uniform sampler2D texture;
uniform vec4 uv_offset;
uniform vec2 repeat;
uniform vec3 sun_dir;

varying vec3 normal;

void main()
{
	vec2 uv = gl_TexCoord[0].xy * repeat;
	uv = uv_offset.xy + ((uv - floor(uv)) * uv_offset.zw);
	
	gl_FragColor = texture2D(texture, uv) * (0.3 + max(dot(normalize(normal), sun_dir), 0.0) * 0.7);
}
