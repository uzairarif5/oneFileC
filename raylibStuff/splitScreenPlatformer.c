
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#define PLAYER_SIZE 40
#define MAX_BULLETS 150
#define GRAVITY 250
#define LARGEST_PLATFORM_WIDTH 1500

typedef struct EnvItem {
	Rectangle rect;
	int blocking;
	Color color;
} EnvItem;

typedef struct MoveControls {
  int left_key;
	int right_key;
  int up_key;
} MoveControls;

typedef enum {
  LEFT = -1,
  RIGHT = 1
} Directions;

typedef struct Bullet{
  int framesSurvived;
  float x;
  float y;
  Directions dir;
} Bullet;

void movePlayer(Rectangle *player1, EnvItem *envItems, int envItemsLength, MoveControls keys, float * secondsForP, float *lastStandPos, bool *playerFalling, int *yRate, int *lastDir);
bool playerFalls(Rectangle *player, float *secondsForP, EnvItem *envItems, int envItemsLength, float lastStandPos, int yRate);
bool platformBelow(Rectangle *player, float prevY, EnvItem envItem);
void drawStuff(RenderTexture *screenCamera, Camera2D camera, int envItemsLength, EnvItem *envItems, Rectangle *player1, Rectangle *player2, Color color1, Color color2, char *topMessage);
void bulletStuff(Rectangle *player1, Rectangle *player2);
bool checkPlayerHit(Rectangle *player, int i);
void shootStuff(Rectangle *player, int shoot_key, Directions dir, int *lastShoot);

//globals
Bullet bullets[MAX_BULLETS];
int frameCounter = 0;
int p1Score = 0;
int p2Score = 0;
char p1Msg[100] = "W/S/A/D to move and SPACE to shoot";
char p2Msg[100] = "UP/DOWN/LEFT/RIGHT to move and ENTER to shoot";

int main(void) {
  const int screenWidth = 800;
  const int screenHeight = 440;

  InitWindow(screenWidth, screenHeight, "raylib [core] example - 2d camera split screen");

  Rectangle player1 = {10, 200, PLAYER_SIZE, PLAYER_SIZE};
  Rectangle player2 = {LARGEST_PLATFORM_WIDTH - 10 - PLAYER_SIZE, 200, PLAYER_SIZE, PLAYER_SIZE};
  float secondsForP1;
  float secondsForP2;
  float lastStandPos1 = 200;
  float lastStandPos2 = 200;
  bool player1Falling = false;
  bool player2Falling = false;
  int p1Rate = 0;
  int p2Rate = 0;
  int lastDir1 = LEFT;
  int lastDir2 = LEFT;
  int lastShoot1 = 0;
  int lastShoot2 = 0;
  
  for (int i = 0; i < 100; i++){
    bullets[i].framesSurvived = -1;
  }

	EnvItem envItems[] = {
    {{0, 0, LARGEST_PLATFORM_WIDTH, 400}, 0, LIGHTGRAY},
    {{0, 400, LARGEST_PLATFORM_WIDTH, 200}, 1, GRAY},
    {{300, 200, 400, 10}, 1, GRAY},
    {{250, 300, 100, 10}, 1, GRAY},
    {{650, 300, 100, 10}, 1, GRAY},
    {{850, 300, 200, 10}, 1, GRAY},
    {{950, 150, 400, 10}, 1, GRAY},
    {{1150, 200, 350, 10}, 1, GRAY},
  };
	int envItemsLength = sizeof(envItems) / sizeof(envItems[0]);

  Camera2D camera1 = {0};
  camera1.target = (Vector2){player1.x, player1.y};
  camera1.offset = (Vector2){200.0f, 200.0f};
  camera1.rotation = 0.0f;
  camera1.zoom = 1.0f;

  Camera2D camera2 = {0};
  camera2.target = (Vector2){player2.x, player2.y};
  camera2.offset = (Vector2){200.0f, 200.0f};
  camera2.rotation = 0.0f;
  camera2.zoom = 1.0f;

  RenderTexture screenCamera1 =
      LoadRenderTexture(screenWidth / 2, screenHeight);
  RenderTexture screenCamera2 =
      LoadRenderTexture(screenWidth / 2, screenHeight);

  // Build a flipped rectangle the size of the split view to use for drawing
  // later
  Rectangle splitScreenRect = {0.0f, 0.0f, (float)screenCamera1.texture.width,
                               (float)-screenCamera1.texture.height};

  SetTargetFPS(60);  // Set our game to run at 60 frames-per-second
  //--------------------------------------------------------------------------------------

  // Main game loop
  while (!WindowShouldClose()) {
    frameCounter++;

    movePlayer(&player1, envItems, envItemsLength, (MoveControls){KEY_A, KEY_D , KEY_W}, &secondsForP1, &lastStandPos1, &player1Falling, &p1Rate, &lastDir1);
    movePlayer(&player2, envItems, envItemsLength, (MoveControls){KEY_LEFT, KEY_RIGHT, KEY_UP}, &secondsForP2, &lastStandPos2, &player2Falling, &p2Rate, &lastDir2);

    bulletStuff(&player1, &player2);

    if(frameCounter - lastShoot1 > 100)
      shootStuff(&player1, KEY_SPACE, lastDir1, &lastShoot1);
    if(frameCounter - lastShoot2 > 100)
      shootStuff(&player2, KEY_ENTER, lastDir2, &lastShoot2);

    camera1.target = (Vector2){player1.x, player1.y};
    camera2.target = (Vector2){player2.x, player2.y};

    drawStuff(&screenCamera1, camera1, envItemsLength, envItems, &player1, &player2, RED, BLUE, p1Msg);
    drawStuff(&screenCamera2, camera2, envItemsLength, envItems, &player2, &player1, BLUE, RED, p2Msg);

    // Draw both views render textures to the screen side by side
    BeginDrawing();
    ClearBackground(BLACK);

    DrawTextureRec(screenCamera1.texture, splitScreenRect, (Vector2){0, 0}, WHITE);
    DrawTextureRec(screenCamera2.texture, splitScreenRect, (Vector2){screenWidth / 2.0f, 0}, WHITE);

    DrawRectangle(GetScreenWidth() / 2 - 2, 0, 4, GetScreenHeight(), GRAY);
    EndDrawing();
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  UnloadRenderTexture(screenCamera1);  // Unload render texture
  UnloadRenderTexture(screenCamera2);  // Unload render texture

  CloseWindow();  // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}

void movePlayer(Rectangle *player, EnvItem *envItems, int envItemsLength, MoveControls keys, float *secondsForP, float *lastStandPos, bool *playerFalling, int *yRate, int *lastDir){

  if (IsKeyDown(keys.right_key)){
    if(player->x < LARGEST_PLATFORM_WIDTH - PLAYER_SIZE) player->x += 3.0f;
    *lastDir = RIGHT;
  }
  else if (IsKeyDown(keys.left_key)){
    if(player->x > 0) player->x -= 3.0f;
    *lastDir = LEFT;
  }
  if (IsKeyPressed(keys.up_key) && (!(*playerFalling))){
    (*playerFalling) = true;
    (*lastStandPos) = player->y;
    (*secondsForP) = 0;
    (*yRate) = -250;
  }

  if(*playerFalling)
    (*playerFalling) = playerFalls(player, secondsForP, envItems, envItemsLength, *lastStandPos, *yRate);
  else{
    (*yRate) = 0;
    for (int i = 0; i < envItemsLength; i++){
      if (platformBelow(player, player->y, envItems[i])){
        (*playerFalling) = false;
        break;
      }
      else{
        (*playerFalling) = true;
        (*lastStandPos) = player->y;
        (*secondsForP) = 0;
      }
    }
  }
  
}

bool playerFalls(Rectangle *player, float *secondsForP, EnvItem *envItems, int envItemsLength, float lastStandPos, int yRate){
  bool playerFalling = true;
  float prevY = player->y;
  (*secondsForP) += 1.0/60.0;
  player->y = ((yRate*(*secondsForP)) + (0.5*GRAVITY*(*secondsForP)*(*secondsForP))) + lastStandPos;
  if(prevY < player->y){
    for (int i = 0; i < envItemsLength; i++){
      if (platformBelow(player, prevY, envItems[i])){
        player->y = envItems[i].rect.y - player->height;
        playerFalling = false;
        break;
      };
    }
  }
  return playerFalling;
}

bool platformBelow(Rectangle *player, float prevY, EnvItem envItem){
  return ((player->x + player->width) > envItem.rect.x) &&
    (player->x < (envItem.rect.x + envItem.rect.width)) &&
    ((prevY + player->height) <= envItem.rect.y) &&
    ((player->y + player->height) >= envItem.rect.y);
}

void drawStuff(RenderTexture *screenCamera, Camera2D camera, int envItemsLength, EnvItem *envItems, Rectangle *player1, Rectangle *player2, Color color1, Color color2, char *topMessage){
  BeginTextureMode(*screenCamera);
  ClearBackground(LIGHTGRAY);

  BeginMode2D(camera);
  
  for (int i = 0; i < envItemsLength; i++)
    DrawRectangleRec(envItems[i].rect, envItems[i].color);

  DrawRectangleRec(*player2, color2);
  DrawRectangleRec(*player1, color1);

  for (int i = 0; i < MAX_BULLETS; i++)
    if(bullets[i].framesSurvived != -1)
      DrawRectangleRec((Rectangle){bullets[i].x, bullets[i].y, 10, 10}, YELLOW);

  EndMode2D();

  DrawRectangle(0, 0, GetScreenWidth() / 2, 30, Fade(RAYWHITE, 0.6f));
  DrawText(topMessage, 10, 10, 10, BLACK);

  EndTextureMode();
}

void bulletStuff(Rectangle *player1, Rectangle *player2){
  for (int i = 0; i < MAX_BULLETS; i++){
    if(bullets[i].framesSurvived != -1){
      bullets[i].framesSurvived += 1;
      bullets[i].x += (float) (bullets[i].dir*8);
      if (checkPlayerHit(player1, i)) {
        p2Score += 1;
        sprintf(p2Msg, "P2 Score: %d", p2Score);
        bullets[i].framesSurvived = -1;
      }
      else if (checkPlayerHit(player2, i)){
        p1Score += 1;
        sprintf(p1Msg, "P1 Score: %d", p1Score);
        bullets[i].framesSurvived = -1;
      }
      else if (bullets[i].x < -100 || bullets[i].x > LARGEST_PLATFORM_WIDTH + 100)
        bullets[i].framesSurvived = -1;
    }
  }
}

bool checkPlayerHit(Rectangle *player, int i){
  return (
    bullets[i].x > player->x &&
    bullets[i].x < (player->x + player->width) &&
    bullets[i].y > player->y &&
    bullets[i].y < (player->y + player->height)
  );
}

void shootStuff(Rectangle *player, int shoot_key, Directions dir, int *lastShoot){
  if(IsKeyPressed(shoot_key)){
    (*lastShoot) = frameCounter;
    for (int i = 0; i < MAX_BULLETS; i++){
      if(bullets[i].framesSurvived == -1){
        if(dir > 0) bullets[i].x = player->x + player->width;
        else bullets[i].x = player->x - 10;
        bullets[i].y = player->y + (player->height/2);
        bullets[i].dir = dir;
        bullets[i].framesSurvived = 0;
        break;
      }
    }
  }
}