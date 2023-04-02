#include <stdio.h>
#include <math.h>
#include <cstdlib>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

#include <SDL2/SDL.h>
#include "generator.h"

int main(int argc, char** argv) {
    int row = 20;
    int col = 20;

    if (argc == 3) {
        int arg1 = atoi(argv[1]);
        int arg2 = atoi(argv[2]);

        row = arg1 < 5 ? row : arg1;
        col = arg2 < 5 ? col : arg2;
    }

    srand(time(0));

    roomVector rooms = getRooms(row, col);
    std::vector<POINT> remainedPoints = getRemainedPoints(row, col, rooms);
    std::map<std::string, bool> remainedPointsMap = prepareRemainedPointsMap(remainedPoints);

    elementVector map;
    std::map<std::string, std::vector<std::string>> wallRecords;
    while (labyrintProgress(wallRecords, row, col, remainedPoints.size() - 1) == false) {
        for (int i = 0; i < remainedPoints.size(); i++) {
            createRoads(remainedPoints.at(i).x, remainedPoints.at(i).y, row, col, &wallRecords, &map, remainedPointsMap);
        }
        printf("a cycle has been completed!\n");
    }

    spacesMap spaces;
    prepareMazeSpaces(map, &spaces);
    prepareRoomSpaces(rooms, &spaces);

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window;
    window = SDL_CreateWindow("procedural-map-generation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 900, 900, SDL_WINDOW_RESIZABLE);

    SDL_Renderer* renderer =  SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

    int directionsArray[] = { UP, DOWN, LEFT, RIGHT };
    int leftSpace = 0;
    int topSpace = 0;
    int height = 40;
    int width = 5;
    for (int i = 0; i < col; i++) {
        for (int j = 0; j < row; j++) {
            for (int k = 0; k < sizeof(directionsArray) / sizeof(int); k++) {
                int direction = directionsArray[k];

                if (spaces[std::to_string(i) + ":" + std::to_string(j)][direction] != true) {
                    switch (direction) {
                        case UP:
                            SDL_Rect r1;
                            r1.x = height + leftSpace;
                            r1.y = height + topSpace;
                            r1.h = width;
                            r1.w = height;

                            SDL_RenderFillRect(renderer, &r1);
                            break;
                        case LEFT:
                            SDL_Rect r2;
                            r2.x = height + leftSpace;
                            r2.y = height + topSpace;
                            r2.h = height;
                            r2.w = width;

                            SDL_RenderFillRect(renderer, &r2);
                            break;
                        case DOWN:
                            SDL_Rect r3;
                            r3.x = height + leftSpace;
                            r3.y = height + topSpace + (height - width);
                            r3.h = width;
                            r3.w = height;

                            SDL_RenderFillRect(renderer, &r3);
                            break;
                        case RIGHT:
                            SDL_Rect r4;
                            r4.x = height + leftSpace + (height - width);
                            r4.y = height + topSpace;
                            r4.h = height;
                            r4.w = width;

                            SDL_RenderFillRect(renderer, &r4);
                            break;
                    }
                }
            }
            topSpace += (height - width);
        }

        leftSpace = leftSpace + (height - width);
        topSpace = 0;
    }

    SDL_RenderPresent(renderer);

    SDL_Event e;
    bool quit = false;
    while (!quit){
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT){
                quit = true;
            }
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}