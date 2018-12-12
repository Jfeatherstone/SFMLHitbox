#include "Hitbox.hpp"

/*
BUG:
line ~170
For some reason, whenever we check outside of the bounds of the vector, we find that the held
value is one, which results in some of the bottom most border being oddly parameterized

BUG:
line ~120
The level of detail supplied doesn't actually make a difference because when we are determining
the inside, we pretty much remove any details present.
*/

/*
    CONSTRUCTORS
*/

Hitbox::Hitbox() {
    // Really nothing needs to happen in this constructor
}

Hitbox::Hitbox(Texture* texture, Detail detail, bool generateNow) {
    // We begin with variable declarations
    setTexture(texture);    
    m_detail = detail;
    // Now we generate our hitbox
    if (generateNow)
        generate();
}

/*
    INTERSECT
*/

bool Hitbox::intersects(FloatRect rect) {

}

/*
    GENERATE
*/

void Hitbox::generate() {
    // First, we want to make sure we have the most updated pixel array
    getPixels();
    // Next, we want to go through every color and if it isn't empty or on the ignore list, we
    // add it to our include for the hitbox calculation
    m_hitboxInclude.clear();
    m_hitboxInclude.resize(m_pixels.size());

    m_textureSize.x = getTexture()->getSize().x;
    m_textureSize.y = getTexture()->getSize().y;

    int i = 0;

    for (Color c: m_pixels) {
        if (!contains(m_ignoredColors, c) && (int) c.a > 0) {
            //cout << (int)c.r << " " << (int)c.g << " " << (int)c.b << " " << (int)c.a << endl;
            m_hitboxInclude[i] = 1;
        }
        else
            m_hitboxInclude[i] = 0;
        i++;
    }

    cout << endl;
    drawHitboxText();
    cout << endl;
    cout << endl;

    // We now have an array of pixels that should be included in our hitboxes
    // The next task is to somehow fit n rectangles to best represent those areas

    /*
    Approach #1 - Square Magnification
    In this approach, we will find any groupings of 4 included pixels and group them together,
    and then repeat for groupings of 16, and so on.
     
    vector<int> newHitbox;
    for (int i = 0; i < m_hitboxInclude.size(); i+= 2) {
        if (m_hitboxInclude[i] == 1 && m_hitboxInclude[i + 1] == 1
        && m_hitboxInclude[i + getTexture().getSize().x] == 1 && m_hitboxInclude[i + getTexture().getSize().x + 1] == 1)
            newHitbox.push_back(1);

        if (m_hitboxInclude[i] == 0 && m_hitboxInclude[i + 1] == 0
        && m_hitboxInclude[i + getTexture().getSize().x] == 0 && m_hitboxInclude[i + getTexture().getSize().x + 1] == 0)
            newHitbox.push_back(0);

        if (i % m_textureSize.x == 0)
            i += m_textureSize.x;
    }

    m_textureSize.x /= 2;
    m_textureSize.y /= 2;

    m_hitboxInclude = newHitbox;

    Status: Doesn't work at all
    */

   /*
    Approach #2 - Outline
    Here, we can assume that only the outline of the sprite matters for the hitbox, and so we can just fill in the rest
    inside
    This still leaves the problem of fitting rectangles, but maybe gets us a step closer :)
     */

    for (int i = 0; i < m_textureSize.y; i++) {
        int first, last;
        for (int j = 0; j < m_textureSize.x; j++) {
            if (m_hitboxInclude[i*m_textureSize.x + j] == 1) {
                first = j;
                break;
            }
        }

        for (int j = m_textureSize.x; j > 0; j--) {
            if (m_hitboxInclude[i*m_textureSize.x + j] == 1) {
                last = j;
                break;
            }
        }

        for (int j = first; j < last; j++) {
            if (m_hitboxInclude[(i-1)*m_textureSize.x + j] == 1 || m_hitboxInclude[(i+1)*m_textureSize.x + j] == 1)
                m_hitboxInclude[i*m_textureSize.x + j] = 1;
        }
    }
    cout << "Detailed" << endl;
    cout << endl;
    drawHitboxText();
    cout << endl;
    cout << endl;

    // Now, we go through and remove everything except for the outline
    vector<int> m_newHitbox;
    m_newHitbox.resize(m_hitboxInclude.size());

    for (int i = 0; i < m_textureSize.y; i++) {
        for (int j = 0; j < m_textureSize.x; j++) {
            // We check if everything surrounding it is a 1 which would denote something inside
            /*
            An explanation of this ungodly if statement:
            To determine whether a pixel is inside of the sprite -- and therefore not needed to determine the
            hitbox -- we have to look at any of the pixels are it.
            The first line of the statement makes sure that the value to the left is included in the hitbox, and
            that it isn't on the end of the previous line (since this isn't actually a 2d array, rather its 1d)

            The second line does the same as the first, but with the right side, checking that the value is 1 and
            that it isn't past the end of the line (onto the next one)

            Next, the line checks that both above and below the pixel are included, which don't need weird
            cases like left and right since they are just up and down

            Lastly, we make sure that we aren't at the bottom line. This was causing an error where some
            on the last line weren't included and I have no idea why, but this seemed like a good way
            to fix it :)
            */
            m_newHitbox[i*m_textureSize.x + j] = m_hitboxInclude[i*m_textureSize.x + j];

            if ((m_hitboxInclude[i*m_textureSize.x + j - 1] == 1 && i*m_textureSize.x + j - 1 >= i*m_textureSize.x)
            && (m_hitboxInclude[i*m_textureSize.x + j + 1] == 1 && i*m_textureSize.x + j + 1 < (i+1)*m_textureSize.x)
            && (m_hitboxInclude[(i-1)*m_textureSize.x + j] == 1) && (m_hitboxInclude[(i+1)*m_textureSize.x + j] == 1)
            && ((i+1)*m_textureSize.x + j < m_hitboxInclude.size()))
            
            m_newHitbox[i*m_textureSize.x + j] = 2;
        }
    }
    m_hitboxInclude = m_newHitbox;

    cout << endl;
    drawHitboxText();
    cout << endl;
    cout << endl;

    //m_newHitbox.clear();

    // The final process is to remove points in between straight sections, which would cause
    // uneccessary points on our hitbox
    for (int i = 0; i < m_textureSize.y; i++) {
        for (int j = 0; j < m_textureSize.x; j++) {
            // First we check for vertical lines
            /*
            Another horrible if statement, I know...
            
            The first line checks that the pixel itself is included. This is first such that we don't needlessly
            evaluate the rest of the statements when it doesn't really matter either way because the pixel is
            already not included.
            The second line checks that the pixel to the left is either not included or in the previous line
            The third line checks that the pixel to the right is either not included or in the next line
            The fourth line looks at the two pixels to the up right and up left that they are not included
            The fifth line looks at the two pixels to the down right and down left that they are not included
                
            
            if ((m_hitboxInclude[(i-1)*m_textureSize.x + j] == 1 && (i-1)*m_textureSize.x + j >= 0)
            && (m_hitboxInclude[(i+1)*m_textureSize.x + j] == 1 && (i+1)*m_textureSize.x + j + 1 < m_hitboxInclude.size())
            && (m_hitboxInclude[i*m_textureSize.x + j + 1] != 1)
            && (m_hitboxInclude[i*m_textureSize.x + j - 1] != 1))
            m_newHitbox[i*m_textureSize.x + j] = 3;
                */
            if (m_hitboxInclude[i*m_textureSize.x + j] == 1
            && ((m_hitboxInclude[i*m_textureSize.x + j - 1] == 0 || m_hitboxInclude[i*m_textureSize.x + j - 1] == 2) || i*m_textureSize.x + j - 1 < i*m_textureSize.x)
            && ((m_hitboxInclude[i*m_textureSize.x + j + 1] == 0 || m_hitboxInclude[i*m_textureSize.x + j + 1] == 2) || i*m_textureSize.x + j + 1 > (i+1)*m_textureSize.x)
            && (m_hitboxInclude[(i-1)*m_textureSize.x + j - 1] == 0 || m_hitboxInclude[(i-1)*m_textureSize.x + j - 1] == 2)
            && (m_hitboxInclude[(i-1)*m_textureSize.x + j + 1] == 0 || m_hitboxInclude[(i-1)*m_textureSize.x + j + 1] == 2)
            && (m_hitboxInclude[(i+1)*m_textureSize.x + j - 1] == 0 || m_hitboxInclude[(i+1)*m_textureSize.x + j - 1] == 2)
            && (m_hitboxInclude[(i+1)*m_textureSize.x + j + 1] == 0 || m_hitboxInclude[(i+1)*m_textureSize.x + j + 1] == 2))
            m_newHitbox[i*m_textureSize.x + j] = 3;
                
                /*
            /*
            This statement is much simpler, and the previous could probably be too, but I don't feel like doing that
            right now. This just checks that both the left and right pixels are either on a different line or are
            included.
            */
            if ((m_hitboxInclude[i*m_textureSize.x + j - 1] == 1 && i*m_textureSize.x + j - 1 >= i*m_textureSize.x)
            && (m_hitboxInclude[i*m_textureSize.x + j + 1] == 1 && i*m_textureSize.x + j + 1 < (i+1)*m_textureSize.x))
            m_newHitbox[i*m_textureSize.x + j] = 3;

        }
    }
    m_hitboxInclude = m_newHitbox;

    cout << endl;
    drawHitboxText();
    cout << endl;
    cout << endl;

    /*
    Crunch time!
    We now have every pixel that will represent a vertex in our hitbox polygon, the problem is
    now to put them in order so that it makes a corherent shape.

    We can't just go across and add each one, because then the shape would be zig-zagged and not
    work.

    We need to find a way to assign each pixel in the outline by following the path around our
    shape.

    Since we have an outline of the sprite (where the lines should go) indicated by a 3 in our vector,
    we should be able to begin at a given pixel and move along in one direction
     */
    Vector2f currPixel;
    int vertexIndex = 0;
    m_hitboxVertices.clear();
    m_hitboxVertices.resize(m_hitboxInclude.size()); // This is a little overkill, but that's okay

    // Setup our polygon
    countVerticies();
    setPointCount(m_numVerticies);

    while (vertexIndex < m_numVerticies) {
        if (m_hitboxInclude[currPixel.y*m_textureSize.x + currPixel.x] == 1 || m_hitboxInclude[currPixel.y*m_textureSize.x + currPixel.x] == 3) {
            // Even if it isn't an actual vertex, we record it in our other vector
            m_hitboxVertices.push_back(currPixel);
            
            if (m_hitboxInclude[currPixel.y*m_textureSize.x + currPixel.x] == 1)
                // We record the vertex in our polygon
                setPoint(vertexIndex++, currPixel);
            // We now look for the next pixel that is marked either as a 3 or a 1
            /*
            We check starting at the top and move clockwise 
             */
            //cout << currPixel.x << " " << currPixel.y << " " << vertexIndex << endl;
            //for (Vector2f v: m_hitboxVertices)
              //  cout << v.x << " " << v.y << " --- ";

            // Top
            if ((m_hitboxInclude[(currPixel.y - 1)*m_textureSize.x + currPixel.x] == 1 
            || m_hitboxInclude[(currPixel.y - 1)*m_textureSize.x + currPixel.x] == 3)
            && !hitboxContainsPoint(Vector2f(currPixel.x, currPixel.y - 1))) {
                currPixel.x += 0;
                currPixel.y += -1;
                continue;
            } else
            // Top right
            if ((m_hitboxInclude[(currPixel.y - 1)*m_textureSize.x + currPixel.x + 1] == 1 
            || m_hitboxInclude[(currPixel.y - 1)*m_textureSize.x + currPixel.x + 1] == 3)
            && !hitboxContainsPoint(Vector2f(currPixel.x + 1, currPixel.y - 1))) {
                currPixel.x += 1;
                currPixel.y += -1;
                continue;
            } else
            // Right
            if ((m_hitboxInclude[(currPixel.y)*m_textureSize.x + currPixel.x + 1] == 1 
            || m_hitboxInclude[(currPixel.y)*m_textureSize.x + currPixel.x + 1] == 3)
            && !hitboxContainsPoint(Vector2f(currPixel.x + 1, currPixel.y))) {
                currPixel.x += 1;
                currPixel.y += 0;
                continue;
            } else
            // Bottom right
            if ((m_hitboxInclude[(currPixel.y + 1)*m_textureSize.x + currPixel.x + 1] == 1 
            || m_hitboxInclude[(currPixel.y + 1)*m_textureSize.x + currPixel.x + 1] == 3)
            && !hitboxContainsPoint(Vector2f(currPixel.x + 1, currPixel.y + 1))) {
                currPixel.x += 1;
                currPixel.y += 1;
                continue;
            } else
            // Bottom
            if ((m_hitboxInclude[(currPixel.y + 1)*m_textureSize.x + currPixel.x] == 1 
            || m_hitboxInclude[(currPixel.y + 1)*m_textureSize.x + currPixel.x] == 3)
            && !hitboxContainsPoint(Vector2f(currPixel.x, currPixel.y + 1))) {
                currPixel.x += 0;
                currPixel.y += 1;
                continue;
            } else
            // Bottom left
            if ((m_hitboxInclude[(currPixel.y + 1)*m_textureSize.x + currPixel.x - 1] == 1 
            || m_hitboxInclude[(currPixel.y + 1)*m_textureSize.x + currPixel.x - 1] == 3)
            && !hitboxContainsPoint(Vector2f(currPixel.x - 1, currPixel.y + 1))) {
                currPixel.x += -1;
                currPixel.y += 1;
                continue;
            } else
            // Left
            if ((m_hitboxInclude[(currPixel.y)*m_textureSize.x + currPixel.x - 1] == 1 
            || m_hitboxInclude[(currPixel.y)*m_textureSize.x + currPixel.x - 1] == 3)
            && !hitboxContainsPoint(Vector2f(currPixel.x - 1, currPixel.y))) {
                currPixel.x += -1;
                currPixel.y += 0;
                continue;
            } else
            // Top left
            if ((m_hitboxInclude[(currPixel.y - 1)*m_textureSize.x + currPixel.x - 1] == 1 
            || m_hitboxInclude[(currPixel.y - 1)*m_textureSize.x + currPixel.x - 1] == 3)
            && !hitboxContainsPoint(Vector2f(currPixel.x - 1, currPixel.y - 1))) {
                currPixel.x += -1;
                currPixel.y += -1;
                continue;
            } else
                break;
            
        } else {
            // Just move horizontally until we get on the path of the first vertex
            currPixel.x += 1;
        }
    }
    setOrigin(getLocalBounds().width / 2, getLocalBounds().height / 2);

    for (int i = 0; i < getPointCount(); i++) {
        cout << getPoint(i).x << " " << getPoint(i).y << endl;
    }
   
}

/*
    UPDATE
*/
void Hitbox::update(Vector2f spriteCenter) {
    // First we see if the user supplied an actual vector or if we should use the one we have already
    if (spriteCenter.x != -1 && spriteCenter.y != -1) {
        m_center.x = spriteCenter.x;
        m_center.y = spriteCenter.y;
    }

    setPosition(m_center.x, m_center.y);
    //setFillColor(Color::Blue);
    setOutlineColor(Color::Red);
    //setOutlineThickness(2);
    setScale(3, 3);
}

/*
    SETTERS
*/

void Hitbox::addColorToIgnore(Color c) {
    m_ignoredColors.push_back(c);
}

/*
    GETTERS
*/

void Hitbox::drawHitboxText() {
    for (int i = 0; i < m_hitboxInclude.size(); i++) {
        if (i % m_textureSize.x == 0 && i > 0)
            cout << endl;
        cout << m_hitboxInclude[i];

    }
}

/*
    PRIVATE STUFF
*/

void Hitbox::getPixels() {

    // Convert our texture to an image
    Image image = getTexture()->copyToImage();

    // This will be the number of pixels in the image
    int length = image.getSize().x * image.getSize().y;

    // Reassign our pixel vector
    m_pixels.clear();

    // The actual array of pixels
    const Uint8* arr = image.getPixelsPtr();

    // Separate out each color and create an object to add to our vector
    for (int i = 0; i < length; i++) {
        int red = (int) arr[4*i];
        int green = (int) arr[4*i + 1];
        int blue = (int) arr[4*i + 2];
        int alpha = (int) arr[4*i + 3];
        Color c(red, green, blue, alpha);
        m_pixels.push_back(c);
    }

}

bool Hitbox::contains(vector<Color> vec, Color c) {
    for (Color col: vec) {
        if (col == c)
            return true;
    }
    return false;
}

bool Hitbox::hitboxContainsPoint(Vector2f point) {
    //cout << point.x << " " << point.y << endl;
    for (int i = 0; i < m_hitboxVertices.size(); i++) {
        if (point.x == m_hitboxVertices[i].x && point.y == m_hitboxVertices[i].y) {
            //cout << "True" << endl;
            return true;
        }
    }
    //cout << "False" << endl;
    return false;
}

void Hitbox::countVerticies() {
    m_numVerticies = 0;
    for (int i: m_hitboxInclude) {
        if (i == 1)
            m_numVerticies++;
    }
}