#include "graphic.h"
#include "SDL2/SDL.h"

void buildView(int row, int col, spacesMap *spaces) {
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window;
    window = SDL_CreateWindow("procedural-map-generation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 900, 900, SDL_WINDOW_RESIZABLE);

    SDL_Renderer* renderer =  SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

    int leftSpace = 0;
    int topSpace = 0;
    int height = 40;
    int width = 5;
    for (int i = 0; i < col; i++) {
        for (int j = 0; j < row; j++) {
            std::string xyKey = std::to_string(i) + ":" + std::to_string(j);
            if ((*spaces)[xyKey][UP] != true) {
                SDL_Rect r1;
                r1.x = height + leftSpace;
                r1.y = height + topSpace;
                r1.h = width;
                r1.w = height;

                SDL_RenderFillRect(renderer, &r1);
            }

            if ((*spaces)[xyKey][LEFT] != true) {
                SDL_Rect r2;
                r2.x = height + leftSpace;
                r2.y = height + topSpace;
                r2.h = height;
                r2.w = width;

                SDL_RenderFillRect(renderer, &r2);
            }

            if ((*spaces)[xyKey][DOWN] != true) {
                SDL_Rect r3;
                r3.x = height + leftSpace;
                r3.y = height + topSpace + (height - width);
                r3.h = width;
                r3.w = height;

                SDL_RenderFillRect(renderer, &r3);
            }

            if ((*spaces)[xyKey][RIGHT] != true) {
                SDL_Rect r4;
                r4.x = height + leftSpace + (height - width);
                r4.y = height + topSpace;
                r4.h = height;
                r4.w = width;

                SDL_RenderFillRect(renderer, &r4);
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
}