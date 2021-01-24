#include "fadeColorPicker.h"

fadeColorPicker::fadeColorPicker(double fade)
{
    /* your code here */
    fadeFactor = fade;
}

HSLAPixel fadeColorPicker::operator()(point p)
{
    /* your code here */
    int xDist = abs(p.x - p.c.x); // x-diff between point and its centre
    int yDist = abs(p.y - p.c.y);
    int dist = xDist * xDist + yDist * yDist; // squared Euclidean distance
    
    HSLAPixel px;
    double oldL = p.c.color.l;
    px.l = oldL * pow(fadeFactor, sqrt(dist));
    px.h = p.c.color.h;
    px.s = p.c.color.s;
    return px;
}
