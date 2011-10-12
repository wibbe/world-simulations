
uniform sampler2D texture;
uniform vec4 uv_offset_rock;
uniform vec4 uv_offset_sand;
uniform vec2 repeat;
uniform vec3 sun_dir;

varying vec3 normal;

void main()
{
	vec2 uv = gl_TexCoord[0].xy * repeat;
	vec2 uv_stone = uv_offset_rock.xy + ((uv - floor(uv)) * uv_offset_rock.zw);
	vec2 uv_sand = uv_offset_sand.xy + ((uv - floor(uv)) * uv_offset_sand.zw);

	vec4 stone_color = texture2D(texture, uv_stone);
	vec4 sand_color = texture2D(texture, uv_sand);
	vec4 water_color = mix(vec4(1, 1, 1, 1), vec4(0, 0, 1, 1), min(gl_TexCoord[0].w / 5.0, 1.0));
	
	gl_FragColor = (mix(stone_color, sand_color, gl_TexCoord[0].z) * (0.3 + max(dot(normalize(normal), sun_dir), 0.0) * 0.7)) * water_color;
}
