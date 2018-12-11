#include "Hitbox.hpp"

/*
    CONSTRUCTORS
*/

Hitbox::Hitbox() {
    // Really nothing needs to happen in this constructor
}

Hitbox::Hitbox(Texture texture, int numberOfRectangles, bool generateNow) {
    // We begin with variable declarations
    m_texture = texture;
    m_numberOfRectangles = numberOfRectangles;
}

/*
    INTERSECT
*/

bool Hitbox::intersects(FloatRect rect) {

}

/*
    GENERATE
*/

void Hitbox::generate(Vector2f spriteCenter) {
    // First we see if the user supplied an actual vector or if we should use the one we have already
    if (spriteCenter != Vector2f(-1, -1))
        m_center = spriteCenter;
}

/*
    SETTERS
*/

void Hitbox::setTexture(Texture texture) {
    m_texture = texture;
}

void Hitbox::setNumberOfRectangles(int numRects) {
    m_numberOfRectangles = numRects;
}

void Hitbox::setCenter(Vector2f center) {
    m_center.x = center.x;
    m_center.y = center.y;
}

/*
    GETTERS
*/

Texture Hitbox::getTexture() {
    return m_texture;
}

int Hitbox::getNumberOfRectangles() {
    return m_numberOfRectangles;
}

Vector2f Hitbox::getCenter() {
    return m_center;
}