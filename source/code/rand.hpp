#ifndef RANDOM_HPP 
#define RANDOM_HPP

#include <time.h>
//-----------------------------------------------------------------------------

#define a7 78125 
#define a11 48828125 
#define a13 1220703125 
#define a15 452807053 
#define a17 582758085 
#define mask 2147483647 
#define then

#ifndef MAX
#define MAX(x,y) ( (x) < (y) ? (y) : (x) )
#endif
#define MIN(x,y) ( (x) < (y) ? (x) : (y) ) 


/** Class for generating random integer numbers. Modified code from 
* previous class of Daniele Pretolani. 3 routines can be used and a sign routine.
* \author Lars Relund Nielsen.
* \version 1.0
*/
class Random { 
public:
    
    /** Seed Initialization using clock value. */
    int ClockSeed();                
    
    /** Initialization of random routine 1 (use X17). */
    void InitR1(int seed);    
    
    /** Get integer.
    * \return An integer in the interval [min,max]. 
    */
    int GetR1(int min,int max);     
    
    /** Initialization of random routine 1 (use X11). */
    void InitR2(int seed);  
    
    /** Get integer.
    * \return An integer in the interval [min,max]. 
    */
    int GetR2 (int min,int max);      
    
    /** Initialization of sign random routine 1 (use X7). */
    void InitSign(int seed);          
    
    /** Get integer.
    * \return An zero/one value. 
    */
    int Sign();                         
    
    /** Initialization of random routine 1 (use X15). */
    void InitR3(int seed);     
    
    /** Get integer.
    * \return An integer in the interval [min,max]. 
    */
    int GetR3(int min, int max);     // Returns an integer, in the interval [mid..mad]
    
    
private:
    
    int rnd7(int X);
    int rnd11(int X);
    int rnd13(int X);
    int rnd15(int X);
    int rnd17(int X);
    
    int IX7;
    int IX11;
    int IX13;
    int IX15;
    int IX17;
    int bitmask;
};

//-----------------------------------------------------------------------------

#endif
