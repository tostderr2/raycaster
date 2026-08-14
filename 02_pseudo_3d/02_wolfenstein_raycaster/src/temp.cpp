/*
*
* this way i can draw the bg that looks like a sky with a setting sun
* need a bit more working, currenlty bottom half it teal(blue)
* upper hald is orange.
* need to lerp from orange to teal as it goes up, as sun is setting and 
* around that its orange and the sky above slowly looks more and more blue
*
* then theres also place below sun where it might become visible form our view
* which shoudlnt be blue, maybe we add an ocean or something
* for later
*
*/
//
// #include <SDL3/SDL_pixels.h>
// #include <SDL3/SDL_render.h>
//
// SDL_FColor teal = { 145.0f / 255.0f, 217.0f / 255.0f, 214.0f / 255.0f, 1.0f };
// SDL_FColor orange = { 181.0f / 255.0f, 93.0f / 255.0f, 11.0f / 255.0f, 1.0f };
//
// float w = 800.0f; 
// float h = 600.0f;
// float mid_y = h * 0.5f;
//
// SDL_Vertex vertices[4] = {
//     // Top-Left (Orange)
//     { .position = { 0.0f, 0.0f },  .color = orange },
//     // Top-Right (Orange)
//     { .position = { w,    0.0f },  .color = orange },
//     // Mid-Left (Teal)
//     { .position = { 0.0f, mid_y }, .color = teal },
//     // Mid-Right (Teal)
//     { .position = { w,    mid_y }, .color = teal }
// };
//
// int indices[6] = { 0, 1, 2, 1, 3, 2 };
//
// SDL_SetRenderDrawColorFloat(renderer, teal.r, teal.g, teal.b, teal.a);
// SDL_RenderClear(renderer); // Fills the whole screen with teal first
//
// SDL_RenderGeometry(renderer, NULL, vertices, 4, indices, 6);
