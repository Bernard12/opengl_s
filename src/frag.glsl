#version 330

out vec4 colour;
in vec4 bruh;
uniform float u_time;

float plot(vec2 coord, float plt) {
    return smoothstep(plt - 0.01f, plt, coord.y) -
           smoothstep(plt, plt + 0.01f, coord.y);
}

vec3 w = vec3(0.589,0.665,1.000);
vec3 b = vec3(0.910,0.412,0.228);

void main() {
    vec2 coord = gl_FragCoord.xy / 800;
    vec3 ptc = vec3(coord.y);
    ptc.r = smoothstep(0.0, 1.0, coord.x);
    ptc.g = sin(u_time / 10.0) * 0.5 + 0.3;
    ptc.b = pow(sin(u_time / 5) * 0.5 + 0.6, 1.951);
    vec3 col = mix(b, w, ptc);
    colour = vec4(col, 1.0);
}
