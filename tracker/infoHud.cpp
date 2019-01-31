#include "infoHud.h"
#include <sstream>

InfoHud::InfoHud() :
  Hud(),
  infoHudFrame({
    gd.getXmlInt("infoHud/loc/x"),
    gd.getXmlInt("infoHud/loc/y"),
    gd.getXmlInt("infoHud/width"),
    gd.getXmlInt("infoHud/height"),
  })
  {}


void InfoHud::draw() const{
  if (showHud) {
    
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 255,255,255,255/2);
    SDL_RenderFillRect(renderer, &infoHudFrame);

    SDL_SetRenderDrawColor(renderer, 255,0,0,255/2);
    SDL_RenderDrawRect(renderer, &infoHudFrame);

    //io.setFontSize(20);
    //io.setFontColor(0,0,0);
    //std::string line = "***************************";
    //io.writeText(line, 50, 125);
    std::stringstream ss;
    io.setFontColor(255,0,0);
    ss << "Stegosaurus: ";
    io.writeText(ss.str(), 40, 140);

    ss.clear();
    ss.str("");
    ss << "Diplodocus: ";
    io.writeText(ss.str(), 40, 180);

    ss.clear();
    ss.str("");
    ss << "Parasaurolophus: ";
    io.writeText(ss.str(), 40, 220);

    ss.clear();
    ss.str("");
    ss << "Triceratops: ";
    io.writeText(ss.str(), 40, 260);

    // ss.clear();
    // ss.str("");
    // ss << "Super Stegosaurus: ";
    // io.writeText(ss.str(), 40, 290);

    // ss.clear();
    // ss.str("");
    // ss << "Vegetable: ";
    // io.writeText(ss.str(), 40, 320);

    //io.setFontColor(255,0,0);
    //io.writeText("Stegosaurus: ", 60, 170);
    //io.setFontColor(255,0,0);
    //io.writeText("Diplodocus: ", 60, 200);
    //io.setFontColor(255,0,0);
    //io.writeText("Parasaurolophus: ", 60, 230);
    //io.setFontColor(255,0,0);
    //io.writeText("Steak", 60, 260);
    //io.setFontColor(255,0,0);
    //io.writeText("Vegetable: ", 60, 290);
    //io.setFontColor(255,0,0);
    //io.writeText("Super Stegosaurus: ", 60, 320);
    //io.setFontColor(0,0,0);
    //io.writeText(line, 50,375);
  }
}
