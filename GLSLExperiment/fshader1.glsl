#version 150

out vec4  fColor;
uniform vec4 newColor;

void
main()
{
    fColor = newColor;
}
