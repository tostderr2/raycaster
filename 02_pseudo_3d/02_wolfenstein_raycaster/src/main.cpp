#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_oldnames.h>

#include "player.h"

void render(SDL_Renderer *renderer);
void processInput(SDL_Event *event, Player player, double dt);
bool running = true;

struct Color {
    float r;
    float g;
    float b;
    float a;
};

Color teal = {
    .r = 145 / 255.0f,
    .g = 217 / 255.0f,
    .b = 214 / 255.0f,
    .a = 1.0f,
};

int main(int argc, char *argv[]) {
    // init and setup basic window and renderer
    /* We will use this renderer to draw into this window every frame. */
    static SDL_Window *window = NULL;
    static SDL_Renderer *renderer = NULL;
    constexpr int winWid = 1080;
    constexpr int winHt = 720;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("examples/renderer/clear", winWid, winHt, SDL_WINDOW_RESIZABLE,
                                     &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    /*
     * This function sets the width and height of the logical rendering output.
     * The renderer will act as if the current render target is always the
     * requested dimensions, scaling to the actual resolution as necessary.
     * SDL_LOGICAL_PRESENTATION_INTEGER_SCALE   < The rendered content is scaled up by integer
     * multiples to fit the output resolution
     */
    if (!SDL_SetRenderLogicalPresentation(renderer, winWid, winHt,
                                          SDL_LOGICAL_PRESENTATION_INTEGER_SCALE)) {
        SDL_Log("Couldn't Set render logical presentation: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    // init done

    Player player;
    SDL_Event event;
    constexpr double fixedFrameSpeed = 16 / 1000.0f;
    double startTime = static_cast<double>(SDL_GetTicks());
    while (running) {
        double currentTime = static_cast<double>(SDL_GetTicks());
        double dt = currentTime - startTime;
        if (dt < fixedFrameSpeed)
            continue;

        startTime = currentTime;
        processInput(&event, player, dt);

        render(renderer);
    }
    return 0;
}
void render(SDL_Renderer *renderer) {
    // draws a bloody sunset color

    SDL_RenderClear(renderer);
    // draw the bg
    SDL_SetRenderDrawColorFloat(renderer, teal.r, teal.g, teal.b, teal.a);
    SDL_RenderPresent(renderer);
}

void processInput(SDL_Event *event, Player player, double dt) {
    SDL_PollEvent(event);
    if (event->type == SDL_EVENT_QUIT)
        running = false;

    switch (event->key.key) {
    case SDLK_ESCAPE: {

        running = false;
        break;
    }
    case SDLK_W: {
        player.moveForward(dt);
        break;
    }
    case SDLK_A: {
        player.moveLeft(dt);
        break;
    }
    case SDLK_S: {
        player.moveBackward(dt);
        break;
    }

    case SDLK_D: {
        player.moveRight(dt);
        break;
    }
    default:
        break;
    }
}
