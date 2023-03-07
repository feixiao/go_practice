#ifndef _AES_H
#define _AES_H


#include <iostream>
#include <string>


int aes_cbc_decrypt(const std::string& ciphertext, const std::string& key,
     const std::string& iv, std::string& plaintext);


#endif
