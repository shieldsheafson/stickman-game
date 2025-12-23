#include "stickmanconfig.h"
#include "collision.h"
#include "float2.h"
#include "game.h"
#include "level.h"
#include "box.h"
#include "inputs.h"
#include "gamemode.h"
#include "recorder.h"

#define SDL_MAIN_USE_CALLBACKS 1
#include <fstream>
#include <filesystem>
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
static Recorder recorder;

static Uint64 firstFrameTime = 0;
static Uint64 lastFrameTime = 0;
static float frames = 0;

static bool paused = false;
static float pausedTime = 0.0;
char* recordingFilename = NULL;

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

  GameMode mode = GameMode::NORMAL;
  
  // Parse command line arguments
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-record") == 0) {
      mode = GameMode::RECORD;
      if (i + 1 < argc) {
        recordingFilename = argv[i + 1];
        i++; // Skip next argument
      } else {
        fprintf(stderr, "Error: -record requires a filename\n");
        return SDL_APP_FAILURE;
      }
    }
    else if (strcmp(argv[i], "-replay") == 0) {
      mode = GameMode::REPLAY;
      if (i + 1 < argc) {
        recordingFilename = argv[i + 1];
        i++;
      } else {
        fprintf(stderr, "Error: -replay requires a filename\n");
        return SDL_APP_FAILURE;
      }
    }
  }

  recorder.mMode = mode;
  if (mode == GameMode::REPLAY) {
    std::ifstream ifs(recordingFilename);
    ifs >> recorder;
  }

  texture_width = surface->w;
  texture_height = surface->h;

  texture = SDL_CreateTextureFromSurface(renderer, surface);
  if (!texture) {
      SDL_Log("Couldn't create static texture: %s", SDL_GetError());
      return SDL_APP_FAILURE;
  }

  SDL_DestroySurface(surface);  /* done with this, the texture has a copy of the pixels now. */

  Level level;
  std::ifstream ifs("../Levels/level1.txt");
  ifs >> level;
  std::vector<Level> levels;
  levels.push_back(level);
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
  if (event->type == SDL_EVENT_KEY_DOWN) {
    if (event->key.scancode == SDL_SCANCODE_SPACE) {
      paused = !paused;
    }
  }
  return SDL_APP_CONTINUE;
}

// Runs once each frame
SDL_AppResult SDL_AppIterate(void *appstate) {
  if (paused) {
    Uint64 currentTime = SDL_GetTicks();
    float deltaTime = (currentTime - lastFrameTime) / 1000.0f; // Convert to seconds
    lastFrameTime = currentTime;
    pausedTime += deltaTime;
    return SDL_APP_CONTINUE;
  }

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);

  Uint64 currentTime = SDL_GetTicks();
  float deltaTime = (currentTime - lastFrameTime) / 1000.0f; // Convert to seconds
  lastFrameTime = currentTime;
  if (frames == 0) {
    firstFrameTime = lastFrameTime;
  }


  const bool *keystate = SDL_GetKeyboardState(NULL);
  Inputs inputs(keystate);
  if (recorder.mMode == GameMode::REPLAY) {
    if (recorder.mCurrentIndex >= recorder.mFrames.size()) {
      return SDL_APP_SUCCESS;
    }
    inputs = recorder.mFrames.at(recorder.mCurrentIndex).mInputs;
    recorder.mCurrentIndex++;
  } else if (recorder.mMode == GameMode::RECORD) {
    recorder.mFrames.push_back(DebugFrame {inputs, deltaTime });
  }
  game->Update(inputs, deltaTime);
  game->Render(renderer);
  frames++;
  
  return SDL_APP_CONTINUE;  /* carry on with the program! */
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
  // float totalSeconds = (lastFrameTime - firstFrameTime) / 1000.0f;
  // float avgFPS = frames / totalSeconds;
  // std::cout << "Frames: " << frames << std::endl;
  // std::cout << "Average FPS: " << avgFPS << " *currently broken if you pause" << std::endl;

  if (recorder.mMode == GameMode::RECORD) {
    std::ofstream ofs(recordingFilename);
    ofs << recorder;
  }

  if (texture) {
    SDL_DestroyTexture(texture);
  }
}
