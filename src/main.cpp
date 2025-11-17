/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#include <iostream>
#include <fstream>
#include <vector> 
#include <cstdlib>
#include <ctime>

// declarations (definations at the bottom)
bool checkIntr(int x, int y, int r, int px, int py, char orn, int s1, int s2);

void adjustBlockPos(std::vector<char> &orn, std::vector<double> &posX, std::vector<double> &posY, 
        std::vector<int> &s1, std::vector<int> &s2, 
        int screenWidth, int screenHeight);

void fileInput(std::string path, int &sw, int &sh,
        std::vector<char> &orn, 
        std::vector<int> &s1, 
        std::vector<int> &s2, 
        std::vector<double> &ratioX, 
        std::vector<double> &ratioY);

void drawWalls(std::vector<char> &orn, std::vector<double> &posX, std::vector<double> &posY, 
        std::vector<int> &s1, std::vector<int> &s2);

void setBallCords(int screenWidth, int screenHeight, 
        int &ballX, int &ballY, int R,
        std::vector<char> &orn, 
        std::vector<double> &posX, std::vector<double> &posY, 
        std::vector<int> &s1, std::vector<int> &s2);

void setOrn(char orn, int &w, int &h, int s1, int s2);

void checkBorderColl(int &ballX, int &ballY, int R, int screenWidth, int screenHeight, int &xd, int &yd);

int main ()
{
	// Tell the window to use vsync and work on high DPI displays 
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI); //(this came with the raylib template)

        // Taking input from file
        int screenWidth, screenHeight;
        std::vector<char> orn; std::vector<int> s1, s2; std::vector<double> posX, posY;
        fileInput("/Users/abdurrehman/Documents/inp/A3_abdurrehman_33607/resources/walls_corners_plus.txt", 
        screenWidth, screenHeight, orn, s1, s2, posX, posY);

	// Create the window and OpenGL context
	InitWindow(screenWidth, screenHeight, "Abdur Rehman - Assignment 3");
        SetTargetFPS(60);

        //changing postion of walls to center
        adjustBlockPos(orn, posX, posY, s1, s2, screenWidth, screenHeight);
        
        //initalising ball details and setting appropriate coordinates
	int R = 10, ballX, ballY, speed = 2, xd = 1, yd = 1;
        setBallCords(screenWidth, screenHeight, ballX, ballY, R, orn, posX, posY, s1, s2);
        
	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		// drawing
		BeginDrawing();

		// Setup the back buffer for drawing walls and ball
		ClearBackground(WHITE);
                drawWalls(orn, posX, posY, s1, s2);
                DrawCircle(ballX, ballY, R, BLUE);

                //check border collisions
                checkBorderColl(ballX, ballY, R, screenWidth, screenHeight, xd, yd);

                for(int i = 0; i < orn.size(); ++i){
                        int w,h; setOrn(orn[i], w, h, s1[i], s2[i]);
                        
                        //check collision with i-th wall
                        //checkBallColl(ballX, ballY, R, posX[i], posY[i], xd, yd);
                        if(ballY+R <= posY[i]+h && ballY+R >= posY[i] && ballX-R <= posX[i]+w && ballX-R >= posX[i]) xd*=-1;
                        else if(ballY-R <= posY[i]+h && ballY-R >= posY[i] && ballX-R <= posX[i]+w && ballX-R >= posX[i]) xd*=-1; //if between the y and y+h and ballLeft intercepts block reverse xd

                        if(ballY+R <= posY[i]+h && ballY+R >= posY[i] && ballX+R <= posX[i]+w && ballX+R >= posX[i]) xd*=-1;
                        else if(ballY-R <= posY[i]+h && ballY-R >= posY[i] && ballX+R <= posX[i]+w && ballX+R >= posX[i]) xd*=-1;

                        if(ballX+R <= posX[i]+w && ballX+R >= posX[i] && ballY-R <= posY[i]+h && ballY-R >= posY[i]) yd*=-1;
                        else if(ballX-R <= posX[i]+w && ballX-R >= posX[i] && ballY-R <= posY[i]+h && ballY-R >= posY[i]) yd*=-1;

                        if(ballX+R <= posX[i]+w && ballX+R >= posX[i] && ballY+R <= posY[i]+h && ballY+R >= posY[i]) yd*=-1;
                        else if(ballX-R <= posX[i]+w && ballX-R >= posX[i] && ballY+R <= posY[i]+h && ballY+R >= posY[i]) yd*=-1;
                }
                
                //simulating speed
                ballX += speed*xd, ballY += speed*yd;

		EndDrawing();
	}

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}

bool checkIntr(int x, int y, int r, int px, int py, char orn, int s1, int s2){
        int ballR = x+r;
        int ballL = x-r;
        int ballT = y-r;
        int ballB = y+r;
        
        int w,h; setOrn(orn, w, h, s1, s2);

        if(ballR > px && ballR < px+w && y < py+h && y > py-h) return true;
        if(ballL > px && ballL < px+w && y < py+h && y > py-h) return true;
        if(ballT > py && ballT < py+h && x < px+w && x > px-w) return true;
        if(ballB > py && ballB < py+h && x < px+w && x > px-w) return true;

        return false;
}

void fileInput(const std::string path, int &sw, int &sh,
        std::vector<char> &orn, 
        std::vector<int> &s1, 
        std::vector<int> &s2, 
        std::vector<double> &ratioX, 
        std::vector<double> &ratioY)
{
        std::fstream fin(path);
        fin >> sw >> sh;

        char orn_i;
        while(fin >> orn_i){
                orn.push_back(orn_i);

                int s1_i, s2_i;
                double x, y;
                fin >> s1_i >> s2_i >> x >> y;

                s1.push_back(s1_i);
                s2.push_back(s2_i);
                ratioX.push_back(x);
                ratioY.push_back(y);
        }
}

void adjustBlockPos(std::vector<char> &orn, std::vector<double> &posX, std::vector<double> &posY, 
        std::vector<int> &s1, std::vector<int> &s2, 
        int screenWidth, int screenHeight)
{
        int n = posX.size();
        for(int i = 0; i < n; ++i){
                if(orn[i] == 'v')
                        posX[i] = (screenWidth*posX[i])-(std::min(s1[i], s2[i])/2), 
                        posY[i] = (screenHeight*posY[i])-(std::max(s1[i], s2[i])/2);
                else 
                        posX[i] = (screenWidth*posX[i])-(std::max(s1[i], s2[i])/2), 
                        posY[i] = (screenHeight*posY[i])-(std::min(s1[i], s2[i])/2);
        }
}

void drawWalls(std::vector<char> &orn, std::vector<double> &posX, std::vector<double> &posY, 
        std::vector<int> &s1, std::vector<int> &s2)
{
        int n = orn.size();
        for(int i = 0; i < n; ++i){
                if(orn[i] == 'v')
                        DrawRectangle(posX[i], posY[i], std::min(s1[i], s2[i]), std::max(s1[i],s2[i]), RED);
                else 
                        DrawRectangle(posX[i], posY[i], std::max(s1[i], s2[i]), std::min(s1[i],s2[i]), RED);

        }
}

void setBallCords(int screenWidth, int screenHeight, 
        int &ballX, int &ballY, int R,
        std::vector<char> &orn, 
        std::vector<double> &posX, std::vector<double> &posY, 
        std::vector<int> &s1, std::vector<int> &s2)
{
        bool ok = 0; 
        std::srand(std::time(0));
        while(!ok){
                ballX = (std::rand() % (screenWidth-R+1)) + R;
                ballY = (std::rand() % (screenHeight-R+1)) + R;
                ok = 1;
                for(int i = 0; i < orn.size(); ++i){
                        if(checkIntr(ballX, ballY, R, posX[i], posY[i], orn[i], s1[i], s2[i])) {
                                std::cerr << ballX << " " << ballY << "\n";
                                ok = 0;
                                break;
                        }
                }
        }

}

void setOrn(char orn, int &w, int &h, int s1, int s2){
        if(orn == 'v') w = std::min(s1, s2), h = std::max(s1, s2); 
        else w = std::max(s1, s2), h = std::min(s1, s2);
}

void checkBorderColl(int &ballX, int &ballY, int R, int screenWidth, int screenHeight, int &xd, int &yd){
        if(ballX-R <= 0 || ballX+R >= screenWidth) xd *= -1;
        if(ballY-R <= 0 || ballY+R >= screenHeight) yd *= -1;
}
