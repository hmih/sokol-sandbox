@vs vs
struct vin
{
    float4 color [[user(locn0)]];
    float4 gl_Position [[position]];
};

struct vout
{
    float4 position [[attribute(0)]];
    float4 color [[attribute(1)]];
};

vertex vout main(vin in [[stage_in]])
{
    vout out = {};
    out.gl_Position = in.position;
    out.color = in.color;
    return out;
}
@end

@fs fs
struct vin
{
    float4 frag_color [[color(0)]];
};

struct vout
{
    float4 color [[user(locn0)]];
};

fragment vout main(vin in [[stage_in]])
{
    vout out = {};
    out.frag_color = in.color;
    return out;
}
@end

#pragma sokol @program texcube vs fs
