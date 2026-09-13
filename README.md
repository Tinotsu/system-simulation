# Project for me to learn OpenGL with The Cherno tutorials

## Pipeline

1. We store some data (vertex buffer) in the GPU
2. We execute shaders (=program for GPU) to display those data on the screen

## Vertex Buffer

- Vertices are not position, they are just point with attribute (attribute can contains position thought).
- Vertex Buffer (VBO) : GPU buffer containing vertex attribute data.
- Attribute : information of the vertex like position, colors etc.
- Strides : size of Vertex in bytes.
- Offset/Pointer (in OpenGL) : pointer in the attribute, offset inside GPU buffer storage.

## Shaders

- Program which runs on GPU
- Vertex Shaders : Tell openGL where the vertex will be on the screen -> Provide vertex position.
- Fragment Shaders : Program that is called for each pixels on the screen, process called rasterization stage.

## Index Buffer

Abstraction for reusing vertex data

## Vertex Array (VAO)

OpenGL original way to bind vertex buffer with special specification.

## Blending

- Blending determines how we combine out output color with is already in our target buffer.
- Output = the color we output from our fragment shader (know as source).

### Three ways to control blending

- Enable and disable :
    `glEnabe(GL_BLEND)` - `glDisable(GL_BLEND)`
- Modify the factor of the source and the destination :
    `glBlendFunc(src, dest)`
    src = how the src RGBA factor is computed (default is GL_ONE)
    dest = how the dest RGBA factor is computed (default is GL_ZERO)
- Adding the source and the destination :
    `glBlendEquation(mode)`
    mode = how we combine the src and the dest colors (default is GL_FUNC_ADD)

## Projection Matrix

Projection = how to go from arbitrary in a 3D world to map them on a 2D window (screen pc).
A normalized space is a space between -1 and 1 for x and y.
A projection matrix is the factor that transform our coordinates into a normalized space.

### Two type of projection

- Orthographic, usually for 2D, everything on the Z axis is the same size
- Perspective, usually 3D, things bwhatehind are smaller than things above

## Model View of Projection Matrix (MVP matrix)

This is the transformation pipeline that goes through every vertex.

The model and the view matrix are different from the projection matrix :

- The View matrix (or eye matrix) is the view of the camera, what is kind of a simulation of the camera.
Transform of the camera (position, scale, rotation)
- The Model matrix is a way to simulate the model and its matrix id just a transformation of the model.
Transform of the object (position, scale, rotation)

They are just mathematical numbers.
Transformation TRS = translation (position), rotation, scale.
