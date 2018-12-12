# Hitbox

The purpose of this project is to create an easy way to generate hitboxes from a given texture, with a varying degree of accuracy specified by the user.

# What it can do

As of right now (and probably for the foreseeable future) this code *cannot* actually create hitboxes for SFML textures. What it can do so far, is to take a texture, break it into pixels, and find every pixel that should be included in a hitbox. It will then fill in any inside gaps and remove everything but the outline of the texture. It will then clean up this outline and remove any pixels that are not verticies (i.e. anything that doesn't define a change in how the polygon would look).

# What it can't do

The whole reason I started this project was to create a modular way to assign hitboxes for large amounts of textures in a game. The reason I am no longer working on this project is because the ability to do this is not actually possible given the structure of my idea. sf::ConvexPolygon -- the class that I would be creating here -- does not actually have an intersect function like sf::FloatRect, which is something that I only found out after several hours of coding. This means that even if we could properly model our texture as this polygon class, it would be no more useful that the actual texture itself.

I am currently trying to figure out another way to detect collisions in a modular fashion, and will probably link that repo here when I find some success. 