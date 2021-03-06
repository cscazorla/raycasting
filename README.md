# Raycasting
Implementing a basic Raycasting renderer in C similiar to the [Wolfenstein 3D](https://github.com/id-Software/wolf3d) technique. 

![Demo](./assets/demo.gif)

# Caveat
Assumptions in my implementation:

* Walls are perpendicular with the floor
* Floor is flat
* Walls are made of cubes with the same size

As I'm using angles to represent orientation, I require expensive functions like sine, cosine and tangent. Therefore, this is not the fastest raycasting implementation. If you pursue performance, you shoud look into the famous [Lodev article](lodev.org/cgtutor/raycasting.html), that uses vectors (x,y) to represent orientation.

# Instructions
Use the key arrows to move around the map. Press `m` for the minimap.

# Compilation
This project is built in C99 and it compiles perfectly with GCC. The project uses [SDL](https://www.libsdl.org/) to deal with pixels, keyboard, etc.

# Textures
The textures and sprites that I'm using in this project belong to ID Software. I just recreated them for educational purposes. To read these PNG files I'm using [uPNG](https://github.com/elanthis/upng).

# To Do
* Animate sprites
* Profiling
* The logic that handles if a sprite is visible or not shouln't run on the drawSpriteProjection(). This should happen in the movePlayer(), as the PoV gets updated there. drawSpriteProjection() should render only