#ifndef ONEWAYSPRITE__H
#define ONEWAYSPRITE__H
#include <string>
#include "drawable.h"

class OwoWaySprite : public Drawable {
public:
  OwoWaySprite(const std::string&);
  OwoWaySprite(const std::string&, const Vector2f& pos, const Vector2f& vel, 
         const Image*);
  OwoWaySprite(const OwoWaySprite&);
  virtual ~OwoWaySprite() { } 
  OwoWaySprite& operator=(const OwoWaySprite&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Image* getImage() const { return image; }
  virtual const SDL_Surface* getSurface() const { 
    return image->getSurface();
  }
  int getScaledWidth()  const { return getScale()*image->getWidth();  } 
  int getScaledHeight() const { return getScale()*image->getHeight(); } 

private:
  const Image * image;

protected:
  int worldWidth;
  int worldHeight;

  int getDistance(const OwoWaySprite*) const;
  Vector2f makeVelocity(int, int) const;
};
#endif
