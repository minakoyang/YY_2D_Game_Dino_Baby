#ifndef PLAYER__H
#define PLAYER__H

#include "twoWayMultisprite.h"
#include <list>
#include <vector>
#include "bullet.h"


// In this example the player is derived from MultiSprite.
// However, there are many options.

class SmartSprite;
class ExplodingSprite;


class Player : public TwoWayMultisprite {
public:
  Player(const std::string&, const std::vector<std::string>&);
  Player(const Player&);
  ~Player();

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  void attach(SmartSprite* o) { observers.push_back(o); }
  void detach(SmartSprite*);

  void collided() { collision = true; }
  void missed() { collision = false; }
  virtual void explode();
  Player& operator=(const Player&);

  void right();
  void left();
  void up();
  void down();
  void jump();
  void dead();
  void stop();
  void shoot();
  int getBulletlistSize() const { return bulletlist.size();}
  int getFreelistSize() const { return freelist.size();}
  int getShootCount() const { return shootCount;}
  bool isDead() {return  currentFrameGroup == DEADLEFT ||currentFrameGroup == DEADRIGHT; }
  std::list<Bullet>::iterator getBulletlistFront() { return bulletlist.begin(); }
  std::list<Bullet>::iterator getBulletlistEnd() { return bulletlist.end(); }
  std::list<Bullet>& getBulletlist() { return bulletlist; }
  std::list<Bullet>& getFreelist() { return freelist; }

protected:
  std::list<SmartSprite*> observers;
private:
  enum PlayerActions {
    STANDRIGHT, STANDLEFT,
    WALKRIGHT, WALKLEFT,
    RUNRIGHT, RUNLEFT,
    DEADRIGHT, DEADLEFT,
    JUMPRIGHT, JUMPLEFT,
    SHOOTRIGHT, SHOOTLEFT
  };
  bool collision;
  ExplodingSprite* explosion;
  Vector2f initialVelocity;
  Vector2f initialPosition;
  std::string bulletName;
  std::list<Bullet> bulletlist;
  std::list<Bullet> freelist;
  int shootCount;
  bool shootUpwards;
  float minSpeed;
  float bulletInterval;
  float timeSinceLastShoot;
  bool jumping;
  float timeSinceJump;
  virtual void advanceFrame(Uint32 ticks);


};
#endif
