#ifndef WMNCIPHER_H
#define WMNCIPHER_H

#include "stdint.h"

#define WMN_CIPHER_BLOCK_SIZE   16  // Size of the cipher block, in bytes

class WmnCipher
{
public:
  WmnCipher();

  void encryptBuffer(uint8_t * buffer_in, uint8_t * buffer_out, uint8_t* key);
  void decryptBuffer(uint8_t * buffer_in, uint8_t * buffer_out, uint8_t* key);


private:
  // The Key input to the AES Program
  uint8_t* mKey;
  static const uint8_t mSBox[256];
  static const uint8_t mRSbox[256];
  static const uint8_t mRcon[255];

  // The array that stores the round keys.
  uint8_t mRoundKey[176];

  static uint8_t mgetSBoxValue(uint8_t num);
  static uint8_t mgetSBoxInvert(uint8_t num);

  void SubBytes(void);
  void InvSubBytes(void);
  void AddRoundKey(uint8_t round);
  void KeyExpansion(void);
  void Cipher(void);
  void InvCipher(void);

};

#endif // WMNCIPHER_H
