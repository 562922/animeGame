#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include <cmath>
#include "../logic/cpp/GameLogic.h"

int main(int argc, char **argv) {
  std::string assets = "game/assets";
  GameLogic logic(assets);
  if (!logic.loadAssets()) {
    std::cerr << "Failed to load assets" << std::endl;
    return 1;
  }

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cerr << "SDL Init Error: " << SDL_GetError() << std::endl;
    return 1;
  }
  SDL_Window *win = SDL_CreateWindow("AnimeGame Beta - C++ Renderer", 100, 100, 1024, 768, SDL_WINDOW_SHOWN);
  if (!win) { 
    std::cerr << "SDL CreateWindow Error: " << SDL_GetError() << std::endl; 
    SDL_Quit(); 
    return 1; 
  }
  SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!ren) { 
    SDL_DestroyWindow(win); 
    SDL_Quit(); 
    return 1; 
  }

  bool running = true;
  Uint32 last = SDL_GetTicks();
  int frame = 0;
  while (running) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) running = false;
      if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) running = false;
    }
    Uint32 now = SDL_GetTicks();
    float dt = (now - last) / 1000.0f;
    last = now;

    logic.update(dt);
    auto p = logic.getPlayer();
    auto en = logic.getEnemy();
    auto npcs = logic.getNPCs();

    // Clear background
    SDL_SetRenderDrawColor(ren, 30, 30, 40, 255);
    SDL_RenderClear(ren);

    // Draw grid
    SDL_SetRenderDrawColor(ren, 80, 80, 100, 100);
    for (int i = 0; i < 1024; i += 64) {
      SDL_RenderDrawLine(ren, i, 0, i, 768);
    }
    for (int j = 0; j < 768; j += 64) {
      SDL_RenderDrawLine(ren, 0, j, 1024, j);
    }

    // Draw player (blue rectangle)
    SDL_Rect pr = { (int)p.X - 16, (int)p.Y - 24, 32, 48 };
    SDL_SetRenderDrawColor(ren, 0, 120, 255, 255);
    SDL_RenderFillRect(ren, &pr);
    SDL_SetRenderDrawColor(ren, 200, 200, 255, 255);
    SDL_RenderDrawRect(ren, &pr);

    // Draw enemy (red rectangle)
    SDL_Rect er = { (int)en.X - 18, (int)en.Y - 26, 36, 52 };
    SDL_SetRenderDrawColor(ren, 200, 40, 40, 255);
    SDL_RenderFillRect(ren, &er);
    SDL_SetRenderDrawColor(ren, 255, 150, 150, 255);
    SDL_RenderDrawRect(ren, &er);

    // Draw line between player and enemy
    SDL_SetRenderDrawColor(ren, 255, 255, 0, 128);
    SDL_RenderDrawLine(ren, (int)p.X, (int)p.Y, (int)en.X, (int)en.Y);

    // Draw NPCs (green)
    SDL_SetRenderDrawColor(ren, 40, 200, 40, 255);
    for (const auto &n : npcs) {
      SDL_Rect nr = { (int)n.X - 12, (int)n.Y - 16, 24, 32 };
      SDL_RenderFillRect(ren, &nr);
    }

    // Draw distance text info (simple placement)
    float dist = std::sqrt((p.X - en.X) * (p.X - en.X) + (p.Y - en.Y) * (p.Y - en.Y));
    
    // Render text using SDL (note: SDL2 doesn't have built-in text; we print to console for now)
    if (frame % 30 == 0) {
      std::cout << "[Frame " << frame << "] Player: (" << p.X << ", " << p.Y << ") | Enemy: (" 
                << en.X << ", " << en.Y << ") | Dist: " << dist << " | NPCs:" << npcs.size() << std::endl;
      for (const auto &n : npcs) std::cout << " - " << n.npcName << "@(" << n.X << "," << n.Y << ") map=" << n.mapID << std::endl;
    }

    SDL_RenderPresent(ren);
    SDL_Delay(16);
    frame++;
  }

  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();
  std::cout << "AnimeGame C++ environment renderer shut down gracefully." << std::endl;
  return 0;
}
