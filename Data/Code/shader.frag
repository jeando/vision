
uniform sampler2D Texture;
void main(void)
{
//		gl_FragColor = texture2D(Texture,gl_TexCoord[0].xy);
		vec4 color = texture2D(Texture,gl_TexCoord[0].st);
		gl_FragColor = color;
//		gl_FragColor = texture2D(Texture,gl_FragCoord.xy);
//		gl_FragColor = vec4(1.,0.,0.,1.);
//		gl_FragColor = vec4(1.,0.,0.,1.);
		gl_FragColor.rg = gl_TexCoord[0].st;
}
