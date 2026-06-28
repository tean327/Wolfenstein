In this project I'm trying to recreate the famous game Wolfenstein using C and OpenGL.
For now I have a playable character with collision onto the grid. 
The next step is to be able to make the player rotate correctly and then I will go "3D" and implement the raycasting.

To compile:  gcc main.c glad.c LinkedList.c -L"./Librairies/lib" -lglfw3 -lopengl32 -lgdi32 -o raycaster
Then execute with ./raycaster
