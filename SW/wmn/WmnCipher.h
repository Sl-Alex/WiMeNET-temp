#ifndef WMNCIPHER_H
#define WMNCIPHER_H

#include "stdint.h"

/*
 * AES 128 implementation.
 * Based on https://github.com/kokke/tiny-AES128-C
 */

/// Key (and also block) length in bytes [128 bit]
/// Do not change
#define WMN_CIPHER_BLOCK_SIZE   16

class WmnCipher
{
public:
  WmnCipher();

  void encryptBuffer(uint8_t * buffer_in, uint8_t * buffer_out, uint8_t* key);
  void encryptBuffer(uint8_t * buffer_in, uint8_t * buffer_out, uint8_t blocks, uint8_t* key);
  void decryptBuffer(uint8_t * buffer_in, uint8_t * buffer_out, uint8_t* key);
  void decryptBuffer(uint8_t * buffer_in, uint8_t * buffer_out, uint8_t blocks, uint8_t* key);

private:
  typedef uint8_t state_t[4][4];

  // state - array holding the intermediate results during decryption.
  state_t* mState;

  // The Key input to the AES Program
  uint8_t* mKey;
  static const uint8_t mSBox[256];
  static const uint8_t mRSbox[256];
  static const uint8_t mRcon[255];

  // The array that stores the round keys.
  uint8_t mRoundKey[176];

  static uint8_t mgetSBoxValue(uint8_t num);
  static uint8_t mgetSBoxInvert(uint8_t num);

  void mAddRoundKey(uint8_t round);
  void mKeyExpansion(void);

  void mShiftRows(void);
  void mMixColumns(void);
  void mSubBytes(void);

  void mInvShiftRows(void);
  void mInvMixColumns(void);
  void mInvSubBytes(void);

  uint8_t mMultiply(uint8_t x, uint8_t y);

  void mCipher(void);
  void mInvCipher(void);
};

#endif // WMNCIPHER_H
