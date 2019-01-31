#include "twoWayMultisprite.h"
#include "gameData.h"
#include "imageFactory.h"
#include <cstdlib>
#include "explodingSprite.h"

void TwoWayMultisprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = currentFrameGroup * (numberOfFrames / 2) + (currentFrame + 1) % (numberOfFrames / 2);
    timeSinceLastFrame = 0;
	}
}

TwoWayMultisprite::~TwoWayMultisprite() { if (explosion) delete explosion; }

TwoWayMultisprite::TwoWayMultisprite( const std::string& name, bool imageInFolder) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  images(),
	explosion(nullptr),
  currentFrame(0),
  currentFrameGroup(0),
  moveRight(true),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  collided(false)
{
  if (imageInFolder) {
    images = ImageFactory::getInstance().getFolderImages(name);
  }
  else {
    images = ImageFactory::getInstance().getImages(name);
  }
}

TwoWayMultisprite::TwoWayMultisprite( const std::string& name, const std::vector<std::string>& actions) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  images(ImageFactory::getInstance().getFolderImages(name, actions)),
	explosion(nullptr),
  currentFrame(0),
  currentFrameGroup(0),
  moveRight(true),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  collided(false)
{ }

TwoWayMultisprite::TwoWayMultisprite(const TwoWayMultisprite& s) :
  Drawable(s),
  images(s.images),
	explosion(s.explosion),
  currentFrame(s.currentFrame),
  currentFrameGroup(s.currentFrameGroup),
  moveRight(s.moveRight),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  collided(s.collided)
  { }

TwoWayMultisprite& TwoWayMultisprite::operator=(const TwoWayMultisprite& s) {
  Drawable::operator=(s);
  images = (s.images);
	explosion = s.explosion;
  currentFrame = (s.currentFrame);
  currentFrameGroup = s.currentFrameGroup;
  moveRight = (s.moveRight);
  numberOfFrames = ( s.numberOfFrames );
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  worldWidth = ( s.worldWidth );
  worldHeight = ( s.worldHeight );
  collided = s.collided;
  return *this;
}

void TwoWayMultisprite::explode() {
	if (!explosion) {
		Sprite sprite(getName(), getPosition(), getVelocity(), images[currentFrame]);
		explosion = new ExplodingSprite(sprite);
	}
}

void TwoWayMultisprite::draw() const {
	if (explosion) {
		explosion->draw();
	}
	else {
  	images[currentFrame]->draw(getX(), getY(), getScale());
	}
}

void TwoWayMultisprite::update(Uint32 ticks) {
	if (explosion) {
		explosion->update(ticks);
		if (explosion->chunkCount()==0) {
			delete explosion;
			explosion = NULL;
		}
		return;
	}
  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-getScaledHeight()) {
    setVelocityY( -fabs( getVelocityY() ) );
  }

  if ( getX() < 0) {
    setVelocityX( fabs( getVelocityX() ) );
    if (!moveRight) {
      currentFrameGroup--;
    }
    moveRight = true;
  }
  if ( getX() > worldWidth-getScaledWidth()) {
    setVelocityX( -fabs( getVelocityX() ) );
    if (moveRight) {
      currentFrameGroup++;
    }
    moveRight = false;
  }
}

void TwoWayMultisprite::changeAction(int num) {
	currentFrameGroup = (currentFrameGroup + 2) % (2* num);
}
