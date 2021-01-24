/**
 * @file deque.cpp
 * Implementation of the Deque class.
 *
 */

/* No argument constructor */
template <class T>
Deque<T>::Deque() : n1(0), n2(0){}
// n1: number of entries
// n2: index of left entry


/**
 * Adds the parameter object to the right of the Deque.
 *
 * @param newItem object to be added to the Deque.
 */
template <class T>
void Deque<T>::pushR(T newItem)
{
    /**
     * @todo Your code here!
     */
    data.push_back(newItem);
    n1++;
}

/**
 * Removes the object at the left of the Deque, and returns it to the
 * caller.
 *
 * See .h file for implementation notes. 
 *
 * @return The item that used to be at the left of the Deque.
 */
template <class T>
T Deque<T>::popL()
{
    /**
     * @todo Your code here! 
     */
    T retval = data[n2];
    n1--;
    
    // NOTE: Possibly not O(1) with vector.erase()
    //data.erase( data.begin() + n2 );
    
    // Increment n2 will work in same way, vector will still contain the element
    n2++;
    
    if ( n1 <= n2 ) {
        // downsize and move data
        vector<T> newVec;
        for ( int i=n2; i<(n1+n2); i++ )
            newVec.push_back(data[i]);
        data = newVec;
        n2 = 0;
    }
    
    return retval;
}



/**
 * Removes the object at the right of the Deque, and returns it to the
 * caller.
 *
 * @return The item that used to be at the right of the Deque.
 */
template <class T>
T Deque<T>::popR()
{
    /**
     * @todo Your code here! 
     */
    n1--;
    T retval = data[n1+n2];
    data.erase( data.begin() + (n1+n2) );
    
    if ( n1 <= n2 ) {
        // downsize and move data
        vector<T> newVec;
        for ( int i=n2; i<(n1+n2); i++ )
            newVec.push_back(data[i]);
        data = newVec;
        n2 = 0;
    }
    return retval;
}

/**
 * Finds the object at the left of the Deque, and returns it to the
 * caller. Unlike popL(), this operation does not alter the deque.
 *
 * @return The item at the front of the deque.
 */
template <class T>
T Deque<T>::peekL()
{
    /**
     * @todo Your code here! 
     */
    return data[n2];
}



/**
 * Finds the object at the right of the Deque, and returns it to the
 * caller. Unlike popR(), this operation does not alter the deque.
 *
 * @return the value of The item at the right of the deque.
 */
template <class T>
T Deque<T>::peekR()
{
    /**
     * @todo Your code here! 
     */
     return data[n1+n2-1];
}



/**
 * Determines if the Deque is empty.
 *
 * @return bool which is true if the Deque is empty, false otherwise.
 */
template <class T>
bool Deque<T>::isEmpty() const
{
    /**
     * @todo Your code here! 
     */
    return ( n1==0 );
}

