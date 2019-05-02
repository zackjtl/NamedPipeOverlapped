#include "stdafx.h"
#pragma hdrstop
//---------------------------------------------------------------------------
#include "randomc.h"
//-----------------------------------------------------------------------------
TRandomMersenne::TRandomMersenne(DWORD Seed)
{
  RandomInit(Seed);
}
//-----------------------------------------------------------------------------
TRandomMersenne::TRandomMersenne(const TRandomMersenne& rhs)
{
  mti = rhs.mti;

  for (int i = 0; i < MERS_N; ++i)
    mt[i] = rhs.mt[i];
}
//-----------------------------------------------------------------------------
TRandomMersenne& TRandomMersenne::operator=(const TRandomMersenne& rhs)
{
  mti = rhs.mti;

  for (int i = 0; i < MERS_N; ++i)
    mt[i] = rhs.mt[i];

  return *this;
}
//-----------------------------------------------------------------------------
void TRandomMersenne::RandomInit(DWORD Seed)
{
  mt[0]= Seed;

  for (mti=1; mti < MERS_N; mti++) {
    mt[mti] = (1812433253UL * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti);
  }
}
//-----------------------------------------------------------------------------
DWORD TRandomMersenne::BRandom(void)
{
  DWORD y;

  if (mti >= MERS_N) {
    // generate MERS_N words at one time
    const DWORD LOWER_MASK = (1LU << MERS_R) - 1; // lower MERS_R bits
    const DWORD UPPER_MASK = -1L  << MERS_R;      // upper (32 - MERS_R) bits
    static const DWORD mag01[2] = {0, MERS_A};

    int kk;
    for (kk=0; kk < MERS_N-MERS_M; kk++) {
      y = (mt[kk] & UPPER_MASK) | (mt[kk+1] & LOWER_MASK);
      mt[kk] = mt[kk+MERS_M] ^ (y >> 1) ^ mag01[y & 1];}

    for (; kk < MERS_N-1; kk++) {
      y = (mt[kk] & UPPER_MASK) | (mt[kk+1] & LOWER_MASK);
      mt[kk] = mt[kk+(MERS_M-MERS_N)] ^ (y >> 1) ^ mag01[y & 1];}

    y = (mt[MERS_N-1] & UPPER_MASK) | (mt[0] & LOWER_MASK);
    mt[MERS_N-1] = mt[MERS_M-1] ^ (y >> 1) ^ mag01[y & 1];
    mti = 0;}

  y = mt[mti++];

  // Tempering (May be omitted):
  y ^=  y >> MERS_U;
  y ^= (y << MERS_S) & MERS_B;
  y ^= (y << MERS_T) & MERS_C;
  y ^=  y >> MERS_L;
  return y;
}
//-----------------------------------------------------------------------------
double TRandomMersenne::Random(void)
{
  return (double)BRandom() * (1./((double)(DWORD)(-1L)+1.));
}
//-----------------------------------------------------------------------------
DWORD TRandomMersenne::IRandom(DWORD min, DWORD max)
{
  return (DWORD)((max - min + 1) * Random()) + min;
}
//-----------------------------------------------------------------------------
void TRandomMersenne::Generate(unsigned char* InBuf, unsigned int Size)
{
  DWORD* ptr = reinterpret_cast<DWORD*>(InBuf);

  for (unsigned int count = Size / sizeof(DWORD); count; --count) {
    *ptr++ = IRandom(0, 0x7ffffffe);
  }

  Size = Size % sizeof(DWORD);

  if (Size) {
    InBuf = reinterpret_cast<unsigned char*>(ptr);

    for (; Size; --Size) {
      *InBuf++ = IRandom(0, 0xfe);
    }
  }
}
//-----------------------------------------------------------------------------

