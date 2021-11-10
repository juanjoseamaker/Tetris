#include <iostream>
#include <SDL2/SDL.h>

#include "board.hpp"

const int s_ssize = 600;

int main(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cout << "Failed to initialize the SDL2 library" << std::endl;
        std::cout << "SDL2 Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Window *window = SDL_CreateWindow("Tetris",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          s_ssize,
                                          s_ssize,
                                          0);

    if (!window) {
        std::cout << "Failed to create window" << std::endl;
        return -1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        std::cout << "Failed to get the renderer from the window" << std::endl;
        std::cout << "SDL2 Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    Board board(s_ssize / g_dim);
    bool running = true;

    while (running) {
        SDL_Event e;

        while (SDL_PollEvent(&e) > 0) {
            switch (e.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                switch (e.key.keysym.sym)
                {
                case SDLK_UP:
                    board.changeCurrentRotation(1);
                    break;
                case SDLK_DOWN:
                    board.changeCurrentRotation(-1);
                    break;
                case SDLK_LEFT:
                    board.changeCurrentPieceX(-1);
                    break;
                case SDLK_RIGHT:
                    board.changeCurrentPieceX(1);
                    break;
                }
                break;
            }
        }

        // Update
        board.update();

        // Background
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
        SDL_RenderClear(renderer);

        // Draw
        board.draw(renderer);

        // Render
        SDL_RenderPresent(renderer);

        SDL_Delay(150);
    }
}
