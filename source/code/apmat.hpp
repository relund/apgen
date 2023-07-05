#ifndef APMAT_HPP
#define APMAT_HPP

//-----------------------------------------------------------------------------
#include <iostream>
#include <set>
#include <stdlib.h>        // For use of exit command
#include <math.h>          // For use of fabs
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include <libxml/xpathInternals.h>
#include "basicdt.hpp"
#include "apsolution.hpp"
#include "cell.hpp"
using namespace std;
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------

/** The matrix defining the assignment problem.
* Implemented as a 2-dimensinal array (a matrix) of Cell objects.
* Moreover, APMat contains the current solution (if an AP solver has been
* called) oSol.
* \author Christian Roed Pedersen and Lars Relund Nielsen.
* \version 1.0
* \see Cell
* \see APSolution
*/
class APMat
{
public:

    /** Default constructor. */
    APMat():dim(0),method(0),shape(0)
    {
        strcpy(costRange,"");
        strcpy(entryRange,"");
        strcpy(fileIn,"");
        strcpy(fileOut,"");
    }

    /** Read the problem from file. */
    APMat(char filename[150]);

    /** Free the memory. */
    ~APMat();

    /** Print the matrix, that is, its entries. */
    void PrintEntries()
    {
        for (int i = 0; i < dim; i++)
            for (int j = 0; j < dim; j++)
            {
                cout << "\nCell["<<i<<","<<j<<"] "; aCells[i][j].Print();
            }
    }

    void SetVerbose(bool boolean) {verbose=boolean;}    ///< True if output verbose.

    /** Reads the dimension of the instance in the input file */
    int ReadDim(xmlNode * pXmlNode);

    /** Allocates memory for a double indexed array of cells.
    * \note Don't allocate memory for each cell element.
    */
    void AllocateMem(int dimension);


    /** Allocate memory to Cell[row,column] in the matrix
    * \see Cell
    */
    void AllocateMemCell(int row,int column,int entries)
    {
        aCells[row][column].AllocateMem(entries);
    }


    /** Sets the weighted cost and sort the entries. */
    void SetWCAndSort(flt l);

    /** Sets the weighted cost to the first cost and sort the entries. */
    void SetWCto1AndSort();


    /** Get the dimension.
    * \return Problem dimension.
    */
    int GetSize() const {return dim;}

    /** Print the problem to file. */
    void PrintToXmlFile(const char * file);

        //void Checklap2();
        // Check the linear assignment solution

    /** Reset the APMat internals so that nothing is fixed/removed. */
    void Reset()
    {
        oRSet = oIdx;
        oCSet = oIdx;
        for (int i = 0; i < dim; i++)
            for (int j = 0; j < dim; j++)
                aCells[i][j].Reset();
    }

    /** Fix cell (i,j) to entry k, set the solution in oSol
    and remove i and j from oRSet and oCSet.
    */
    void Fix(idx i,idx j,idx k)
    {
        aCells[i][j].Fix(k);
        oSol.colsol[j]=i;
        oSol.rowsol[i]=j;
        oRSet.erase(i);
        oCSet.erase(j);
        /*set<idx>::iterator ite;
        cout<<"\ncol={";
        for (ite=oCSet.begin();ite!=oCSet.end();++ite) cout<<*ite<<",";
        cout<<"} row={";
        for (ite=
            oRSet.begin();ite!=oRSet.end();++ite) cout<<*ite<<",";
        cout<<"}"<<endl;*/
    }

    /** Remove entry k in cell (i,j).
    */
    void Remove(idx i,idx j,idx k)
    {
        aCells[i][j].Remove(k);
        //cout << "Remove entry "<<k<<" in cell ("<<i<<","<<j<<")\n";
    }


    /** Print the reduced costs.
    \pre Assume that the subproblem is defined and that the dual variables are stored in oSol.
    */
    void PrintReducCosts()
    {
        idx i;
        idx j;
        if (oSol.cost<INF)
        {
            cout << "Reduced costs:\n";
            for (i = 0; i < dim; i++)
            {
                for (j = 0; j < dim; j++) cout<<aCells[i][j].MinEntryWC()-oSol.u[i]-oSol.v[j]<<"\t";
                cout<<"\n";
            }
        }
        cout<<"\n";
    }

    /** Print the costs of the current subproblem.
    \pre Assume that the subproblem is defined.
    */
    void PrintCosts()
    {
        idx i;
        idx j;
        if (oSol.cost<INF)
        {
            cout << "Costs:\n";
            for (i = 0; i < dim; i++)
            {
                for (j = 0; j < dim; j++) cout<<aCells[i][j].MinEntryWC()<<"\t";
                cout<<"\n";
            }
        }
        cout<<"\n";
    }




    CellPtr * aCells;    ///< The 2 dim array.
    APSolution oSol;     ///< AP solution representation.
    set<idx> oRSet;      ///< A set of indices defining the rows not fixed.
    set<idx> oCSet;      ///< A set of indices defining the columns not fixed.
    set<idx> oIdx;       ///< The full set of indices, i.e. {0,...,dim-1}.
    idx dim;             ///< Dimension of the problem

    // parameters
    char costRange[50];  ///< The cost range (used only for information).
    char entryRange[50]; ///< The entry range (used only for information).
    int seed;            ///< The seed used when generating the problem (used only for information)
    int method;          ///< The method used when generating the problem (used only for information)
    int shape;           ///< The shape used when generating the problem (used only for information)
    char fileIn[150];    ///< The filename of the problem.
    char fileOut[50];    ///< The intial filename of the output files.
    bool verbose;        ///< True if output verbose.
    bool append;         ///< Append output to end of the output file.

private:

    /** Read the cells from the file */
    void ReadCells(xmlNode * pXmlNode);

    /** Print error msg for xmlwriter */
    void PrintError(int rc)
    {
        if(rc<0)
        {
            cout << "Error writing nondominated set to file!\n"<<flush;
            exit(1);
        }
    }

    /**
     * Converts @in into UTF-8 for processing with libxml2 APIs
     * \param in String in a given encoding.
     * \param encoding  The encoding used.
     * \return The converted UTF-8 string, or NULL in case of error.
     */
    xmlChar * ConvertInput(const char *in, const char *encoding);

    /** Set oIdx={\f$0,...,\f$dim-1}. */
    void SetIndices(idx dim)
    {
        for(int i=dim-1;i>=0;i--) oIdx.insert(i);
    }
};

//-----------------------------------------------------------------------------

#endif
