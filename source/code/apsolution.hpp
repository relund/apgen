#ifndef APSOLUTION_HPP
#define APSOLUTION_HPP

//-----------------------------------------------------------------------------

#include <stdlib.h>        // For use of exit command
#include "basicdt.hpp"
#include "point.hpp"

/** Pointer to a APSolution object
* @see APSolution
*/
typedef class APSolution* APSolutionPtr;

//-----------------------------------------------------------------------------
/** Current solution for the assignment problem.
* \author Lars Relund and Christian Roed Pedersen.
* \version 1.0
*/
class APSolution
{
public:

    /** Default constructor (do nothing). */
    APSolution();

    /** Free memory. */
    ~APSolution();

    /** Allocates memory for the AP solution. */
    void AllocateMem(int dim);

    /** Print the current objective values (bicriterion case) and the primal solution. */
    void PrintBiSol();

    /** Print the current objective value and the primal solution. */
    void PrintSol();

    /** Print the current objective values (bicriterion cost). */
    void PrintC(){cout<<"("<<oC.x<<","<<oC.y<<")";}

    /** Print the dual solution.
    \pre Assumes that the dual variabeles u and v have been calcutated.
    */
    void PrintDualSol();

    idx *colsol; ///< Row assigned to column in solution, i.e. primal solution.
    idx *rowsol; ///< Column assigned to row in solution, i.e. primal solution.

    flt *u;    ///< Dual variables, row reduction numbers.
    flt *v;    ///< Dual variables, column reduction numbers.
    flt cost;  ///< Object function value.
    Point oC;  ///< Object function values (1. and 2. coordinate).
    idx dim;   ///< Dimension of the solution.
};

//-----------------------------------------------------------------------------

#endif
