#include <cmath>
#include <random>
#include <functional>
#include "oneWaySprite.h"
#include "gameData.h"
#include "imageFactory.h"

Vector2f OwoWaySprite::makeVelocity(int vx, int vy) const {
  float newvx = Gamedata::getInstance().getRandFloat(vx-50,vx+50);;
  float newvy = Gamedata::getInstance().getRandFloat(vy-50,vy+50);;
  newvx *= [](){ if(rand()%2) return -1; else return 1; }();
  newvy *= [](){ if(rand()%2) return -1; else return 1; }();

  return Vector2f(newvx, newvy);
}

OwoWaySprite::OwoWaySprite(const string& n, const Vector2f& pos, const Vector2f& vel,
               const Image* img):
  Drawable(n, pos, vel), 
  image( img ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

OwoWaySprite::OwoWaySprite(const std::string& name) :
  Drawable(name,
           Vector2f(rand() % Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    rand() % Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(
                    -fabs(rand() % Gamedata::getInstance().getXmlInt(name+"/speedX")), 
                    rand() % Gamedata::getInstance().getXmlInt(name+"/speedY")) 
           ),
  image( ImageFactory::getInstance().getImage(name) ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

OwoWaySprite::OwoWaySprite(const OwoWaySprite& s) :
  Drawable(s), 
  image(s.image),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

OwoWaySprite& OwoWaySprite::operator=(const OwoWaySprite& rhs) {
  Drawable::operator=( rhs );
  image = rhs.image;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  return *this;
}

inline namespace{
  constexpr float SCALE_EPSILON = 2e-7;
}

void OwoWaySprite::draw() const { 
  if(getScale() < SCALE_EPSILON) return;
  image->draw(getX(), getY(), getScale()); 
}

void OwoWaySprite::update(Uint32 ticks) { 
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
    setVelocityY( std::abs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-getScaledHeight()) {
    setVelocityY( -std::abs( getVelocityY() ) );
  }

  if ( getX() < 0) {
    setX( worldWidth-getScaledWidth() );
  }
  if ( getX() > worldWidth-getScaledWidth()) {
    setVelocityX( -std::abs( getVelocityX() ) );
  }  
}
