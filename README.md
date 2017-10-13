####### Thomas Back Email: Tjb295@nau.edu

####Usage####
Raycasting Program, takes in a csv with objects and their properties and renders them onto a view plan via raycasting methods" 
Run the makefile in your/home/ and Tjbparser should be built in your /bin directory as an executable

The arguments required are as follows, the first argument must be the width of the view plane in mm, while the second argument is the height in mm, representing a film strip.
The third argument must be a csv file correctly formatted to input spheres, camera, and plane objects with properties and colors respective to each object
  -Camera has only a width and height,
  -Plane has a color, position, and normal vectors
  -Sphere has a color, position, and radius vectors 
The fourth argument must be the name of the ppm file you want to write to for the raycasting.
Known Issues
When the width and height of the camera are very small and the z values of the objects are close to zero they sometimes do not appear or they appear very zoomed up. Could not p
in point where in my logic this went wrong but for the most part casts everything.
