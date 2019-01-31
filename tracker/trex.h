#ifndef TREX__H
#define TREX__H

#include "twoWayMultisprite.h"
#include <list>
#include <vector>
#include "bullet.h"

class ExplodingSprite;
class Player;

class Trex : public TwoWayMultisprite {
public:
  Trex(const std::string&, const std::vector<std::string>&);
  Trex(const Trex&);
  ~Trex();

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  void collided() { collision = true; }
  //void missed() { collision = false; }
  virtual void explode();
  Trex& operator=(const Trex&);

  void shoot();
  void setPlayer(Player* p);

  int getBulletlistSize() const { return bulletlist.size(); }
  int getFreelistSize() const { return freelist.size(); }
  std::list<Bullet>::iterator getBulletlistFront() { return bulletlist.begin(); }
  std::list<Bullet>::iterator getBulletlistEnd() { return bulletlist.end(); }
  bool getExplosionCompletion() const { return explosionCompleted; }
  std::list<Bullet>& getBulletlist() { return bulletlist; }
  std::list<Bullet>& getFreelist() { return freelist; }

private:
  bool collision;
  ExplodingSprite* explosion;
  Vector2f initialVelocity;
  Vector2f initialPosition;
  std::string bulletName;
  std::list<Bullet> bulletlist;
  std::list<Bullet> freelist;
  float minSpeed;
  float bulletInterval;
  float timeSinceLastShoot;
  Player* player;

  bool explosionCompleted;
};

#endif
