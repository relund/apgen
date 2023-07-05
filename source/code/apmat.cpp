#include "apmat.hpp"

//----------------------------------------------------------------------------

APMat::APMat(char filename[150]):dim(0)
{
    //cout << "APMat::APMat\n"<<flush;
    cout << "Reading BiEAP problem from file " << filename << "\n";

    strcpy(costRange,"");
    strcpy(entryRange,""); 
    strcpy(fileIn,filename);
    strcpy(fileOut,"");    
    verbose=true;
    append=false;

    /*
     * this initialize the library and check potential ABI mismatches
     * between the version it was compiled for and the actual shared
     * library used.
    */
    LIBXML_TEST_VERSION

    xmlDoc * pXmlDoc = NULL;
    xmlNode * pXmlNode = NULL;
    pXmlDoc = xmlReadFile(filename, NULL, 0);   // Parse the file and get the DOM 
    if (pXmlDoc == NULL) cout << "Error: could not parse the xml input file.\n";
    pXmlNode = xmlDocGetRootElement(pXmlDoc);   // Get the root element node 
    dim=ReadDim(pXmlNode);
    SetIndices(dim);
    pXmlNode=pXmlNode->children;  // consider cell elements
    cout << "Dimension for the assignment problem is " << dim << "x" << dim << "\n";
    AllocateMem(dim);
    ReadCells(pXmlNode);
    xmlFreeDoc(pXmlDoc);    // Free the document 
    xmlCleanupParser(); // Free the global variables that may have been allocated by the parser.
    //cout << "End: APMat::APMat\n"<<flush;   
} 

//----------------------------------------------------------------------------

APMat::~APMat()
{
    //cout << "APMat::~APMat\n";
    for (int i = 0; i < dim; i++)
    {
        delete [] aCells[i];
    }
    delete [] aCells;
    
} // end APMat::~APMat

//----------------------------------------------------------------------------

int APMat::ReadDim(xmlNode * pXmlNode)
{
    xmlChar *tmp;

    //cout << "APMat::ReadDim\n"<<flush;
    for (; pXmlNode; pXmlNode = pXmlNode->next) 
    {
        if (pXmlNode->type == XML_ELEMENT_NODE && 
            xmlStrcmp(pXmlNode->name,(const xmlChar *)"apmat")==0)
        {
            tmp=xmlGetProp(pXmlNode,(const xmlChar *)"version");
            if (tmp != NULL)
            {
	      if ((!strcmp((char *)tmp,"1.0")==0) && (!strcmp((char *)tmp,"2.0")==0))
                {
                    cout << "Error reading problem instance from file wrong version!\n";
                    exit(1);
                }
            }
            tmp=xmlGetProp(pXmlNode,(const xmlChar *)"costrange");
            if (tmp != NULL)
            {
                strcpy(costRange,(const char *)tmp);
                xmlFree(tmp);
            }
            tmp=xmlGetProp(pXmlNode,(const xmlChar *)"entryrange");
            if (tmp != NULL)
            {
                strcpy(entryRange,(const char *)tmp);
                xmlFree(tmp);
            }
            tmp=xmlGetProp(pXmlNode,(const xmlChar *)"seed");
            if (tmp != NULL)
            {
                seed=atoi((const char *)tmp);
                xmlFree(tmp);
            }
            return atoi((const char *)xmlGetProp(pXmlNode,(const xmlChar *)"dimension"));
        }
    }
    cout << "Error: Could not find apmat element in xml input file!\n";
    exit(1);
}

//----------------------------------------------------------------------------

void APMat::AllocateMem(int dimension)
{
    //cout << "APMat::AllocateMem\n"<<flush;
    dim = dimension;
    aCells = new CellPtr[dim];
    if (aCells==NULL)
    {
        cout <<"Error: Insufficient memory, exit in APMat::AllocateMem";
        exit(1);
    }
    for (int i = 0; i < dim; i++)
    {
        aCells[i] = new Cell[dim];
	if (aCells[i]==NULL)
	  {
	    cout <<"Error: Insufficient memory, exit in APMat::AllocateMem";
	    exit(1);
	  }
    }
    oSol.AllocateMem(dim);
    
} // end APMat::AllocateMem

//----------------------------------------------------------------------------

void APMat::ReadCells(xmlNode * pXmlNode)
{
    //cout << "APMat::ReadCells\n"<<flush;
    xmlNodePtr pXNode;
    int row,column,entries,i;
    flt c1,c2;

    for (;pXmlNode; pXmlNode = pXmlNode->next) 
    {
        //cout << pXmlNode->name <<"\n"<< flush;
        if (pXmlNode->type == XML_ELEMENT_NODE) 
        {
            entries = atoi((const char *)xmlGetProp(pXmlNode,(const xmlChar *)"entries"));
            row = atoi((const char *)xmlGetProp(pXmlNode,(const xmlChar *)"row"));
            column = atoi((const char *)xmlGetProp(pXmlNode,(const xmlChar *)"column"));
            //cout << "row/column:" << row << "/" << column << " entries: " << entries << " " <<flush;
            
            aCells[row][column].AllocateMem(entries);
            pXNode=pXmlNode->children;
            //cout << "= ";
            i=0;
            for (;pXNode; pXNode = pXNode->next) 
            {   
                //cout << pXNode->name <<"\n"<< flush;
                if (pXNode->type == XML_ELEMENT_NODE) 
                {
                    c1 = atof((const char *)xmlGetProp(pXNode,(const xmlChar *)"c1"));
                    c2 = atof((const char *)xmlGetProp(pXNode,(const xmlChar *)"c2"));
                    //cout << "(" << c1 << "," << c2 << ") ";
                    aCells[row][column].SetValues(i,c1,c2);
                    pXNode=pXNode->next;
                    i++;
                }
            }
            // finally set dummy node to large number (infinity).
            aCells[row][column].SetValues(i,2*INF,2*INF);
            //cout<<"\n";
        }
    }
    //PrintEntries();       
} 

//----------------------------------------------------------------------------

void APMat::SetWCAndSort(flt l)
{
    //cout << "APMat::SetWCAndSort\n"<<flush;
    for (int i = 0; i < dim; i++)
        for (int j = 0; j < dim; j++)
        {
            aCells[i][j].Sort(l);
        }
    //PrintEntries();
    //cout << "End: APMat::SetWCAndSort\n"<<flush;
} 

//----------------------------------------------------------------------------

void APMat::SetWCto1AndSort()
{
    //cout << "APMat::SetWCto1AndSort\n"<<flush;
    for (int i = 0; i < dim; i++)
        for (int j = 0; j < dim; j++)
        {
            aCells[i][j].Wto1Sort();
        }
    //PrintEntries();
    //cout << "End: APMat::SetWCto1AndSort\n"<<flush;
} 

//----------------------------------------------------------------------------

void APMat::PrintToXmlFile(const char * file)
{
    const char * MY_ENCODING="ISO-8859-1";
    int rc;
    xmlTextWriterPtr writer;
    xmlChar *tmp;
    xmlDocPtr doc;
    Point oP;

    /* Create a new XmlWriter for DOM, with no compression. */
    writer = xmlNewTextWriterDoc(&doc, 0);
    if (writer == NULL) {
        printf("testXmlwriterDoc: Error creating the xml writer\n");
        exit(1);
    }
    xmlTextWriterSetIndent(writer, TRUE);
    const xmlChar MY_INDENT_STR[5] = "    ";	// Your indent String is 4 blanks
    xmlTextWriterSetIndentString(writer, MY_INDENT_STR);


    /* Start the document with the xml default for the version,
     * encoding ISO 8859-1 and the default for the standalone
     * declaration. */
    rc = xmlTextWriterStartDocument(writer, NULL, MY_ENCODING, NULL);
    PrintError(rc);

    /* Write a comment
     * Please observe, that the input to the xmlTextWriter functions
     * HAS to be in UTF-8, even if the output XML is encoded
     * in iso-8859-1 */
    tmp = ConvertInput("Cost matrix for the bicriterion (multi modal)\nassignment problem generated using APGen.\nLars Relund Nielsen and Christian Roed Pedersen (c)",
                       MY_ENCODING);
    rc = xmlTextWriterWriteComment(writer, tmp);
    PrintError(rc);
    if (tmp != NULL) xmlFree(tmp);

    /* Start element. Since thist is the first
     * element, this will be the root element of the document. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "apmat");
    PrintError(rc);

    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "dimension",xmlXPathCastNumberToString(dim));
    PrintError(rc);
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "version",BAD_CAST "2.0");
    PrintError(rc);
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "costrange",BAD_CAST costRange);
    PrintError(rc);
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "entryrange",BAD_CAST entryRange);
    PrintError(rc);
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "method",xmlXPathCastNumberToString(method));
    PrintError(rc);
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "shape",xmlXPathCastNumberToString(shape));
    PrintError(rc);
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "seed",xmlXPathCastNumberToString(seed));
    PrintError(rc);

    for (int i = 0; i < dim; i++)
        for (int j = 0; j < dim; j++)
        {
              /* Start an element "point" */
            rc = xmlTextWriterStartElement(writer, BAD_CAST "cell");
            PrintError(rc);
            rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "row",xmlXPathCastNumberToString(i));
            PrintError(rc);
            rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "column",xmlXPathCastNumberToString(j));
            PrintError(rc);
            rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "entries",xmlXPathCastNumberToString(aCells[i][j].size-1));
            PrintError(rc);
            for (int k=0;k<aCells[i][j].size-1;k++) // remember last entry for INF
            {
                oP.SetP(aCells[i][j].GetValues(k));
                rc = xmlTextWriterStartElement(writer, BAD_CAST "entry");
                PrintError(rc);
                /* Add an attributes. */
                rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "c1",xmlXPathCastNumberToString(oP.x));
                PrintError(rc);
                rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "c2",xmlXPathCastNumberToString(oP.y));
                PrintError(rc);
                    /* Close the element. */
                rc = xmlTextWriterEndElement(writer);
                PrintError(rc);
            }
            rc = xmlTextWriterEndElement(writer);
            PrintError(rc);
        }


    /* Here we could close the element using the
     * function xmlTextWriterEndElement, but since we do not want to
     * write any other elements, we simply call xmlTextWriterEndDocument,
     * which will do all the work. 
    */
    rc = xmlTextWriterEndDocument(writer);
    PrintError(rc);

    xmlFreeTextWriter(writer);
    //char * tmp1 = strcat(file,".xml");
    xmlSaveFileEnc(file, doc, MY_ENCODING);
    xmlFreeDoc(doc);
    cout << "Print problem (APMat) to file: " << file << " ... done.\n";
} 

//----------------------------------------------------------------------------

xmlChar * APMat::ConvertInput(const char *in, const char *encoding)
{
    xmlChar *out;
    int ret;
    int size;
    int out_size;
    int temp;
    xmlCharEncodingHandlerPtr handler;

    if (in == 0)
        return 0;

    handler = xmlFindCharEncodingHandler(encoding);

    if (!handler) {
        printf("ConvertInput: no encoding handler found for '%s'\n",
               encoding ? encoding : "");
        return 0;
    }

    size = (int) strlen(in) + 1;
    out_size = size * 2 - 1;
    out = (unsigned char *) xmlMalloc((size_t) out_size);

    if (out != 0) {
        temp = size - 1;
        ret = handler->input(out, &out_size, (const xmlChar *) in, &temp);
        if ((ret < 0) || (temp - size + 1)) {
            if (ret < 0) {
                printf("ConvertInput: conversion wasn't successful.\n");
            } else {
                printf
                    ("ConvertInput: conversion wasn't successful. converted: %i octets.\n",
                     temp);
            }

            xmlFree(out);
            out = 0;
        } else {
            out = (unsigned char *) xmlRealloc(out, out_size + 1);
            out[out_size] = 0;  /*null terminating out */
        }
    } else {
        printf("ConvertInput: no mem\n");
    }

    return out;
}

//----------------------------------------------------------------------------*/
/*
void APMat::Checklap2()
{
  row  i;
  col  j;
  cost lapcost = 0, redcost = 0;
  boolean *matched;
  char wait;

  matched = new boolean[dim];
  if (matched==NULL)
  {
  cout <<"Error: Insufficient memory, exit in APMat::Checklap2";
  exit(1);
  }

  for (i = 0; i < dim; i++)
    for (j = 0; j < dim; j++)
      // if ((redcost = aCells[i][j].pMinEntry->weight - oSol.u[i] - oSol.v[j]) < 0)
      if ((redcost = aCells[i][j].pMinEntry->weight - oSol.u[i] - oSol.v[j]) < -PRECISION) // comparing floats
      {
        cout << "\n");
        cout << "negative reduced cost i %d j %d redcost %20.15f\n", i, j, redcost);
        cout << "\n\ndim %5d - press key\n", dim);
        scanf("%d", &wait);
        break;
      }

  for (i = 0; i < dim; i++)
    if (fabs(redcost = aCells[i][oSol.rowsol[i]].pMinEntry->weight - oSol.u[i] - oSol.v[oSol.rowsol[i]]) > PRECISION)
    {
      cout << "\n");
      cout << "non-null reduced cost i %d soli %d redcost %d\n", i, oSol.rowsol[i], redcost);
      cout << "\n\ndim %5d - press key\n", dim);
      scanf("%d", &wait);
      break;
    }

  for (j = 0; j < dim; j++)
    matched[j] = FALSE;

  for (i = 0; i < dim; i++)
    if (matched[oSol.rowsol[i]])
    {
      cout << "\n");
      cout << "column matched more than once - i %d soli %d\n", i, oSol.rowsol[i]);
      cout << "\n\ndim %5d - press key\n", dim);
      scanf("%d", &wait);
      break;
    }
    else
      matched[oSol.rowsol[i]] = TRUE;


  for (i = 0; i < dim; i++)
    if (oSol.colsol[oSol.rowsol[i]] != i)
    {
      cout << "\n");
      cout << "error in row solution i %d soli %d solsoli %d\n", i, oSol.rowsol[i], oSol.colsol[oSol.rowsol[i]]);
      cout << "\n\ndim %5d - press key\n", dim);
      scanf("%d", &wait);
      break;
    }

  for (j = 0; j < dim; j++)
    if (oSol.rowsol[oSol.colsol[j]] != j)
    {
      cout << "\n");
      cout << "error in col solution j %d solj %d solsolj %d\n", j, oSol.colsol[j], oSol.rowsol[oSol.colsol[j]]);
      cout << "\n\ndim %5d - press key\n", dim);
      scanf("%d", &wait);
      break;
    }

  delete[] matched;
  return;
} // end APMat::Checklap2

*/
//----------------------------------------------------------------------------
