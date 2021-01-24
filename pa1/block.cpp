#include "block.h"

/**
 * Returns the width of the block.
 */
int Block::width() const {
    if (data.size() > 0) {
        return (data[0]).size();
    }
    return 0;
}

/**
 * Returns the height of the block.
 */
int Block::height() const {
    return (this->data).size();
}

/**
 * Default Block constructor.
 */
Block::Block() {/* nothing */}

/**
 * Useful Block constructor.
 * Makes a block from the rectangle of width by height pixels in im
 * whose upper-left corner is at position (x,y).
 */
Block::Block(PNG & im, int x, int y, int width, int height) {
    if ( (y + height > im.height()) || (x + width > im.width()) ){
        cout << "block.cpp, line " << __LINE__ << " - Warning: Wanted block goes outside image" << endl;
        return;
    }
    
    vector< vector <HSLAPixel> > data(height, vector<HSLAPixel>(width, HSLAPixel()));
    for (int i=0; i<height; i++) {
        for (int j=0; j<width; j++) {
            data[i][j] = *(im.getPixel(x + j, y + i));
            //cout << "block.cpp, line " << __LINE__ << endl;
        }
    }
    this->data = data;
}

/**
 * Draws the block at position (x,y) in the image im
 */
void Block::render(PNG & im, int x, int y) const {
    /*cout << "PNG im =" << im << endl;
    cout << "x = " << x << ", y = " << y << endl;
    cout << "this->width = " << this->width() << ", this->height = " << this->height() << endl;*/
    if ( (x + this->width() > im.width()) || (y + this->height() > im.height()) ) {
        cout << "block.cpp, line " << __LINE__ << ": "
            << "WARNING: render() goes outside image" << endl;
    }

    int i, j;
    for ( i=0; i<height(); i++ ) {
        for ( j=0; j<width(); j++ ) {
            HSLAPixel *p = im.getPixel(x + j, y + i);
            *p = data[i][j];
        }
    }

    // CHECK THIS FUNCTION! SEGMENTATION FAULT
    //cout << "block.cpp, line " << __LINE__ <<  ": Block::render(cs221util::PNG, int, int)" << endl;
    /*int blockWidth = width();
    int blockHeight = height();
    if ( (y + blockHeight > im.height()) || (x + blockWidth > im.width()) ){
        cout << "block.cpp, line " << __LINE__ << " - Warning: render() goes outside image" << endl;
    }
    
    for (int i=0; i<blockHeight; i++) {
        for (int j=0; j<blockWidth; j++) {
            //HSLAPixel px = data[i][j];
            //*(im.getPixel(x + j, y + i)) = px;
            *(im.getPixel(x + j, y + i)) = data[i][j];
        }
    }*/
}

/**
 * Changes the saturation of every pixel in the block to 0,
 * which removes the color, leaving grey.
 */
void Block::greyscale() {
    int blockWidth = this->width();
    int blockHeight = this->height();
    
    for(int i=0; i<blockHeight; i++) {
        for(int j=0; j<blockWidth; j++) {
            data[i][j].s=0;
        }
    }
}
