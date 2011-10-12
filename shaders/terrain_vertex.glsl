
varying vec3 normal;

void main()
{
	gl_Position = ftransform();
	gl_TexCoord[0] = gl_MultiTexCoord0;
	normal = /*gl_NormalMatrix * */normalize(gl_Normal);
}
