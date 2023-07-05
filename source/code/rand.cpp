#include "rand.hpp"

/*********************************************************************/ /*
Random Basic Number Generators                                         */
/*********************************************************************/


int Random::rnd7 (int IX ) {
  IX = IX * a7;
  if (IX < 0)
  then IX = IX & mask;
  return (IX);
};

int Random::rnd11 (int IX ) {
  IX = IX * a11;
  if (IX < 0)
  then IX = IX & mask;
  return (IX);
};

int Random::rnd13 (int IX ) {
  IX = IX * a13;
  if (IX < 0)
  then IX = IX & mask;
  return (IX);
};

int Random::rnd15 (int IX ) {
  IX = IX * a15;
  if (IX < 0)
  then IX = IX & mask;
  return (IX);
};

int Random::rnd17 (int IX ) {
  IX = IX * a17;
  if (IX < 0)
  then IX = IX & mask;
  return (IX);
};

int Random::ClockSeed() {
  time_t times;
  int seed;
  unsigned int st;
  times = time(0);
  st = (unsigned int) times / 2;
  seed = (int) st;
  if ( (seed%2) == 0 ) seed = seed + 1;
  return(seed);
}


void Random::InitR1(int seed) {
  IX17 = rnd17(seed);
}

int Random::GetR1 (int min,int max) {
   int j, range;
   double f;
     range = max - min + 1;
     IX17 = rnd17(IX17);
     f = (double) (IX17 * 0.4656613E-9);
     j = min + (int) ( f * range );
     return(j);
 }

void  Random::InitR2(int seed) {
  IX11 = rnd11(seed);
}

int Random::GetR2(int min,int max) {  
   int j, range;
   double f;
     range = max - min + 1;
     IX11 = rnd11(IX11);
     f = (double) (IX11 * 0.4656613E-9);
     j = min + (int) ( f * range );
     return(j);
 }


void Random::InitSign(int seed) {
  IX7 = rnd7(seed);
  bitmask = 1<<15;
};

int Random::Sign () { /* returns a zero/one value **/
     IX7 = rnd7(IX7);
     if (IX7 & bitmask)
     then return(1);
     else return(0);
 }


void Random::InitR3(int seed) {
  IX15 = rnd15(seed);
}

int Random::GetR3(int min,int max) { 
   int j, range;
   double f;
     range = max - min + 1;
     IX15 = rnd15(IX15);
     f = (double) (IX15 * 0.4656613E-9);
     j = min + (int) ( f * range );
     return(j);
 }

/********************************************************************/
