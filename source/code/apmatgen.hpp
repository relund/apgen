#ifndef GENAPMAT_HPP
#define GENAPMAT_HPP

//-----------------------------------------------------------------------------
#include "apmat.hpp"
#include "rand.hpp"
//-----------------------------------------------------------------------------

/** Class for generating problem instances for BiAP and BiMMAP.
* The problem will be output as an xml file.
* \author Lars Relund Nielsen and Christian Roed Pedersen.
* \version 2.0
*/
class APMatGen
{
public:

    /** Initialize the object. */
    APMatGen():d(1),minE(1),maxE(1),maxC(10),shape(0),method(1)
    {
        seed = oRand.ClockSeed();
        //cout <<"seed:" << seed << "\n";
        cout << "Generator APMatGen called for generating problem instances of the\n" <<
                "bicriterion (multi modual) assignment problem.\n";
    }

    /** Set input values */
    void SetValues(char * outFile, int dim,int minEntries,
        int maxEntries,int maxCost,int seedn,int genShape,int genMethod);

    /** Generate the problem instance and output it to an xml file
    * (specified by SetValues).
    */
    void Generate();

private:

    /** Print input values. */
    void PrintValues() const;

    /** Internal generate costs routine using method 1 on input parameters.
    * \return True if succeeded in generating all costs (no negative costs generated).
    */
    void GenCosts(int row,int column,int startIdx,int entries,
        int lbX,int ubX,int lbY,int ubY);

    /** Generate costs using method 1. */
    void GenCostsM1();

    /** Generate costs using method 2. */
    void GenCostsM2();

    /** Generate costs using method 3. */
    void GenCostsM3();

    /** Check if x or y is negative. */
    bool Negative(int x,int y) const
    {
        if (x<0 || y<0)
        {
            cout << "Generate negative costs. Try to generate again!\n";
            return true;
        }
        return false;
    }

    /** Set minX and maxY using gap */
    void SetMinXAndMaxY(int x,int y,int & minX,int & maxY,int gap) const;

    /** Print error msg */
    void PrintE() const;

    /** Check if errors in parameters */
    void CheckParam() const;



    APMat oAPMat;    ///< Problem instance to be generated.
    Random oRand;    ///< Random routines.

    char out[150];  ///< Output filename.
    int d;    ///< Dimension of the matrix.
    int minE;   ///< Minimal number of entries in a cell.
    int maxE;   ///< Maximum number of entries in a cell.
    int maxC;      ///< Maximum cost.
    int seed;      ///< Seed.
    int shape;  ///< Shape parameter.
    int method; ///< Method used by the generator.
};

//-----------------------------------------------------------------------------

#endif
