#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "sprite.h"
#include "multisprite.h"
#include "oneWaySprite.h"
#include "twoWayMultisprite.h"
#include "player.h"
#include "gameData.h"
#include "engine.h"
#include "frameGenerator.h"
#include "collisionStrategy.h"
#include "smartSprite.h"
#include "trex.h"

class SpriteLess {
public:
  bool operator()(const Drawable* lhs, const Drawable* rhs) const {
    return lhs->getScale() < rhs->getScale();
  }
};

Engine::~Engine() {
  delete player;
  delete trex;
  for (Drawable* sprite : sprites){
    delete sprite;
  }
  sprites.clear();

  for (SmartSprite* smartSprite : smartSprites){
    delete smartSprite;
  }
  smartSprites.clear();

  for (Drawable* farSprite : farSprites) {
    delete farSprite;
  }
  farSprites.clear();

  for (Drawable* closeSprite : closeSprites) {
    delete closeSprite;
  }
  closeSprites.clear();

  for (Drawable* multisprite : TwoWayMultisprites) {
    delete multisprite;
  }
  TwoWayMultisprites.clear();

  delete poolHud;
  delete introHud;
  delete infoHud;
  delete collisionStrategy;
  delete healthBar;

  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IoMod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc.getRenderer() ),
  sound(SDLSound::getInstance()),
  cloud("Cloud", Gamedata::getInstance().getXmlInt("Cloud/factor") ),
  mountain("Mountain", Gamedata::getInstance().getXmlInt("Mountain/factor")),
  grassland("Grassland", Gamedata::getInstance().getXmlInt("Grassland/factor")),
  viewport( Viewport::getInstance() ),
  healthBar(new HealthBar("HealthBar")),
  actions(),
  trexaction(),
  player(),
  trex(),
  farSprites(),
  closeSprites(),
  smartSprites(),
  collisionStrategy(new PerPixelCollisionStrategy),
  sprites(),
  TwoWayMultisprites(),
  currentSprite(0),
  introHud(new IntroHud()),
  infoHud(new InfoHud()),
  poolHud(),
  hudDelay(0),
  trexDelay(0),
  godMode(false),

  makeVideo( false ),
  gameOver(false)
  // drawTrex(false)
{
  actions.push_back("Idle");
  actions.push_back("Walk");
  actions.push_back("Run");
  actions.push_back("Dead");
  actions.push_back("Jump");

  player = new Player("Dino", actions);
  Viewport::getInstance().setObjectToTrack(player);


  trexaction.push_back("Act");
  trex = new Trex("RunningTrex", trexaction);
  trex->setPlayer(player);

  farSprites.reserve(5);
  for (int i =0; i<5; i++) {
    farSprites.push_back (new TwoWayMultisprite("Flyingbird"));
    float scale = Gamedata::getInstance().getRandFloat(0.05, 0.1);
    static_cast<TwoWayMultisprite*>(farSprites[i])->setCurrentFrame(rand() % 4);
    farSprites[i]->setScale(scale);
    farSprites[i]->setVelocity(Vector2f(10, 0));
    farSprites[i]->setPosition(Vector2f(std::rand()%Gamedata::getInstance().getXmlInt("Flyingbird/startLoc/x"),
                                        std::rand()%Gamedata::getInstance().getXmlInt("Flyingbird/startLoc/y")));
  }
  std::sort(farSprites.begin(), farSprites.end(), SpriteLess());

  closeSprites.reserve(5);
  for (int i =0; i<5; i++) {
    closeSprites.push_back (new TwoWayMultisprite("Flyingbird"));
    float scale = Gamedata::getInstance().getRandFloat(0.2, 0.3);
    static_cast<TwoWayMultisprite*>(closeSprites[i])->setCurrentFrame(rand() % 4);
    closeSprites[i]->setScale(scale);
    closeSprites[i]->setVelocity(Vector2f(50, 0));
    closeSprites[i]->setPosition(Vector2f(std::rand()%Gamedata::getInstance().getXmlInt("Flyingbird/startLoc/x"),
                                        std::rand()%Gamedata::getInstance().getXmlInt("Flyingbird/startLoc/y")));
  }
  std::sort(closeSprites.begin(), closeSprites.end(), SpriteLess());

  sprites.push_back(new Sprite("Steak"));
  for (int i = 0; i < 3;) {
    TwoWayMultisprite* t = new TwoWayMultisprite("Walkingstegosaurus");
    t->setX(1200 * (++i));
    sprites.push_back(t);
  }

  Vector2f pos = player->getPosition();
  int w = player->getScaledWidth();
  int h = player->getScaledHeight();
  for (int i = 0; i < 5; i++) {
    // smartSprites.push_back(new SmartSprite("Walkingstegosaurus", pos, w, h));
    smartSprites.push_back(new SmartSprite("StegosaurusPink", pos, w, h));
    smartSprites.push_back(new SmartSprite("Diplodocus", pos, w, h));
    smartSprites.push_back(new SmartSprite("Parasaurolophus", pos, w, h));
    smartSprites.push_back(new SmartSprite("Triceratops", pos, w, h));

    player->attach(smartSprites[i * 4 + 0]);
    player->attach(smartSprites[i * 4 + 1]);
    player->attach(smartSprites[i * 4 + 2]);
    player->attach(smartSprites[i * 4 + 3]);
    // player->attach(smartSprites[i*5+4]);
  }

  poolHud = new PoolHud(*player);

  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  if(introHud->isShowHud()){
    cloud.draw();
    mountain.draw();
    grassland.draw();
    introHud->draw();
  }
  else{
    cloud.draw();
    for (const Drawable* farSprite : farSprites) {
      farSprite->draw();
    }
    mountain.draw();
    for (const Drawable* closeSprite : closeSprites) {
      closeSprite->draw();
    }
    grassland.draw();

    player->draw();

    if (count() == 20 && trex) {
      trex->draw();
    }

    for (Drawable* sprite : smartSprites) {
      sprite -> draw();
    }
    for (Drawable* sprite : sprites) {
      sprite -> draw();
    }

    healthBar->draw();
    if (poolHud->isShowHud()) {
      poolHud->draw();
    }
    infoHud->draw();
    drawStats();

    if (gameOver) {
      SDL_Rect rect = {590, 140, 400, 200};
      SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255/2);
      SDL_RenderFillRect(renderer, &rect);
      SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255/2);
      SDL_RenderDrawRect(renderer, &rect);
      IoMod::getInstance().setFontColor(255, 0, 0);
      if (healthBar->getCurrentLength() > 0) {
        IoMod::getInstance().writeText("Congratulations!!!", 700, 200);
      }
      if (healthBar->getCurrentLength() == 0) {
        IoMod::getInstance().writeText("Game Over!", 720, 200);
      }
      IoMod::getInstance().writeText("Press R to Restart the Game!", 640, 240);
      clock.pause();
    }
    viewport.draw();

    std::stringstream ss;
    ss << Clock::getInstance().getFps();
    std::string fps;
    ss >> fps;
    IoMod::getInstance().writeText("FPS: " + fps, 30, 65);

  }
  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {
  if (introHud->isShowHud()){
    // sound.playIntroMusic();
    if (hudDelay < 500) {
      hudDelay++;
      sound.playIntroMusic();
    }
    else{
      introHud->toggle();
    }
  }
  else if (gameOver) {
    if (healthBar->getCurrentLength() == 0) {
      sound.playLoseMusic();
    }
    else {
      sound.playEndMusic();
    }       
  }
  else {
    sound.playGameMusic();
    for (Drawable* farSprite : farSprites) {
      farSprite->update(ticks);
    }
    for (Drawable* closeSprite : closeSprites) {
      closeSprite->update(ticks);
    }

    if (count() == 20 && trex) {
      trex->shoot();
      sound.playFightMusic();
      if (!godMode) {
        if (trex->getBulletlistSize() > 0) {
          auto bluebullet = trex->getBulletlistFront();
          while (bluebullet != trex->getBulletlistEnd()) {
            if (collisionStrategy->execute(*bluebullet, *player)) {
              healthBar->loseHealth(0.05);
              trex->getFreelist().push_back(*bluebullet);
              bluebullet = trex->getBulletlist().erase(bluebullet);
              SDLSound::getInstance()[2];
            }
            else {
              bluebullet++;
            }           
          }
        }
        if (healthBar->getCurrentLength() == 0) {
          player->dead();
          gameOver = true;
        }
      }

      if (player->getBulletlistSize() > 0) {
        auto fireball = player->getBulletlistFront();
        while (fireball != player->getBulletlistEnd()) {
          if (collisionStrategy->execute(*fireball, *trex)) {
            countHit ++;
            // std::cout<< countHit<<std::endl;
            player->getFreelist().push_back(*fireball);
            fireball = player->getBulletlist().erase(fireball);
          }
          else {
            fireball++;
          }       
        }
      }

      if (countHit >= 30) {
        trex->explode();
        if (trex && trex->getExplosionCompletion()) {
          delete trex;
          trex = NULL;
        }
        if (trexDelay >= 100) {
          gameOver=true;
        }
        trexDelay++;
        // std::cout<< trexDelay << std::endl;
      }
    }


    std::vector<Drawable*>::iterator it = sprites.begin();
    while (it != sprites.end()) {
      if (collisionStrategy->execute(*player, **it)) {
        Sprite* steak = dynamic_cast<Sprite*>(*it);
        if (steak) {
          healthBar->reset();
          Drawable* doa = *it;
          doa->explode();
        }
        else {
          TwoWayMultisprite* tw = dynamic_cast<TwoWayMultisprite*>(*it);
          if (tw) {
            if (!tw->isCollided() && !godMode) {
              healthBar->loseHealth(0.05);
              tw->setCollision(true);
              tw->explode();
            }
            else if (!tw->getExplosion()){
              tw->setCollision(false);
            }
          } 
        }
      }
      it++;
    }

    if (player->getBulletlistSize() > 0) {
      auto fireball = player->getBulletlistFront();
      while (fireball != player->getBulletlistEnd()) {
        std::vector<Drawable*>::iterator itr = sprites.begin();
        while(itr != sprites.end()) {
          TwoWayMultisprite* tw = dynamic_cast<TwoWayMultisprite*>(*itr);
          if (tw) {
            if (collisionStrategy->execute(*fireball, *tw)) {
              tw->explode();
              tw->setCollision(true);
            }
          }
          itr++;
        }
        std::vector<SmartSprite*>::iterator its = smartSprites.begin();
        while(its != smartSprites.end()) {
          if (collisionStrategy->execute(*fireball, **its)) {
            SmartSprite* dinoType = *its;

            if (dinoType->getExplosion()==NULL) {
              std::string Type = dinoType->getName();
              player->detach(dinoType);
              //delete dinoType;
              dinoType->setCollision();
              dinoType->explode();
              // std::cout << Type<<std::endl;
              if (Type.compare("StegosaurusPink") == 0) {
               countStego++;
               //std::cout<<countStego<<std::endl;
              }
              else if (Type.compare("Diplodocus") == 0) {
               countDiplo++;
              // std::cout <<countDiplo<<std::endl;
              }
              else if (Type.compare("Parasaurolophus") == 0) {
               countPara++;
               //std::cout<<countPara<<std::endl;
              }
              else if (Type.compare("Triceratops") == 0) {
                countTrice++;
              }
            }
          }
          its++;
        }
        fireball++;
      }
    }

    player->update(ticks);
    if (trex) {
      trex->update(ticks);
    }   
    for (Drawable* sprite : sprites ) {
      sprite -> update(ticks);
    }
    for (Drawable*sprite : smartSprites) {
      sprite -> update(ticks);
    }

    auto its = smartSprites.begin();
    while (its!=smartSprites.end()) {
      SmartSprite* dinoType = *its;
      dinoType->update(ticks);
      if (dinoType->isCollided() && !dinoType->getExplosion()){
        its = smartSprites.erase(its);
        delete dinoType;
      }
      else {
        its++;
      }
    }

    introHud->update();
    cloud.update();
    mountain.update();
    grassland.update();
    viewport.update(); // always update viewport last
  }
}

void Engine::switchAction() {
  player->changeAction(actions.size());
}

void Engine::drawStats() const {
  std::stringstream ss;
  ss << countStego << "";
  IoMod::getInstance().setFontColor(255,0,0);
  IoMod::getInstance().writeText(ss.str(), 280, 140);
  ss.clear();
  ss.str("");
  ss << countDiplo << "";
  IoMod::getInstance().writeText(ss.str(), 280, 180);
  ss.clear();
  ss.str("");
  ss << countPara << "";
  IoMod::getInstance().writeText(ss.str(), 280, 220);
  ss.clear();
  ss.str("");
  ss << countTrice << "";
  IoMod::getInstance().writeText(ss.str(), 280, 260);
  ss.clear();
  ss.str("");
  ss << "God Mode: " << "";
  IoMod::getInstance().writeText(ss.str(), 40, 300);
  ss.clear();
  ss.str("");
  if (godMode) {
    ss << "ON";
  }
  else {
    ss << "OFF";
  }
  IoMod::getInstance().writeText(ss.str(), 280, 300);
  ss.clear();
  ss.str("");
  ss << "Total Fireball Used"<< "";
  IoMod::getInstance().writeText(ss.str(), 40, 340);
  ss.clear();
  ss.str("");
  ss << player->getShootCount();
  IoMod::getInstance().writeText(ss.str(), 280, 340);
}

bool Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;
  // sound.playGameMusic();
  // sound.playStartMusic();

  while ( !done ) {
    keystate = SDL_GetKeyboardState(NULL);
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {     
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDL_SCANCODE_T] ) {
          switchAction();
        }
        if (keystate[SDL_SCANCODE_F1]) {
          introHud->toggle();
          hudDelay = 0;
        }
        if (keystate[SDL_SCANCODE_F2]){
          poolHud->toggle();
        }

        if (keystate[SDL_SCANCODE_R]){
        //  static_cast<HealthBar*>(healthBar)->reset();
          clock.unpause();
          return true;
        }

        if (keystate[SDL_SCANCODE_J]) {
          player->jump();
        }
        if (keystate[SDL_SCANCODE_G]) {
          if (godMode) {
            godMode = false;
          }
          else {
            godMode = true;
          }
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
    }

    // In this section of the event loop we allow key bounce:

    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
      if (keystate[SDL_SCANCODE_A]) {
        player->left();
      }
      if (keystate[SDL_SCANCODE_D]) {
        player->right();
      }
      if (keystate[SDL_SCANCODE_W]) {
        player->up();
      }
      if (keystate[SDL_SCANCODE_SPACE]) {
        player->shoot();
      }
      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
  return false;
}
