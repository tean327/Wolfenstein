In this project I'm trying to recreate the famous game Wolfenstein using C and OpenGL.
For now I have a playable character with collision onto the grid, the rays and the 3D working for both Linux and Windows system.
The next step is to create a crosshair in the middle of the screen. Then I will do either the shooting mechanic or make an enemy appear.

To compile(windows):  gcc main.c glad.c LinkedList.c -L"./Librairies/lib" -lglfw3 -lopengl32 -lgdi32 -o raycaster
Then execute with ./raycaster

Linux: gcc main.c glad.c LinkedList.c -I"./Librairies/include" -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lm -o raycaster
then ./raycaster

Disclaimer in order to compile you will need the libraries install on your system
