# Raycasting
Implementing a basic Raycasting renderer in C similiar to the [Wolfenstein 3D](https://github.com/id-Software/wolf3d) technique (angles and trigonometry). As I'm using expensive functions like sine, cosine and tangent, this is not the fastest raycasting implementation.

Assumptions in my implementation:

* Walls are perpendicular with the floor
* Floor is flat
* Walls are made of cubes with the same size

I'm using [SDL](https://www.libsdl.org/) to deal with pixels, keyboard, etc.