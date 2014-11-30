//
//  AES.h
//  tg_temp
//
//  Created by Artemy Pestretsov on 30.11.14.
//  Copyright (c) 2014 Artemy Pestretsov. All rights reserved.
//

#ifndef __tg_temp__AES__
#define __tg_temp__AES__

#include <stdio.h>
#include "hardcode.hpp";

vector<unsigned char> AES256_ige_decrypt(vector<unsigned char> encrypted_answer,vector<unsigned char> tmp_aes_iv,vector<unsigned char> tmp_aes_key);

#endif /* defined(__tg_temp__AES__) */
