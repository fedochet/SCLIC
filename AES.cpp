//
//  AES.cpp
//  tg_temp
//
//  Created by Artemy Pestretsov on 30.11.14.
//  Copyright (c) 2014 Artemy Pestretsov. All rights reserved.
//

#include "AES.h"

vector<unsigned char> AES256_ige_decrypt(vector<unsigned char> encrypted_answer,vector<unsigned char> tmp_aes_iv,vector<unsigned char> tmp_aes_key)
{
    vector<unsigned char> res(encrypted_answer.size(),0);
    AES_KEY key;

//    vector<unsigned char> x_0(tmp_aes_iv.begin(), tmp_aes_iv.begin()+10);
  //  vector<unsigned char> y_0(tmp_aes_iv.begin()+10, tmp_aes_iv.end());
    
   // tmp_aes_iv = mergeVectors(y_0, x_0);
    
    AES_set_decrypt_key(&tmp_aes_key[0], 256, &key);
    
    AES_ige_encrypt(&encrypted_answer[0], &res[0], encrypted_answer.size(), &key, &tmp_aes_iv[0], AES_DECRYPT);
    return res;
}
