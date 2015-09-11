#ifndef _HEAD_SDLBASE_
#define _HEAD_SDLBASE_
#include "OS.h"
#ifdef _PLATFORM_C4DROID
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_test.h>
#endif // _OS_C4DROID_

#ifdef _PLATFORM_WINDOWS

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_net.h>
#include <SDL_test.h>
#endif

#endif // _HEAD_SDLBASE_
