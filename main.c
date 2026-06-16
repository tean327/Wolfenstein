#include <stdio.h>
#include <math.h>
#include "Librairies\include\glad\glad.h"
#include "Librairies\include\GLFW\glfw3.h"
#include "LinkedList.h"
#define WIDTH 1200
#define HEIGHT 800
#define GRID_HEIGHT 8
#define GRID_WIDTH 10
#define NUMBER_OF_RAYS 360
#define FOV PI / 3

#define PLAYERSIZE 20
#define PI 3.141592653589793

int TileSizeX;
int TileSizeY;

float model[16] = {1, 0, 0, 0,
                   0, 1, 0, 0,
                   0, 0, 1, 0,
                   0, 0, 0, 1};

float view[16] = {1, 0, 0, 0,
                  0, 1, 0, 0,
                  0, 0, 1, 0,
                  0, 0, 0, 1};

const char *fragment = "#version 330 core\n"
                       "out vec4 FragColor;\n"
                       "in vec3 color;\n"
                       "void main()\n"
                       "{\n"
                       "FragColor = vec4(color, 1.0f);\n"
                       "}\n";

const char *vertex = "#version 330 core\n"
                     "layout (location=0) in vec3 aPos;\n"
                     "layout (location=1) in vec3 aColor;\n"
                     "out vec3 color;\n"
                     "uniform mat4 mvp;\n"
                     "void main()\n"
                     "{\n"
                     "gl_Position = mvp*mat4(1)*mat4(1) * vec4(aPos, 1.0);\n"
                     "color = aColor;\n"
                     "}\n";

GLFWwindow *window;
GLFWwindow *window3D;
int map[GRID_HEIGHT][GRID_WIDTH] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 1, 1},
    {1, 0, 0, 0, 1, 0, 1, 1, 0, 1},
    {1, 0, 0, 0, 1, 0, 1, 0, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 1, 0, 1, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

GLfloat gridVertices[3 * 6 * GRID_HEIGHT * GRID_WIDTH];
GLfloat gridColor[3 * 6 * GRID_HEIGHT * GRID_WIDTH];
GLuint Program;

GLfloat vertices[18];
GLfloat color[] = {
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f};

GLfloat RayVertices[6 * NUMBER_OF_RAYS];
GLfloat RayColor[6 * NUMBER_OF_RAYS];

GLfloat Wall3DVert[18 * NUMBER_OF_RAYS];
GLfloat Wall3DColor[18 * NUMBER_OF_RAYS];

Ray *rays[NUMBER_OF_RAYS];

float deltaTime = 0.0f,
      playerPosX = 620, playerPosY = 410, playerAngle;
float mouseSpeed = 0.005f;
int speed = 100; // pixels

unsigned int loc;

ListWall *head;
Vector2 *mousePos;
Vector2 *lastMousePos;
Vector2 *playerPos;
Vector2 *origin;

int Init();
void CreateGrid();
void CreatePlayer(unsigned int *VAO_Player, unsigned int *VBO_Player, unsigned int *VBO_Color_Player);
void Player();
float ConvertToOpenGLX(float pCoord);
float ConvertToOpenGLY(float pCoord);
int CheckCollision(float pPosX, float pPosY);
Vector2 *ReturnCollisionPos(float pPosX, float pPosY);
void RotatePlayer();
int IsMouseMoving();
void MultiplyMatrices(float result[], float a[], float b[]);
void MultiplyMatriceToVector(float result[], float a[], Vector2 *b);
void DrawRays(unsigned int *VAO_Player, unsigned int *VBO_Player, unsigned int *VBO_Color_Player);
float CalculRayDistance(float xA, float yA, float xB, float yB, float rayAngle);
void Create3DWalls(unsigned int *VAO_Walls, unsigned int *VBO_Walls, unsigned int *VBO_Color_Walls);
void CreateRayVAOsVBOs(unsigned int *VAO_Ray, unsigned int *VBO_RayVertices, unsigned int *VBO_Color);
void CreateWallsVAOsVBOs(unsigned int *VAO_Walls, unsigned int *VBO_Wallvertices, unsigned int *VBO_Color);
int KeyPressed(int key);

int main(int argc, char *argv[])
{
    mousePos = (Vector2 *)malloc(sizeof(Vector2));
    lastMousePos = (Vector2 *)malloc(sizeof(Vector2));
    playerPos = (Vector2 *)malloc(sizeof(Vector2));
    origin = (Vector2 *)malloc(sizeof(Vector2));
    origin->X = ConvertToOpenGLX(WIDTH / 2 - PLAYERSIZE / 2);
    origin->Y = ConvertToOpenGLY(HEIGHT / 2 - PLAYERSIZE / 2);
    if (Init() == 1)
        return 1;

    for (int i = 0; i < NUMBER_OF_RAYS * 6; i += 6)
    {
        RayColor[i] = 0.0f;
        RayColor[i + 1] = 1.0f;
        RayColor[i + 2] = 0.0f;
        RayColor[i + 3] = 0.0f;
        RayColor[i + 4] = 1.0f;
        RayColor[i + 5] = 0.0f;
    }
    // Tell opengl the area of our window so that we can make changes after
    glViewport(0, 0, WIDTH, HEIGHT);

    glClearColor(1.0f, 0.5f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    CreateGrid();

    glUseProgram(Program);
    unsigned int VBOVertices, VBOColor, VBO_Player, VBO_Color_Player, VBO_RayVertices, VBO_RayColor, VBO_3D_Vert, VBO_3D_Color, VAO_Grid, VAO_Player, VAO_Rays, VAO_Walls;
    glGenVertexArrays(1, &VAO_Grid);
    glBindVertexArray(VAO_Grid);

    glGenBuffers(1, &VBOVertices);
    glBindBuffer(GL_ARRAY_BUFFER, VBOVertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gridVertices), gridVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &VBOColor);
    glBindBuffer(GL_ARRAY_BUFFER, VBOColor);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gridColor), gridColor, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(1);

    int count = sizeof(gridVertices) / sizeof(gridVertices[0]);

    glBindVertexArray(0);

    glGenVertexArrays(1, &VAO_Player);
    glBindVertexArray(VAO_Player);

    CreatePlayer(&VAO_Player, &VBO_Player, &VBO_Color_Player);

    glBindVertexArray(0);

    glGenVertexArrays(1, &VAO_Rays);
    CreateRayVAOsVBOs(&VAO_Rays, &VBO_RayVertices, &VBO_RayColor);

    glBindVertexArray(0);

    glfwMakeContextCurrent(window3D);
    glGenVertexArrays(1, &VAO_Walls);
    CreateWallsVAOsVBOs(&VAO_Walls, &VBO_3D_Vert, &VBO_3D_Color);

    float crntTime, lastFrame;

    glClearColor(1.0f, 0.5f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(Program);

    glBindVertexArray(VAO_Grid);

    glBindVertexArray(VAO_Player);
    glfwSwapBuffers(window);

    float timer;
    loc = glGetUniformLocation(Program, "mvp");

    while (!glfwWindowShouldClose(window) && !glfwWindowShouldClose(window3D))
    {
        glfwMakeContextCurrent(window);

        for (int i = 0; i < 16; i++)
            model[i] = view[i];
        glUniformMatrix4fv(loc, 1, GL_FALSE, model);
        crntTime = glfwGetTime();
        deltaTime = crntTime - lastFrame;
        lastFrame = crntTime;

        timer += deltaTime;
        glClearColor(1.0f, 0.5f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(Program);

        glBindVertexArray(VAO_Grid);
        glDrawArrays(GL_TRIANGLES, 0, count);

        DrawRays(&VAO_Rays, &VBO_RayVertices, &VBO_RayColor);

        glBindVertexArray(VAO_Rays);
        glDrawArrays(GL_LINES, 0, NUMBER_OF_RAYS * 2);
        glBindVertexArray(0);

        Player();

        glBindVertexArray(VAO_Player);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindVertexArray(0);

        glfwSwapBuffers(window);

        glfwMakeContextCurrent(window3D);
        glClearColor(1.0f, 0.5f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(Program);
        Create3DWalls(&VAO_Walls, &VBO_3D_Vert, &VBO_3D_Color);
        glBindVertexArray(VAO_Walls);
        glDrawArrays(GL_TRIANGLES, 0, NUMBER_OF_RAYS * 6);
        glfwSwapBuffers(window3D);

        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwDestroyWindow(window3D);
    glfwTerminate();

    free(origin);
    free(mousePos);
    free(lastMousePos);
    free(playerPos);
    FreeListWallMemory(head);
    return 0;
}

int Init()
{
    // Initialize glfw
    glfwInit();
    // Give hint to glfw which version of opengl we are using (here 3.3)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Tell glfw we are using the core profile which means we only will have the modern functions
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create the window (parameters are size x then y, name of the window, if you want to put it in full screen and not important)
    window = glfwCreateWindow(WIDTH, HEIGHT, "NutellaLeBoss", NULL, NULL);
    window3D = glfwCreateWindow(WIDTH, HEIGHT, "3DNutella", NULL, NULL);

    // Just to make sure that if the window does not render the program does not break
    if (window == NULL || window3D == NULL)
    {
        glfwTerminate();
        return 1;
    }
    // This is to tell opengl that we want to use this window if we don't tell him it will be bugs with the following lines of code
    glfwMakeContextCurrent(window);
    // Load glad and all of its function/method so it configures Opengl
    gladLoadGL();

    // Tell opengl the area of our window so that we can make changes after
    glViewport(0, 0, WIDTH, HEIGHT);

    glClearColor(1.0f, 0.5f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment, NULL);
    glCompileShader(fragmentShader);

    Program = glCreateProgram();
    glAttachShader(Program, vertexShader);
    glAttachShader(Program, fragmentShader);
    glLinkProgram(Program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return 0;
}

void CreateGrid()
{
    int index = 0;
    TileSizeX = WIDTH / GRID_WIDTH;
    // int yFactor
    TileSizeY = HEIGHT / GRID_HEIGHT;
    head = (ListWall *)malloc(sizeof(ListWall));
    head->next = NULL;
    head->VALUE = NULL;
    for (int x = 0; x < WIDTH; x += TileSizeX)
    {
        for (int y = 0; y < HEIGHT; y += TileSizeY)
        {
            float xS[6] = {x, x, x + TileSizeX - 5, x, x + TileSizeX - 5, x + TileSizeX - 5};
            float yS[6] = {y, y + TileSizeY - 5, y + TileSizeY - 5, y, y, y + TileSizeY - 5};
            switch (map[(y / TileSizeY)][(x / TileSizeX)])
            {
            case 0:
                // Create floor
                printf("Floor at x: %i y: %i\n", x, y);

                for (int i = 0; i < 6; i++)
                {
                    gridColor[index] = 0.0f;
                    gridVertices[index++] = ConvertToOpenGLX(xS[i]);
                    gridColor[index] = 0.0f;
                    gridVertices[index++] = ConvertToOpenGLY(yS[i]);
                    gridColor[index] = 0.0f;
                    gridVertices[index] = 0.0f;
                    if (index + 1 <= 3 * 6 * GRID_HEIGHT * GRID_WIDTH)
                        index++;
                    else
                        break;
                }
                break;

            case 1:
                // Create Wall
                printf("Wall at x: %i y: %i\n", x, y);
                Vector2 *lPos = (Vector2 *)malloc(sizeof(Vector2));
                lPos->X = x;
                lPos->Y = y;

                Vector2 *lSize = (Vector2 *)malloc(sizeof(Vector2));
                lSize->X = x + TileSizeX;
                lSize->Y = y + TileSizeY;

                Wall *lWall = (Wall *)malloc(sizeof(Wall));
                lWall->position = lPos;
                lWall->size = lSize;

                lWall->position->X = x;
                lWall->position->Y = y;
                lWall->size->X = x + TileSizeX;
                lWall->size->Y = y + TileSizeY;
                AddAtEndOfListWall(lWall, head);
                for (int i = 0; i < 6; i++)
                {
                    gridColor[index] = 1.0f;
                    gridVertices[index++] = ConvertToOpenGLX(xS[i]);
                    gridColor[index] = 1.0f;
                    gridVertices[index++] = ConvertToOpenGLY(yS[i]);
                    gridColor[index] = 1.0f;
                    gridVertices[index] = 0.0f;
                    if (index + 1 <= 3 * 6 * 8 * 10)
                        index++;
                    else
                        break;
                }
                break;
            }
        }
    }
    printf("Grid Create\n");
}

float ConvertToOpenGLX(float pCoord)
{
    return (pCoord - WIDTH / 2) / (WIDTH / 2);
}

float ConvertToOpenGLY(float pCoord)
{
    return (pCoord - HEIGHT / 2) / (HEIGHT / 2);
}

void CreatePlayer(unsigned int *VAO_Player, unsigned int *VBO_Player, unsigned int *VBO_Color_Player)
{
    float xS[6] = {playerPosX, playerPosX, playerPosX + PLAYERSIZE, playerPosX, playerPosX + PLAYERSIZE, playerPosX + PLAYERSIZE};
    float yS[6] = {playerPosY, playerPosY + PLAYERSIZE, playerPosY + PLAYERSIZE, playerPosY, playerPosY, playerPosY + PLAYERSIZE};
    int index = 0;
    for (int i = 0; i < 6; i++)
    {
        vertices[index++] = ConvertToOpenGLX(xS[i]);
        vertices[index++] = ConvertToOpenGLY(yS[i]);
        vertices[index] = 0.0f;
        if (index + 1 <= 18)
            index++;
    }

    glGenBuffers(1, VBO_Player);
    glBindBuffer(GL_ARRAY_BUFFER, *VBO_Player);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, VBO_Color_Player);
    glBindBuffer(GL_ARRAY_BUFFER, *VBO_Color_Player);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    glUniformMatrix4fv(loc, 1, GL_FALSE, model);
}

void Player()
{
    if (KeyPressed(GLFW_KEY_W) && CheckCollision(playerPosX + PLAYERSIZE + cos(playerAngle) * deltaTime * speed, playerPosY + sin(playerAngle) * deltaTime * speed) == 0)
    {
        playerPosX += cos(playerAngle) * deltaTime * speed;
        playerPosY -= sin(playerAngle) * deltaTime * speed;
    }
    if (KeyPressed(GLFW_KEY_S) && CheckCollision(playerPosX - cos(playerAngle) * deltaTime * speed, playerPosY - sin(playerAngle) * deltaTime * speed) == 0)
    {
        playerPosX -= cos(playerAngle) * deltaTime * speed;
        playerPosY += sin(playerAngle) * deltaTime * speed;
    }
    if (KeyPressed(GLFW_KEY_D) && CheckCollision(playerPosX + sin(playerAngle) * deltaTime * speed, playerPosY + PLAYERSIZE - cos(playerAngle) * deltaTime * speed) == 0)
    {
        playerPosX -= sin(playerAngle) * deltaTime * speed;
        playerPosY -= cos(playerAngle) * deltaTime * speed;
    }
    if (KeyPressed(GLFW_KEY_A) && CheckCollision(playerPosX - sin(playerAngle) * deltaTime * speed, playerPosY + cos(playerAngle) * deltaTime * speed) == 0)
    {
        playerPosX += sin(playerAngle) * deltaTime * speed;
        playerPosY += cos(playerAngle) * deltaTime * speed;
    }

    // Reset if the player is locked somewhere
    if (KeyPressed(GLFW_KEY_F))
    {
        playerPosX = WIDTH / 2;
        playerPosY = HEIGHT / 2;
        playerAngle = 0;
    }
    playerPos->X = ConvertToOpenGLX(playerPosX);
    playerPos->Y = ConvertToOpenGLY(playerPosY);

    MultiplyMatriceToVector(model, model, playerPos);

    RotatePlayer();

    glUniformMatrix4fv(loc, 1, GL_FALSE, model);
}

int CheckCollision(float pPosX, float pPosY)
{
    ListWall *searchNode = (ListWall *)malloc(sizeof(ListWall));
    searchNode = head->next;
    while (searchNode->next != NULL)
    {
        if ((*searchNode).VALUE->position->X <= pPosX && (*searchNode).VALUE->size->X >= pPosX && (*searchNode).VALUE->position->Y <= pPosY && (*searchNode).VALUE->size->Y >= pPosY)
        {
            return 1;
        }
        searchNode = searchNode->next;
    }
    return 0;
}

Vector2 *ReturnCollisionPos(float pPosX, float pPosY)
{
    ListWall *searchNode = (ListWall *)malloc(sizeof(ListWall));
    searchNode = head->next;
    while (searchNode->next != NULL)
    {
        if ((*searchNode).VALUE->position->X <= pPosX && (*searchNode).VALUE->size->X >= pPosX && (*searchNode).VALUE->position->Y <= pPosY && (*searchNode).VALUE->size->Y >= pPosY)
        {
            Wall *lWall = searchNode->VALUE;
            Vector2 *lVector = (Vector2 *)malloc(sizeof(Vector2));
            float distLeft = fabs(pPosX - lWall->position->X);
            float distRight = fabs(lWall->size->X - pPosX);
            float distTop = fabs(pPosY - lWall->position->Y);
            float distBottom = fabs(lWall->size->Y - pPosY);

            float minDist = distLeft;
            lVector->X = lWall->position->X;
            lVector->Y = pPosY;
            if (distTop < minDist)
            {
                lVector->X = pPosX;
                lVector->Y = lWall->position->Y;
            }
            if (distBottom < minDist)
            {
                lVector->X = pPosX;
                lVector->Y = lWall->size->Y;
            }
            if (distRight < minDist)
            {
                lVector->X = lWall->size->X;
                lVector->Y = pPosY;
            }
            return lVector;
        }
        searchNode = searchNode->next;
    }
    return NULL;
}

void RotatePlayer()
{
    if (KeyPressed(GLFW_KEY_LEFT))
        playerAngle += deltaTime;
    if (KeyPressed(GLFW_KEY_RIGHT))
        playerAngle -= deltaTime;

    lastMousePos->X = mousePos->X;
    lastMousePos->Y = mousePos->Y;
    glfwGetCursorPos(window, (double *)&mousePos->X, (double *)&mousePos->Y);
    if (IsMouseMoving() == 1)
        mouseSpeed = 0.0f;
    else
        mouseSpeed = 0.01f;
    playerAngle += mouseSpeed * deltaTime * (float)(WIDTH / 2 - ((*mousePos).X));

    if (playerAngle > 2 * PI)
        playerAngle = 0;
    if (playerAngle < 0)
        playerAngle = 2 * PI;

    float rotationMatrice[16] = {
        cos(playerAngle), -sin(playerAngle), 0, 0,
        sin(playerAngle), cos(playerAngle), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1};

    MultiplyMatriceToVector(model, model, origin);
    MultiplyMatrices(model, model, rotationMatrice);
    float toPlayer[16];
    MultiplyMatriceToVector(toPlayer, toPlayer, playerPos);
    MultiplyMatrices(model, model, toPlayer);
}

int IsMouseMoving()
{
    if (lastMousePos->X != mousePos->X)
        return 0;
    return 1;
}

void DrawRays(unsigned int *VAO_Ray, unsigned int *VBO_RayVertices, unsigned int *VBO_Color)
{
    float startAngle = playerAngle - FOV / 2;
    float finishAngle = playerAngle + FOV / 2;
    float diffAngle = (FOV) / NUMBER_OF_RAYS;
    for (int i = 0, j = 0; i < NUMBER_OF_RAYS; i++, j += 6)
    {
        float angle = startAngle + diffAngle * i;
        RayVertices[j] = playerPos->X;
        RayVertices[j + 1] = playerPos->Y;
        RayVertices[j + 2] = 0.0f;
        float x = playerPosX;
        float y = playerPosY;
        int iteration = 0;
        int distance = 1;

        Vector2 *lVector = NULL;
        while (lVector == NULL && iteration < (8 * 10 * 20 / distance))
        {
            lVector = ReturnCollisionPos(x, y);
            iteration++;
            x += distance * cos(angle);
            y -= distance * sin(angle);
        }

        RayVertices[j + 3] = ConvertToOpenGLX(x);
        RayVertices[j + 4] = ConvertToOpenGLY(y);
        RayVertices[j + 5] = 0.0f;

        Ray *lRay = (Ray *)malloc(sizeof(Ray));
        Vector2 *lPointA = (Vector2 *)malloc(sizeof(Vector2));
        lPointA->X = playerPosX;
        lPointA->Y = playerPosY;
        lRay->pointA = lPointA;
        lRay->norme = CalculRayDistance(playerPosX, playerPosY, x, y, angle);
        lRay->rayAngle = angle;
        lRay->pointB = lVector;
        rays[i] = lRay;
    }

    glBindVertexArray(*VAO_Ray);
    glBindBuffer(GL_ARRAY_BUFFER, *VBO_RayVertices);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(RayVertices), RayVertices);
}

void Create3DWalls(unsigned int *VAO_Walls, unsigned int *VBO_Walls, unsigned int *VBO_Color_Walls)
{
    float zoom = 0.01f; // the smaller this is the bigger the walls will be
    float projPlaneDist = (WIDTH / 2.0f) / tanf(FOV / 2.0f);

    int index = 0;
    int indexColor = 0;
    for (int i = 0; i < NUMBER_OF_RAYS; i++)
    {
        int xLeft = (int)((float)WIDTH * i / NUMBER_OF_RAYS);
        int xRight = (int)((float)WIDTH * (i + 1) / NUMBER_OF_RAYS);

        float wallHeight = projPlaneDist / (rays[i]->norme * zoom);
        float yTop = HEIGHT / 2 - wallHeight / 2;
        float yBottom = yTop + wallHeight;

        float xS[6] = {
            xLeft, xLeft, xRight,
            xLeft, xRight, xRight};

        float yS[6] = {yTop, yBottom, yBottom, yTop, yTop, yBottom};

        for (int j = 0; j < 6; j++)
        {
            Wall3DVert[index++] = ConvertToOpenGLX(xS[j]);
            Wall3DVert[index++] = ConvertToOpenGLY(yS[j]);
            Wall3DVert[index++] = 0.0f;

            Wall3DColor[indexColor++] = 1.0f;
            Wall3DColor[indexColor++] = 0.0f;
            Wall3DColor[indexColor++] = 1.0f;
        }
    }

    glBindVertexArray(*VAO_Walls);
    glBindBuffer(GL_ARRAY_BUFFER, *VBO_Walls);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Wall3DVert), Wall3DVert);
}

void MultiplyMatrices(float result[], float a[], float b[])
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            float sum = 0.0f;
            for (int k = 0; k < 4; k++)
            {
                sum += (a[i * 4 + k] * b[k * 4 + j]);
            }
            result[i * 4 + j] = sum;
        }
    }
}

void MultiplyMatriceToVector(float result[], float a[], Vector2 *b)
{
    for (int i = 0; i < 16; i++)
        result[i] = 0.0f;
    result[0] = 1.0f;
    result[5] = 1.0f;
    result[10] = 1.0f;
    result[15] = 1.0f;

    result[12] = b->X;
    result[13] = b->Y;
    result[14] = 0.0f;
}

float CalculRayDistance(float xA, float yA, float xB, float yB, float rayAngle)
{
    float normalRayDistance = sqrt(pow(xB - xA, 2) + pow(yB - yA, 2));
    return normalRayDistance * cos(rayAngle - playerAngle);
}

void CreateRayVAOsVBOs(unsigned int *VAO_Ray, unsigned int *VBO_RayVertices, unsigned int *VBO_Color)
{
    glBindVertexArray(*VAO_Ray);
    glGenBuffers(1, VBO_RayVertices);
    glBindBuffer(GL_ARRAY_BUFFER, *VBO_RayVertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(RayVertices), RayVertices, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, VBO_Color);
    glBindBuffer(GL_ARRAY_BUFFER, *VBO_Color);
    glBufferData(GL_ARRAY_BUFFER, sizeof(RayColor), RayColor, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void CreateWallsVAOsVBOs(unsigned int *VAO_Wall, unsigned int *VBO_Wall_Vertices, unsigned int *VBO_Color)
{
    glBindVertexArray(*VAO_Wall);
    glGenBuffers(1, VBO_Wall_Vertices);
    glBindBuffer(GL_ARRAY_BUFFER, *VBO_Wall_Vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Wall3DVert), Wall3DVert, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, VBO_Color);
    glBindBuffer(GL_ARRAY_BUFFER, *VBO_Color);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Wall3DColor), Wall3DColor, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

int KeyPressed(int key)
{
    return glfwGetKey(window, key) || glfwGetKey(window3D, key);
}