#ifndef CELL_HPP
#define CELL_HPP

//-----------------------------------------------------------------------------
#include <stdlib.h>        // For use of exit command
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include "basicdt.hpp"     // For flt
#include "centry.hpp"
using namespace std;
//-----------------------------------------------------------------------------

typedef class Cell* CellPtr;


/** Function object for comparing two CEntry's.
* Used when sorting entries.
*/
struct Less
{
    typedef class CEntry* CEntryPtr;

    bool operator() (CEntryPtr pCE1,CEntryPtr pCE2) const
    {
        return (pCE1->w < pCE2->w);
    }
};

//-----------------------------------------------------------------------------

/** A cell in APMat. Contains an unordered list of the entries
* and an ordered list ordering the parametric costs in the entries.
* \note A dummy entry is added to the array containing the entries with
* cost set to infinity. This entry is used if all entries are removed.
* \author Lars Relund Nielsen and Christian Roed Pedersen.
* \version 1.0
* \see APMat.
*/
class Cell
{
public:

    /** Default constructor.
    * \note Don't initialize the Cell arrays.
    */
    Cell(){/*cout << "Cell::Cell\n"<<flush;*/}

    /** Print entries. */
    void Print();

    /** Allocate memory for the Cell arrays.
    * \param dim Number of entries in the cell.
    * \note Allocate memory for the dummy entry also, i.e. array size is dim+1.
    */
    void AllocateMem(int dim) //:aOrder(dim)
    {
        //cout << "Cell::AllocateMem\n"<< flush;
        size=dim+1;
        aOrder.resize(size);
        aCList = new CEntry[size];
        if (aCList==NULL)
        {
            cout <<"Error: Insufficient memory, exit in Cell::AllocateMem";
            exit(1);
        }
        IniOrder();
        index=0;
    }

    /** Remove the object from the free store. */
    ~Cell()
    {
        //cout << "Cell::~Cell\n"<<flush;
        delete [] aCList;
    }

    /** Set the values in entry i to (c1,c2) and set the parametric cost to w1.
    * \note Don't order anything.
    */
    void SetValues(int i,flt c1,flt c2)
    {
        aCList[i].SetValues(c1,c2);
        aCList[i].SetWto1();
    }

    /** Get (c1,c2) in entry i. */
    Point GetValues(int i){return aCList[i].W;}

    /** Set the parametric cost for all entries.
    * \param l Lambda value.
    */
    void SetWC(flt l) {for(int i=0;i<size;i++) aCList[i].SetWC(l);}

    /** Set the parametric cost for all entries to the first coordinate. */
    void SetWto1() {for(int i=0;i<size;i++) aCList[i].SetWto1();}

    /** Set the parametric cost using parametric value l, sort the entries and store it in the array
    * ordering the cost.
    * \param l Lambda for the parametric cost.
    */
    void Sort(flt l)
    {
        SetWC(l);
        sort(aOrder.begin(),aOrder.end(),Less() );
        index=0;
        //Print();
    }

    /** Set the parametric cost to the first weight and sort the entries and store it in the array
    * ordering the cost.
    */
    void Wto1Sort()
    {
        //cout << "Cell::Wto1Sort\n"<<flush;
        SetWto1();
        sort(aOrder.begin(),aOrder.end(),Less() );
        index=0;
        //Print();
        //cout << "End: Cell::Wto1Sort\n"<<flush;
    }

    /** Returns the parametric cost of the minimal entry with respect to the parametric cost.
    * \see CEntry.
    */
    flt MinEntryWC() {return aOrder[index]->w;}

    /** Returns the first coordinate of the minimal entry with respect to the parametric cost.
    * \see CEntry.
    */
    flt MinEntryW1() {return aOrder[index]->W.x;}

    /** Returns the second coordinate of the minimal entry with respect to the parametric cost.
    * \see CEntry.
    */
    flt MinEntryW2() {return aOrder[index]->W.y;}

    /** Reset the cell so that nothing is fixed/removed */
    void Reset(){index=0;}

    /** Fix the cell to entry k in the ordered list. */
    void Fix(idx k) {index=k;}

    /** Remove entry k in the ordered list.
    \note Since we are interested in storing the last index not removed
    in the set of ordered entries, we only set it if k
    is greater than the index already stored.
    */
    void Remove(idx k) {if (k+1>index) index=k+1;}

    /** Get the difference between the current parametric cost for the index in the ordered
    list and the entry after.
    */
    flt GetDiff()
    {
        return aOrder[index+1]->w-aOrder[index]->w;
    }




    idx size;            ///< Array size, i.e. number of entries (dummy entry included).
    idx index;           ///< Index in the ordered list under consideration (might be fixed). Negative if none.

private:

    typedef class CEntry* CEntryPtr;

    /** Initialize the ordering. */
    void IniOrder()
    {
        for(int i=0;i<size;i++) aOrder[i]=aCList+i;
    }

    /** Set the parametric cost of entry i in the unordered list.
    * \param l Lambda value, i.e the parametric cost becomes w1*l+w2.
    * \param i Entry index.
    */
    void SetWC(int i,flt l) {aCList[i].SetWC(l);}

    vector<CEntryPtr> aOrder;   ///< Vector of pointers to CEntry's
    CEntryPtr aCList;    ///< Array of CEntry's

};

//-----------------------------------------------------------------------------

#endif
