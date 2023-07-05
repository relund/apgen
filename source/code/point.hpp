#ifndef POINT_HPP
#define POINT_HPP

//-----------------------------------------------------------------------------
//#include <fstream.h>
#include <iostream>
#include "basicdt.hpp"
using namespace std;
//-----------------------------------------------------------------------------

/** Class defining a point.
* \author Lars Relund Nielsen and Christian Roed Pedersen.
* \version 1.0
*/
class Point
{
public:
    /** Creates a point. Don't set the values. */
    Point() {}

    /** Allocate values to the point */
    Point(flt c1,flt c2) {x=c1;y=c2;}

    /** Global function for comparing two points. Assume equal if their difference
    * if less than PRECISION.
    * \return True if equal.
    */
    bool EqualP(Point oP)
    {
        return (Equal(oP.x,x) && Equal(oP.y,y));
    }

    /** Print the point, that is, (x,y).
    * \note Do not put a newline.
    */
    void PrintP() const {cout << "(" << x << "," << y << ")";}

    /** Allocate values to the point */
    void SetP(flt & c1,flt & c2) {x=c1;y=c2;}

    /** Allocate values to the point */
    void SetP(Point oP) {x=oP.x;y=oP.y;}
    // Was previously void SetP(Point & oP) {x=oP.x;y=oP.y;}. Changed because of compiler error

    /** Calculate the weighted cost.
    * \return The weighted cost.
    * \param l Value of lambda.
    */
    flt CalcWC(flt & l) {return x*l+y;}

    // public variables which can be accesed directely.
    flt x; ///< First coordinate.
    flt y; ///< Second coordinate.
};

//-----------------------------------------------------------------------------

#endif
