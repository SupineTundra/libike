/*
 *	This file is a part of libike library.
 *	Copyright (c) 2003-2011 Alex Pankratov. All rights reserved.
 *
 *	http://swapped.cc/libike
 */

/*
 *	The program is distributed under terms of BSD license. 
 *	You can obtain the copy of the license by visiting:
 *
 *	http://www.opensource.org/licenses/bsd-license.php
 */

/*
 *	$Id: cipher_bf.c,v 1.1 2003/04/27 21:37:37 alex Exp $
 */

#include "x4/crypto/cipher.h"

#include <openssl/blowfish.h>

/*  */
static size_t _bf_init_kl(size_t kbits)
{
  // accept only 8 bit aligned keys between 40 and 448 bits in length
  return (! (kbits & 0x07) && 40 <= kbits && kbits <= 448) ? 
         kbits : 
         kbits ? 0 : 448;            
}

/*  */
static x4s_cipher_key * _bf_init_ks(size_t kbits, const void * key)
{
  x4s_cipher_key * ck;

  x4_assert(key);
  
  if (!kbits || !_bf_init_kl(kbits))
    return 0;

  ck = x4_malloc(sizeof(*ck)-1+sizeof(BF_KEY));
  if (! ck)
    return 0;

  ck->klen = kbits/8;
  BF_set_key((BF_KEY*)ck->opaque, kbits/8, key);

  return ck;
}

/*  */
static void _bf_encrypt(x4s_cipher_key * key, uint8 * iv,
                         const void * in, void * out, size_t n)
{
  x4_assert(key && iv && in && out && n);  
  
  BF_cbc_encrypt(in, out, 8*n, (BF_KEY*)key->opaque, iv, BF_ENCRYPT);
}

/*  */
static void _bf_decrypt(x4s_cipher_key * key, uint8 * iv,
                         const void * in, void * out, size_t n)
{
  x4_assert(key && iv && in && out && n);  
  
  BF_cbc_encrypt(in, out, 8*n, (BF_KEY*)key->opaque, iv, BF_DECRYPT);
}

/*
 *
 */
static x4s_cipher_alg ca_bf = { 8, 
                                _bf_init_kl,
                                _bf_init_ks, 
                                _bf_encrypt, 
                                _bf_decrypt };

x4s_cipher_alg * x4v_blowfish = &ca_bf;
