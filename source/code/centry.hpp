#ifndef CENTRY_HPP
#define CENTRY_HPP

//-----------------------------------------------------------------------------
//#include <iostream.h>
#include <stdlib.h>        // For use of exit command
#include <iostream>
#include "basicdt.hpp"
#include "point.hpp"
using namespace std;
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------

/** An entry in a Cell. Represents an entry in a cell containing a nondominated point and its
* weighted value.
* \author Lars Relund Nielsen and Christian Roed Pedersen.
* \version 1.0
*/
class CEntry
{
public:

    /** Print the entry. */
    void Print()
    {
        cout << "("<<W.x<<","<<W.y<<") WC:"<<w;
    }

    /** Set the point values.
    * \param c1 First coordinate.
    * \param c2 Second coordinate.
    * \note Do not set the weighted cost.
    */
    void SetValues(flt c1,flt c2)
    {
        W.x=c1;
        W.y=c2;
    }

    /** Set the point values and calculate the weighted cost.
    * \param l Value of lambda, i.e. w=x*l+y.
    * \param c1 First coordinate.
    * \param c2 Second coordinate.
    */
    void SetValues(flt l,flt c1,flt c2)
    {
        W.x=c1;
        W.y=c2;
        w = W.CalcWC(l);
    }

    /** Set the weighted cost.
    * \pre Assume that the point has already been assigned values.
    * \param l Value of lambda.
    */
    void SetWC(flt l) {w = W.CalcWC(l);}

    /** Set the weighted cost to the first coordinate.
    * \pre Assume that the point already has been assigned values.
    */
    void SetWto1() {w=W.x;}

    Point W;
    flt w;
};

//-----------------------------------------------------------------------------

#endif
