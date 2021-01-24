/**
 * @file vor.cpp
 * Implementation of functions in the filler namespace. 
 *
 */
//included within  "vor.h" for template instantiation


/* vorDotDFS given as an example for PA2 */
animation filler::vorDotDFS(PNG& img, double density,
                                int dotGrid,int dotSize, int frameFreq) {
    dotColorPicker a(dotGrid,dotSize);
    return vor<Stack>(img, density, a, frameFreq);
}

animation filler::vorSolidDFS(PNG& img, double density, int frameFreq) {
    /**
     * @todo Your code here! 
     */
    solidColorPicker s;
    return vor<Stack>(img, density, s, frameFreq);
}

animation filler::vorFadeDFS(PNG& img, double density, double fadeFactor, int frameFreq) {
    /**
     * @todo Your code here! 
     */
    fadeColorPicker f(fadeFactor);
    return vor<Stack>(img, density, f, frameFreq);
}


/* vorDotBFS given as an example for PA2 */
animation filler::vorDotBFS(PNG& img, double density,
                                int dotGrid,int dotSize, int frameFreq) {
    
    dotColorPicker a(dotGrid, dotSize);
    return vor<Queue>(img, density, a, frameFreq);
}

animation filler::vorSolidBFS(PNG& img, double density, int frameFreq) {
    /**
     * @todo Your code here! 
     */
    solidColorPicker s;
    return vor<Queue>(img, density, s, frameFreq);
}

animation filler::vorFadeBFS(PNG& img, double density, double fadeFactor, int frameFreq) {
    /**
     * @todo Your code here! 
     */
    fadeColorPicker f(fadeFactor);
    return vor<Queue>(img, density, f, frameFreq);
}

/*
// **** OWN COLOR PICKER **** 
animation filler::vorCustomDFS(PNG& img, double density, int frameFreq) {
    customColorPicker c;
    return vor<Stack>(img, density, c, frameFreq);
}

animation filler::vorCustomBFS(PNG& img, double density, int frameFreq) {
    customColorPicker c;
    return vor<Queue>(img, density, c, frameFreq);
}
*/


bool filler::withinDistance(point p) {
    int xDist = abs(p.x - p.c.x);
    int yDist = abs(p.y - p.c.y);
    int distSq = xDist * xDist + yDist * yDist;
    int levelSq = p.level * p.level;
    return ( distSq <= levelSq );
}

/* 
* processPoint checks conditions described below:
* Among the 8 candidate neighbors of curr (which has level L), a candidate should
* be added to the OS if
*       i) it is on the image,
*       ii) not previously processed,
*       iii) it's location is within Euclidean distance (L+1) of its center.
* If all of those thing are true, then the candidate's Level should be set to L+1,
* it should be marked processed, its color set, and it should be added to the OS.
*/
void filler::processPoint(PNG& img, point p, colorPicker& fillColor,
        int* processed, animation& a, OrderingStructure<point>& os,
        int& count, const int frameFreq) {
    
    if ( 0 <= p.x && p.x < img.width() && 0 <= p.y && p.y < img.height() ) {
        if ( ! processed[p.y * img.width() + p.x] ) {
            if ( withinDistance(p) ) {
                // Process image
                HSLAPixel * pix = img.getPixel(p.x, p.y);
                *pix = fillColor(p);
                
                // Add to OS and mark as processed
                os.add(p);
                processed[p.y * img.width() + p.x] = 1;

                // Add to animation if we should
                count++;
                if ( count % frameFreq == 0 ) a.addFrame(img);
            }
        }
    } // end if's

}


void filler::processNeighbours(PNG& img, point p, colorPicker& fillColor,
        int* processed, animation& a, OrderingStructure<point>& os,
        int& count, const int frameFreq) {
    /*
    * Add its neighbours to the ordering structure in order
    *       UP(-y), UPLEFT(-x,-y), LEFT(-x), LEFTDOWN(-x,+y),
    *       DOWN(+y), DOWNRIGHT(+x,+y), RIGHT(+x), RIGHTUP(+x,-y)
    * by calling processPoint()
    */
    point neighbour(p.x, p.y, p.c, p.level+1);

    neighbour.y--; // UP
    processPoint(img, neighbour, fillColor, processed, a, os, count, frameFreq);
    neighbour.x--; // UPLEFT
    processPoint(img, neighbour, fillColor, processed, a, os, count, frameFreq);
    
    neighbour.y++; // LEFT
    processPoint(img, neighbour, fillColor, processed, a, os, count, frameFreq);
    neighbour.y++; // LEFTDOWN
    processPoint(img, neighbour, fillColor, processed, a, os, count, frameFreq);
    
    neighbour.x++; // DOWN
    processPoint(img, neighbour, fillColor, processed, a, os, count, frameFreq);
    neighbour.x++; // DOWNRIGHT
    processPoint(img, neighbour, fillColor, processed, a, os, count, frameFreq);
    
    neighbour.y--; // RIGHT
    processPoint(img, neighbour, fillColor, processed, a, os, count, frameFreq);
    neighbour.y--; // RIGHTUP
    processPoint(img, neighbour, fillColor, processed, a, os, count, frameFreq);
    
}


template <template <class T> class OrderingStructure>
animation filler::vor(PNG& img, double density, colorPicker& fillColor, int frameFreq) {

    /**
     * @todo You need to implement this function!
     *
     * This is a general description of a space filling algorithm, where 
     * the space is partitioned into a set of regions each color of which
     * is determined by the color of a point 
     * chosen from an original image. We call those original points
     * "centers." (See pictures in the specification.)
     *
     * The algorithm requires ordering structures used to orchestrate
     * the sequence of points filled. This structure type info is passed to the 
     * fill algorithm via its template parameter. For a breadth-first fill,
     * we use Queues, and for a depth-first fill, we use Stacks. Function
     * vorDotDFS provides an example call to function vor.
     * 
     * The algorithm includes the following steps:
     * 1) you will first select a random set
     * of coordinates (and their colors) to use as centers from which to
     * begin the fill. (this function is in file vor_given.cpp -- for 
     * testing purposes, you'll use regularly spaced points, rather than 
     * random.)
     * 2) Initialize a structure to use to maintain a collection of 
     * ordering structures
     *
     * 3) Initialize the fill: For each center c, in round-robin order, 
     *     do the following:
     *     a)  add the center to the its ordering structure. (every center 
     *         its own!
     *     b)  mark the location of that center as processed (the way you 
     *         do this is up to you!)
     *     c)  we have a choice to actually set the color of the pixel in 
     *         the fill when we add or when we remove. In our test cases, 
     *         we assume that you will change a color when a point is 
     *         added to a structure. (this is mostly relevant for checking 
     *         the animations.)
     *
     * 5) Complete the fill: Until all ordering structures (OS) are empty, 
     *    do the following for each OS, in a round robin order, indexing
     *    the iterations by k (an integer distance from the center):
     *    
     *      For each point less than or equal to distance k from its center:
     *      a)Remove the  point p from the ordering structure, and then...
     *
     *        i.    add p's unprocessed neighbors to the ordering structure if 
     *              they are within the appropriate distance from the center. We 
     *              describe this more carefully below. We call these the "valid"
     *              neighbors.
     *        ii.    use the colorPicker to set the new color of the valid neighbors.
     *        iii.    mark the valid neighbors as processed.
     *        iv.    as each neighbor is processed, if it is an appropriate 
     *              frame, send the current PNG to the
     *              animation (as described below).
     *
     *      b) When implementing your breadth-first and depth-first fills, 
     *            you will need to explore neighboring pixels in some order.
     *
     *        For this assignment, each pixel p has *up to* 8 neighbors, consisting of 
     *        the 8 pixels who share an edge with p. (We leave it to
     *        you to describe those 8 pixel locations, relative to the location
     *        of p.)
     *
     *        While the order in which you examine neighbors does not matter
     *        for a proper fill, you must use the same order as we do for
     *        your animations to come out like ours! 
     *
     *        The order you should put
     *        neighboring pixels **ONTO** the queue or stack is as follows:
     *        ** UP(-y), UPLEFT(-x,-y), LEFT(-x), LEFTDOWN(-x,+y), DOWN(+y), 
     *        DOWNRIGHT(+x,+y), RIGHT(+x), RIGHTUP(+x,-y)**
     *
     *        If you do them in a different order, your fill may
     *        still work correctly, but your animations will be different
     *        from the grading scripts!
     *
     *        TWO IMPORTANT NOTES: 
     *        1) *UP* here means towards the top of the image, so since an image has
     *        smaller y coordinates at the top, this is in the *negative y*
     *        direction. Similarly, *DOWN* means in the *positive y*
     *        direction. 
     *        2) not all of the 8 neighbors will be processed at every iteration of the
     *        while loop. You must assure that all vertices distance k or less
     *        from the center are processed (colored and put on the queue) before
     *        ANY whose distance is *greater than* k. k is a non-negative integer. 
     *        Finally, distance is the standard Euclidean distance  (computed 
     *        using the Pythagorean theorem). It is this requirement that assures
     *        that the fill grows outward from the center in the shape of a circle,
     *        rather than a diamond or a square. 
     *
     *      c) For every g pixels filled, **starting at the gth pixel**, you
     *        must add a frame to the animation, where g = frameFreq.
     *
     *        For example, if frameFreq is 4, then after the 4th pixel has
     *        been filled you should add a frame to the animation, then again
     *        after the 8th pixel, etc.  You must only add frames for the
     *        number of pixels that have been filled, not the number that
     *        have been checked. So if frameFreq is set to 1, a pixel should
     *        be filled every frame.
     *      d) Finally, as you leave the function, send one last frame to the
     *        animation. This frame will be the final result of the fill, and 
     *        it will be the one we test against.
     */



     /* Your code here. As a point of reference, we used three different helper
      * functions to complete this code. You may add as many as you wish, since
      * we will be grading vor.h. File "vor_given.cpp also includes the function
      * used to generate the original set of centers. 
      */

    
    

    
    vector<center> centers = randSample(img, density);

    // Initialize some necessary or useful variables
    int width = img.width();
    int height = img.height();

    PNG newImage(width, height);
    int* processed = new int[width * height];
    for (int i = 0; i < width*height; i++)
        processed[i] = 0; // 0 = unprocessed

    Queue< OrderingStructure<point> > osQueue;
    animation a;

    // Initialize the fill: For each center c make ordering structure
    int count = 0;
    int currLevel = 0;
    for (center c : centers) {
        point p(c); // Level defaults to 0 when initializing with center
        OrderingStructure<point> os;
        // processPoint adds p to the OS
        processPoint(newImage, p, fillColor, processed, a, os, count, frameFreq);
        osQueue.enqueue(os);
    }
    
    // Process neighbours in turn
    //int countOs=0;
    while ( !osQueue.isEmpty() ) {
        // Remove OS from the queue
        OrderingStructure<point> os = osQueue.dequeue();
        
        if ( os.isEmpty() ) {
            cout << "OS is empty, skipping" << endl;
            continue;
        }

        // If level of first element is different from the current level, all points
        // at the current level are processed and we should continue
        if ( os.peek().level != currLevel )
            currLevel = os.peek().level;

        // Do necessary processing of neighbours
        while ( !os.isEmpty() && os.peek().level == currLevel ) {
            point curr = os.remove();
            //if ( curr.level >= currLevel*currLevel ) currLevel++;
            processNeighbours(newImage, curr, fillColor, processed, a, os, count, frameFreq);
        }

        // If not empty, enqueue for new processing round
        if ( !os.isEmpty() ){
            osQueue.enqueue(os);
        }
    } // end while

    // Deallocate memory and send final frame to animation
    delete[] processed;
    a.addFrame(newImage);
    
    return a;
} 
