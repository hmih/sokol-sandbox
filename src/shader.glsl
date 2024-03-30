@vs vs
in vec3 pos0;
in vec4 col0;

out vec4 col1;

void main()
{
    gl_Position = vec4(pos0.x, pos0.y, pos0.z, 1.0);
    col1 = col0;
}
@end

@fs fs
in vec4 col1;

out vec4 col2;

void main()
{
    col2 = col1;
}
@end

#pragma sokol @program simple vs fs
