#ifndef BASICDT_HPP
#define BASICDT_HPP

//#include <fstream.h>
//#include <string>
//#include <sstream>
//using namespace std;

// Basic datatypes and definitions
//-----------------------------------------------------------------------------

#ifndef NULL
#define NULL 0
#endif

#define MIN(x,y) ( (x) < (y) ? (x) : (y) )  ///< If x smaller than y then return x, otherwise return y
#define MAX(x,y) ( (x) > (y) ? (x) : (y) )  ///< If x larger than y then return x, otherwise return y

typedef double flt;   ///< A floating number datatype. */
typedef short int idx;  ///< A datatype for storing array indexes etc. Note don't make it unsigned since then have to cast!!

const flt INF=18000000000000000.0;      ///< Infinity (or values above).
const flt PRECISION = 1e-10;  ///< used for comparison floats
const int FALSE = 0;
const int TRUE = 1;

/** Global function for comparing two floats. Assume equal if their difference 
* if less than PRECISION.
* \return True if equal.
*/
inline bool Equal(flt n1,flt n2)
{
    return ((n2-PRECISION)<=n1 && n1<=(n2+PRECISION));
};

/** Global function for converting a flt to a string */
/*inline string flt2String(const flt i)
{
  ostringstream stream;
  stream << i;
  return stream.str();
};*/

#endif
