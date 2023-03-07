#include <iostream>

int aes_cbc_encrypt(const std::string& plaintext, const std::string& key, 
     const std::string& iv, std::string& ciphertext);

int aes_cbc_decrypt(const std::string& ciphertext, const std::string& key,
     const std::string& iv, std::string& plaintext);