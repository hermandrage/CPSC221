/**
 *
 * Balanced Quad Tree (pa3)
 *
 * This file will be used for grading.
 *
 */

#include "QTree.h"

// Return the biggest power of 2 less than or equal to n
int biggestPow2(int n) {
    if( n < 1 ) return 0;
    int v = 1;
    while( v <= n ) v <<= 1;
    return v >> 1;
}

QTree::Node::Node(PNG & im, pair<int,int> ul, int sz, Node *par)
  : upLeft(ul),size(sz),parent(par) //,nw(NULL),ne(NULL),sw(NULL),se(NULL)
{   
    this->nw=NULL;
    this->ne=NULL;
    this->sw=NULL;
    this->se=NULL;
    var = varAndAvg(im,ul,size,avg);
}

QTree::Node::Node(const Node* & n) : size(n->size),parent(NULL),avg(n->avg),
    var(n->var)//,nw(NULL),ne(NULL),sw(NULL),se(NULL) 
    {
        this->nw=NULL;
        this->ne=NULL;
        this->sw=NULL;
        this->se=NULL;
        this->upLeft = make_pair(n->upLeft.first, n->upLeft.second);
    }

/*
bool QTree::Node::operator< (Node const & other) const { return (this->var < other.var); }
bool QTree::Node::operator== (Node const & other) const { return (this->var > other.var); }
bool QTree::Node::operator> (Node const & other) const { return (this->var > other.var); }
*/

int QTree::Node::getX() const {
    return this->upLeft.first;
}

int QTree::Node::getY() const {
    return (this->upLeft.second);
}

/*
int QTree::getX(Node * t) const {
    return (t->upLeft.first);
}

int QTree::getY(Node * t) const {
    return (t->upLeft.second);
}
*/


QTree::~QTree(){
    clear();
}

QTree::QTree(const QTree & other) {
    copy(other);
}


QTree & QTree::operator=(const QTree & rhs){
    if (this != &rhs) {
        clear();
        copy(rhs);
    }
    return *this;
}


QTree::QTree(PNG & imIn, int leafB, RGBAPixel frameC, bool bal)
  : leafBound(leafB), balanced(bal), drawFrame(true), frameColor(frameC)
{
    /* YOUR CODE HERE */
    this->im = PNG(imIn); // uses copy constructor
    int w = im.width();
    int h = im.height();
    cout << "width: " << w << endl;
    cout << "heiight: " << h << endl;
    cout << "min: " << std::min(w,h) << endl;
    this->treeSize = biggestPow2( std::min(w,h) );
    cout << treeSize << endl;

    // create root node
    pair<int, int> p(0,0);
    this->root = new Node(im, p, treeSize, NULL); // all ptrs initialize to NULL
    nodeQueue.push(root);
    numLeaf = 1;
    
    while ( (numLeaf < leafBound) && (!nodeQueue.empty()) ) {
        Node *toSplit = nodeQueue.top();
        nodeQueue.pop();
    // cout << "QTree.cpp, line " << __LINE__ << endl;
        split(toSplit);
    // cout << "QTree.cpp, line " << __LINE__ << endl;
        //break; // for debugging
    }

}


QTree::QTree(PNG & imIn, int leafB, bool bal)
  : leafBound(leafB), balanced(bal), drawFrame(false)
{
    /* YOUR CODE HERE */
    this->im = PNG(imIn); // copy
    int w = im.width();
    int h = im.height();
    this->treeSize = biggestPow2( std::min(w, h) );
    cout << treeSize << endl;

    // create root node
    pair<int, int> p(0,0);
    this->root = new Node(im, p, treeSize, NULL); // all ptrs initialize to NULL
    nodeQueue.push(root);
    numLeaf = 1;
    
    while ( (numLeaf < leafBound) && (!nodeQueue.empty()) ) {
        Node *toSplit = nodeQueue.top();
        nodeQueue.pop();
        split(toSplit);
    }

}


bool QTree::isLeaf( Node *t ) {
    
    /* YOUR CODE HERE */
    return ( t->nw == NULL && t->ne == NULL && t->sw == NULL && t->se == NULL );
}
  
void QTree::split( Node *t ) {
    
    /* YOUR CODE HERE */

    // FOR BALANCED QTREES-------------------------------------------------
    // A split might cause one or two nbrs of the parent of t to split
    // to maintain balance.  Note that these two nbrs exist (unless they're
    // not in the image region) because the current set of leaves are
    // balanced.
    // if( t is a NW (or NE or SW or SE) child ) then we need to check that
    // the North and West (or North and East or South and West or
    // South and East) nbrs of t->parent have children. If they don't
    // we need to split them.

    numLeaf--; // since node t is no longer a leaf after split
    pair<int, int> corner(t->upLeft.first, t->upLeft.second);
    //cout << "------ upLeft pair nw: " << corner.first << ", " << corner.second << endl;
    t->nw = new Node(this->im, corner, t->size/2, t);
    // cout << "------ t->size/2 = " << t->size/2 << endl;
    nodeQueue.push(t->nw);
    
    corner.first += t->size/2;
    // cout << "------ upLeft pair ne: " << corner.first << ", " << corner.second << endl;
    // cout << "------ t->size/2 = " << t->size/2 << endl;
    t->ne = new Node(this->im, corner, t->size/2, t);
    nodeQueue.push(t->ne);

    
    corner.second += t->size/2;
    // cout << "------ upLeft pair sw: " << corner.first << ", " << corner.second << endl;
    // cout << "------ t->size/2 = " << t->size/2 << endl;
    t->sw = new Node(this->im, corner, t->size/2, t);
    nodeQueue.push(t->sw);
   

    corner.first -= t->size/2;
    // cout << "------ upLeft pair se: " << corner.first << ", " << corner.second << endl;
    // cout << "------ t->size/2 = " << t->size/2 << endl;
    t->se = new Node(this->im, corner, t->size/2, t);
    nodeQueue.push(t->se);
    if (t->upLeft.second == treeSize*11/16 && t->upLeft.first == treeSize*5/16)
    {
        cout << "---------------- heyo --------------" << endl;
    }
    
    
    numLeaf += 4; // have created four new leaves (and removed one previously)

    if (balanced) {
        cout << "Balance tree" << endl;
        // check appropriate neighbours and split if needed
        Node * southNbr = SNbr(t);
        // cout << southNbr << endl;
        // cout << "QTree.cpp, split, line " << __LINE__ << endl;
        if (southNbr != NULL) cout << "southNbr: " << southNbr->size << " t: " << t->size << endl;
        if ((southNbr != NULL) && ((southNbr->size) > (t->size))){
            cout << "--------------------- split for balance SNbr ----------" << endl;
            split(southNbr);
        }
        // cout << "QTree.cpp, split, line " << __LINE__ << endl;
        Node * northNbr=NNbr(t);
        // cout << "QTree.cpp, split, line " << __LINE__ << endl;
        //if (northNbr != NULL) cout << "northNbr: " << northNbr->size << " t: " << t->size << endl;
        if (northNbr!=NULL && ((northNbr->size) >= (t->size*2))){
            cout << "--------------------- split for balance NNbr ----------" << endl;
            split(northNbr);
        }
        // cout << "QTree.cpp, split, line " << __LINE__ << endl;
        Node * westNbr=WNbr(t);
        // cout << "QTree.cpp, split, line " << __LINE__ << endl;
        // if (westNbr != NULL) cout << "westNbr: " << westNbr->size << " t: " << t->size << endl;
        if (westNbr!=NULL && (westNbr->size > t->size)){
            cout << "--------------------- split for balance WNbr ----------" << endl;
            split(westNbr);
        }
        // cout << "QTree.cpp, split, line " << __LINE__ << endl;
        Node * eastNbr=ENbr(t);
        // cout << "QTree.cpp, split, line " << __LINE__ << endl;
        // if (eastNbr != NULL) cout << "eastNbr: " << eastNbr->size << " t: " << t->size << endl;
        if (eastNbr != NULL && (eastNbr->size > t->size)){
            cout << "--------------------- split for balance ENbr ----------" << endl;
            split(eastNbr);
        }
        // do some splitting or something
    }
    
}


/* NNbr(t)
 * return the same-sized quad tree node that is north of Node t.
 * return NULL if this node is not in the QTree.
 */
QTree::Node * QTree::NNbr(Node *t) {

    /* YOUR CODE HERE */
    // TODO: Rewrite gith getX() and getY() functions
    // - t->size
    // cout << "(*t).getY()" << (*t).getY() << endl;
    if ( (*t).getY() <= 0 ) { // NNbr is out of image
        // cout << "QTree.cpp, NNbr, out of bound, line " << __LINE__ << endl;
        return NULL;
    }
    // cout << "QTree.cpp, NNbr, line " << __LINE__ << endl;
    // cout << "t->parent = " << t->parent << endl;
    if ((*(t->parent)).getY() == (*t).getY()) { // 
        // cout << "QTree.cpp, NNbr, line " << __LINE__ << endl;
        Node * parentNorth = NNbr(t->parent);
        if(isLeaf(parentNorth)) return parentNorth;
        if((*parentNorth).getX() < (*t).getX()) return parentNorth->se;
        // cout << "QTree.cpp, NNbr, line " << __LINE__ << endl;
        return parentNorth->sw;
    }
    // cout << "QTree.cpp, NNbr, line " << __LINE__ << endl;
    if((*(t->parent)).getY() < (*t).getY()){
        if((*(t->parent)).getX() == (*t).getX()) return t->parent->nw;
        // cout << "QTree.cpp, NNbr, line " << __LINE__ << endl;
        return t->parent->ne;
    }

    return NULL; // remove when finished
}

/* SNbr(t)
 * return the same-sized quad tree node that is south of Node t.
 * return NULL if this node is not in the QTree.
 */
QTree::Node * QTree::SNbr(Node *t) {

    /* YOUR CODE HERE */
    
    // check if outside bottom of QTree square ( >= biggestPow2 )
    // cout << "(*t).getY()" << t->upLeft.second << endl;
    if ((*t).getY() + t->size >= treeSize) {
        // cout << "QTree.cpp, SNbr, out of bound, line " << __LINE__ << endl;
        return NULL;
    } 
    
    if((*(t->parent)).getY() < (*t).getY()){
        Node * parentSouth=SNbr(t->parent);
        
        if(isLeaf(parentSouth)) return parentSouth;
        
        if((*parentSouth).getX() < (*t).getX()) return parentSouth->ne;
        return parentSouth->nw;
    }
    if((*(t->parent)).getY() == (*t).getY()){
        if((*(t->parent)).getX()==(*t).getX()) return t->parent->se;
        return t->parent->sw;
    }
    

    return NULL; // remove when finished
}

/* ENbr(t)
 * return the same-sized quad tree node that is east of Node t.
 * return NULL if this node is not in the QTree.
 */
QTree::Node * QTree::ENbr(Node *t) {

    /* YOUR CODE HERE */

    // check if outside right edge of QTree square ( >= biggestPow2 )
    // cout << "(*t).getX()" << (*t).getX() << endl;
    if ((*t).getX() + t->size >= treeSize) {
        // cout << "QTree.cpp, ENbr, out of bound, line " << __LINE__ << endl;
        return NULL;
    }
    if ((*(t->parent)).getX() < t->getX()){
        Node * parentEast = ENbr(t->parent);
        //if (parentEast->getX()==treeSize*)
        if(isLeaf(parentEast)) return parentEast;
        if((*parentEast).getY() < (*t).getY()) return parentEast->nw;
        return parentEast->sw;
    }
    if((*(t->parent)).getX() == (*t).getX()){
        if((*(t->parent)).getY() == (*t).getY()) return t->parent->ne;
        return t->parent->se;
    }
    
    return NULL; // remove when finished
}

/* WNbr(t)
 * return the same-sized quad tree node that is west of Node t.
 * return NULL if this node is not in the QTree.
 */
QTree::Node * QTree::WNbr(Node *t) {

    /* YOUR CODE HERE */

    // check if outside left side of image ( <= 0 )
    // - t->size
    // cout << "(*t).getX()" << t->upLeft.first << endl;
    if ((*t).getX() <= 0 ) {
        // cout << "QTree.cpp, WNbr, out of bound, line " << __LINE__ << endl;
        return NULL;
    }
    if ((*(t->parent)).getX() == (*t).getX()){
        Node * parentWest = WNbr(t->parent);
        /*
        if (parentWest->getX() == treeSize/2 && parentWest->getY() == treeSize/2 && t->size == treeSize/4) {
            cout << "parentWest size: " << parentWest->size << endl;
            cout << "t size: " << t->size << endl;
            cout << "parentWest position: " << parentWest->getX() << ", " << parentWest->getY() << endl;
            cout << "parentWest isLeaf: " << isLeaf(parentWest) << endl;
        }
        */
        if(isLeaf(parentWest)) return parentWest;
        if((*parentWest).getY() < (*t).getY()) return parentWest->sw;
        return parentWest->nw;
    }
    if((*(t->parent)).getX() < (*t).getX()){
        if((*(t->parent)).getY() == (*t).getY()) return t->parent->ne;
        return t->parent->se;
    }
    
    return NULL; // remove when finished
}

bool QTree::write(string const & fileName){

    /* YOUR CODE HERE */
    
    PNG writeImage(this->im);
    writeImage.resize(this->root->size, this->root->size);
    writeRecursive(root, writeImage);

    // include the following line to write the image to file.
    return(writeImage.writeToFile(fileName));
}

void QTree::clear() {
    
    /* YOUR CODE HERE */
    
    clearRecursive(root);
    delete root;
    root = NULL;

    numLeaf = 0;
    this->im = PNG();
}


void QTree::copy(const QTree & orig) {
    /* YOUR CODE HERE */
    this->root = NULL;
    this->im = orig.im;
    this->leafBound = orig.leafBound;
    this->balanced = orig.balanced;
    this->drawFrame = orig.drawFrame;
    this->frameColor = orig.frameColor;

    this->treeSize = orig.treeSize;

    copyRecursive(this->root, orig.root);
}


/* =================== own helper functions ============== */
void QTree::writeRecursive(Node * t, PNG & newImage) {
    if (!isLeaf(t)){
        writeRecursive(t->ne, newImage);
        writeRecursive(t->nw, newImage);
        writeRecursive(t->se, newImage);
        writeRecursive(t->sw, newImage);
        return;
    }
    // iterate trough all the pixels writing the leaf and color them with the average colour
    RGBAPixel *pixel;
    for (int x = (*t).getX(); x<(*t).getX()+t->size; x++) {
        for (int y = (*t).getY(); y<(*t).getY() + t->size; y++) {
            pixel = newImage.getPixel(x,y); 
            *pixel = t->avg;
        }
    }

    if (drawFrame) {
        // color in borders
        for (int i = 0; i < t->size; i++) {
            pixel = newImage.getPixel((*t).getX() + i, (*t).getY());
            *pixel = frameColor;
            pixel = newImage.getPixel((*t).getX() + i, (*t).getY() + t->size - 1);
            *pixel = frameColor;
            pixel = newImage.getPixel((*t).getX(), (*t).getY() + i);
            *pixel = frameColor;
            pixel = newImage.getPixel((*t).getX() + t->size - 1, (*t).getY() + i);
            *pixel = frameColor;
        }
    }
}



void QTree::clearRecursive(Node * toClear) {

    // Clear this node and it's children
    if (toClear->nw != NULL){
        clearRecursive(toClear->nw);
        delete toClear->nw;
        toClear->nw = NULL;
    }
    if (toClear->ne != NULL){
        clearRecursive(toClear->ne);
        delete toClear->ne;
        toClear->ne = NULL;
    }
    if (toClear->sw != NULL){
        clearRecursive(toClear->sw);
        delete toClear->sw;
        toClear->sw = NULL;
    }
    if (toClear->se != NULL){
        clearRecursive(toClear->se);
        delete toClear->se;
        toClear->se = NULL;
    }
    //delete toClear->parent;
    toClear->parent = NULL;
}


void QTree::copyRecursive(Node * & subRoot, const Node * toCopy) {

    if ( toCopy == NULL ) {
        subRoot = NULL;
        return;
    }
    
    // Copy this node and it's children
    subRoot = new Node(toCopy);
    
    if (toCopy->nw != NULL){
        copyRecursive(subRoot->nw, toCopy->nw);
        subRoot->nw->parent = subRoot;
    }
    if (toCopy->ne != NULL){
        copyRecursive(subRoot->ne, toCopy->ne);
        subRoot->ne->parent = subRoot;
    }
    if (toCopy->sw != NULL){
        copyRecursive(subRoot->sw, toCopy->sw);
        subRoot->sw->parent = subRoot;
    }
    if (toCopy->se != NULL){
        copyRecursive(subRoot->se, toCopy->se);
        subRoot->se->parent = subRoot;
    }
    
}