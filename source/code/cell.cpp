#include "cell.hpp"

//----------------------------------------------------------------------------

void Cell::Print()
{
    int i;

    cout << "Entries: ";
    for(i=0;i<size;i++)
    {
        cout << i << ":";
        aCList[i].Print();
        cout << " ";    
    }
    cout<< "\n";
    cout << "Sorted: ";
    for(i=0;i<size;i++)
    {
        cout << i << ":";
        aOrder[i]->Print();
        cout << " ";    
    }
    cout<< "\n";
}

//----------------------------------------------------------------------------
