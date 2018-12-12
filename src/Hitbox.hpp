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

This class's purpose is to create a polygon variable that represents any given
texture to a user-specified to a hopefully good amount of detail
*/

enum class Detail {More, Less};

class Hitbox: public ConvexShape {

private:

	/*
	We also need the center of the sprite to properly detect collisions
	*/
	Vector2f m_center;

	Vector2i m_textureSize;

	/*
	This will define whether we want to outline every detail in our sprite or just get
	the general shape of it. The big difference here is whether we outline concave parts
	of the texture or not.
	*/
	Detail m_detail;

	/*
	The actual hitbox itself will be of the type sf::ConvexShape
	I decided to go with extending the ConvexShape class onto this one, so we don't actually
	need this variable, we can just use this
	*/
	//ConvexShape m_hitbox;
	int m_numVerticies;

	/*
	We also may want to access the array of colors in the texture, so we store that

	Along with this, we also want have a minimal version of the pixels that only states whether they
	will be included in the hitbox calcuation or not, denoted by 0 or 1
	*/
	vector<Color> m_pixels;
	vector<int> m_hitboxInclude;

	vector<Vector2f> m_hitboxVertices;
	/*
	Since we are checking through every pixel and can get its exact rgb code, we can whitelist certain
	colors if we want to have a visual there but no corresponding hitbox.
	*/
	vector<Color> m_ignoredColors;

	/*
	And a method to turn a texture in a vector of colors
	*/
	void getPixels();
	bool contains(vector<Color> vec, Color c);
	bool hitboxContainsPoint(Vector2f point);
	void countVerticies();
public:

	/*
	We will have two constructors for the class:
	One that takes in a texture and generates the hitbox then,
	and one that just creates the variable to be generated later
	*/
	Hitbox();
	Hitbox(Texture* texture, Detail detail, bool generateNow = true);

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
	void generate();

	/*
	We also want to just be able to update the position of the hitboxes, not redraw all of them
	*/
	void update(Vector2f spriteCenter = Vector2f(-1, -1));

	/*
	Several setters/getters for general use/modification:
	*/
	
	ConvexShape getDrawable();

	void addColorToIgnore(Color c);

	void drawHitboxText();
};