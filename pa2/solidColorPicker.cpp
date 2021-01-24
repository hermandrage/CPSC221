#include "solidColorPicker.h"

solidColorPicker::solidColorPicker()
{
    /* your code here  :)  */
    
    // nothing
}

HSLAPixel solidColorPicker::operator()(point p)
{
    /* your code here */
    return p.c.color;
}
