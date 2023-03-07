#include <iostream>
#include "aes.h"

#include "mbedtls/aes.h"

// https://blog.csdn.net/baidu_39191253/article/details/116603328

int aes_cbc_encrypt(const std::string& plaintext, const std::string& key, 
     const std::string& iv, std::string& ciphertext) {
    
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
    if (plaintext.size() < ciphertext.size()) {
        plaintext.resize(ciphertext.size());
    }

    size_t newsize = ciphertext.size();

    std::string tmp_iv(iv);
    const unsigned char* input = (const unsigned char*)ciphertext.data();
    unsigned char* output = (unsigned char*)plaintext.data();

    

    mbedtls_aes_context ctx;
    mbedtls_aes_init(&ctx);

    rc = mbedtls_aes_setkey_dec(&ctx, (const unsigned char*)key.data(), key.size() * 8);

    rc = mbedtls_aes_crypt_cbc(&ctx,
        MBEDTLS_AES_DECRYPT,
        newsize,
        (unsigned char*)tmp_iv.data(),
        input,
        output);

    // 兼容openssl, 填充位填充的数据为填充的长度
    // {
    //     int fill_len = output[plaintext.size() - 1];
    //     if (fill_len > 0 && fill_len < 16)
    //     {
    //         bool is_ok = true;
    //         for (size_t i = plaintext.size() - fill_len; i < plaintext.size(); i++)
    //         {
    //             if (output[i] != fill_len) {
    //                 is_ok = false;
    //                 break;
    //             }
    //         }

    //         if (is_ok) {
    //             plaintext.resize(plaintext.size() - fill_len);
    //         }
    //         else {
    //             rc = -2;
    //         }
    //     }
    // }

    mbedtls_aes_free(&ctx);

    return rc;
}
