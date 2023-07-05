#include "apmatgen.hpp"

//-----------------------------------------------------------------------------

void APMatGen::Generate()
{
    PrintValues();
    oAPMat.AllocateMem(d);
    oRand.InitR1(seed);   // x values
    oRand.InitR2(seed);   // y values
    oRand.InitR3(seed);   // entries
    if (method==1 || maxE==1) GenCostsM1();
    if (method==2) GenCostsM2();
    if (method==3) GenCostsM3();
    oAPMat.PrintToXmlFile(strcat(out,".xml"));
}

//-----------------------------------------------------------------------------

void APMatGen::GenCostsM1()
{
    if (maxE==1) cout<< "Generate costs for the biAP problem with shape parameter " << shape << "\n\n";
    else cout << "Generate costs using method 1 and shape parameter " << shape << "\n\n"; 
    int entries;
    int i,j;

    for (i = 0; i < d; i++)
        for (j = 0; j < d; j++)
        {
            entries = oRand.GetR3(minE,maxE);
            //cout << "\n" << i << " " << j << " " << entries << ":\n"<<flush;
            oAPMat.AllocateMemCell(i,j,entries);
            GenCosts(i,j,0,entries,0,maxC,0,maxC);
        }
        //cout << "\n"<<flush;
}

//-----------------------------------------------------------------------------

void APMatGen::GenCostsM2()
{
    cout << "Generate costs using method 2 and shape parameter " << shape << " (i.e. groups)\n\n";
    if (shape<=0) 
    {
        cout<<"Warning: Zero or negative groups specified use 1 instead!\n"; 
        shape=1;
    }
    
    int lbX,ubX,lbY,ubY;
    int entries;
    int i,j,q,n;
    int gapLgd;
    int gaps;
    int ent;
    bool gen;

    if (shape == 1) gapLgd = maxC;
    else gapLgd = maxC/(2*shape);
    gaps=shape;     // store # of gaps
    if (gapLgd==0) gapLgd=1;
    shape=0;
    //cout << "gapLgd:"<< gapLgd << "\n";
    for (i = 0; i < d; i++)
        for (j = 0; j < d; j++)
        {
            gen = true;
            lbX=0;
            ubY=maxC;
            entries = oRand.GetR3(minE,maxE);
            if (entries<=gaps*2)
            {
                cout << "Warning: Number of entries generated less than 2*groups!\n";
                gaps=MIN(gaps/2,entries/2);
                cout << "Only use " << gaps << " groups.\n";
            }
            oAPMat.AllocateMemCell(i,j,entries);
            ent = entries/gaps;
            //cout << "\n" << i << " " << j << " " << entries << " " << ent << ":\n"<<flush;
            n=0;
            for(q=1;q<2*gaps;q++)
            {
                ubX=lbX+gapLgd;
                lbY=ubY-gapLgd;
                //cout << "gapLgd:"<< gapLgd << "\n";
                //cout << "X in [" << lbX << "," << ubX << "] Y in [" << lbY << "," << ubY << "]\n"; 
                if (gen) 
                {
                    GenCosts(i,j,n,ent,lbX,ubX,lbY,ubY);
                    n+=ent;
                }
                lbX=ubX+1;
                ubY=lbY-1;
                gen = !gen;
            }
            // last run
            ubX=maxC;
            lbY=0;
            if (gen) GenCosts(i,j,n,entries-n,lbX,ubX,lbY,ubY);
            else if (entries-n!=0) GenCosts(i,j,n,entries-n,lbX,ubX,lbY,ubY);

        }
}

//-----------------------------------------------------------------------------

void APMatGen::GenCostsM3()
{
    cout << "Generate costs using method 3 and shape parameter " << shape << "\n\n"; 
    int entries;
    int i,j;
    bool upper = true;

    for (i = 0; i < d; i++)
        for (j = 0; j < d; j++)
        {
            entries = oRand.GetR3(minE,maxE);
            //cout << "\n" << i << " " << j << " " << entries << ":\n"<<flush;
            oAPMat.AllocateMemCell(i,j,entries);
            if (upper) GenCosts(i,j,0,entries,0,maxC/2,maxC/2,maxC);
            else GenCosts(i,j,0,entries,maxC/2,maxC,0,maxC/2);
            upper=!upper;
        }
        cout << "\n"<<flush;
}

//-----------------------------------------------------------------------------

void APMatGen::SetValues(char * outFile, int dim,int minEntries,
    int maxEntries,int maxCost,int seedn,int genShape,int genMethod)
{
    char tmp[50];

    strcpy(out,outFile);
    d = dim;    
    minE = minEntries;  
    maxE = maxEntries;  
    maxC = maxCost;    
    if (!seedn==0) seed = seedn;
	oAPMat.seed=seed;
    shape=oAPMat.shape=genShape; 
    method=oAPMat.method=genMethod;

    strcpy(oAPMat.costRange,"0-");
    sprintf(tmp,"%d",maxC);
    strcat(oAPMat.costRange,tmp);
    
    sprintf(oAPMat.entryRange,"%d",minE);
    strcat(oAPMat.entryRange,"-");
    sprintf(tmp,"%d",maxE);
    strcat(oAPMat.entryRange,tmp);
 
    PrintValues();
    CheckParam();
}

//-----------------------------------------------------------------------------

void APMatGen::GenCosts(int row,int column,int startIdx,int entries,
    int lbX,int ubX,int lbY,int ubY)
{
    //cout << "X in [" << lbX << "," << ubX << "] Y in [" << lbY << "," << ubY << "]\n"; 
    int minX,maxX,minY,maxY;
    int x,y;
    int k,ctr;
    int gap;
 
    if (maxE!=1)    // if consider the bimmap
    {
        if (shape>0) gap = ((ubY-lbY)/entries)*shape/100;
        else gap = ((ubX-lbX)/entries)*-shape/100;
        if (gap==0) gap=1;
        //cout << "ent:" << entries << "\n";
        //cout << "gap:" << gap << "\n";
	    //cout << "startIdx:" << startIdx << "\n";
        ctr=0;
        do
        {
            maxY = ubY;
            minX = lbX;
            for(k = 0; k<entries; k++)
            {
                minY = (int)(maxY - (maxY-lbY)/(entries-k));
                maxX = (int)(minX + (ubX-minX)/(entries-k));
                //cout << "X: [" << minX << "," << maxX << "] Y:[" << minY << "," << maxY << "]\n"<<flush; 
                x=oRand.GetR1(minX,maxX);
                y=oRand.GetR2(minY,maxY);
                //cout << "(x,y)=(" << x << "," << y << ")\n"<<flush; 
                if (Negative(x,y) || x>maxC || y>maxC) break;
                SetMinXAndMaxY(x,y,minX,maxY,gap);
                //cout << "k:"<< k+startIdx << "\n"<<flush;
                oAPMat.aCells[row][column].SetValues(k+startIdx,x,y);
            }
            //cout << "\n";
            if (k==entries) break;
            ctr++;
            if (ctr==100)
            {
                cout << "Error - Generate negative costs!!\nTry different seed.\n";
                exit(1);
            }
        } while (true);
    }
    else    // consider the bi-ap
    {
        if (shape==0)
        {
            x=oRand.GetR1(lbX,ubX);
            y=oRand.GetR2(lbY,ubY);
        }
        if (shape==1)
        {
            x=oRand.GetR1(lbX,ubX);
            y=ubX+lbX-x;    // complement of x
            if (y<x) y=oRand.GetR2(lbX,y);
            if (y>x) y=oRand.GetR2(y,ubX);      // note that may be x=y=(ubX+lbX)/2 if (ubX+lbX) is even 
        }
        oAPMat.aCells[row][column].SetValues(0,x,y);
    }
}

//-----------------------------------------------------------------------------

void APMatGen::PrintValues() const
{
    cout << "\nThe following input values are used:\n" <<
            "Dimension  : " << d << "\n" <<
            "Entry range: [" << minE << "," << maxE << "]\n" <<
            "Cost range : [0," << maxC << "]\n" <<
            "Seed       : " << seed << "\n\n";
}

//-----------------------------------------------------------------------------

void APMatGen::SetMinXAndMaxY(int x,int y,int & minX,int & maxY,int gap) const
{
    if (shape>0)
    {
        minX = x+1;
        if (y > gap) maxY = y-gap;
        else maxY=y-1;
    }
    else
    {
        maxY = y-1;
        if (x+gap <= maxC) minX = x+gap;
        else minX=x+1;
    }
}

//-----------------------------------------------------------------------------

void APMatGen::PrintE() const
{
    cout <<  "\nCompulsory arguments neglected to be given!\n";
    cout <<  "The following options must be specified\n";
    cout <<  "  -dim     : Dimension of problem instance to be generated.\n";
    cout <<  "  -maxEnt  : Upper bound for entry range.\n";
    cout <<  "  -out     : Output file name without xml extension.\n";
    cout <<  "  -maxCost : Upper bound for cost range.\n\n";
    cout <<  "Optional argument is\n";
    cout <<  "  -minEnt  : Lower bound for entry range - if not specified it is set to 1.\n";
    cout <<  "             This entry must be no larger than maxEntries.\n";
    cout <<  "  -shape   : Shape parameter. Possible values depends on which method used (see manual).\n\n";
    cout <<  "  -method  : Method used by generator (default 1).\n";
    cout <<  "  -seed    : The seed (if not specified generate one randomly.\n";
    cout <<  "             Must belong to interval [0," << INT_MAX <<"]\n";
    cout <<  "Observe that maxCost must be chosen sufficiently large (>= maxEnt-1)\n";
    cout <<  "for each entry to have a chance of being nondominated.\n\n";
}

//-----------------------------------------------------------------------------

void APMatGen::CheckParam() const
{
    bool ok=true;
    
    if (d==0 || maxE==0 || maxC==0 || minE > maxE || 
        maxC < maxE-1 || strcmp(out,"")==0) ok=false;
    if (!(seed<=INT_MAX && seed>=0)) ok=false;

    if (maxE!=1)
    {
        if (method==1 && !(shape>=-100 && shape<=100)) ok=false;
        if (method==2 && shape<1) ok=false;
        if (method==3 && !(shape>=-100 && shape<=100)) ok=false;
    }
    else
    {
        if (!(shape==0 || shape==1)) ok=false;
    }

    if (!ok) 
    {
        PrintE();
        exit(1);
    }
}

//-----------------------------------------------------------------------------
