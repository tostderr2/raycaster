#include "raycaster.h"

#include <iostream>
#include <ostream>

#include "types.h"

namespace rc {

RcHit castRay(float lookAngle, float rayAngle, float originX, float originY, const RcMap &map) {

    int stepX = 0;
    int stepY = 0;
    float dist = 0.0f;
    constexpr float rayStep = 0.01f;

    Vec2f direction = Vec2f(cosf(rayAngle), sinf(rayAngle));
    bool stop = false;
    while (!stop) {
        stepX = static_cast<int>(originX + direction.x * dist);
        stepY = static_cast<int>(originY + direction.y * dist);
        if (stepX < 0 || stepX >= map.width || stepY < 0 || stepY >= map.height) {
            stop = true;
            break;
        }
        if (map.cells[map.width * stepY + stepX] != 0) {
            stop = true;
        }
        dist += rayStep;
    }
    // ray dist done
    // raw dist will be longer at the edges (-fov/2 and +fov/2)
    // so using perpendicular dist from the players y pos is much better visually
    // removes the fish eye effect (where the view plane looks a bit like a globe)
    float perpDist = dist * cosf(rayAngle - lookAngle);

    if (perpDist < 0.1f) {
        perpDist = 0.1f;
    }
    // ends here

    return RcHit{perpDist, stepX, stepY, map.cells[map.width * stepY + stepX]};
}

void castFOV(float lookAngle, float fov, float originX, float originY, const RcMap &map,
             RcHit *outHits, int numColumn, int &called) {
    // at fov start to end cast ray

    static float angleStep = fov / static_cast<float>(numColumn);
     float FOVBy2 = fov / 2.0f;

    if (called == 0) {
        std::cout << "angle step: " << angleStep << " ,fov by 2: " << FOVBy2 << std::endl;
        std::cout << "in main\n" << called << '\n';
        called--;
    } else if (called == 1) {
        std::cout << "angle step: " << angleStep << " ,fov by 2: " << FOVBy2 << std::endl;
        std::cout << "in loop\n" << called << '\n';
        called++;
    }

    for (int col = 0; col < numColumn; ++col) {
        float rayAngle = lookAngle - FOVBy2 + (static_cast<float>(col) * angleStep);

        outHits[col] = castRay(lookAngle, rayAngle, originX, originY, map);
    }
}

// utility

} // namespace rc
