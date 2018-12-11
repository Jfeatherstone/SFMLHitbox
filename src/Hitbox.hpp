/*
MIT License

Copyright (c) 2018 Jack Featherstone

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

DEPENDENCIES:
std::string
std::map
sf::Texture
sf::FloatRect
std::vector
std::stringstream
*/

#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;
using namespace sf;

/*

SPECIFICATIONS:
SFML 2.5.1
2018 Q4

This class's purpose is to create a grouping of FloatRect variables that represent any given
texture to a user-specified degree of accuracy, which is inversely related to performance impact.
*/

class Hitbox {

private:

	/*
	We may as well store the texture variable that we are passed in for future use
	*/
	Texture m_texture;

	/*
	We also need the center of the sprite to properly detect collisions
	*/
	Vector2f m_center;

	/*
	Since we may not want or need to define every pixel as its own rectangle, we want to be able to define
	some integer that represents the accuracy or number of rectangles in the hitbox
	*/
	int m_numberOfRectangles;

	/*
	The main piece of the class will be a vector of varying length that will hold several FloatRect
	objects.
	*/
	vector<FloatRect> m_rects;
public:

	/*
	We will have two constructors for the class:
	One that takes in a texture and generates the hitbox then,
	and one that just creates the variable to be generated later
	*/
	Hitbox();
	Hitbox(Texture texture, int numberOfRectangles, bool generateNow = true);

	/*
	Of course the point of this class is to be able to detect collisions with other objects,
	and so we have to have a method that can do that. Essentially, this will run through all
	of the hitbox rects and see if any of them individually intersect.

	As far as the future, it may be helpful to know which part of the hitbox intersects, but I
	can't think of a good way to denote this without going into specifics about the sprite.
	i.e. If we do it by Head, Feet, Arms, etc. then how could a snake animal be denoted or something
	that isn't a human
	*/
	bool intersects(FloatRect rect);

	/*
	We also want to be able to generate our hitbox based on the most recent variable assignments,
	including placing the hitbox at the appropriate location on the screen

	This position should be the center of the sprite on the screen
	*/
	void generate(Vector2f spriteCenter = Vector2f(-1, -1));

	/*
	Several setters/getters for general use/modification:
	*/
	
	void setTexture(Texture texture);
	void setNumberOfRectangles(int numRects);
	void setCenter(Vector2f center);

	Texture getTexture();
	int getNumberOfRectangles();
	Vector2f getCenter();

	vector<FloatRect> getFloatRects();
	vector<RectangleShape> getHitboxDrawable();

};