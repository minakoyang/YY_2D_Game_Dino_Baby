#include "introHud.h"

IntroHud::IntroHud() :
  Hud(),
  introHudFrame({
    gd.getXmlInt("introHud/loc/x"),
    gd.getXmlInt("introHud/loc/y"),
    gd.getXmlInt("introHud/width"),
    gd.getXmlInt("introHud/height"),
  }),
  image(ImageFactory::getInstance().getImage("dribbble")),
  imageWidth(image->getWidth()),
  view(Viewport::getInstance()),
  posX(gd.getXmlInt("introHud/loc/x") + static_cast<int>(view.getX())),
  posY(gd.getXmlInt("introHud/loc/y") + 50 + static_cast<int>(view.getY()))

{}

void IntroHud::draw() const{
  if (showHud) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 255,255,255,255/2);
    SDL_RenderFillRect(renderer, &introHudFrame);
    image->draw(posX, posY);
    SDL_SetRenderDrawColor(renderer,255,0,0,255/2);
    SDL_RenderDrawRect(renderer, &introHudFrame);

    io.setFontColor(255,0,0);
    io.writeText(gd.getXmlStr("introHud/name"), 640, 100);

    //io.setFontSize(22);
    io.setFontColor(0,0,0);
    std::string line = "***********************************";
    io.writeText(line, 540, 135);
    io.setFontColor(255,0,0);
    io.writeText(gd.getXmlStr("introHud/left"), 555, 170);
    io.writeText(gd.getXmlStr("introHud/right"), 555, 200);
    io.writeText(gd.getXmlStr("introHud/space"), 555, 230);
    io.writeText(gd.getXmlStr("introHud/up"), 555, 260);
    io.writeText(gd.getXmlStr("introHud/jump"), 555, 290);
    // io.writeText(gd.getXmlStr("introHud/switch"), 555, 320);
    io.writeText(gd.getXmlStr("introHud/god"), 555, 320);
    io.writeText(gd.getXmlStr("introHud/restart"), 555, 350);
    io.writeText(gd.getXmlStr("introHud/hud"), 555, 380);
    io.writeText(gd.getXmlStr("introHud/hud1"), 555, 410);
    io.setFontColor(0,0,0);
    // io.writeText(line, 540, 395);
  }
}

void IntroHud::update() {
  posX = gd.getXmlInt("introHud/loc/x") + static_cast<int>(view.getX());
  // std::cout << view.getX() << " " << posX << std::endl;
  posY = gd.getXmlInt("introHud/loc/y") + 50 + static_cast<int>(view.getY());
}
