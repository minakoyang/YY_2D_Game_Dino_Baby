#ifndef ENGINE__H
#define ENGINE__H

#include <vector>
#include <SDL.h>
#include "clock.h"
#include "world.h"
#include "sound.h"
#include "viewport.h"
#include "introHud.h"
#include "infoHud.h"
#include "poolHud.h"
#include "healthBar.h"

class Player;
class SmartSprite;
class CollisionStrategy;
class Trex;

class Engine {
public:
  Engine ();
  ~Engine ();
  bool play();
  void switchAction();
  void switchSprite();
  void drawStats() const;

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;

private:
  int countStego = 0, countDiplo = 0, countPara = 0, countSteak = 0, countTrice=0, countSuperStego = 0, countHit = 0;
  const RenderContext& rc;
  const IoMod& io;
  Clock& clock;

  SDL_Renderer * const renderer;
  SDLSound& sound;
  World cloud;
  World mountain;
  World grassland;

  Viewport& viewport;

  HealthBar* healthBar;
  std::vector<std::string> actions;
  std::vector<std::string> trexaction;
  Player* player;
  Trex* trex;
  std::vector<Drawable*> farSprites;
  std::vector<Drawable*> closeSprites;
  std::vector<SmartSprite*> smartSprites;
  CollisionStrategy* collisionStrategy;
  std::vector<Drawable*> sprites;
  std::vector<Drawable*> TwoWayMultisprites;


  int currentSprite;
  IntroHud* introHud;
  InfoHud* infoHud;
  PoolHud* poolHud;

  int hudDelay;
  int trexDelay;
  bool godMode;
  bool makeVideo;
  bool gameOver;
  // bool drawTrex;

  void draw() const;
  void update(Uint32);

  void printScales() const;
  void checkForCollisions();
  int count() const { return countPara + countDiplo + countStego + countTrice + countSuperStego; }
};

#endif
