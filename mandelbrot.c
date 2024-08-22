#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
//#include<cmath.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_mixer.h>

#define WIN_WIDTH 1000
#define WIN_HEIGHT 1000

struct complex
{
    double x;
    double y;
};

struct complex square(struct complex c)
{
    struct complex res;
    res.x = c.x * c.x - c.y * c.y;
    res.y = 2 * c.x * c.y;

    return res;
}

double value(struct complex c)
{
    return c.x * c.x + c.y * c.y;
}

double distance(struct complex c)
{
    return sqrt(c.x * c.x + c.y * c.y); 
}


int main(int argv, char **argc)
{
    int iterations, flag;
    double dist;
    struct complex z,c;

    if(argv >= 2)
        iterations = atoi(argc[1]);
    else
        iterations = 50;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Mandelbrot set", 
                                         SDL_WINDOWPOS_UNDEFINED, 
                                         SDL_WINDOWPOS_UNDEFINED, 
                                         WIN_WIDTH, WIN_HEIGHT, 
                                         SDL_WINDOW_SHOWN);

    if (window == NULL) 
    {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) 
    {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    for(double i = -2; i <= 2; i += 0.001)
    {
        for(double j = -2; j <= 2; j += 0.001)
        {
            c.x = i;
            c.y = j;
            z.x = 0;
            z.y = 0;
            flag = 1;

            for(int k = 0; k < iterations; k++)
            {
                z = square(z);
                z.x += c.x;
                z.y += c.y;
                if(value(z) > 4)
                {    
                    flag = 0;
                    break;
                }
            }
            if(flag)
            {
                dist = distance(c) / 2.0;
                
                SDL_SetRenderDrawColor(renderer, (255) * dist, 0, 255 * (2 - dist), 255);

                SDL_RenderDrawPoint(renderer, c.x * 300 + (WIN_WIDTH / 2 + 80), c.y * 300 + (WIN_HEIGHT / 2));
            }
        }
    }

    SDL_RenderPresent(renderer);

    SDL_Event event;
    while (SDL_WaitEvent(&event)) 
    {
        if (event.type == SDL_QUIT) 
        {
            break;
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
