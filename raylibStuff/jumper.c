#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_ENEMIES 6

typedef enum GameScreen { TITLE = 0, GAMEPLAY } GameScreen;

typedef struct EnemyRectangle {
    float x;                // Rectangle top-left corner position x
    float y;                // Rectangle top-left corner position y
    float width;            // Rectangle width
    float height;           // Rectangle height
    float speed;
} EnemyRectangle;

int main(void) {
  const int screenWidth = 800;
  const int screenHeight = 450;
  GameScreen currentScreen = TITLE;

  InitWindow(screenWidth, screenHeight, "Jumpy man");

  SetTargetFPS(60); // 60 frames per second

  int defaultYPos = 280;
  int playerWidth = 20;
  int defaultXPos = (screenWidth-playerWidth)/2;
  Rectangle player = { defaultXPos, defaultYPos, 20, 40};
  EnemyRectangle *enemies[MAX_ENEMIES];
  for (int i = 0; i < MAX_ENEMIES; i++) {
    enemies[i] = NULL;
  }

  bool playerJumping;
  float yRate = -220;  // px/sec
  float gravity = 250; //px/sec^2
  float secondsElapsed = 0;
  float score = 0.0;
  char scoreStr[15];

  while (!WindowShouldClose()){
    BeginDrawing();
    ClearBackground(RAYWHITE);
    switch(currentScreen){
      case GAMEPLAY: {
        
        if (IsKeyDown(KEY_UP) && !playerJumping){
          secondsElapsed = 0;
          playerJumping = true;
        }
        
        if(playerJumping){
          player.y = ((yRate*secondsElapsed) + (0.5*gravity*secondsElapsed*secondsElapsed)) + defaultYPos;
          if(player.y > defaultYPos){
            player.y = defaultYPos;
            playerJumping = false;
          }
          else
            secondsElapsed += (1.0/60.0);
        }

        if (IsKeyDown(KEY_LEFT) && player.x > 10){
          player.x -= 2;
        }

        if (IsKeyDown(KEY_RIGHT) && (player.x < (screenWidth - player.width) -10)){
          player.x += 2;
        }

        for (int i = 0; i < MAX_ENEMIES; i++) {
          if(enemies[i]){

            (*enemies[i]).x -= (*enemies[i]).speed;
            if(player.x < (*enemies[i]).x){
              if(
                ((*enemies[i]).x - player.x < player.width) &&
                (player.y+player.height >= (*enemies[i]).y)
              ) currentScreen = TITLE;
            }
            else if (
              (player.x - (*enemies[i]).x < (*enemies[i]).width) &&
              (player.y+player.height >= (*enemies[i]).y)
            ) currentScreen = TITLE;

            if((*enemies[i]).x < -100){
              free(enemies[i]);
              enemies[i] = NULL;
            }

          }
          else{
            enemies[i] = malloc(sizeof(EnemyRectangle));
            (*enemies[i]).x = 900;
            (*enemies[i]).y = 290;
            (*enemies[i]).width = (float)GetRandomValue(20, 50);
            (*enemies[i]).height = 30;
            (*enemies[i]).speed = (( ((float) rand()) / (float) RAND_MAX)*6) + 1;
          }
        }
        score += 0.01;
        sprintf(scoreStr, "Score: %d", (int) (score*100));
        DrawText(scoreStr, 640, 10, 20, RED);
      } break;
      
      case TITLE: {
        DrawText("PRESS ENTER to play", 300, 100, 20, DARKGREEN);

        if(score)
          DrawText(scoreStr, 350, 150, 20, RED);
        if (IsKeyPressed(KEY_ENTER)){
          score = 0.0;
          playerJumping = false;
          player.y = defaultYPos;
          player.x = defaultXPos;
          currentScreen = GAMEPLAY;
          for (int i = 0; i < MAX_ENEMIES; i++) {
            enemies[i] = NULL;
          }
        }
      } break;
      default: break;
    }
    
    DrawRectangleRec(player, RED);

    for (int i = 0; i < MAX_ENEMIES; i++) {
      if(enemies[i]){
        Rectangle enemyRectangle = {
          (*enemies[i]).x, 
          (*enemies[i]).y, 
          (*enemies[i]).width, 
          (*enemies[i]).height
        };
        DrawRectangleRec(enemyRectangle, DARKGREEN);
      }
    }
    DrawRectangle(0, 320, 800, screenWidth - 320, DARKGRAY);
    EndDrawing();
  }
  
  CloseWindow();

  return 0;
}
