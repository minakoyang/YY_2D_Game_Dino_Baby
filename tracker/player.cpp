#include "player.h"
#include "smartSprite.h"
#include "gameData.h"
#include "renderContext.h"
#include "explodingSprite.h"
#include "sound.h"


Player::Player( const std::string& name, const std::vector<std::string>& actions) :
  TwoWayMultisprite(name, actions),
  observers(),
  collision(false),
  explosion(nullptr),
  initialVelocity(getVelocity()),
  initialPosition(getPosition()),
  bulletName(Gamedata::getInstance().getXmlStr(name+"/bullet")),
  bulletlist(),
  freelist(),
  shootCount(0),
  shootUpwards(false),
  minSpeed(Gamedata::getInstance().getXmlInt(bulletName+"/minSpeedX")),
  bulletInterval(Gamedata::getInstance().getXmlInt(bulletName+"/interval")),
  timeSinceLastShoot(0),
  jumping(false),
  timeSinceJump(0)

{
  setVelocityX(0);
}

Player::Player(const Player& s) :
  TwoWayMultisprite(s),
  observers(s.observers),
  collision(s.collision),
  explosion(s.explosion),
  initialVelocity(s.getVelocity()),
  initialPosition(s.getPosition()),
  bulletName(s.bulletName),
  bulletlist(s.bulletlist),
  freelist(s.freelist),
  shootCount(s.shootCount),
  shootUpwards(s.shootUpwards),
  minSpeed(s.minSpeed),
  bulletInterval(s.bulletInterval),
  timeSinceLastShoot(s.timeSinceLastShoot),
  jumping(s.jumping),
  timeSinceJump(s.timeSinceJump)
  { }

Player& Player::operator=(const Player& s) {
  TwoWayMultisprite::operator=(s);
  collision = s.collision;
  explosion=s.explosion;
  initialVelocity = s.initialVelocity;
  initialPosition = s.initialPosition;
  bulletName = s.bulletName;
  bulletlist = s.bulletlist;
  freelist = s.freelist;
  shootCount = s.shootCount;
  shootUpwards = s.shootUpwards;
  minSpeed = s.minSpeed;
  bulletInterval = s.bulletInterval;
  timeSinceLastShoot = s.timeSinceLastShoot;
  jumping = s.jumping;
  timeSinceJump = s.timeSinceJump;

  return *this;
}

Player::~Player() {
  if (explosion){
    delete explosion;
  }
}

void Player::explode() {
  if (!explosion) {
    Sprite sprite(getName(), getPosition(), getVelocity(), images[currentFrame]);
    explosion = new ExplodingSprite(sprite);
  }
}

void Player::advanceFrame(Uint32 ticks) {
  timeSinceLastFrame += ticks;
  if (currentFrameGroup == DEADLEFT) {
    while (currentFrame < 63) {
      currentFrame ++;
      timeSinceLastFrame = 0;
    }
  }
  if (currentFrameGroup == DEADRIGHT) {
    while (currentFrame < 55) {
      currentFrame ++;
      timeSinceLastFrame = 0;
    }
  }
  else {
    if (timeSinceLastFrame > frameInterval) {
      currentFrame = currentFrameGroup * (numberOfFrames / 2) + (currentFrame + 1) % (numberOfFrames / 2);
      timeSinceLastFrame = 0;
  	}
  }
}


void Player::stop() {
  //setVelocity( Vector2f(0, 0) );
  setVelocityX( 0.93*getVelocityX() );
  setVelocityY(0);
  shootUpwards=false;
  if (!isDead() && fabs(getVelocityX()) < 60) {
    if (moveRight) {
      currentFrameGroup = STANDRIGHT;
    }
    else {
      currentFrameGroup = STANDLEFT;
    }
  }
}

void Player::right() {
  if (!isDead() && getX() < worldWidth-getScaledWidth()) {
    // if (!moveRight) {
      // currentFrameGroup--;
      if (!jumping) {
      currentFrameGroup = WALKRIGHT;
    }
    // }
    moveRight = true;
    setVelocityX(initialVelocity[0]);
  }
}
void Player::left()  {
  if (!isDead() && getX() > 0) {
    // if (moveRight) {
      // currentFrameGroup++;
      if (!jumping) {
      currentFrameGroup = WALKLEFT;
    }
    // }
    moveRight = false;
    setVelocityX(-initialVelocity[0]);
  }
}
void Player::up()    {
  // if ( getY() > 0) {
  //   setVelocityY( -initialVelocity[1] );
  // }
  // if (moveRight) {
  //   currentFrameGroup = WALKRIGHT;
  // }
  // else {
  //   currentFrameGroup = WALKLEFT;
  // }
  shootUpwards = true;
}

void Player::dead() {
  if (moveRight) {
    currentFrameGroup = DEADRIGHT;
    //setCurrentFrame(7);


    setVelocityX(0);
  }
  else {
    currentFrameGroup = DEADLEFT;
    setVelocityX(0);
  }
  currentFrame = currentFrameGroup * (numberOfFrames / 2);

}

void Player::jump() {
  if (!jumping) {
    if (getY() > 0) {
      if (moveRight) {
        currentFrameGroup = JUMPRIGHT;
      }
      else {
        currentFrameGroup = JUMPLEFT;
      }
      setVelocityY(-initialVelocity[1]/2.5);
    }
    jumping = true;
    timeSinceJump = 0;
    SDLSound::getInstance()[1];
  }
}

void Player::shoot() {
  if (timeSinceLastShoot < bulletInterval) return;
  float minSpeedY = 0;
  double angle = 0;
  Vector2f pos, vel;
  if (shootUpwards) {
    minSpeedY = -minSpeed;
  }
  if (moveRight) {
    currentFrameGroup = WALKRIGHT;
    float deltaX = Gamedata::getInstance().getXmlInt(bulletName+"/startRLoc/x");
    float deltaY = Gamedata::getInstance().getXmlInt(bulletName+"/startRLoc/y");

    if (shootUpwards) {
      angle= -45.0;
      deltaY -= 10.0;
    }
    pos = getPosition() + Vector2f(deltaX, deltaY);
    vel = getVelocity() + Vector2f(minSpeed, minSpeedY);
    //currentFrame = 83;  //why??
  }
  else {
    currentFrameGroup = WALKLEFT;
    angle = 180.0;
    float deltaX = Gamedata::getInstance().getXmlInt(bulletName+"/startLLoc/x");
    float deltaY = Gamedata::getInstance().getXmlInt(bulletName+"/startLLoc/y");
    if (shootUpwards) {
      angle = 225.0;
      deltaY -=10.0;
    }
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
  shootCount++;
  SDLSound::getInstance()[0];
}

// void Player::down()  {
//   if ( getY() < worldHeight-getScaledHeight()) {
//     setVelocityY( initialVelocity[1] );
//   }
//   if (moveRight) {
//     currentFrameGroup = WALKRIGHT;
//   }
//   else {
//     currentFrameGroup = WALKLEFT;
//   }
// }

void Player::draw() const {
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

void Player::update(Uint32 ticks) {
  if (explosion) {
    explosion->update(ticks);
    if (explosion->chunkCount()==0) {
      delete explosion;
      explosion = NULL;
    }
  }
  else {
    if (! collision) advanceFrame(ticks);

    if (!jumping) {

      Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
      setPosition(getPosition() + incr);

      stop();
    }
    else {
      if (getY() < initialPosition[1] +10) {
        // if (std::abs(getVelocityY())<10){
        //   shoot();
        // }
        if (moveRight) {
          setVelocityX(50);
        }
        else {
          setVelocityX(-50);
        }

        timeSinceJump += ticks;
        Vector2f incr = getVelocity() * static_cast<float>(timeSinceJump) * 0.001;
        setPosition(getPosition()+incr);
        setVelocity(getVelocity()+Vector2f(0,20*static_cast<float>(timeSinceJump)*0.001));
        if (getY()>initialPosition[1]+10) {
          setY(initialPosition[1]);
          stop();
          jumping=false;
        }
      }
      else {
        jumping = false;
        setY(initialPosition[1]);
        stop();
      }
    }
  }

  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while (ptr != observers.end()) {
    (*ptr)->setPlayerPos( getPosition());
    ++ptr;
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


  // if ( getY() < 0) {
  //   setVelocityY( fabs( getVelocityY() ) );
  // }
  // if ( getY() > worldHeight-getScaledHeight()) {
  //   setVelocityY( -fabs( getVelocityY() ) );
  // }

  if ( getX() < 0) {
    setX(0);
  }
  if ( getX() > worldWidth-getScaledWidth()) {
    setX( worldWidth-getScaledWidth() );
  }
// }
}

void Player::detach( SmartSprite* o) {
  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while (ptr != observers.end()) {
    if (*ptr == o ) {
      ptr = observers.erase(ptr);
      return;
    }
    ++ptr;
  }
}
