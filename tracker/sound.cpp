#include <string>
#include <iostream>
#include <ctime>
#include "sound.h"
using std::string;

SDLSound& SDLSound::getInstance() {
  static SDLSound instance;
  return instance;
}

SDLSound::~SDLSound() {
  std::cout << "Cleaning up sounds ..." << std::endl;
  std::clock_t start = std::clock();
  Mix_HaltMusic();
  //Mix_FreeMusic(music);
  for (auto music : musics) {
    Mix_FreeMusic(music);
  }
  for (unsigned int i = 0; i < sounds.size(); ++i) {
    Mix_FreeChunk(sounds[i]);
  }
  Mix_CloseAudio();
  std::clock_t duration = std::clock() - start;
  std::cout << "Clean up took " << duration << " ticks\n";
}

SDLSound::SDLSound() :
  volume(SDL_MIX_MAXVOLUME/2),
  currentSound(-1),
  currentMusic(START),
  audioRate(22050),
  audioChannels(2),
  audioBuffers(4096),
  musics(),
  sounds(),
  channels()
{

  if(Mix_OpenAudio(audioRate, MIX_DEFAULT_FORMAT, audioChannels,
                   audioBuffers)){
    throw string("Unable to open audio!");
  }
  //music = Mix_LoadMUS("sound/100Years.mp3");
  // Need to install midi to play the following:
  // music = Mix_LoadMUS("sound/ballad2.mid");
  musics.push_back( Mix_LoadMUS("sound/start.mp3"));
  musics.push_back( Mix_LoadMUS("sound/game.mp3"));
  musics.push_back( Mix_LoadMUS("sound/end.mp3"));
  musics.push_back( Mix_LoadMUS("sound/intro_hud.mp3"));
  musics.push_back( Mix_LoadMUS("sound/fight.mp3"));
  musics.push_back( Mix_LoadMUS("sound/lose.mp3"));

  sounds.push_back( Mix_LoadWAV("sound/fire.wav") );
  sounds.push_back( Mix_LoadWAV("sound/jump.wav") );
  sounds.push_back( Mix_LoadWAV("sound/hitbytrex.mp3") );


  // if (!music) throw string("Couldn't load 100Years.mp3")+Mix_GetError();
  //
  // startMusic();
  //
  // sounds.push_back( Mix_LoadWAV("sound/Boom.wav") );
  // sounds.push_back( Mix_LoadWAV("sound/Gun.wav") );
  // sounds.push_back( Mix_LoadWAV("sound/Thump.wav") );
  // sounds.push_back( Mix_LoadWAV("sound/phaser.wav") );
  // sounds.push_back( Mix_LoadWAV("sound/Putt1.wav") );
  // sounds.push_back( Mix_LoadWAV("sound/explosion.wav") );
  // sounds.push_back( Mix_LoadWAV("sound/cannon.wav") );
  // sounds.push_back( Mix_LoadWAV("sound/Larc.wav") );
  for (unsigned int i = 0; i < sounds.size(); ++i) channels.push_back(i);
  std::cout << "Music and Sound is loaded" << std::endl;
}

void SDLSound::toggleMusic() {
  if( Mix_PausedMusic() ) {
    Mix_ResumeMusic();
  }
  else {
    Mix_PauseMusic();
  }
}

void SDLSound::operator[](int index) {
  if (currentSound >= 0) Mix_HaltChannel(currentSound);
  currentSound = index;
  Mix_VolumeChunk(sounds[index], volume);
  channels[index] =
     Mix_PlayChannel(-1, sounds[index], 0);
}

void SDLSound::playStartMusic() {
  if (currentMusic != START) {
    Mix_HaltMusic();
    Mix_VolumeMusic(volume);
    Mix_PlayMusic(musics[0], -1);
    currentMusic = START;
  }  
}

void SDLSound::playGameMusic() {
  if (currentMusic != GAME && currentMusic != FIGHT) {
    Mix_HaltMusic();
    Mix_VolumeMusic(volume);
    Mix_PlayMusic(musics[1], -1);
    currentMusic = GAME;
  }  
}

void SDLSound::playEndMusic() {
  if (currentMusic != END) {
    Mix_HaltMusic();
    Mix_VolumeMusic(volume);
    Mix_PlayMusic(musics[2], -1);
    currentMusic = END; 
  } 
}

void SDLSound::playIntroMusic() {
  if (currentMusic != INTRO) {
    Mix_HaltMusic();
    Mix_VolumeMusic(volume);
    Mix_PlayMusic(musics[3], -1);
    currentMusic = INTRO;
  }    
}

void SDLSound::playFightMusic() {
  if (currentMusic != FIGHT) {
    // std::cout << currentMusic;
    Mix_HaltMusic();
    Mix_VolumeMusic(volume);
    Mix_PlayMusic(musics[4], -1);
    currentMusic = FIGHT;
  }
}

void SDLSound::playLoseMusic() {
  if (currentMusic != LOSE)  {
    Mix_HaltMusic();
    Mix_VolumeMusic(volume);
    Mix_PlayMusic(musics[5], -1);
    currentMusic = LOSE;
  }
}

void SDLSound::stopMusic() {
  Mix_HaltMusic();
  for (auto music : musics) {
    Mix_FreeMusic(music);
  }
}
