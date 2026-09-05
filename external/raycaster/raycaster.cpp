#include "raycaster.h"

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

    // draw the map for this one column of window with the dist value
    // get the wall ht
    // will be put into if else block later with more accurate dist
    // to ht representation
    if (perpDist < 0.1f) {
        perpDist = 0.1f;
    }
    // ends here

    return RcHit{perpDist, stepX, stepY, map.cells[map.width * stepY + stepX]};
}

void castFOV(float lookAngle, float fovRadians, float originX, float originY, const RcMap &map,
             RcHit *outHits, int numColumn) {
    // at fov start to end cast ray

    static float angleStep = fovRadians / static_cast<float>(numColumn);
    static float fovRadiansBy2 = fovRadians / 2.0f;

    for (int col = 0; col < numColumn; ++col) {
        float rayAngle = lookAngle - fovRadiansBy2 + (static_cast<float>(col) * angleStep);

        // RcHit castRay(float lookAngle, float rayAngle, float originX,float originY, RcMap &map) {
		outHits[col] = castRay(lookAngle, rayAngle, originX, originY, map);
        // outHits[col] = castRay(lookAngle, rayAngle, originX, originY, map);
    }
}

// utility

} // namespace rc
