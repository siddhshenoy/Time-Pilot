# Time Pilot v0.1a
#### by Siddharth Satish Shenoy (shenoys@tcd.ie)

## About the project

This game is a 3D remake of the retro-game named Time Pilot.

## What the game does at this point of time

Currently, the game is in its early stages and will only load a fighter plane in the scene. The game has support for font rendering and has minimal game logic implemented in it.

The game has support for ImGui which will help you change the parameters of the loaded objects, lights and text.

## Architecture Support

The only supported architecture by this game is x64. Hence, the game will not build on x86 architecture (this is due to the missing assimp library for x86 architecture).

## Libraries / Dependencies

1. AssImp - Model Loading (https://github.com/assimp/assimp)
2. GLEW - OpenGL function wrapper library (https://glew.sourceforge.net/)
3. GLFW - Windowing library (https://www.glfw.org/)
4. FreeType - Font loading library (http://freetype.org/)
5. GLM - OpenGL maths library (https://github.com/g-truc/glm)
6. And ofcourse, the opengl libraries provided by Windows and Linux

## Controls

1. Use arrow keys to rotate the plane.
2. Press U key to detach the camera from the plane, press it again to re-attach (**Note**: this will put you in free camera mode, you can then use arrow keys to move around).
3. Press M key to toggle the plane translation.
4. Press C in free camera mode to toggle to cursor mode to use the ImGui functionality.
5. Hold Y , G to translate the plane forward & backward respectively at a higher speed.
6. Press Left CTRL to shoot bullets.

## Models used

The models in game are extracted from another game (except for the bullet model which I designed myself in Blender). The game is named 'GTA: San Andreas'. The fighter plane is "Hydra" and the asteroid rocks are rocks from their game. I do not own any of the models above and all the copyrights belong to Rockstar Games.

The format of all the models are FBX.