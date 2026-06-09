#include <stdio.h>
#include <math.h>
#include "Librairies\include\glad\glad.h"
#include "Librairies\include\GLFW\glfw3.h"
#include "LinkedList.h"
#define WIDTH 1200
#define HEIGHT 800

#define PLAYERSIZE 20
#define PI 3.141592653589793

float model[16] = {1, 0, 0, 0,
                   0, 1, 0, 0,
                   0, 0, 1, 0,
                   0, 0, 0, 1};

float view[16] = {1, 0, 0, 0,
                  0, 1, 0, 0,
                  0, 0, 1, 0,
                  0, 0, 0, 1};

float projection[16] = {1, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 1, 0,
                        0, 0, 0, 1};
float mvp[16] = {1, 0, 0, 0,
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
int map[8][10] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 1, 1},
    {1, 0, 0, 0, 1, 0, 1, 1, 0, 1},
    {1, 0, 0, 0, 1, 0, 1, 0, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 1, 0, 1, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

GLfloat gridVertices[3 * 6 * 8 * 10];
GLfloat gridColor[3 * 6 * 8 * 10];
GLuint Program;

GLfloat vertices[18];
GLfloat color[] = {
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f};

float deltaTime = 0.0f, playerPosX = 600, playerPosY = 400, playerAngle;
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
void RotatePlayer();
int IsMouseMoving();
void MultiplyMatrices(float result[], float a[], float b[]);
void MultiplyMatriceToVector(float result[], float a[], Vector2 *b);

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

    // Tell opengl the area of our window so that we can make changes after
    glViewport(0, 0, WIDTH, HEIGHT);

    glClearColor(1.0f, 0.5f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    CreateGrid();

    glUseProgram(Program);
    unsigned int VBOVertices, VBOColor, VBO_Player, VBO_Color_Player, VAO_Grid, VAO_Player;
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
    float crntTime, lastFrame;

    glClearColor(1.0f, 0.5f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(Program);
    glBindVertexArray(VAO_Grid);
    glDrawArrays(GL_TRIANGLES, 0, count);

    glBindVertexArray(VAO_Player);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glfwSwapBuffers(window);

    float timer;
    loc = glGetUniformLocation(Program, "mvp");
    // This is to make sure that the program still runs while the window is open
    while (!glfwWindowShouldClose(window))
    {
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
        glUseProgram(Program);

        Player();

        glBindVertexArray(VAO_Player);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
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

    // Just to make sure that if the window does not render the program does not break
    if (window == NULL)
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
    int xFactor = WIDTH / 10;
    int yFactor = HEIGHT / 8;
    head = (ListWall *)malloc(sizeof(ListWall));
    head->next = NULL;
    head->VALUE = NULL;
    for (int x = 0; x < WIDTH; x += xFactor)
    {
        for (int y = 0; y < HEIGHT; y += yFactor)
        {
            float xS[6] = {x, x, x + xFactor - 5, x, x + xFactor - 5, x + xFactor - 5};
            float yS[6] = {y, y + yFactor - 5, y + yFactor - 5, y, y, y + yFactor - 5};
            switch (map[(y / yFactor)][(x / xFactor)])
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
                    if (index + 1 <= 3 * 6 * 8 * 10)
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
                lSize->X = x + xFactor;
                lSize->Y = y + yFactor;

                Wall *lWall = (Wall *)malloc(sizeof(Wall));
                lWall->position = lPos;
                lWall->size = lSize;

                lWall->posX = x;
                lWall->posY = y;
                lWall->sizeX = x + xFactor;
                lWall->sizeY = y + yFactor;
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
    if (glfwGetKey(window, GLFW_KEY_D) && CheckCollision(playerPosX + PLAYERSIZE + cos(playerAngle) * deltaTime * speed, playerPosY + sin(playerAngle) * deltaTime * speed) == 0)
    {
        playerPosX += cos(playerAngle) * deltaTime * speed;
        playerPosY -= sin(playerAngle) * deltaTime * speed;
    }
    // Here A bc of qwerty
    if (glfwGetKey(window, GLFW_KEY_A) && CheckCollision(playerPosX - cos(playerAngle) * deltaTime * speed, playerPosY - sin(playerAngle) * deltaTime * speed) == 0)
    {
        playerPosX -= cos(playerAngle) * deltaTime * speed;
        playerPosY += sin(playerAngle) * deltaTime * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) && CheckCollision(playerPosX + sin(playerAngle) * deltaTime * speed, playerPosY + PLAYERSIZE - cos(playerAngle) * deltaTime * speed) == 0)
    {
        playerPosX -= sin(playerAngle) * deltaTime * speed;
        playerPosY -= cos(playerAngle) * deltaTime * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_W) && CheckCollision(playerPosX - sin(playerAngle) * deltaTime * speed, playerPosY + cos(playerAngle) * deltaTime * speed) == 0)
    {
        playerPosX += sin(playerAngle) * deltaTime * speed;
        playerPosY += cos(playerAngle) * deltaTime * speed;
    }

    playerPos->X = ConvertToOpenGLX(playerPosX);
    playerPos->Y = ConvertToOpenGLY(playerPosY);

    MultiplyMatriceToVector(model, model, playerPos);

    lastMousePos->X = mousePos->X;
    lastMousePos->Y = mousePos->Y;
    glfwGetCursorPos(window, (double *)&mousePos->X, (double *)&mousePos->Y);
    if (IsMouseMoving() == 1)
        mouseSpeed = 0.0f;
    else
        mouseSpeed = 0.01f;
    playerAngle += mouseSpeed * deltaTime * (float)(WIDTH / 2 - ((*mousePos).X));
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

void RotatePlayer()
{
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