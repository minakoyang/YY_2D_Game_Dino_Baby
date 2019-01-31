#ifndef twoWayMultisprite__H
#define twoWayMultisprite__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"

class ExplodingSprite;

class TwoWayMultisprite : public Drawable {
public:
  TwoWayMultisprite(const std::string&, bool = false);
  TwoWayMultisprite(const std::string&, const std::vector<std::string>&);
  TwoWayMultisprite(const TwoWayMultisprite&);
  ~TwoWayMultisprite();

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual void changeAction(int);

  virtual const Image* getImage() const {
    return images[currentFrame];
  }
  int getScaledWidth()  const {
    return getScale()*images[currentFrame]->getWidth();
  }
  int getScaledHeight()  const {
    return getScale()*images[currentFrame]->getHeight();
  }
  void setCurrentFrame(int n) { currentFrame = n; }
  virtual const SDL_Surface* getSurface() const {
    return images[currentFrame]->getSurface();
  }

  virtual void explode();
  virtual ExplodingSprite* getExplosion() const { return explosion; }
  void setCollision(bool b) { collided = b; }
  bool isCollided() const { return collided; }

protected:
  std::vector<Image *> images;
  ExplodingSprite* explosion;

  unsigned currentFrame;
  unsigned currentFrameGroup;
  bool moveRight;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;
  bool collided;

  virtual void advanceFrame(Uint32 ticks);
  TwoWayMultisprite& operator=(const TwoWayMultisprite&);
};

#endif
