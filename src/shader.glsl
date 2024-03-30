@vs vs
in vec3 pos0;

void main()
{
    gl_Position = vec4(pos0.x, pos0.y, pos0.z, 1.0);
}
@end

@fs fs
out vec4 color2;

void main()
{
    color2 = vec4(0.0, 1.0, 0.0, 1.0);
}
@end

#pragma sokol @program simple vs fs
