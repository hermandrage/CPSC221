//
//  customColorPicker.hpp
//  
//
//  Created by Ingrid Elisabeth Tveten on 25/10/2019.
//

#ifndef CUSTOMCOLORPICKER_H
#define CUSTOMCOLORPICKER_H

#include <stdio.h>
#include "colorPicker.h"
#include "point.h"
#include "center.h"
#include <cmath>
#include <vector>
#include <map>
#include <string>
using namespace std;

// For now just copied from dotColorPicker and changed names

/**
 * customColorPicker: a functor that determines the color that should be used
 * given an x and a y coordinate using a pattern of dots on a grid.
 *
 */
class customColorPicker : public colorPicker
{
  public:
    /**
     * Constructs a new customColorPicker.
     */
    customColorPicker();

    /**
     * Picks the color for pixel (x, y).
     *
     * @param p The p to pick a color for.
     * @return The color chosen for (x, y).
     */
    virtual HSLAPixel operator()(point p);

  private:

    map<string, cs221util::HSLAPixel> norway;

};



#endif /* CUSTOMCOLORPICKER_H */
