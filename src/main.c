#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
uint32_t* colorBuffer = NULL;

const int windowWidth = 800;
const int windowHeight = 600;
bool isRunning = false;

void initializeSDL()
{
    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow(
        "My engine",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth,
        windowHeight,
        0);

    renderer = SDL_CreateRenderer(window, -1, 0);

    isRunning = true;
}

void setup() {
    colorBuffer = malloc(sizeof(uint32_t) * windowWidth * windowHeight);
}

void clear() {
    free(colorBuffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void processInput()
{
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type)
    {
    case SDL_QUIT:
        isRunning = false;
        break;

    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE)
        {
            isRunning = false;
        }
        break;
    }
}

int main()
{
    initializeSDL();
    setup();

    while (isRunning)
    {
        processInput();
    }

    clear();

    return 0;
}