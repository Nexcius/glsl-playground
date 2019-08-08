uniform float u_time;

uniform float u_mouse_x;
uniform float u_mouse_y;

void main()
{
	// gl_FragColor = vec4(abs(sin(u_time)),1.0,0.0,1.0);
	// vec2 st = gl_FragCoord.xy/300.0;///u_resolution;
	gl_FragColor = vec4(cos(u_mouse_x / 640.0),cos(u_mouse_y / 640.0),0.0,1.0);
}