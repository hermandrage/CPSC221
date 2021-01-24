#include "grid.h"
#include "grid_given.cpp"

// PA1 functions


/**
 * Destroys the current Grid. This function should ensure that
 * memory does not leak on destruction of a grid.
 */
Grid::~Grid(){
    this->clear();
}

/**
 * Rotate row r (row 0 is the first) by count positions.
 * If row r is ABCDE (left to right) in grid g then row r
 * in grid g should be DEABC after the call g.rotateR(r, 2).
 * Rotate headOfCol_ if necessary.
 */
void Grid::rotateR(int r, int count) {
    int nRows = numRows();
    int nCols = numCols();
    r = r % nRows;
    if ( (r < 0) || (r >= nRows) ) { return; }

    Node* curr = headOfRow_[r];
    count = count % nCols; // need not rotate more than the no of cols
    
    for (int i=0; i<count; i++) {
        // do 'count' times
        for (int r=0; r<nCols; r++) {
            // change all pointers left/right to/from node
            curr->up  = curr->up->right;
            curr->down = curr->down->right;
            curr->up->down = curr;
            curr->down->up = curr;
            // move to node below
            curr = curr->right;
        }
        headOfRow_[r] = headOfRow_[r]->left;

        // if the row is the first row, update headOfCol_
        if ( r==0 ) {
            Node *lastElem = headOfCol_[-1];
            headOfCol_.pop_back();
            headOfCol_.insert(headOfCol_.begin(), lastElem);
        }
    }
}

/**
 * Rotate column c (column 0 is the first) by count positions.
 * If column c is ABCDE (top to bottom) in grid g then column c
 * in grid g should be DEABC after the call g.rotateC(c, 2). 
 * Rotate headOfRow_ if necessary.
 */
void Grid::rotateC(int c, int count) {
    int nRows = numRows();
    int nCols = numCols();
    c = c % nCols;
    if ( (c < 0) || (c >= nCols) ) { return; }
    
    Node* curr = headOfCol_[c];
    count = count % nRows; // need not rotate more than the no of rows
    
    for (int i=0; i<count; i++) {
        // do 'count' times
        for (int r=0; r<nRows; r++) {
            // change all pointers left/right to/from node
            curr->left  = curr->left->down;
            curr->right = curr->right->down;
            curr->left->right = curr;
            curr->right->left = curr;
            // move to node below
            curr = curr->down;
        }
        headOfCol_[c] = headOfCol_[c]->up;

        // if the column is the first column, update headOfRow_
        if ( c==0 ) {
            Node *lastElem = headOfRow_[-1];
            headOfRow_.pop_back();
            headOfRow_.insert(headOfRow_.begin(), lastElem);
        }
    }
}


/**
 * Takes in row number and deallocates memory for all nodes in that
 * row. Edits pointers in the rows above and below the row being
 * deleted. Edits headOfRow_ vector. Own helper function.
 * @param headOfRow Pointer to Node at the head of a row
 */
void Grid::deallocateRow(int r) {
    int nRows = numRows();
    if ( (r < 0) || (r >= nRows ) ) {
        cout << "No row was deallocated in Grid::deallocateRow(). Row number "
            << r << " out of bounds for Grid with " << nRows << " rows.";
        return;
    }

    if (headOfRow_[r] == NULL) return; // row empty
    if (r == 0) {
        // if first row, change headOfCol_
        for (int i=0; i<nRows; i++) { headOfCol_[i] = headOfCol_[i]->down; }
    }

    while (headOfRow_[r]->right != headOfRow_[r]) {
        Node *temp = headOfRow_[r]->right;
        headOfRow_[r]->right = headOfRow_[r]->right->right;
        headOfRow_[r]->right->left = headOfRow_[r];
        temp->up->down = temp->down;
        temp->down->up = temp->up;
        delete temp;
    }
    delete headOfRow_[r];
    headOfRow_[r] = NULL;
    // erase the current headOfRow from vector
    vector<Node*>::iterator it = headOfRow_.begin() + r;
    headOfRow_.erase(it);
}

/**
 * Takes in indices (x, y) to a node in the Grid and returns a
 * pointer to the Node at the given position. Note: 0-indexed.
 * @param x The position of the Node in the x direction
 * @param y The position of the Node in the y direction
*/
Grid::Node* Grid::getNodePtr(int x, int y) const {
    Node *element = headOfRow_[y];
    for ( int i=0; i<x; i++ ) {
        element = element->right;
    }
    return element;
}


/**
 * Destroys all dynamically allocated memory associated with the
 * current Grid class. Clears headOfRow_ and headOfCol_ vectors.
 * Sets bwidth_, bheight_ to zero.
 * After clear() the grid represents an empty grid.
 */
void Grid::clear() {
    cout << "Clear called on Grid at memory location: " << this << endl;
    if( bwidth_ == 0 || bheight_ == 0 ) return; // grid is empty
    int nRows = numRows();
    int nCols = numCols();
    int i;
    // Clear each row and pointers
    for ( i=0; i<nRows; i++ ){
        deallocateRow(0);
    }
    // Clear all column pointers
    for ( i=0; i<nCols; i++ ){
        headOfCol_.erase(headOfCol_.begin());
    }
    bwidth_ = 0;
    bheight_ = 0;
}


/**
 * Makes the current Grid a copy of the "other" Grid.
 * The dimensions should be the same.  The Nodes should
 * contain the same blocks, but the Nodes should be newly
 * allocated. This function is used in both the copy
 * constructor and the assignment operator for Grids.
 */
void Grid::copy(Grid const& other) {
    //cout << "Copy called on Grid at memory location: " << &other << endl;
    //cout << "Copying to Grid at memory location: " << this << endl;
    this->clear();
    
    // Check size of other Grid
    int nRows = other.numRows();
    int nCols = other.numCols();
    if (nRows == 0 || nCols == 0) return;
    
    this->bwidth_ = other.bwidth();
    this->bheight_ = other.bheight();

    // Create nodes by copying Blocks from other Grid
    vector< vector<Node*> > A;
    for( int y=0; y<nRows; y++ ) {
        vector<Node*> temp;
        for( int x=0; x<nCols; x++ ) {
            Grid::Node* p = new Grid::Node(other.getNodePtr(x, y)->block);
            temp.push_back(p);
        }
        A.push_back(temp);
    }

    // Set up pointer structure
    int i, j;
    for( i=0; i<nRows; i++ ) {
        for( j=0; j<nCols; j++ ) {
            Node *p = A[i][j];
            p->up    = A[(i==0) ? nRows-1 : i-1][j]; 
            p->down  = A[(i==nRows-1) ? 0 : i+1][j];
            p->left  = A[i][(j==0) ? nCols-1 : j-1];
            p->right = A[i][(j==nCols-1) ? 0 : j+1];
        }
    }
    for( i=0; i<nRows; i++ ) {
        headOfRow_.push_back(A[i][0]);
    }
    for( i=0; i<nCols; i++ ) {
        headOfCol_.push_back(A[0][i]);
    }
}
