
# 轻量级WebGL库lightgl样例 - cocoonyang的专栏 - CSDN博客


2018年06月10日 17:41:16[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：369



# Computer Graphics
All modern displays are raster-based.  A raster is a 2D matrix of pixels. In computer graphics, graphics or bitmap image is a dot matrix data structure that represents a generally rectangular grid of pixels (points of color), viewable via a monitor, paper, or other display medium[1]. A pixel has two properties: a color and a position. Color is expressed in RGB (Red-Green-Blue) components. The position is expressed in terms of (x, y) coordinates. The origin (0, 0) is located at the top-left corner, with x-axis pointing right and y-axis pointing down[2].
The color values of the pixels are stored in a special part of graphics memory called frame buffer. The GPU writes the color value into the frame buffer. The display reads the color values from the frame buffer row-by-row, from left-to-right, top-to-bottom, and puts each of the values onto the screen. This is known as raster-scan. The display refreshes its screen several dozen times per second, typically 60Hz for LCD monitors and higher for CRT tubes. This is known as the refresh rate[2].
# Graphics Rendering Pipeline
Rendering is the process of producing image on the display from model description.The 3D Graphics Rendering Pipeline accepts data of 3D model in terms of vertices of primitives (such as  point, line, triangle and quad), and produces the color-value for the pixels for the display. It consists of the following stages:
Vertex Processing: Process and transform individual vertices.
Rasterization: Convert primitives (connected vertices, e.g. line, triangle) into a set of fragments. A fragment can be treated as a pixel in 3D spaces, which is aligned with the pixel grid, with attributes such as position, color, normal and texture.
Fragment Processing: Process individual fragments.
Output Merging: Combine the fragments of all primitives (in 3D space) into 2D color-pixel for the display.
In modern GPUs, the vertex processing stage and fragment processing stage are programmable. You can write programs, known as vertex shader and fragment shader to perform your custom transform for vertices and fragments. The shader programs are written in C-like high level languages such as GLSL (OpenGL Shading Language), HLSL (High-Level Shading Language for Microsoft Direct3D), or Cg (C for Graphics by NVIDIA)[2].
On the other hand, the rasterization and output merging stages are not programmable, but configurable - via configuration commands issued to the GPU[2].
# OpenGL
Open Graphics Library (OpenGL) is the premier environment for developing portable, interactive 2D and 3D graphics applications[3]. It provides a cross-language, cross-platform application programming interface (API) for rendering 2D and 3D vector graphics. The API is typically used to interact with a graphics processing unit (GPU), to achieve hardware-accelerated rendering[4].
# WebGL
WebGL (Web Graphics Library) is a JavaScript API for rendering interactive 2D and 3D graphics, and it is completely controlled by the web browser. WebGL programs consist of control code written in JavaScript and shader code that is written in OpenGL Shading Language (GLSL).
The following is a demo of WebGL application:
```python
<!doctype html>
```
```python
<
```
```python
html
```
```python
>
```
```python
<
```
```python
body
```
```python
>
```
```python
<
```
```python
canvas
```
```python
width
```
```python
= "
```
```python
300
```
```python
"
```
```python
height
```
```python
= "
```
```python
300
```
```python
"
```
```python
id
```
```python
= "
```
```python
my_Canvas
```
```python
">
```
```python
</
```
```python
canvas
```
```python
>
```
```python
<
```
```python
script
```
```python
>
```
```python
/*============= Creating a canvas ==================*/
```
```python
var
```
```python
canvas = document.getElementById(
```
```python
'my_Canvas'
```
```python
);
         gl = canvas.getContext(
```
```python
'experimental-webgl'
```
```python
);
```
```python
/*========== Defining and storing the geometry ==========*/
```
```python
var
```
```python
vertices = [
            -
```
```python
0.5
```
```python
,
```
```python
0.5
```
```python
,
```
```python
0.0
```
```python
,
            -
```
```python
0.5
```
```python
,-
```
```python
0.5
```
```python
,
```
```python
0.0
```
```python
,
```
```python
0.5
```
```python
,-
```
```python
0.5
```
```python
,
```
```python
0.0
```
```python
,
```
```python
0.5
```
```python
,
```
```python
0.5
```
```python
,
```
```python
0.0
```
```python
];
```
```python
var
```
```python
colors = [
```
```python
0
```
```python
,
```
```python
0
```
```python
,
```
```python
1
```
```python
,
```
```python
1
```
```python
,
```
```python
0
```
```python
,
```
```python
0
```
```python
,
```
```python
0
```
```python
,
```
```python
1
```
```python
,
```
```python
0
```
```python
,
```
```python
1
```
```python
,
```
```python
0
```
```python
,
```
```python
1
```
```python
,];
         indices = [
```
```python
3
```
```python
,
```
```python
2
```
```python
,
```
```python
1
```
```python
,
```
```python
3
```
```python
,
```
```python
1
```
```python
,
```
```python
0
```
```python
];
```
```python
// Create an empty buffer object and store vertex data
```
```python
var
```
```python
vertex_buffer = gl.createBuffer();
         gl.bindBuffer(gl.ARRAY_BUFFER, vertex_buffer);
         gl.bufferData(gl.ARRAY_BUFFER,
```
```python
new
```
```python
Float32Array
```
```python
(vertices), gl.STATIC_DRAW);
         gl.bindBuffer(gl.ARRAY_BUFFER,
```
```python
null
```
```python
);
```
```python
// Create an empty buffer object and store Index data
```
```python
var
```
```python
Index_Buffer = gl.createBuffer();
         gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, Index_Buffer);
         gl.bufferData(gl.ELEMENT_ARRAY_BUFFER,
```
```python
new
```
```python
Uint16Array
```
```python
(indices), gl.STATIC_DRAW);
         gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER,
```
```python
null
```
```python
);
```
```python
// Create an empty buffer object and store color data
```
```python
var
```
```python
color_buffer = gl.createBuffer ();
         gl.bindBuffer(gl.ARRAY_BUFFER, color_buffer);
         gl.bufferData(gl.ARRAY_BUFFER,
```
```python
new
```
```python
Float32Array
```
```python
(colors), gl.STATIC_DRAW);
```
```python
/*======================= Shaders =======================*/
```
```python
// vertex shader source code
```
```python
var
```
```python
vertCode =
```
```python
'attribute vec3 coordinates;'
```
```python
+
```
```python
'attribute vec3 color;'
```
```python
+
```
```python
'varying vec3 vColor;'
```
```python
+
```
```python
'void main(void) {'
```
```python
+
```
```python
' gl_Position = vec4(coordinates, 1.0);'
```
```python
+
```
```python
'vColor = color;'
```
```python
+
```
```python
'}'
```
```python
;
```
```python
// Create a vertex shader object
```
```python
var
```
```python
vertShader = gl.createShader(gl.VERTEX_SHADER);
```
```python
// Attach vertex shader source code
```
```python
gl.shaderSource(vertShader, vertCode);
```
```python
// Compile the vertex shader
```
```python
gl.compileShader(vertShader);
```
```python
// fragment shader source code
```
```python
var
```
```python
fragCode =
```
```python
'precision mediump float;'
```
```python
+
```
```python
'varying vec3 vColor;'
```
```python
+
```
```python
'void main(void) {'
```
```python
+
```
```python
'gl_FragColor = vec4(vColor, 1.);'
```
```python
+
```
```python
'}'
```
```python
;
```
```python
// Create fragment shader object
```
```python
var
```
```python
fragShader = gl.createShader(gl.FRAGMENT_SHADER);
```
```python
// Attach fragment shader source code
```
```python
gl.shaderSource(fragShader, fragCode);
```
```python
// Compile the fragmentt shader
```
```python
gl.compileShader(fragShader);
```
```python
// Create a shader program object to
```
```python
// store the combined shader program
```
```python
var
```
```python
shaderProgram = gl.createProgram();
```
```python
// Attach a vertex shader
```
```python
gl.attachShader(shaderProgram, vertShader);
```
```python
// Attach a fragment shader
```
```python
gl.attachShader(shaderProgram, fragShader);
```
```python
// Link both the programs
```
```python
gl.linkProgram(shaderProgram);
```
```python
// Use the combined shader program object
```
```python
gl.useProgram(shaderProgram);
```
```python
/* ======== Associating shaders to buffer objects =======*/
```
```python
// Bind vertex buffer object
```
```python
gl.bindBuffer(gl.ARRAY_BUFFER, vertex_buffer);
```
```python
// Bind index buffer object
```
```python
gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, Index_Buffer);
```
```python
// Get the attribute location
```
```python
var
```
```python
coord = gl.getAttribLocation(shaderProgram,
```
```python
"coordinates"
```
```python
);
```
```python
// point an attribute to the currently bound VBO
```
```python
gl.vertexAttribPointer(coord,
```
```python
3
```
```python
, gl.FLOAT,
```
```python
false
```
```python
,
```
```python
0
```
```python
,
```
```python
0
```
```python
);
```
```python
// Enable the attribute
```
```python
gl.enableVertexAttribArray(coord);
```
```python
// bind the color buffer
```
```python
gl.bindBuffer(gl.ARRAY_BUFFER, color_buffer);
```
```python
// get the attribute location
```
```python
var
```
```python
color = gl.getAttribLocation(shaderProgram,
```
```python
"color"
```
```python
);
```
```python
// point attribute to the volor buffer object
```
```python
gl.vertexAttribPointer(color,
```
```python
3
```
```python
, gl.FLOAT,
```
```python
false
```
```python
,
```
```python
0
```
```python
,
```
```python
0
```
```python
) ;
```
```python
// enable the color attribute
```
```python
gl.enableVertexAttribArray(color);
```
```python
/*============Drawing the Quad====================*/
```
```python
// Clear the canvas
```
```python
gl.clearColor(
```
```python
0.5
```
```python
,
```
```python
0.5
```
```python
,
```
```python
0.5
```
```python
,
```
```python
0.9
```
```python
);
```
```python
// Enable the depth test
```
```python
gl.enable(gl.DEPTH_TEST);
```
```python
// Clear the color buffer bit
```
```python
gl.clear(gl.COLOR_BUFFER_BIT);
```
```python
// Set the view port
```
```python
gl.viewport(
```
```python
0
```
```python
,
```
```python
0
```
```python
,canvas.width,canvas.height);
```
```python
//Draw the triangle
```
```python
gl.drawElements(gl.TRIANGLES, indices.length, gl.UNSIGNED_SHORT,
```
```python
0
```
```python
);
```
```python
</
```
```python
script
```
```python
>
```
```python
</
```
```python
body
```
```python
>
```
```python
</
```
```python
html
```
```python
>
```
![这里写图片描述](https://img-blog.csdn.net/2018061509393887?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NvY29vbnlhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# LightGL
Lightgl.js provides a quickly prototype WebGL applications. Here is a simple example of lightgl.js.
.
```python
<!DOCTYPE html>
```
```python
<
```
```python
html
```
```python
>
```
```python
<
```
```python
body
```
```python
>
```
```python
<
```
```python
script
```
```python
src
```
```python
=
```
```python
"./lightgl.js"
```
```python
>
```
```python
</
```
```python
script
```
```python
>
```
```python
<
```
```python
script
```
```python
>
```
```python
var
```
```python
data = {
       vertices: [[
```
```python
0
```
```python
,
```
```python
0
```
```python
,
```
```python
0
```
```python
], [
```
```python
1
```
```python
,
```
```python
0
```
```python
,
```
```python
0
```
```python
], [
```
```python
0
```
```python
,
```
```python
1
```
```python
,
```
```python
0
```
```python
]],
       triangles: [[
```
```python
0
```
```python
,
```
```python
1
```
```python
,
```
```python
2
```
```python
]]
     };
```
```python
var
```
```python
gl = GL.create();
```
```python
var
```
```python
angleX =
```
```python
30
```
```python
;
```
```python
var
```
```python
angleY =
```
```python
45
```
```python
;
```
```python
// var mesh = GL.Mesh.cube({ normals: true });
```
```python
var
```
```python
mesh = GL.Mesh.load( data );
```
```python
var
```
```python
offset =
```
```python
new
```
```python
GL.Vector();
```
```python
var
```
```python
shader =
```
```python
new
```
```python
GL.Shader(
```
```python
'\
  varying vec3 normal;\
  void main() {\
    normal = gl_Normal;\
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\
  }\
'
```
```python
,
```
```python
'\
  varying vec3 normal;\
  void main() {\
    gl_FragColor = vec4(normal * 0.5 + 0.5, 1.0);\
  }\
'
```
```python
);
```
```python
var
```
```python
result;
```
```python
var
```
```python
originalOffset;
gl.onmousedown =
```
```python
function
```
```python
(e)
```
```python
{
```
```python
var
```
```python
tracer =
```
```python
new
```
```python
GL.Raytracer();
```
```python
var
```
```python
ray = tracer.getRayForPixel(e.x, e.y);
  result = GL.Raytracer.hitTestBox(tracer.eye, ray, offset.subtract(
```
```python
1
```
```python
), offset.add(
```
```python
1
```
```python
));
  originalOffset = offset;
};
gl.onmousemove =
```
```python
function
```
```python
(e)
```
```python
{
```
```python
if
```
```python
(e.dragging) {
```
```python
if
```
```python
(result) {
```
```python
var
```
```python
tracer =
```
```python
new
```
```python
GL.Raytracer();
```
```python
var
```
```python
ray = tracer.getRayForPixel(e.x, e.y);
```
```python
var
```
```python
t = result.hit.subtract(tracer.eye).dot(result.normal) / ray.dot(result.normal);
```
```python
var
```
```python
hit = tracer.eye.add(ray.multiply(t));
      offset = originalOffset.add(hit.subtract(result.hit));
    }
```
```python
else
```
```python
{
      angleY += e.deltaX;
      angleX =
```
```python
Math
```
```python
.max(-
```
```python
90
```
```python
,
```
```python
Math
```
```python
.min(
```
```python
90
```
```python
, angleX + e.deltaY));
    }
    gl.ondraw();
  }
};
gl.ondraw =
```
```python
function
```
```python
()
```
```python
{
```
```python
gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
  gl.loadIdentity();
  gl.translate(
```
```python
0
```
```python
,
```
```python
0
```
```python
, -
```
```python
10
```
```python
);
  gl.rotate(angleX,
```
```python
1
```
```python
,
```
```python
0
```
```python
,
```
```python
0
```
```python
);
  gl.rotate(angleY,
```
```python
0
```
```python
,
```
```python
1
```
```python
,
```
```python
0
```
```python
);
```
```python
// Use push and pop to guard the transform so gl is left in world space when
```
```python
// gl.ondraw() ends. This way, when the GL.Raytracer reads the modelview and
```
```python
// projection matrices, the rays it generates will be in world space.
```
```python
gl.pushMatrix();
  gl.translate(offset.x, offset.y, offset.z);
  shader.draw(mesh);
  gl.popMatrix();
};
gl.fullscreen();
gl.enable(gl.DEPTH_TEST);
gl.ondraw();
```
```python
</
```
```python
script
```
```python
>
```
```python
</
```
```python
body
```
```python
>
```
```python
</
```
```python
html
```
```python
>
```
The above code looks as following
![这里写图片描述](https://img-blog.csdn.net/20180610164637437?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NvY29vbnlhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Colored rectangular
```python
<!DOCTYPE html>
```
```python
<
```
```python
html
```
```python
>
```
```python
<
```
```python
body
```
```python
>
```
```python
<
```
```python
script
```
```python
src
```
```python
=
```
```python
"./lightgl.js"
```
```python
>
```
```python
</
```
```python
script
```
```python
>
```
```python
<
```
```python
script
```
```python
>
```
```python
var
```
```python
data = {
       vertices: [[
```
```python
0
```
```python
,
```
```python
0
```
```python
,
```
```python
0
```
```python
], [
```
```python
1
```
```python
,
```
```python
0
```
```python
,
```
```python
0
```
```python
], [
```
```python
0
```
```python
,
```
```python
1
```
```python
,
```
```python
0
```
```python
], [
```
```python
1
```
```python
,
```
```python
1
```
```python
,
```
```python
0
```
```python
]],
       triangles: [[
```
```python
0
```
```python
,
```
```python
1
```
```python
,
```
```python
2
```
```python
], [
```
```python
2
```
```python
,
```
```python
1
```
```python
,
```
```python
3
```
```python
]]
     };
```
```python
var
```
```python
gl = GL.create();
```
```python
var
```
```python
angleX =
```
```python
30
```
```python
;
```
```python
var
```
```python
angleY =
```
```python
45
```
```python
;
```
```python
// var mesh = GL.Mesh.cube({ normals: true });
```
```python
var
```
```python
mesh = GL.Mesh.load( data );
```
```python
var
```
```python
offset =
```
```python
new
```
```python
GL.Vector();
```
```python
var
```
```python
shader =
```
```python
new
```
```python
GL.Shader(
```
```python
'\
  void main() {\
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\
  }\
'
```
```python
,
```
```python
'\
  uniform vec4 color;\
  void main() {\
    gl_FragColor = color;\
  }\
'
```
```python
);
shader.uniforms({
  color: [
```
```python
0.2
```
```python
,
```
```python
0.3
```
```python
,
```
```python
1
```
```python
,
```
```python
1
```
```python
]
}).draw(mesh);
```
```python
var
```
```python
result;
```
```python
var
```
```python
originalOffset;
gl.onmousedown =
```
```python
function
```
```python
(e)
```
```python
{
```
```python
var
```
```python
tracer =
```
```python
new
```
```python
GL.Raytracer();
```
```python
var
```
```python
ray = tracer.getRayForPixel(e.x, e.y);
  result = GL.Raytracer.hitTestBox(tracer.eye, ray, offset.subtract(
```
```python
1
```
```python
), offset.add(
```
```python
1
```
```python
));
  originalOffset = offset;
};
gl.onmousemove =
```
```python
function
```
```python
(e)
```
```python
{
```
```python
if
```
```python
(e.dragging) {
```
```python
if
```
```python
(result) {
```
```python
var
```
```python
tracer =
```
```python
new
```
```python
GL.Raytracer();
```
```python
var
```
```python
ray = tracer.getRayForPixel(e.x, e.y);
```
```python
var
```
```python
t = result.hit.subtract(tracer.eye).dot(result.normal) / ray.dot(result.normal);
```
```python
var
```
```python
hit = tracer.eye.add(ray.multiply(t));
      offset = originalOffset.add(hit.subtract(result.hit));
    }
```
```python
else
```
```python
{
      angleY += e.deltaX;
      angleX =
```
```python
Math
```
```python
.max(-
```
```python
90
```
```python
,
```
```python
Math
```
```python
.min(
```
```python
90
```
```python
, angleX + e.deltaY));
    }
    gl.ondraw();
  }
};
gl.ondraw =
```
```python
function
```
```python
()
```
```python
{
```
```python
gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
  gl.loadIdentity();
  gl.translate(
```
```python
0
```
```python
,
```
```python
0
```
```python
, -
```
```python
10
```
```python
);
  gl.rotate(angleX,
```
```python
1
```
```python
,
```
```python
0
```
```python
,
```
```python
0
```
```python
);
  gl.rotate(angleY,
```
```python
0
```
```python
,
```
```python
1
```
```python
,
```
```python
0
```
```python
);
```
```python
// Use push and pop to guard the transform so gl is left in world space when
```
```python
// gl.ondraw() ends. This way, when the GL.Raytracer reads the modelview and
```
```python
// projection matrices, the rays it generates will be in world space.
```
```python
gl.pushMatrix();
  gl.translate(offset.x, offset.y, offset.z);
  shader.draw(mesh);
  gl.popMatrix();
};
gl.fullscreen();
gl.enable(gl.DEPTH_TEST);
gl.ondraw();
```
```python
</
```
```python
script
```
```python
>
```
```python
</
```
```python
body
```
```python
>
```
```python
</
```
```python
html
```
```python
>
```
![这里写图片描述](https://img-blog.csdn.net/20180610172933780?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NvY29vbnlhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[  ](https://img-blog.csdn.net/20180610172933780?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NvY29vbnlhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
> [1]
> [https://en.wikipedia.org/wiki/Raster_graphics](https://en.wikipedia.org/wiki/Raster_graphics)

> [2]
> [https://www.ntu.edu.sg/home/ehchua/programming/opengl/CG_BasicsTheory.html](https://www.ntu.edu.sg/home/ehchua/programming/opengl/CG_BasicsTheory.html)

> [3]
> [https://www.opengl.org/](https://www.opengl.org/)

> [4]
> [https://en.wikipedia.org/wiki/OpenGL](https://en.wikipedia.org/wiki/OpenGL)

> [5]
> [https://github.com/evanw/lightgl.js](https://github.com/evanw/lightgl.js)

> [6]
> [https://www.tutorialspoint.com/webgl/index.htm](https://www.tutorialspoint.com/webgl/index.htm)


