#include "raylib.h"

struct  AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

bool isOnGround(AnimData data, int windowHeight)
{
    return data.pos.y >= windowHeight - data.rec.height;
}

AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame)
{
    // UPDATE RUNNING TIME
    data.runningTime += deltaTime;
    if (data.runningTime >= data.updateTime)
    {
        data.runningTime = 0.0;
        // UPDATE ANIMATION FRAME
        data.rec.x = data.frame * data.rec.width;
        data.frame++;
        if (data.frame > maxFrame)
        {
            data.frame = 0;
        }
    }
    return data;
}

int main()
{ 
    // creating the array for window dimensions
    int windowDimensions[2];
    windowDimensions[0] = 512;
    windowDimensions[1] = 380;

    // initialize the window
    InitWindow(windowDimensions[0], windowDimensions[1], "Dapper-Dasher!");

    // acceleration due to gravity (pixels/S)/S
    const int gravity{1'000};

    // nebula variables
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

    const int sizeOfNebulae{10};
    AnimData nebulae[sizeOfNebulae]{};

    for (int i = 0; i < sizeOfNebulae; i++)
    {
        nebulae[i].rec.x = 0.0;
        nebulae[i].rec.y = 0.0;
        nebulae[i].rec.width = nebula.width/8;
        nebulae[i].rec.height = nebula.height/8;
        nebulae[i].pos.y = windowDimensions[1] - nebula.height/8;
        nebulae[i].frame = 0;
        nebulae[i].runningTime = 0.0;
        nebulae[i].updateTime = 1.0/16.0;
        nebulae[i].pos.x = windowDimensions[0] + i * 300;
    }

    float finishLine{ nebulae[sizeOfNebulae - 1].pos.x };

    //nebula x velocity (pixels/second)
    int nebVel{-200};

    // Scarfy Variables
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    AnimData scarfyData;
    scarfyData.rec.width = scarfy.width/20;
    scarfyData.rec.height = scarfy.height;
    scarfyData.rec.x = 0;
    scarfyData.rec.y = 0;
    scarfyData.pos.x = windowDimensions[0]/2 - scarfyData.rec.width/2;
    scarfyData.pos.y = windowDimensions[1] - scarfyData.rec.height;
    scarfyData.frame = 0;
    scarfyData.updateTime = 1.0/12.0;
    scarfyData.runningTime = 0.0;

    // is rectangle in the air
    bool isInAir{};
    // jump velocity(pixels/s) 
    const int jumpVel{-600};

    int velocity{0};

    Texture2D background = LoadTexture("textures/background.png");
    float bgX{};
    Texture2D midground = LoadTexture("textures/midground.png");
    float mgX{};
    Texture2D foreground = LoadTexture("textures/foreground.png");
    float fgX{};
    
    bool collision{};
    
    // Game Logic Begins
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        // delta time (time since last frame)
        const float dT{GetFrameTime()};

     
        // Start Drawing
       BeginDrawing();
       ClearBackground(WHITE);




       //SCROLL BACKGROUND
       bgX -= 10 * dT;
       if (bgX <= -background.width*2)
       {
            bgX = 0.0;
       }

       //SCROLL MIDGROUND
       mgX -= 40 * dT;
       if (mgX <= -midground.width*2)
       {
            mgX = 0;
       }

       //SCROLL FOREGROUND
       fgX -= 80 * dT;
       if (fgX <= -foreground.width*2)
       {
            fgX = 0;
       }
       
       
       

       //DRAW THE BACKGROUND
       Vector2 bg1Pos{bgX, 0.0};
       DrawTextureEx(background, bg1Pos, 0.0, 2.0, WHITE);
       Vector2 bg2Pos{bgX + background.width * 2, 0.0};
       DrawTextureEx(background, bg2Pos, 0.0, 2.0, WHITE);

       //DRAW THE MIDGROUND
       Vector2 mg1Pos{mgX, 0.0};
       DrawTextureEx(midground, mg1Pos, 0.0, 2.0, WHITE);
       Vector2 mg2Pos{mgX + midground.width*2, 0.0};
       DrawTextureEx(midground, mg2Pos, 0.0, 2.0, WHITE);

       //DRAW THE FOREGROUND
       Vector2 fg1Pos{fgX, 0.0};
       DrawTextureEx(foreground, fg1Pos, 0.0, 2.0, WHITE);
       Vector2 fg2Pos{fgX + foreground.width*2, 0.0};
       DrawTextureEx(foreground, fg2Pos, 0.0, 2.0, WHITE);



       //GROUND CHECK GROUND CHECK GROUND CHECK GROUND CHECK
       if (isOnGround(scarfyData, windowDimensions[1]))
       {
            // rectangle is on the ground
            velocity = 0;
            isInAir = false;
       }
       else
       {
            // rectangle is on the air
            velocity += gravity * dT;
            isInAir = true;
       }


        // JUMP CHECK
        if (IsKeyDown(KEY_SPACE) && !isInAir)
        {
            velocity += jumpVel;
        }



        for (int i = 0; i < sizeOfNebulae; i++)
        {
            // updating the position of all nebulas
            nebulae[i].pos.x += nebVel * dT;
        }


        // UPDATE FINISH LINE
        finishLine += nebVel * dT; 
        

        
        // Update Scarfy position
        scarfyData.pos.y += velocity * dT;


    
        //update running time
        if (!isInAir)
        {
           scarfyData = updateAnimData(scarfyData, dT, 19);
        }

        for (int i = 0; i < sizeOfNebulae; i++)
        {
            // update nebula animation frame
            nebulae[i] = updateAnimData(nebulae[i], dT, 7);
        }


        
        for (AnimData nebula : nebulae)
        {
            float pad{25};
            Rectangle nebRec{
                nebula.pos.x + pad,
                nebula.pos.y + pad,
                nebula.rec.width - 2*pad,
                nebula.rec.height - 2*pad
            };
            Rectangle scarfyRec{
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rec.width - pad,
                scarfyData.rec.height - pad
            };
            if (CheckCollisionRecs(nebRec, scarfyRec))
            {
                collision = true;
            }
            
        }

        if (collision)
        {
            // LOSE THE GAME
            DrawText("GAME OVER", 
            windowDimensions[0]/4, 
            windowDimensions[1]/2,
            50,
            RED);
        }
        else if (scarfyData.pos.x >= finishLine)
        {
            // WIN THE GAME
            DrawText("YOU WIN!", 
            windowDimensions[0]/4, 
            windowDimensions[1]/2,
            50,
            RED);
        }
        

            else
            {
                for (int i = 0; i < sizeOfNebulae; i++)
                {
                    //draw nebula
                    DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
                }
            
                //draw Scarfy
                DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
            }
            
            
        
        
        
                   
        

        
       // Stop Drawing
       EndDrawing();
    }
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow(); 
}