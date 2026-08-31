/*
 * info:
 * problems that we have rn in this phase of commit
 * 1 Fish-eye lens distortion from raw Euclidean distance
 * 2 Computational inefficiency and spatial aliasing in fixed-step ray marching
 * 3 Lack of surface orientation and depth perception (flat, unshaded walls)
 * 4 Absence of wall texture mapping (affine texture coordinate projection)
 * 5 Untextured floor and ceiling surfaces (lack of horizontal scanline projection)
 * 6 Inconsistent frame timing and input stutter from brittle fixed-delay loop polling
 *
 */

