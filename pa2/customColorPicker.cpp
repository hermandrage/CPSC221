//
//  customColorPicker.cpp
//  
//
//  Created by Ingrid Elisabeth Tveten on 25/10/2019.
//

#include "customColorPicker.h"


customColorPicker::customColorPicker() {

    
    HSLAPixel red(351, 0.82, 0.40);
    HSLAPixel white(0, 0.0, 1.0);
    HSLAPixel blue(219, 1.0, 0.18);
    //this->norway = vector<cs221util::HSLAPixel>({red, white, blue});
    
    this->norway = map<string, cs221util::HSLAPixel>();
    norway["red"] = red;
    norway["white"] = white;
    norway["blue"] = blue;
}


HSLAPixel customColorPicker::operator()(point p) {
    // widths of 6:1:2:1:12 = 22
    // lengths of 6:1:2:1:6 = 15
    int x = p.x;
    int y = p.y;
    int cx = p.c.x;
    int cy = p.c.y;

    if ( x==cx || y==cy )
        return norway["blue"];
    else if ( abs(x-cx)==1 || abs(y-cy)==1 )
        return norway["white"];
    else
        return norway["red"];

    //return norway[p.level % 3];
}