#include "trex.h"
#include "gameData.h"
#include "renderContext.h"
#include "explodingSprite.h"
#include "sound.h"
#include "player.h"

Trex::Trex( const std::string& name, const std::vector<std::string>& actions):
  TwoWayMultisprite(name, actions),
  collision(false),
  explosion(nullptr),
  initialVelocity(getVelocity()),
  initialPosition(getPosition()),
  bulletName(Gamedata::getInstance().getXmlStr(name+"/bullet")),
  bulletlist(),
  freelist(),
  minSpeed(Gamedata::getInstance().getXmlInt(bulletName+"/minSpeedX")),
  bulletInterval(Gamedata::getInstance().getXmlInt(bulletName+"/interval")),
  timeSinceLastShoot(0),
  player(nullptr),
  explosionCompleted(false)
  {
    setVelocityX(initialVelocity[0]);
  }

Trex::Trex(const Trex& t) :
  TwoWayMultisprite(t),
  collision(t.collision),
  explosion(t.explosion),
  initialVelocity(t.getVelocity()),
  initialPosition(t.getPosition()),
  bulletName(t.bulletName),
  bulletlist(t.bulletlist),
  freelist(t.freelist),
  minSpeed(t.minSpeed),
  bulletInterval(t.bulletInterval),
  timeSinceLastShoot(t.timeSinceLastShoot),
  player(t.player),
  explosionCompleted(t.explosionCompleted)
  { }

Trex& Trex::operator=(const Trex& t) {
  TwoWayMultisprite::operator=(t);
  collision = t.collision;
  explosion = t.explosion;
  initialVelocity = t.initialVelocity;
  initialPosition = t.initialPosition;
  bulletName = t.bulletName;
  bulletlist = t.bulletlist;
  freelist = t.freelist;
  minSpeed = t.minSpeed;
  bulletInterval = t.bulletInterval;
  timeSinceLastShoot = t.timeSinceLastShoot;
  player = t.player;
  explosionCompleted = t.explosionCompleted;

  return *this;
  }

Trex::~Trex() {
  if (explosion) {
    delete explosion;
  }
}

void Trex::explode() {
  if(!explosion) {
    Sprite sprite(getName(),getPosition(), getVelocity(), images[currentFrame]);
    explosion = new ExplodingSprite(sprite);
  }
}

void Trex::shoot() {
  if (timeSinceLastShoot < bulletInterval) return;
  float minSpeedY = 0;
  double angle = 0;
  Vector2f pos, vel;
  // if (shootUpwards) {
  //   minSpeedY = -minSpeed;
  // }
  if (moveRight) {
  //  currentFrameGroup--;
    float deltaX = Gamedata::getInstance().getXmlInt(bulletName+"/startRLoc/x");
    float deltaY = Gamedata::getInstance().getXmlInt(bulletName+"/startRLoc/y");

    // if (shootUpwards) {
    //   angle= -45.0;
    //   deltaY -= 10.0;
    // }
    pos = getPosition() + Vector2f(deltaX, deltaY);
    vel = getVelocity() + Vector2f(minSpeed, minSpeedY);
    //currentFrame = 83;  //why??
  }
  else {
  //  currentFrameGroup++;
    angle = 180.0;
    float deltaX = Gamedata::getInstance().getXmlInt(bulletName+"/startLLoc/x");
    float deltaY = Gamedata::getInstance().getXmlInt(bulletName+"/startLLoc/y");
    // if (shootUpwards) {
    //   angle = 225.0;
    //   deltaY -=10.0;
    // }
    pos = getPosition() + Vector2f(deltaX, deltaY);
    vel = getVelocity() - Vector2f(minSpeed, -minSpeedY);
    //currentFrame = 93;  //why??
  }
  if (freelist.empty()) {
    Bullet bullet(bulletName, 1, angle, pos, vel);
    bullet.setScale(2);
    bulletlist.push_back(bullet);
  }
  else {
    Bullet bullet = freelist.front();
    freelist.pop_front();
    bullet.reset();
    bullet.setScale(2);
    bullet.setVelocity(vel);
    bullet.setPosition(pos);
    bullet.setAngle(angle);
    bulletlist.push_back(bullet);
  }
  timeSinceLastShoot = 0;
  //shootCount++;
  SDLSound::getInstance()[0];
}

void Trex::setPlayer(Player* p) {
  player = p;
}

void Trex::draw() const {
  if (explosion) {
    explosion->draw();
  }
  else {
    TwoWayMultisprite::draw();
  }
  for (const Bullet& bullet:bulletlist) {
    bullet.draw();
  }
}

void Trex::update(Uint32 ticks) {
  if (explosion) {
    explosion->update(ticks);
    if (explosion->chunkCount()==0) {
      delete explosion;
      explosion = NULL;
      explosionCompleted = true;
    }
  }
  else {
    if (!collision) advanceFrame(ticks);
    Vector2f incr = getVelocity()*static_cast<float>(ticks)*0.001;
    //Vector2f incr = setVelocityX(300)*static_cast<float>(ticks)*0.001;
    setPosition(getPosition() + incr);

    if (player->getX() > getX() || getX() < 0) {
      setVelocityX( fabs( getVelocityX() ) );
      if (!moveRight) {
        currentFrameGroup--;
      }
      moveRight = true;
    }
    if ( player->getX() < getX() || getX() > worldWidth-getScaledWidth()) {
      setVelocityX( -fabs( getVelocityX() ) );
      if (moveRight) {
        currentFrameGroup++;
      }
      moveRight = false;
    }
  }

  timeSinceLastShoot += ticks;
  std::list<Bullet>::iterator it = bulletlist.begin();
  while (it!=bulletlist.end()) {
    it->update(ticks);
    if (it->goneTooFar()) {
      freelist.push_back(*it);
      it = bulletlist.erase(it);
    }
    else {
      it++;
    }
  }
}
