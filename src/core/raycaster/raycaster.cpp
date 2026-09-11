#include "raycaster.h"

#include <algorithm>
#include <cmath>

#include "types.h"

namespace rc {

RcHit castRay(float lookAngle, float rayAngle, float originX, float originY, const RcMap &map) {
    Vec2f rayDir = Vec2f(cosf(rayAngle), sinf(rayAngle));

    // current grid cell position of the ray origin
    Vec2i mapPos = Vec2i(static_cast<int>(originX), static_cast<int>(originY));

    // which direction to step in map
    Vec2i stepDir = {
        (rayDir.x < 0.0f) ? -1 : 1,
        (rayDir.y < 0.0f) ? -1 : 1,
    };

    // unit vector triangle vs a triangle of the actual distance ray has to travel
    // both are drawn on top so same angle
    // serach what is a "big number" in cpp. it was a exponent
    Vec2f rayUnitStepSize = {(rayDir.x == 0.0f) ? 999999 : std::abs(1.0f / rayDir.x),
                             (rayDir.y == 0.00f) ? 999999 : std::abs(1.0f / rayDir.y)};

    // initial distance to nearest  grid boundary
    Vec2f sideDist;

    if (rayDir.x > 0.0f) {
        sideDist.x = (static_cast<float>(mapPos.x) + 1 - originX) * rayUnitStepSize.x;
    } else {
        sideDist.x = (originX - static_cast<float>(mapPos.x)) * rayUnitStepSize.x;
    }
    if (rayDir.y > 0.0f) {
        sideDist.y = (static_cast<float>(mapPos.y) + 1 - originY) * rayUnitStepSize.y;
    } else {
        sideDist.y = (originY - static_cast<float>(mapPos.y)) * rayUnitStepSize.y;
    }

    // DDA stepping
    bool hit = false;
    // 1 for x 0 for y
    int side = 0;

    while (!hit) {
        // step to the next grid cell along the shorter axis distance
        if (sideDist.x < sideDist.y) {
            sideDist.x += rayUnitStepSize.x;
            mapPos.x += stepDir.x;
            side = 1;
        } else {
            sideDist.y += rayUnitStepSize.y;
            mapPos.y += stepDir.y;
            side = 0;
        }
        // boundary check
        if (mapPos.x < 0 || mapPos.x >= map.width || mapPos.y < 0 || mapPos.y >= map.height) {
            break;
        }
        // collision
        if (map.cells[mapPos.y * map.width + mapPos.x] != 0) {
            hit = true;
        }
    }

    // fisheye effect
    float perpWallDist = 0.0f;
    int hitCellType = 0;

    if (hit) {
        // which wall was hit
        hitCellType = map.cells[mapPos.y * map.width + mapPos.x];
        // get per dist
        if (side) {
            perpWallDist = sideDist.x - rayUnitStepSize.x;
        } else {
            perpWallDist = sideDist.y - rayUnitStepSize.y;
        }
        perpWallDist *= cosf(rayAngle - lookAngle);
    }

    // fixme:
    // what to do if hit is false, what shall be the perpwalldist. rn map is closed so it doesnt
    // matter, but later, if opened up, it will cause kMapwidth/0 stuff
    return RcHit{perpWallDist, mapPos.x, mapPos.y, hitCellType, hit, side};
}

void castFOV(float lookAngle, float fov, float originX, float originY, const RcMap &map,
             RcHit *outHits, int numColumn) {
    // at fov start to end cast ray
    float angleStep = fov / static_cast<float>(numColumn);
    float FOVBy2 = fov / 2.0f;

    for (int col = 0; col < numColumn; ++col) {
        float rayAngle = lookAngle - FOVBy2 + (static_cast<float>(col) * angleStep);

        outHits[col] = castRay(lookAngle, rayAngle, originX, originY, map);
    }
}

// utility

} // namespace rc
