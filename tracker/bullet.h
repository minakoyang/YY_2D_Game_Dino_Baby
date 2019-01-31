#ifndef BULLET__H
#define BULLET__H

#include <iostream>
#include <vector>
#include "drawable.h"
#include "gameData.h"

class ExplodingSprite;

class Bullet : public Drawable {
public:
  //explicit Bullet(const string& name) :
  Bullet(const std::string&, int, double, const Vector2f&, const Vector2f&);
  Bullet(const Bullet&);
  virtual ~Bullet();

  virtual void draw() const;
  //   Sprite(name),
  //   distance(0),
  //   maxDistance(Gamedata::getInstance().getXmlInt(name+"/distance")),
  //   tooFar(false)
  // { }
  virtual void update(Uint32 ticks);
  virtual const Image* getImage() const{
    return images[currentFrame];
  }
  virtual int getCurrentFrame() const {
    return currentFrame;
  }
  virtual int getScaledWidth() const {
    return getScale()*images[currentFrame]->getWidth();
  }
  virtual int getScaledHeight() const {
    return getScale()*images[currentFrame]->getHeight();
  }
  virtual const SDL_Surface* getSurface() const {
    return images[currentFrame]->getSurface();
  }

  virtual void explode();
  Bullet& operator=(const Bullet&);
  bool goneTooFar() const {return tooFar;}

  void reset() {
    tooFar = false;
    distance = 0;
  }

  void setAngle(double a) {angle = a;}

private:
  std::vector<Image *> images;
  unsigned currentFrame;
  ExplodingSprite* explosion;
  int worldWidth;
  int worldHeight;
  double angle;
  float distance;
  float maxDistance;
  bool tooFar;


};





#endif
