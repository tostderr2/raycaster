

#include <cmath>
#include <iomanip>
#include <iostream>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_stdinc.h>
// #include <SDL3/SDL_surface.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>

#include "constants.h"
#include "player.h"
#include "raycaster/raycaster.h"

using namespace rc;
// can be used for minimap
void render(SDL_Renderer *renderer, Player &player);

// uses reder geometric and render line which use gpu, hardware based drawing
void renderRaycasted(Player &p);

// uses software based (cpu) calls for filling the buffer and then sending it to
// gpu. making it closer to the original software raycasting renderer
void raycasterFillBuffer(Uint32 *pixleData, Player &player);

void processInput(SDL_Event *event, Player &player, float dt);
Uint32 packColor(Color color);
bool running = true;
SDL_Renderer *renderer{nullptr};
bool vSync = VSYNC_DEFAULT;

int called = 0;

Color wallColor(int wall) {
    Color color;
    switch (wall) {
    case Empty_ID:
        color = KEmpty; // FIXME: alpha is 0 still this renders as solid white
        break;
    case StoneWall_ID:
        color = KStoneWall;
        break;
    case RedWall_ID:
        color = KMaroonWall;
        break;
    case IceBlue_ID:
        color = KIceBlueWall;
        break;
    case Purple_ID:
        color = KPurple;
        break;
    case Green_ID:
        color = KGreen;
        break;
    default:
        color = KDefaultWhite;
        break;
    }

    return color;
}

void raycasterFillBuffer(Uint32 *pixleData, Player &p) {

    static rc::RcMap rcMap{reinterpret_cast<const int *>(KMap), KMapWidth, KMapHeight};
    static std::vector<RcHit> outHits(KWinWidth);

    rc::castFOV(p.m_lookAngle, p.m_FOV, p.m_pos.x, p.m_pos.y, rcMap, outHits.data(), KWinWidth,
                called);

    for (size_t col = 0; col < KWinWidth; ++col) {
        // dda start

        RcHit rayHit = outHits[col];
        float wallHt = KWinHeight / rayHit.perpDist;
        float wallStart = (KWinHeight - wallHt) / 2.0f;
        float wallEnd = wallStart + wallHt;

        bool hitWall = (rayHit.hitX >= 0 && rayHit.hitX < KMapWidth && rayHit.hitY >= 0 &&
                        rayHit.hitY < KMapHeight && KMap[rayHit.hitY][rayHit.hitX] != 0);

        // dda end
        for (size_t row = 0; row < KWinHeight; ++row) {
            Uint32 pixelColor;
            // for this pixle, fill the color
            if (row < wallStart) {
                // todo: have to make the sunset looking glow later
                // paint blue
                pixelColor = packColor(KTeal);
            } else if (row < wallEnd) {
                int valueInsideGrid = hitWall ? KMap[rayHit.hitY][rayHit.hitX] : StoneWall_ID;
                pixelColor = packColor(wallColor(valueInsideGrid));
            } else {
                // draw the ground
                pixelColor = packColor(KMudFloor);
            }
            // std::cout << "color is: " << pixelColor;

            pixleData[row * KWinWidth + col] = pixelColor;
        }
    }
}

std::ostream &operator<<(std::ostream &os, const Color &color) {
    os << "RGBA(" << static_cast<int>(color.r) << ", " << static_cast<int>(color.g) << ", "
       << static_cast<int>(color.b) << ", " << static_cast<int>(color.a) << ")";
    return os;
}

int main(int /* argc */, char ** /* argv*/) {
    // init and setup basic window and renderer
    /* We will use this renderer to draw into this window every frame. */
    SDL_Window *window{nullptr};
    SDL_Texture *screenTex{nullptr};
    std::vector<Uint32> pixelBuffer(KWinWidth * KWinHeight);

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

    screenTex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
                                  KWinWidth, KWinHeight);
    if (!screenTex) {
        SDL_Log("Couldn't create streaming texture: %s\n", SDL_GetError());
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

    // winSurface = SDL_GetWindowSurface(window);
    // if (!winSurface) {
    //     SDL_Log("Couldn't get window surface: %s\n", SDL_GetError());
    //     return -1;
    // }
    SDL_SetRenderVSync(renderer, vSync);
    // init done

    Player player;
    SDL_Event event;

    // performance metrics
    Uint64 frameCount = 0;
    float fpsTimer = 0.0f;
    float fps = 0.0f;

    // Other than dt(seconds) everything is in miliseconds
    // constexpr Uint32 fixedFrameSpeed = 8;
    Uint64 prevTime = SDL_GetTicks();

    while (running) {
        Uint64 currentTime = SDL_GetTicks();
        Uint64 frameTimeMs = currentTime - prevTime;

        // testing naive frame rate
        // note: relaxing of cpu turned off
        // if (frameTimeMs < fixedFrameSpeed) {
        //     SDL_Delay(fixedFrameSpeed - static_cast<Uint32>(frameTimeMs));
        //     // gotta reset after the sleep
        //     currentTime = SDL_GetTicks();
        //     frameTimeMs = currentTime - prevTime;
        // }

        prevTime = currentTime;
        float dt = static_cast<float>(frameTimeMs) / 1000.0f;

        // perf:
        frameCount++;
        fpsTimer += dt;
        if (fpsTimer >= 1.0f) {
            fps = static_cast<float>(frameCount) / fpsTimer;
            frameCount = 0;
            fpsTimer -= 1.0f;
            // push the fps and vsync state to the window title instead of std out
            std::string vSyncState = "VSync on";
            if (!vSync) {
                vSyncState = "VSync off";
            }

            float frameTime = 1000.0f / fps;

            std::ostringstream titleStream;
            titleStream << "FPS: " << std::fixed << std::setprecision(1) << fps
                        << " | frame time: " << std::setprecision(2) << frameTime << "ms "
                        << vSyncState;
            std::string title = titleStream.str();

            SDL_SetWindowTitle(window, title.c_str());
        }
        // perf end

        processInput(&event, player, dt);

        // renders the 2d top down view
        // render(renderer, player);
        // renderRaycasted(player);

        // pure software renderer
        raycasterFillBuffer(pixelBuffer.data(), player);
        SDL_UpdateTexture(screenTex, nullptr, pixelBuffer.data(), KWinWidth * sizeof(Uint32));
        SDL_RenderTexture(renderer, screenTex, nullptr, nullptr);
        SDL_RenderPresent(renderer);
        // pure software renderer end
    }
    return 0;
}

Color GetColor(int num) {
    Color wall;
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
    default:
        wall = {0, 0, 0, 255};
        break;
    }
    return wall;
}

Uint32 packColor(Color color) {
    return static_cast<Uint32>(color.r) << 24   // last 8 bits
           | static_cast<Uint32>(color.g) << 16 // next 8
           | static_cast<Uint32>(color.b) << 8  // next
           | static_cast<Uint32>(color.a);      // first 8
}
//
// void renderRaycasted(Player &p) {
//
//     float midY = KWinHeight / 2.0f;
//     SDL_Vertex vertices[4] = {// Top-Left (Orange)
//                               {.position = {0.0f, 0.0f}, .color = KTealF},
//                               // Top-Right (Orange)
//                               {.position = {KWinWidth, 0.0f}, .color = KTealF},
//                               // Mid-Left (Teal)
//                               {.position = {0.0f, midY}, .color = KOrange},
//                               // Mid-Right (Teal)
//                               {.position = {KWinWidth, midY}, .color = KOrange}};
//
//     int indices[6] = {0, 1, 2, 1, 3, 2};
//
//     // draw the bg
//     SDL_SetRenderDrawColorFloat(renderer, KOrange.r, KOrange.g, KOrange.b, KOrange.a);
//     SDL_RenderClear(renderer);
//     // end of bg
//     SDL_RenderGeometry(renderer, NULL, vertices, 4, indices, 6);
//
//     float angleStep = p.m_FOVRad / KWinWidth;
//
//     Color color;
//     /*
//      * DDA concept
//      * get the distance the ray travels for 1 unit dist in x and same in y
//      * for grid to other grid, when x and y jump is one, where will the ray head reach
//      * ie: distX = 1(x unit) / cos(ray angle)
//      * distY = 1(y unit) / sin(ray angle)
//      *
//      *
//      */
//     for (size_t col = 0; col < KWinWidth - 1; ++col) {
//         float rayStep = 0.01f;
//         float dist = 0.0;
//
//         // cast the ray ath the ray angle and get the distance from player to the wall
//         float rayAngle = p.m_lookAngleRad - p.m_FOVBy2Rad + (static_cast<float>(col) *
//         angleStep);
//         Vec2f direction = Vec2f(cosf(rayAngle), sinf(rayAngle)); bool stop = false;
//         while (!stop) {
//             int stepX = static_cast<int>(p.Posf().x + direction.x * dist);
//             int stepY = static_cast<int>(p.Posf().y + direction.y * dist);
//             if (stepX < 0 || stepX >= KMapWidth || stepY < 0 || stepY >= KMapHeight) {
//                 stop = true;
//                 break;
//             }
//             if (KMap[stepY][stepX] != 0) {
//                 switch (KMap[stepY][stepX]) {
//                 case Empty_ID:
//                     color = KEmpty; // FIXME: alpha is 0 still this renders as solid white
//                     break;
//                 case StoneWall_ID:
//                     color = KStoneWall;
//                     break;
//                 case RedWall_ID:
//                     color = KMaroonWall;
//                     break;
//                 case IceBlue_ID:
//                     color = KIceBlueWall;
//                     break;
//                 case Purple_ID:
//                     color = KPurple;
//                     break;
//                 case Green_ID:
//                     color = KGreen;
//                     break;
//                 default:
//                     color = KDefaultWhite;
//                     break;
//                 }
//
//                 stop = true;
//             }
//             dist += rayStep;
//         }
//         // ray dist done
//         // raw dist will be longer at the edges (-fov/2 and +fov/2)
//         // so using perpendicular dist from the players y pos is much better visually
//         // removes the fish eye effect (where the view plane looks a bit like a globe)
//         float perpDist = dist * cosf(rayAngle - p.m_lookAngleRad);
//
//         // draw the map for this one column of window with the dist value
//         // get the wall ht
//         // will be put into if else block later with more accurate dist
//         // to ht representation
//         if (perpDist < 0.1f) {
//             perpDist = 0.1f;
//         }
//
//         float screenCentre = KWinHeight / 2.0f;
//         float wallHt = KWinHeight / perpDist;
//         float wallStart = screenCentre - wallHt / 2.0f;
//         float wallEnd = screenCentre + wallHt / 2.0f;
//
//         // bg
//         // SDL_SetRenderDrawColor(renderer, KTeal.r, KTeal.g, KTeal.b, KTeal.a);
//         // SDL_RenderLine(renderer, static_cast<float>(col), wallStart, static_cast<float>(col),
//         //                KWinHeight);
//
//         // walls
//         SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
//         SDL_RenderLine(renderer, static_cast<float>(col), wallStart, static_cast<float>(col),
//                        wallEnd);
//
//         // floor
//         color = KMudFloor;
//         SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
//         SDL_RenderLine(renderer, static_cast<float>(col), wallEnd, static_cast<float>(col),
//                        KWinHeight);
//
//         // bg is blue with sunset shade, so sky is already drawn
//     }
//
//     SDL_RenderPresent(renderer);
// }

void processInput(SDL_Event *event, Player &player, float dt) {
    while (SDL_PollEvent(event)) {
        if (event->type == SDL_EVENT_QUIT) {
            running = false;
        }

        if (event->type == SDL_EVENT_KEY_DOWN) {
            if (event->key.key == SDLK_ESCAPE) {
                running = false;
            } else if (event->key.key == SDLK_P) {
                vSync = !vSync;
                SDL_SetRenderVSync(renderer, vSync);
            }
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
        called += 2;
    }
    if (keypressed[SDL_SCANCODE_LEFT]) {
        player.TurnLeft(dt);
    }
    if (keypressed[SDL_SCANCODE_SPACE]) {
        player.Shoot(dt);
    }
}

// will someday clean up and remove globals so just keeping this a bit cleaner. maybe just using
// template <size_t Rows, size_t Cols>
// // this should return some data, dist of the wall hit, if hit
// void castRay(Vec2f startPt, float viewAngle, const int (&map)[Rows][Cols]) {
// }

// todo: player pos is changed from win related to map related. change the impl here
// this can be used for mini map maybe
// void render(SDL_Renderer *renderer, Player &p) {
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
//         currentY += static_cast<float>(KDrawBoxHeight);
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
