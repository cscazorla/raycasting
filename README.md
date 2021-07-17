# Raycasting
Implementing a basic Raycasting renderer in C similiar to the [Wolfenstein 3D](https://github.com/id-Software/wolf3d) technique (angles and trigonometry). As I'm using expensive functions like sine, cosine and tangent, this is not the fastest raycasting implementation.

![Demo](./assets/demo.gif)

# Caveat
Assumptions in my implementation:

* Walls are perpendicular with the floor
* Floor is flat
* Walls are made of cubes with the same size

# Compilation

This project is built in C99 and it compiles perfectly with GCC. The project uses [SDL](https://www.libsdl.org/) to deal with pixels, keyboard, etc.