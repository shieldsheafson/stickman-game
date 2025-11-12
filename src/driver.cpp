#include "stickmanconfig.h"
#include "collision.h"
#include "float2.h"
#include "game.h"
#include "level.h"
#include "player.h"
#include "terrain.h"
#include "utils.h"

#define SDL_MAIN_USE_CALLBACKS 1
#include <fstream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 750

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;
static int texture_width = 0;
static int texture_height = 0;

static std::optional<Game> game;

static Uint64 firstFrameTime = 0;
static Uint64 lastFrameTime = 0;
static float frames = 0;

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

  // Terrain a(Float2(0, 400), 100, 100);
  // Terrain b(Float2(600, 100), 100, 200);
  // Terrain c(Float2(300, 400), 200, 100);
  // Terrain d(Float2(700, 700), 100, 200);
  // Terrain e(Float2(400, 400), 200, 100);
  // Terrain f(Float2(500, 250), 50, 50);
  // Terrain g(Float2(800, 500), 60, 60);
  // Terrain h(Float2(850, 300), 100, 100);

  // std::vector<Terrain> terrain = {a, b, c, d, e, f, g, h};
  // terrain.push_back(a);
  // Level level = Level(terrain);
  Level level;
  std::ifstream ifs("../Levels/level1.txt");
  ifs >> level;
  std::vector<Level> levels;
  levels.push_back(level);
  Player p = Player(texture, Float2(0,0), 50, 100);
  p.SetOnGround(true);
  game.emplace(texture, levels, WINDOW_WIDTH, WINDOW_HEIGHT);
  lastFrameTime = SDL_GetTicks();
  SDL_SetRenderVSync(renderer, 1); // prevent screen tearing
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
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);

  Uint64 currentTime = SDL_GetTicks();
  float deltaTime = (currentTime - lastFrameTime) / 1000.0f; // Convert to seconds
  lastFrameTime = currentTime;
  if (frames == 0) {
    firstFrameTime = lastFrameTime;
  }
  
  const bool *keystate = SDL_GetKeyboardState(NULL);

  game->Update(keystate, deltaTime);
  game->Render(renderer);
  frames++;
  
  return SDL_APP_CONTINUE;  /* carry on with the program! */
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
  float totalSeconds = (lastFrameTime - firstFrameTime) / 1000.0f;
  float avgFPS = frames / totalSeconds;
  std::cout << "Frames: " << frames << std::endl;
  std::cout << "Average FPS: " << avgFPS << std::endl;
  if (texture) {
    SDL_DestroyTexture(texture);
  }
}
