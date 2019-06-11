# Computer Graphics Coding Assignments:
### All projects developed using Object Oriented Programming concepts.

## 1) Gear using Parameterized Object Primitives
### Introduction
This assignment was made by building parameterized object primitives. The Scene graph is a 3 dimensional model. Sig Library is used in this assignment. Different classes and functions of the library were used such as GsModel class.
In order to meet requirement 1 and 2 of the assignment, different functions were made for gears that are of different sizes. Most of the functions are receiving 3 parameters, i.e. the radius (r1, r2) and the thickness (d) of the gear. Translation function of the GsMat class has been used to specify the center point for each gear. The number of teeth of each gear has been decided according to the respective angles between the edges of the triangle of that gear. To meet requirement 3, I have added gears that are of different shapes so that the animated scene graph can form an interesting motion (option 1). Refer the result section of this document. All gears are placed on different Z-axis. For rotating the gears, getrans() and setrans() functions have been used.
In shape 1, ‘set()’ function is used to assign values to vertices by incrementing the angle. ‘Push()’ function has been called to create triangle’s between assigned vertices. In shape 2, triangles were placed linearly forming an array of triangles.

### Conclusion
It was concluded that the running time was more when all the gears were animated together as compared to when they were animated individually.


## 2) Utah Teapot using Beizer curves

### Introduction
This assignment based on Parametric Surfaces was very interesting and challenging. Firstly the control points were read from the given file. After loading the points, a polyhedron cage was made by using triangles and lines separately. ‘SnLines’ class was used to draw lines between defined coordinates. Next step was to make a teapot by using the control points and coefficients of the Bezier equations.
Normal vectors were successfully generated each vertex. Firstly, separate vectors were generated on each point in the U and V direction. After getting values of these 2 vectors, cross product was performed between them to get a new Vector for that specific point.
In order to meet requirement 2 of the assignment, Shadowing is done. The vector for light source is kept unchanged. A triangle is being projected onto the XY plane using the light vector.

### Machine Specification on which this assignment was made and code testing was done:
Processor: Intel i3 - 3217U CPU @ 1.80GHz (8GB RAM)
Graphic card details: Internal Intel® HD Graphics 4000

### Project Behavior on different machines:
Intel i3 : 1.384193msec, Intel i5 : 1.382199msec, Intel i7 : 1.374656msec

### Conclusion:
The resolution of the Teapot can be changed by adjusting the number of discretization. For
example in Figure 5, each patch of the teapot is being divided in 5 rows and 5 column i.e. there
will be 25 vertices in each patch according to which new Bezier coordinates will be generated.
As the number of rows and columns in each patch increase, the resolution of the teapot will
accordingly increase. In Figure 6, the teapot is very smooth and is of high resolution. Here, each
patch is being divided in 100 rows and 100 columns. Figure 7 is to show the normal vectors that
are being generated in the project.

## 3) Dimensional City Model using Transformations and Fractals

### Introduction:
A 3-Dimensional Architectural view of a City model has been created as a part of Computer Graphics
final project. Sig Library is used for implementation of the model. Different classes and functions of the
library have been used. For a good Landscape view, key concepts such as moving objects, Building of
different shapes and sizes, Sign boarding and trees were built.

### Main objects that are built in the City Model:
• Megaplex – large group of building
• A Self-driven Bicycle
• Sign boarding on Road
• Trees of different shapes and sizes

### How are these objects made?
Megaplex: To design all the above mentioned things, several functions from SIG Library have been used
such ‘set()’ function that is used to assign values to vertices, ‘Push()’ function has been called to create
triangle’s between assigned vertices. For designing an array of building, functions from SnPrimitve and
GsPrimitive have been used. After defining a particular shape and color for each building, add_model
function is used to add model to the scene graph. As a parameter of add_model function, Vector value
has been passed to define the coordinates of the object. Refer Figure 1.

A Self-driven Bicycle: (For making the bicycle to move, Transform button in the left-up corner has to
be clicked). The Bicycle that is made has been kept self-driven. The auto-driven bicycle is made up of a
collection of Cylinders and shapes from GsPrimitive class. For making the wheels, basic concept of
triangles is used. Firstly, the values are assigned to the vertices and then triangle is generated between
the assigned vertices. V Array of GsArray class is used to store the value of vertices. As a main concept of
this project, Transformation is used to rotate the wheels and translate the other parts of the bi-cycle
with respect to X-axis. 
Research paper on transformation was found and studied to implement concept
in a correct manner. The research paper described how a 4x4 matrix can be used to translate and rotate
the primitives. After going through the research paper, translation matrix was used to translate the cycle
on the road along the X axis. The wheels are being rotated and are also being translated. For making
wheels to rotate, ‘getrans’ and ‘setrans’ functions of the GsMat class are used.

### Why was function Over-Riding needed: 
The add_model() function have been overridden. This is
done because there are both static and dynamic models in the model. So, in the code there are 2
add_model() function and both have different parameters. The static objects such as building and sign
boarding are using one function. And the Second function is for objects that are transformed such as
different parts of bicycle which are using an additional parameter that is a pointer to SnManipulator
class.

### Interesting conclusion:
An interesting thing was seen while collecting the results and the final testing phase. The smoothness of
the translation matrix varied after implementing the concept of fractals and trees were made. It would
not be a problem if the trees were also dynamic and were moving along any axis. However, in this
project the trees are kept static. Additional of trees to the scene graph made the Bicycle to move in
slightly rigid manner. Although, the bicycle is moving in a smooth manner.
