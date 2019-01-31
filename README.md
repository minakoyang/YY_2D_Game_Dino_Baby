# YY_2D_Game_Dino_Baby

Name: Yuzhe Yang	

Course: 6160

Clemson Email Address: yuzhey@g.clemson.edu

Project no: 6

Project due date: Dec 12, 2018

Project description: 
The goal of this project is to build a playable video game that reaches a conclusion.

Musics and sound effects are incorporated in the program. There are 5 different background music. 1) for the intro hud; 2) for the first stage of the game; 3) when encounters boss; 4) when the player wins the game; 5) when the player loses the game. The music changes for the different scnarios. There are several sound effects for firing an fireball, jumping, boss roaring, and player hit by enemies. 

I wrote a new class to implement the boss character. It is a skeleton Trex. It will approaches to the player "Thomas" and keeps fire blue colored bullets towards Thomas. When Thomas is hit by the blue bullet, it will comprimise Thomas' healthbar. I also implemented another enemy class "walking stegosaurus". It can be killed by Thomas' fireball however when Thomas collides with the walking stagosaurus, it will get hurt and healthbar will be affected.

The program ends either when the player shoots all the balloons and defeats the boss Trex (Winning condition) or the player's healthbar length drop to zero (Lost condition). There is a god mode toggled by the "G" button and the status of the God Mode is shown in the info hud.

Note: if you use valgrind to run the game, and if the player wins the game, there won't be a conclusion screen right away. This is because I set a delay ticks for Trex to finish explosion. Under valgrind mode, the ticks are counted differently. In the regular execution of the game (./run), the game reaches conclusion for both conditions and is bug free / memory leak free.

The game can be restarted at anytime by press "R". 

The following part is implemented in project 5:
The player is an baby dinosaur "Thomas" who can shoot fireball to the herbivorous dinosaurs. Press "D" to move the player to the right, and press "A" to move the player to the left. When the player is moving, his animation changes from "idling" to "walking". Press the space key to shoot an fireball. The interval between two fireballs matches the "shoot" animation. The velocity of the fireball is the velocity of the player plus the initial velocity of the fireball. So it changes when the player is moving. The object pool is used when shooting arrows. Holding "W" while pressing the space key allows the player to shoot upwards. Press "J" to jump and shoot. If "W" is held when "J" is pressed, the player jumps and shoots upwards. The player changes to "dead" when the length of his healthbar drops to zero. But it resumes at the same place right after that.

The herbivorous dinosaurs are smartsprites. All NPC dinosuars have random starting positions and random speeds. They runs away when the Thomas approaches. When the fireball collides with a herbivorous dinosaur, the dinosaur explodes. The exploded dinosaur will be removed from the screen. There is a count for how many dinosaurs have been shot by the player. Depending on the type when the dinosaur is shot, the corresponding count increases. The total number of all herbivorous dinosaur is 20. There is also a count for how many fireballs have been used to shoot all dinosaurs. 

The illusion of depth is implemented by the painter pattern. There are 5 flying birds (scale 0.05-0.1) between the cloud layer and the mountain layer. And there are 5 flying birds (scale 0.4-0.5) between the mountain layer and the grass layer. The other 20 dinosaurs (scale 1.0) are on top of the grass layer and can be shot by the player.  

The IntroHud is shown at the beginning of the game and is toggled with F1. When the hud is displayed, all sprites and the player freeze and are hidden from the screen. The hud will disppear automatically after a few seconds or by pressing F1 again. When the hud is disappeared, the other objects start to move again. The PoolHud is shown at the upright corner and is toggled with F2. Due to the shooting interval based on the animation, the max number of arrows in the bulletlist or the freelist is 2.

The video (dinobaby.mp4) is recorded and submitted in a separated bucket as required by Dr. Malloy.  

My game is absolutely memory leak free to the best of my knowledge.

The sprites are retrieved from:

Angel Dinos: https://www.flaticon.com/packs/dinosaur-set
Steak: https://www.flaticon.com/search?word=steak
Thomas: https://opengameart.org/content/free-dino-sprites
Super Stegosaurus: https://www.freelancer.com/contest/Walking-Dinosaur-Sprite-sheet-1093125-byentry-15469344.html
Skeleton T-Rex: https://www.vectorstock.com/royalty-free-vector/t-rex-bone-running-sequence-vector-1690061


Musics from:
Hatsune Miku Project Diva F
Intro/Menu: Tell Your World
Game (early stage): Weekender Girl
Game (when Boss appears): Senbonzakura
GameOver(winning): Weekender Girl
GameOver(losing): random clip from free 2D game website


