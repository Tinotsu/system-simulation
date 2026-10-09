#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

uniform mat4 u_MVP;
uniform float radius;

out vec2 localPosition;

void main()
{
    localPosition = position.xy;

    vec4 scaledPosition = position;
    scaledPosition.xy *= 2.0 * radius;

    gl_Position = u_MVP * scaledPosition;
}

#shader fragment
#version 330 core

in vec2 localPosition;

uniform vec4 color;

out vec4 fragColor;

void main()
{
    if (length(localPosition) > 0.5)
        discard;

    fragColor = color;
}
