#pragma once

#include "types.h"

namespace rc {

// RcHit castRay(float lookAngle, float originX,float originY, RcMap &map);

void castFOV(float lookAngle, float fovRadians, float originX, float originY, const RcMap &map, RcHit *outHits, int numColumn, int &called);

} // rc
