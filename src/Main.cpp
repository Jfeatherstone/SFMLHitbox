#include "Hitbox.hpp"

int main() {
    Texture texture;
    texture.loadFromFile("test2.png");
    Hitbox hb(&texture, Detail::More, false);
    hb.addColorToIgnore(Color(111, 94, 54));
    hb.generate();
    hb.drawHitboxText();
}