#include "WmnCipher.h"
#include "string.h"

// The number of columns comprising a state in AES. This is a constant in AES. Value=4
#define Nb 4
// The number of 32 bit words in a key.
#define Nk 4
// The number of rounds in AES Cipher.
#define Nr 10

/**
 * @brief WmnCipher constructor
 */
WmnCipher::WmnCipher()
{

}

// The lookup-tables are marked const so they can be placed in read-only storage instead of RAM
// The numbers below can be computed dynamically trading ROM for RAM -
// This can be useful in (embedded) bootloader applications, where ROM is often limited.
const uint8_t WmnCipher::mSBox[256] =   {
  //0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
  0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
  0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
  0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
  0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
  0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
  0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
  0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
  0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
  0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
  0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
  0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
  0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
  0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
  0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
  0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
  0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };

const uint8_t WmnCipher::mRSbox[256] =
{ 0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
  0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
  0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
  0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
  0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
  0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
  0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
  0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
  0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
  0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
  0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
  0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
  0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
  0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
  0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
  0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };

// The round constant word array, Rcon[i], contains the values given by
// x to th e power (i-1) being powers of x (x is denoted as {02}) in the field GF(2^8)
// Note that i starts at 1, not 0).
const uint8_t WmnCipher::mRcon[255] = {
  0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a,
  0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39,
  0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a,
  0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8,
  0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef,
  0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc,
  0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b,
  0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3,
  0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94,
  0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20,
  0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35,
  0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f,
  0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04,
  0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63,
  0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd,
  0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb  };

/**
 * @brief Single 16-byte buffer encryption
 * @param input : Pointer to the input array
 * @param output : Pointer to the output array
 * @param key : Pointer to the key
 */
void WmnCipher::encryptBuffer(
        uint8_t* input,
        uint8_t* output,
        uint8_t* key)
{
  // Copy input to output, and work in-memory on output
  memcpy(output, input, WMN_CIPHER_BLOCK_SIZE);
  //BlockCopy(output, input);
  mState = (state_t*)output;

  mKey = key;
  mKeyExpansion();

  // The next function call encrypts the PlainText with the Key using AES algorithm.
  mCipher();
}

/**
 * @brief Several 16-byte buffers encryption
 * @param input : Pointer to the input array
 * @param output : Pointer to the output array
 * @param blocks : Number of blocks to encode
 * @param key : Pointer to the key
 */
void WmnCipher::encryptBuffer(
        uint8_t* input,
        uint8_t* output,
        uint8_t blocks,
        uint8_t* key)
{
  while (blocks > 0)
  {
    encryptBuffer(input, output, key);
    input += WMN_CIPHER_BLOCK_SIZE;
    output += WMN_CIPHER_BLOCK_SIZE;
    blocks--;
  }
}

/**
 * @brief Single 16-byte buffer decryption
 * @param input : Pointer to the input array
 * @param output : Pointer to the output array
 * @param key : Pointer to the key
 */
void WmnCipher::decryptBuffer(
        uint8_t* input,
        uint8_t *output,
        uint8_t* key)
{
  // Copy input to output, and work in-memory on output
  memcpy(output, input, WMN_CIPHER_BLOCK_SIZE);
  //BlockCopy(output, input);
  mState = (state_t*)output;

  // The KeyExpansion routine must be called before encryption.
  mKey = key;
  mKeyExpansion();

  mInvCipher();
}

/**
 * @brief Several 16-byte buffers decryption
 * @param input : Pointer to the input array
 * @param output : Pointer to the output array
 * @param blocks : Number of blocks to decode
 * @param key : Pointer to the key
 */
void WmnCipher::decryptBuffer(
        uint8_t* input,
        uint8_t *output,
        uint8_t blocks,
        uint8_t* key)
{
  while (blocks > 0)
  {
    decryptBuffer(input, output, key);
    input += WMN_CIPHER_BLOCK_SIZE;
    output += WMN_CIPHER_BLOCK_SIZE;
    blocks--;
  }
}

inline uint8_t WmnCipher::mgetSBoxValue(uint8_t num)
{
  return mSBox[num];
}

inline uint8_t WmnCipher::mgetSBoxInvert(uint8_t num)
{
  return mRSbox[num];
}

/**
 * @brief This function produces Nb(Nr+1) round keys.
 * The round keys are used in each round to decrypt the states.
 */
void WmnCipher::mKeyExpansion(void)
{
  uint32_t i, j, k;
  uint8_t tempa[4]; // Used for the column/row operations

  // The first round key is the key itself.
  for(i = 0; i < Nk; ++i)
  {
    mRoundKey[(i * 4) + 0] = mKey[(i * 4) + 0];
    mRoundKey[(i * 4) + 1] = mKey[(i * 4) + 1];
    mRoundKey[(i * 4) + 2] = mKey[(i * 4) + 2];
    mRoundKey[(i * 4) + 3] = mKey[(i * 4) + 3];
  }

  // All other round keys are found from the previous round keys.
  for(; (i < (Nb * (Nr + 1))); ++i)
  {
    for(j = 0; j < 4; ++j)
    {
      tempa[j]=mRoundKey[(i-1) * 4 + j];
    }
    if (i % Nk == 0)
    {
      // This function rotates the 4 bytes in a word to the left once.
      // [a0,a1,a2,a3] becomes [a1,a2,a3,a0]

      // Function RotWord()
      {
        k = tempa[0];
        tempa[0] = tempa[1];
        tempa[1] = tempa[2];
        tempa[2] = tempa[3];
        tempa[3] = k;
      }

      // SubWord() is a function that takes a four-byte input word and
      // applies the S-box to each of the four bytes to produce an output word.

      // Function Subword()
      {
        tempa[0] = mgetSBoxValue(tempa[0]);
        tempa[1] = mgetSBoxValue(tempa[1]);
        tempa[2] = mgetSBoxValue(tempa[2]);
        tempa[3] = mgetSBoxValue(tempa[3]);
      }

      tempa[0] =  tempa[0] ^ mRcon[i/Nk];
    }
    else if (Nk > 6 && i % Nk == 4)
    {
      // Function Subword()
      {
        tempa[0] = mgetSBoxValue(tempa[0]);
        tempa[1] = mgetSBoxValue(tempa[1]);
        tempa[2] = mgetSBoxValue(tempa[2]);
        tempa[3] = mgetSBoxValue(tempa[3]);
      }
    }
    mRoundKey[i * 4 + 0] = mRoundKey[(i - Nk) * 4 + 0] ^ tempa[0];
    mRoundKey[i * 4 + 1] = mRoundKey[(i - Nk) * 4 + 1] ^ tempa[1];
    mRoundKey[i * 4 + 2] = mRoundKey[(i - Nk) * 4 + 2] ^ tempa[2];
    mRoundKey[i * 4 + 3] = mRoundKey[(i - Nk) * 4 + 3] ^ tempa[3];
  }
}

/**
 * @brief Adds the round key to state
 * @param round
 */
void WmnCipher::mAddRoundKey(uint8_t round)
{
  uint8_t i,j;
  for(i=0;i<4;++i)
  {
    for(j = 0; j < 4; ++j)
    {
      (*mState)[i][j] ^= mRoundKey[round * Nb * 4 + i * Nb + j];
    }
  }
}

/**
 * @brief Substitutes the values in the state matrix with values in an S-box.
 */
void WmnCipher::mSubBytes(void)
{
  uint8_t i, j;
  for(i = 0; i < 4; ++i)
  {
    for(j = 0; j < 4; ++j)
    {
      (*mState)[j][i] = mgetSBoxValue((*mState)[j][i]);
    }
  }
}

/**
 * @brief Shifts the rows in the state to the left.
 * Each row is shifted with different offset.
 * Offset = Row number. So the first row is not shifted.
 */
void WmnCipher::mShiftRows(void)
{
  uint8_t temp;

  // Rotate first row 1 columns to left
  temp            = (*mState)[0][1];
  (*mState)[0][1] = (*mState)[1][1];
  (*mState)[1][1] = (*mState)[2][1];
  (*mState)[2][1] = (*mState)[3][1];
  (*mState)[3][1] = temp;

  // Rotate second row 2 columns to left
  temp            = (*mState)[0][2];
  (*mState)[0][2] = (*mState)[2][2];
  (*mState)[2][2] = temp;

  temp            = (*mState)[1][2];
  (*mState)[1][2] = (*mState)[3][2];
  (*mState)[3][2] = temp;

  // Rotate third row 3 columns to left
  temp            = (*mState)[0][3];
  (*mState)[0][3] = (*mState)[3][3];
  (*mState)[3][3] = (*mState)[2][3];
  (*mState)[2][3] = (*mState)[1][3];
  (*mState)[1][3] = temp;
}

static uint8_t xtime(uint8_t x)
{
  return ((x<<1) ^ (((x>>7) & 1) * 0x1b));
}

/**
 * @brief Mixes the columns of the state matrix
 */
void WmnCipher::mMixColumns(void)
{
  uint8_t i;
  uint8_t Tmp,Tm,t;
  for(i = 0; i < 4; ++i)
  {
    t   = (*mState)[i][0];
    Tmp = (*mState)[i][0] ^ (*mState)[i][1] ^ (*mState)[i][2] ^ (*mState)[i][3] ;
    Tm  = (*mState)[i][0] ^ (*mState)[i][1] ; Tm = xtime(Tm);   (*mState)[i][0] ^= Tm ^ Tmp ;
    Tm  = (*mState)[i][1] ^ (*mState)[i][2] ; Tm = xtime(Tm);   (*mState)[i][1] ^= Tm ^ Tmp ;
    Tm  = (*mState)[i][2] ^ (*mState)[i][3] ; Tm = xtime(Tm);   (*mState)[i][2] ^= Tm ^ Tmp ;
    Tm  = (*mState)[i][3] ^ t ;        Tm = xtime(Tm);  (*mState)[i][3] ^= Tm ^ Tmp ;
  }
}

/**
 * @brief Multiply numbers in the field GF(2^8)
 */
uint8_t WmnCipher::mMultiply(uint8_t x, uint8_t y)
{
  return (((y & 1) * x) ^
       ((y>>1 & 1) * xtime(x)) ^
       ((y>>2 & 1) * xtime(xtime(x))) ^
       ((y>>3 & 1) * xtime(xtime(xtime(x)))) ^
       ((y>>4 & 1) * xtime(xtime(xtime(xtime(x))))));
}

/**
 * @brief Mixes the columns of the state matrix.
 * The method used to multiply may be difficult to
 * understand for the inexperienced.
 * Please use the references to gain more information.
 */
void WmnCipher::mInvMixColumns(void)
{
  int i;
  uint8_t a,b,c,d;
  for(i=0;i<4;++i)
  {
    a = (*mState)[i][0];
    b = (*mState)[i][1];
    c = (*mState)[i][2];
    d = (*mState)[i][3];

    (*mState)[i][0] = mMultiply(a, 0x0e) ^ mMultiply(b, 0x0b) ^ mMultiply(c, 0x0d) ^ mMultiply(d, 0x09);
    (*mState)[i][1] = mMultiply(a, 0x09) ^ mMultiply(b, 0x0e) ^ mMultiply(c, 0x0b) ^ mMultiply(d, 0x0d);
    (*mState)[i][2] = mMultiply(a, 0x0d) ^ mMultiply(b, 0x09) ^ mMultiply(c, 0x0e) ^ mMultiply(d, 0x0b);
    (*mState)[i][3] = mMultiply(a, 0x0b) ^ mMultiply(b, 0x0d) ^ mMultiply(c, 0x09) ^ mMultiply(d, 0x0e);
  }
}

/**
 * @brief Substitutes the values in the state matrix with values in an S-box.
 */
void WmnCipher::mInvSubBytes(void)
{
  uint8_t i,j;
  for(i=0;i<4;++i)
  {
    for(j=0;j<4;++j)
    {
      (*mState)[j][i] = mgetSBoxInvert((*mState)[j][i]);
    }
  }
}

void WmnCipher::mInvShiftRows(void)
{
  uint8_t temp;

  // Rotate first row 1 columns to right
  temp=(*mState)[3][1];
  (*mState)[3][1]=(*mState)[2][1];
  (*mState)[2][1]=(*mState)[1][1];
  (*mState)[1][1]=(*mState)[0][1];
  (*mState)[0][1]=temp;

  // Rotate second row 2 columns to right
  temp=(*mState)[0][2];
  (*mState)[0][2]=(*mState)[2][2];
  (*mState)[2][2]=temp;

  temp=(*mState)[1][2];
  (*mState)[1][2]=(*mState)[3][2];
  (*mState)[3][2]=temp;

  // Rotate third row 3 columns to right
  temp=(*mState)[0][3];
  (*mState)[0][3]=(*mState)[1][3];
  (*mState)[1][3]=(*mState)[2][3];
  (*mState)[2][3]=(*mState)[3][3];
  (*mState)[3][3]=temp;
}


/**
 * @brief Main function that encrypts the PlainText.
 */
void WmnCipher::mCipher(void)
{
  uint8_t round = 0;

  // Add the First round key to the state before starting the rounds.
  mAddRoundKey(0);

  // There will be Nr rounds.
  // The first Nr-1 rounds are identical.
  // These Nr-1 rounds are executed in the loop below.
  for(round = 1; round < Nr; ++round)
  {
    mSubBytes();
    mShiftRows();
    mMixColumns();
    mAddRoundKey(round);
  }

  // The last round is given below.
  // The MixColumns function is not here in the last round.
  mSubBytes();
  mShiftRows();
  mAddRoundKey(Nr);
}

/**
 * @brief Main function that decrypts the PlainText.
 */
void WmnCipher::mInvCipher(void)
{
  uint8_t round=0;

  // Add the First round key to the state before starting the rounds.
  mAddRoundKey(Nr);

  // There will be Nr rounds.
  // The first Nr-1 rounds are identical.
  // These Nr-1 rounds are executed in the loop below.
  for(round=Nr-1;round>0;round--)
  {
    mInvShiftRows();
    mInvSubBytes();
    mAddRoundKey(round);
    mInvMixColumns();
  }

  // The last round is given below.
  // The MixColumns function is not here in the last round.
  mInvShiftRows();
  mInvSubBytes();
  mAddRoundKey(0);
}
