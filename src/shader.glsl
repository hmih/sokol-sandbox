@vs vs
in vec3 pos0;
in vec4 color0;

out vec4 color1;

void main()
{
    gl_Position = vec4(pos0.x, pos0.y, pos0.z, 1.0);
    color1 = color0;
}
@end

@fs fs
in vec4 color1;
out vec4 color2;

void main()
{
    color2 = color1;
}
@end

#pragma sokol @program simple vs fs
