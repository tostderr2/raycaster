

#include <cmath>
#include <iostream>
#include <ostream>

#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>

#include "constants.h"
#include "player.h"

void render(SDL_Renderer *renderer, SDL_Surface *winSurface, Player &player);
void renderRaycasted(SDL_Renderer *renderer, SDL_Surface *winSurface, Player &p);
void processInput(SDL_Event *event, Player &player, double dt);
bool running = true;

int main(int argc, char *argv[]) {
    // init and setup basic window and renderer
    /* We will use this renderer to draw into this window every frame. */
    static SDL_Window *window{nullptr};
    static SDL_Renderer *renderer{nullptr};
    static SDL_Surface *winSurface{nullptr};

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_CreateWindowAndRenderer("Wolfenstien raycastor", KWinWidth, KWinHeight,
                                SDL_WINDOW_RESIZABLE, &window, &renderer);
    if (!window) {
        SDL_Log("Couldn't create window and renderer: %s\n", SDL_GetError());
        return -1;
    }

    // todo: check later
    // this makes the actual draw screen of the size of our given wid and ht
    // SDL_LOGICAL_PRESENTATION_STRETCH,   /< The rendered content is stretched to the output
    // resolution
    if (!SDL_SetRenderLogicalPresentation(renderer, KWinWidth, KWinHeight,
                                          SDL_LOGICAL_PRESENTATION_STRETCH)) {
        SDL_Log("Couldn't Set render logical presentation: %s\n", SDL_GetError());
        return -1;
    }

    winSurface = SDL_GetWindowSurface(window);
    if (!winSurface) {
        SDL_Log("Couldn't get window surface: %s\n", SDL_GetError());
        return -1;
    }
    // init done

    Player player;
    SDL_Event event;

    constexpr double fixedFrameSpeed = 0.018f;
    double prevTime = static_cast<double>(SDL_GetTicks()) / 1000.0f;

    while (running) {
        double currentTime = static_cast<double>(SDL_GetTicks()) / 1000.0f;
        double dt = currentTime - prevTime;
        if (dt < fixedFrameSpeed)
            continue;

        prevTime = currentTime;
        processInput(&event, player, dt);

        // renders the 2d top down view
        // render(renderer, winSurface, player);
        renderRaycasted(renderer, winSurface, player);
    }
    return 0;
}

Vec4i GetColor(int num) {
    Vec4i wall;
    switch (num) {
    case 0:
        wall = {0, 0, 0, 255};
        break;
    case 1:
        wall = {100, 50, 25, 255};
        break;
    case 2:
        wall = {25, 100, 50, 255};
        break;
        wall = {0, 0, 0, 255};
    default:
        break;
    }
    return wall;
}
void renderRaycasted(SDL_Renderer *renderer, SDL_Surface *winSurface, Player &p) {
    // draw the bg
    // sky blue color
    SDL_SetRenderDrawColorFloat(renderer, KTeal.r, KTeal.g, KTeal.b, KTeal.a);
    SDL_RenderClear(renderer);
    // end of bg

    float angleStep = p.m_FOVRad / KWinWidth;

    for (size_t col = 0; col < KWinWidth - 1; ++col) {
        float rayStep = 0.1f;
        float dist = 0.0f;
        Color color;

        // cast the ray ath the ray angle and get the distance from player to the wall
        float rayAngle = p.m_lookAngleRad - p.m_FOVBy2Rad + (col * angleStep);
        Vec2f direction = Vec2f(cosf(rayAngle), sinf(rayAngle));
        bool stop = false;
        while (!stop) {
            int stepX = static_cast<int>(p.Posf().x + direction.x * dist);
            int stepY = static_cast<int>(p.Posf().y + direction.y * dist);
            if (stepX < 0 || stepX >= KMapWidth || stepY < 0 || stepY >= KMapHeight) {
                stop = true;
                break;
            }
            if (KMap[stepY][stepX] > 0) {
                switch (KMap[stepY][stepX]) {
                // case MudFloor:
                //     color = KMudFloor;
                //     break;
                case StoneWall_ID:
                    color = KStoneWall;
                    break;
                case RedWall_ID:
                    color = KRedWall;
                    break;
                case IceBlue_ID:
                    color = KIceBlueWall;
                    break;
                default:
                    color = KDefaultWhite;
                    break;
                }

                stop = true;
            }
            dist += rayStep;
        }
        // ray dist done
        // raw dist will be longer at the edges (-fov/2 and +fov/2)
        // so using perpendicular dist from the players y pos is much better visually
		// removes the fish eye effect (where the view plane looks a bit like a globe)
		float perpDist = dist * cosf(p.m_FOVBy2Rad - rayAngle);

        // draw the map for this one column of window with the dist value
        // get the wall ht
        // will be put into if else block later with more accurate dist
        // to ht representation
        if (perpDist < 0.1) {
            perpDist = 0.1f;
        }

        float screenCentre = KWinHeight / 2.0f;
        float wallHt = KWinHeight / perpDist;
        float wallStart = screenCentre - wallHt / 2.0f;
        float wallEnd = screenCentre + wallHt / 2.0f;

        // walls
        SDL_SetRenderDrawColorFloat(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderLine(renderer, (float)col, wallStart, (float)col, wallEnd);

        // floor
        color = KMudFloor;
        SDL_SetRenderDrawColorFloat(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderLine(renderer, (float)col, wallEnd, (float)col, KWinHeight);

        // bg is all blue, so sky is already drawn
    }

    SDL_RenderPresent(renderer);
}

void processInput(SDL_Event *event, Player &player, double dt) {
    while (SDL_PollEvent(event)) {
        if (event->type == SDL_EVENT_QUIT) {
            running = false;
        }

        if (event->type == SDL_EVENT_KEY_DOWN)
            if (event->key.key == SDLK_ESCAPE) {
                running = false;
            }
    }

    const bool *keypressed = SDL_GetKeyboardState(NULL);

    if (keypressed[SDL_SCANCODE_W]) {
        player.MoveForward(dt);
    }
    if (keypressed[SDL_SCANCODE_S]) {
        player.MoveBackward(dt);
    }
    // todo: add strafing here later
    // if (keypressed[SDL_SCANCODE_A]) {
    //     player.MoveLeft(dt);
    // }
    // if (keypressed[SDL_SCANCODE_D]) {
    //     player.MoveRight(dt);
    // }
    if (keypressed[SDL_SCANCODE_RIGHT]) {
        player.TurnRight(dt);
    }
    if (keypressed[SDL_SCANCODE_LEFT]) {
        player.TurnLeft(dt);
    }
}

// will someday clean up and remove globals so just keeping this a bit cleaner. maybe just using
template <size_t Rows, size_t Cols>
// this should return some data, dist of the wall hit, if hit
void castRay(Vec2f startPt, float viewAngle, const int (&map)[Rows][Cols]) {
}

// todo: player pos is changed from win related to map related. change the impl here
// this can be used for mini map maybe
// void render(SDL_Renderer *renderer, SDL_Surface *winSurface, Player &p) {
//     // draw the bg
//     // sky blue color
//     SDL_SetRenderDrawColorFloat(renderer, KTeal.r, KTeal.g, KTeal.b, KTeal.a);
//     SDL_RenderClear(renderer);
//     // end bg
//
//     // draw the map
//     // this will be used by proper enums for color accoridng to the number on map
//     Vec4i w = GetColor(0); // all black for now
//     float currentX = 0.0f;
//     float currentY = 0.0f;
//     SDL_FRect drawRect{};
//     // draw the map
//     for (int x = 0; x < KMapHeight; ++x) {
//         for (int y = 0; y < KMapWidth; ++y) {
//             if (KMap[x][y] == 0) {
//                 // empty spaces drawn idk which color this is, randomly typed the nums data
//                 w = GetColor(1);
//                 SDL_SetRenderDrawColor(renderer, w.r, w.g, w.b, w.a);
//             } else {
//                 // walls are drawn black
//                 w = GetColor(0);
//                 SDL_SetRenderDrawColor(renderer, w.r, w.g, w.b, w.a);
//             }
//             drawRect = {currentX, currentY, KDrawBoxWidth, KDrawBoxHeight};
//             // fixme: not drawing correct ratio of window size to map size
//             // drawRect = {currentX, currentY, DRAW_BOX_WD_G, DRAW_BOX_WD_G};
//             //
//             SDL_RenderFillRect(renderer, &drawRect);
//             // push x one box ahead
//             currentX += KDrawBoxWidth;
//         }
//
//         currentY += (float)KDrawBoxHeight;
//         currentX = 0.0f;
//     }
//     // done map
//
//     // draw the player rect !Debug
//     SDL_SetRenderDrawColor(renderer, p.r, p.g, p.b, p.a);
//     SDL_RenderFillRect(renderer, p.GetPlayerFRect());
//     // done player rect
//
//     // draw a line at the players current angle
//
//     // start at player.x and y
//     // end at a distance 300px with angle calculated distance
//
//     SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//     // fist
//     float lineLegth = 5000.0f;
//     float lengthStep = 0.1f;
//
//     float firstAngleRad = p.m_lookAngleRad - p.m_FOVBy2Rad;
//     float lastAngleRad = p.m_lookAngleRad + p.m_FOVBy2Rad;
//
//     Vec2f direction;
//     Vec2f end;
//     float angleStep = 1.0f * DEG_TO_RAD;
//     Vec2f start = p.Posf();
//
//     std::cout << "First angle: " << firstAngleRad;
//     std::cout << "\nlast angle: " << lastAngleRad << std::endl;
//     for (float angle = firstAngleRad; angle <= lastAngleRad; angle += angleStep) {
//         direction = Vec2f(std::cos(angle), std::sin(angle));
//         float dist = 0.1f;
//         for (dist = 0.1; dist < lineLegth; dist += lengthStep) {
//             // get the vec2f pos of the dot
//             Vec2f checkPoint = start + direction * dist;
//             // convert to map space
//             checkPoint *= Vec2f(KMapSpaceStepRatioX, KMapSpaceStepRatioY);
//             int row = checkPoint.y;
//             int col = checkPoint.x;
//             // check if this point has a wall (is > 0)
//             if (KMap[row][col] > 0) {
//                 break;
//             }
//         }
//         end = start + direction * dist;
//         SDL_RenderLine(renderer, start.x, start.y, end.x, end.y);
//     }
//
//     SDL_RenderPresent(renderer);
// }
