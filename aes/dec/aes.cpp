#include <iostream>
#include "aes.h"

#include "mbedtls/aes.h"

// https://blog.csdn.net/baidu_39191253/article/details/116603328

int aes_cbc_encrypt(std::string& plaintext, const std::string& key, 
     const std::string& iv, std::string& ciphertext) {
    
    int pad = 16 - (plaintext.size() % 16);

    for(int i = 0; i < pad; i++) {
        plaintext.push_back((char)pad);
    }

    if (ciphertext.size() < plaintext.size()) {
        ciphertext.resize(plaintext.size());
    }

    std::string tmp_iv(iv);

    const unsigned char* input = ( const unsigned char*)plaintext.data();

    unsigned char* output = (unsigned char*)ciphertext.data();

    mbedtls_aes_context ctx;
    mbedtls_aes_init(&ctx);
    

    int rc = mbedtls_aes_setkey_enc(&ctx, (const unsigned char*)key.data(), key.size() * 8);
    if( rc !=0 ) {
        return rc;
    }



    // MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH -0x0022
    rc = mbedtls_aes_crypt_cbc(&ctx, MBEDTLS_AES_ENCRYPT, 
            plaintext.size(),   (unsigned char*)tmp_iv.data(),
            input,output);
    if( rc != 0) {
        return rc;
    }

    mbedtls_aes_free( &ctx );
    return 0;
}


int aes_cbc_decrypt(const std::string& ciphertext, const std::string& key, const std::string& iv, std::string& plaintext)
{
    int rc = -1;
    std::string old;
    if (old.size() < ciphertext.size()) {
        old.resize(ciphertext.size());
    }

    size_t newsize = ciphertext.size();

    std::string tmp_iv(iv);
    const unsigned char* input = (const unsigned char*)ciphertext.c_str();
    
    unsigned char* output = (unsigned char*)old.data();

    mbedtls_aes_context ctx;
    mbedtls_aes_init(&ctx);

    rc = mbedtls_aes_setkey_dec(&ctx, (const unsigned char*)key.data(), key.size() * 8);

    rc = mbedtls_aes_crypt_cbc(&ctx,
        MBEDTLS_AES_DECRYPT,
        newsize,
        (unsigned char*)tmp_iv.data(),
        input,
        output);

    int len = ciphertext.size();
    int pad =  (int) output[len - 1];
    memset(output + len - pad, 0, pad);
    plaintext.clear();
    plaintext = old.substr(0, len - pad);


    mbedtls_aes_free(&ctx);

    return rc;
}
