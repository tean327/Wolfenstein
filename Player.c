#include"mathFuncs.h";

#define PLAYER_SIZE 20
#define PLAYER_ORIGINAL_POS_X 620
#define PLAYER_ORIGINAL_POS_Y 410


typedef struct player
{
    Vector2 position;
    float angle;
    int speed;
} Player;

Player* CreatePlayer(int speed)
{
    Player* player = (Player*)malloc(sizeof(Player));
    if(!player)
    {
        printf("Unable to allocate memory for player\n");
        exit(2);
    }

    player->speed = speed;
    player->position.X = PLAYER_ORIGINAL_POS_X;
    player->position.Y = PLAYER_ORIGINAL_POS_Y;
    player->angle = 2*PI;
    return player;
}

void CreatePlayer(unsigned int *VAO_Player, unsigned int *VBO_Player, unsigned int *VBO_Color_Player)
{
    float xS[6] = {playerPosX, playerPosX, playerPosX + PLAYERSIZE, playerPosX, playerPosX + PLAYERSIZE, playerPosX + PLAYERSIZE};
    float yS[6] = {playerPosY, playerPosY + PLAYERSIZE, playerPosY + PLAYERSIZE, playerPosY, playerPosY, playerPosY + PLAYERSIZE};
    int index = 0;
    for (int i = 0; i < 6; i++)
    {
        vertices[index++] = ConvertToOpenGLX(xS[i], WIDTH);
        vertices[index++] = ConvertToOpenGLY(yS[i], HEIGHT);
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
    playerPos->X = ConvertToOpenGLX(playerPosX, WIDTH);
    playerPos->Y = ConvertToOpenGLY(playerPosY, HEIGHT);

    MultiplyMatriceToVector(model, playerPos);

    RotatePlayer();

    glUniformMatrix4fv(loc, 1, GL_FALSE, model);
}

void RotatePlayer()
{
    if (KeyPressed(GLFW_KEY_LEFT))
        playerAngle -= deltaTime;
    if (KeyPressed(GLFW_KEY_RIGHT))
        playerAngle += deltaTime;

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

    MultiplyMatriceToVector(model, origin);
    MultiplyMatrices(model, model, rotationMatrice);
    float toPlayer[16];
    MultiplyMatriceToVector(toPlayer, playerPos);
    MultiplyMatrices(model, model, toPlayer);
}