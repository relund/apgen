#include "apsolution.hpp"

//----------------------------------------------------------------------------

APSolution::APSolution()
{
    //cout << "APSolution::APSolution\n"<<flush;
} // end APSolution:: APSolution

//----------------------------------------------------------------------------

APSolution::~APSolution()
{
    //cout << "APSolution::~APSolution\n"<<flush;
    delete [] rowsol;
    delete [] colsol;
    delete [] u;
    delete [] v;
} // end APSolution::~APSolution

//----------------------------------------------------------------------------

void APSolution::AllocateMem(int rDim)
{
  //cout << "APSolution::AllocateMem\n"<<flush;
  dim = rDim;
  colsol = new idx[rDim];
  if (colsol==NULL)
    {
      cout <<"Error: Insufficient memory, exit in APSolution::AllocateMem";
      exit(1);
    }
  rowsol = new idx[rDim];
  if (rowsol==NULL)
    {
      cout <<"Error: Insufficient memory, exit in APSolution::AllocateMem";
      exit(1);
    }
  u = new flt[rDim];
  if (u==NULL)
    {
      cout <<"Error: Insufficient memory, exit in APSolution::AllocateMem";
      exit(1);
    }
  v = new flt[rDim];
  if (v==NULL)
    {
      cout <<"Error: Insufficient memory, exit in APSolution::AllocateMem";
      exit(1);
    }
} 

//----------------------------------------------------------------------------

void APSolution::PrintBiSol()
{
    idx i;
    
    if (cost<INF) 
    {
        cout << "Solution value is " << cost << ". Costs are: ("<<oC.x<<","<<oC.y<<")\n";
        cout << "Primal row solution: (";
        for (i = 0; i < dim-1; i++)
            cout << i << ",";
        cout << dim-1 << ")=(";
        for (i = 0; i < dim-1; i++)
            cout << rowsol[i] <<",";
        cout << rowsol[dim-1] << ")";
    }
} 

//----------------------------------------------------------------------------

void APSolution::PrintSol()
{
    idx i;
    
    if (cost<INF) 
    {
        cout << "Cost="; 
        cout.width(15); 
        cout.setf(ios::left);
        cout << cost << "";
        cout << "Primal row solution: (";
        for (i = 0; i < dim-1; i++)
            cout << rowsol[i] <<",";
        cout << rowsol[dim-1] << ")";
    }
} 

//----------------------------------------------------------------------------

void APSolution::PrintDualSol()
{
    
    idx i;
    
    if (cost<INF) 
    {
        cout << "Dual solution: u=(";
        for (i = 0; i < dim-1; i++)
            cout << u[i] << ",";
        cout << u[dim-1] << ")  v=(";
        for (i = 0; i < dim-1; i++)
            cout << v[i] <<",";
        cout << v[dim-1] << ")";
    }
} 

