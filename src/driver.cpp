#include "stickmanconfig.h"
#include "collision.h"
#include "float2.h"
#include "player.h"
#include "terrain.h"
#include "utils.h"

#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 750

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;
static int texture_width = 0;
static int texture_height = 0;

Player p(texture, Float2(0,0));
Terrain f(Float2(100,0), Float2(-100,-100));

static Uint64 lastFrameTime = 0;

// Init
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  SDL_Surface *surface = NULL;
  char* path = NULL;

  SDL_SetAppMetadata("Stickman", 
                     STICKMAN_VERSION_STRING, 
                     "com.stickman");

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if (!SDL_CreateWindowAndRenderer("Stickman", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_INPUT_FOCUS, &window, &renderer)) {
    SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  /* Textures are pixel data that we upload to the video hardware for fast drawing. Lots of 2D
      engines refer to these as "sprites." We'll do a static texture (upload once, draw many
      times) with data from a bitmap file. */

  /* SDL_Surface is pixel data the CPU can access. SDL_Texture is pixel data the GPU can access.
      Load a .bmp into a surface, move it to a texture from there. */
  SDL_asprintf(&path, "../Sprites/stickman.bmp");  /* allocate a string of the full file path */
  surface = SDL_LoadBMP(path);
  if (!surface) {
      SDL_Log("Couldn't load bitmap: %s", SDL_GetError());
      return SDL_APP_FAILURE;
  }
  SDL_free(path);  /* done with this, the file is loaded. */

  texture_width = surface->w;
  texture_height = surface->h;

  texture = SDL_CreateTextureFromSurface(renderer, surface);
  if (!texture) {
      SDL_Log("Couldn't create static texture: %s", SDL_GetError());
      return SDL_APP_FAILURE;
  }

  SDL_DestroySurface(surface);  /* done with this, the texture has a copy of the pixels now. */

  p = Player(texture, Float2(0,0));
  p.SetOnGround(true);
  return SDL_APP_CONTINUE;
}

// Event Handler
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  if (event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS;  // Exit gracefully
  }
  return SDL_APP_CONTINUE;
}

// Runs once each frame
SDL_AppResult SDL_AppIterate(void *appstate) {
  SDL_FRect dst_rect;
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);

  Uint64 currentTime = SDL_GetTicks();
  float deltaTime = (currentTime - lastFrameTime) / 1000.0f; // Convert to seconds
  lastFrameTime = currentTime;
  
  const bool *keystate = SDL_GetKeyboardState(NULL);
  

  p.SetMovingLeft(keystate[SDL_SCANCODE_LEFT]);
  p.SetMovingRight(keystate[SDL_SCANCODE_RIGHT]);
  if (keystate[SDL_SCANCODE_Z]) {
    p.Jump();
  }

  p.Update(deltaTime);
  Collision collision = f.Collision(p);

  switch (collision) {
    case Collision::NONE:
      p.SetOnGround(false);
      break;
    case Collision::GROUND:
      p.SetBottom(f.GetTop());
      p.SetVelocityY(0);
      p.SetOnGround(true);
      break;
    case Collision::CEILING:
      p.SetTop(f.GetBottom());
      p.SetVelocityY(0);
      p.SetOnGround(false);
      break;
    case Collision::LEFTWALL:
      p.SetLeft(f.GetRight());
      p.SetVelocityX(0);
      break;
    case Collision::RIGHTWALL:
      p.SetRight(f.GetLeft());
      p.SetVelocityX(0);
      break;
  }

  if (p.GetPosition().y > 340 || p.GetPosition().y < -340) {
    p.GetPosition() = Float2(0,0);
  }

  SDL_FRect floorRect = SDL_FRect{ WorldToScreen(f.GetMin(), Float2(0,0), WINDOW_WIDTH, WINDOW_HEIGHT).x, 
                                   WorldToScreen(f.GetMax(), Float2(0,0), WINDOW_WIDTH, WINDOW_HEIGHT).y, 
                                   f.GetWidth(), 
                                   f.GetHeight() };

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderFillRect(renderer, &floorRect);

  Float2 point = WorldToScreen(p.GetPosition(), Float2(0,0), WINDOW_WIDTH, WINDOW_HEIGHT);
  dst_rect.x = point.x;
  dst_rect.y = point.y - texture_height;
  dst_rect.w = (float) texture_width;
  dst_rect.h = (float) texture_height;
  SDL_RenderTexture(renderer, p.GetTexture(), NULL, &dst_rect);
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderRect(renderer, &dst_rect);
  
  SDL_RenderPresent(renderer);  /* put it all on the screen! */
  return SDL_APP_CONTINUE;  /* carry on with the program! */
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
  /* SDL will clean up the window/renderer for us. */
}
