#ifndef INTROHUD__H
#define INTROHUD__H

#include "hud.h"
#include "viewport.h"

class IntroHud : public Hud {
public:
  IntroHud();
  IntroHud(const IntroHud&) = delete;
  IntroHud& operator = (const IntroHud&) = delete;
  virtual void draw() const;
  virtual void update();
private:
  SDL_Rect introHudFrame;
  Image* const image;
  unsigned imageWidth;
  const Viewport& view;
  int posX;
  int posY;

};

#endif
