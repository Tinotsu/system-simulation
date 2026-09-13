#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

uniform mat4 u_MVP;
out vec2 localPosition;

void main()
{
    localPosition = (position.xy);
    gl_Position = u_MVP * position;
}

#shader fragment
#version 330 core

out vec4 fragColor;

in vec2 localPosition;

uniform ivec2 iResolution;

void main()
{
    vec2 uv = vec2(gl_FragCoord.xy)/vec2(iResolution) * 2.0 - 1.0;
    float aspect = float(iResolution.x) / float(iResolution.y);
    uv.x *= aspect;
    
    fragColor.rg = uv;
    fragColor.b = 0.5;
    
    float distance = step(length(localPosition), 0.5);
    fragColor = vec4(vec3(distance), 1.0);
    
    if (any(greaterThan(uv, vec2(1.0))) || any(lessThan(uv,vec2(-1, -1))))
    {
        fragColor.rgb = vec3(0.0);
    }
}
