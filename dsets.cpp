/* Your code here! */
#include "dsets.h"
#include <iostream>

/**
* Inits the empty trees to the back of our vector with size 1
* Repeats num times
*/
void DisjointSets::addelements(int num) {
    for(int i = 0; i < num; i++) {
        vect_.push_back(-1);
    }
}

/**
* Recursive Function, No helper
*/
int DisjointSets::find(int elem) {
    //Base Case: Finds the size of the path compression
    if(vect_[elem] < 0) { return elem; }
    int temp = find(vect_[elem]); //Recursion until we find path compression
    vect_[elem] = temp; //Set the indexes to contain the correct values
    return temp;
}

void DisjointSets::setunion(int a, int b) {
    //Use find to get index of sizes and make new size
    int a_size_idx = find(a);
    int b_size_idx = find(b);
    int combo_size = vect_[a_size_idx] + vect_[b_size_idx];

    //Find which is bigger
    if(vect_[a_size_idx] > vect_[b_size_idx]) {
        //B -> A
        vect_[a_size_idx] = b_size_idx;
        vect_[b_size_idx] = combo_size;
    } else { // A < B
        //A -> B
        vect_[b_size_idx] = a_size_idx;
        vect_[a_size_idx] = combo_size;
    }
}
