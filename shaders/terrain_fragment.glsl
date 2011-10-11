
uniform sampler2D texture;
uniform vec4 uv_offset;
uniform vec2 repeat;

void main()
{
	vec2 uv = gl_TexCoord[0].xy * repeat;
	uv = uv_offset.xy + ((uv - floor(uv)) * uv_offset.zw);
	
	gl_FragColor = texture2D(texture, uv);
}