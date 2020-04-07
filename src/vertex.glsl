#version 330
layout (location = 0) in vec3 pos;

uniform mat4 transform;
out vec4 bruh;

void main() {
    gl_Position = transform * vec4(pos, 1.0f);
    bruh = vec4(clamp(pos, 0., 1.), 1.0f);
}
