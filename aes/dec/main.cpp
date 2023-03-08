#include <iostream>
#include <string>
#include <zlib.h>
#include <fstream>

#include "aes.h"
#include "encryptor.h"
#include "mbedtls/aes.h"

using namespace std;

void test();
void test2();
void test3();


int main(int argc, char* argv[]) {

    int o = 0;
    const char *optstring = "f:";
    std::string file;
    while ((o = getopt(argc, argv, optstring)) != -1) {
        switch (o) {
            case 'f':
                printf("opt is f, oprarg is: %s\n", optarg);
                file.append(optarg, strlen(optarg));
                break;
            case '?':
                printf("error optopt: %c\n", optopt);
                printf("error opterr: %d\n", opterr);
                break;
        }
    }
    // test();
    // test2();
    test3();

    std::string data("12345678900987654321");

    // printf("data : %s , c2 : %s \n", data.c_str(), c2.c_str());

    // printf("file : %s \n", file.c_str());

    // std::string out = file + ".txt";
    // std::ofstream outFile(out, ios::out | ios::binary );
    //  if(!outFile) {
    //     std::cout << "outFile create failed" << std::endl;
    //     return 0;
    // }
   
    // printf("dst size : %ld \n", dst.size());

    // outFile.write(dst.c_str(), dst.size());
  
    return 0;
}



void test() {
      int i;
    
    mbedtls_aes_context aes_ctx;
    
    //秘银数值
    unsigned char key[16] = "CBCPASSWD1234";
    //iv
    unsigned char iv[16];
    
    //明文空间
    unsigned char plain[64] = "ZhangShiSan!!ZhangShiSan!!ZhangShiSan!!ZhangShiSan!!";
    //解密后明文的空间
    unsigned char dec_plain[64]={0};

    //密文空间
    unsigned char cipher[64]={0};
 
    
    mbedtls_aes_init( &aes_ctx );
    
    //设置加密秘银
    printf("\r\n\r\n\r\nAES CBC:\r\nBefore Encryption: %s\r\n", plain);
    
    // int mbedtls_aes_setkey_enc( mbedtls_aes_context *ctx, const unsigned char *key, unsigned int keybits );
    // int mbedtls_aes_setkey_dec( mbedtls_aes_context *ctx, const unsigned char *key, unsigned int keybits );

    // ctx：句柄
    // key：密钥，必须16、24或32Bytes
    // keybits：密钥长度，128、192或256bit

    mbedtls_aes_setkey_enc( &aes_ctx, key, 128);
    for(i = 0; i < 16; i++)
    {
        iv[i] = 0x01;
    }

    // CBC加/解密，可以加密任意长度的明文（必须补齐为16的整倍数）
    // int mbedtls_aes_crypt_cbc( mbedtls_aes_context *ctx,
    //                 int mode,                // MBEDTLS_AES_ENCRYPT或MBEDTLS_AES_DECRYPT
    //                 size_t length,           // 输入长度（必须补齐为16的整倍数）
    //                 unsigned char iv[16],    // 初始化向量，必须为16字节且可读写。每个块运算完之后会生成新的向量传给下一个块运算，所以加密完成后此值会改变，解密时应该重新赋值。
    //                 const unsigned char *input,
    //                 unsigned char *output );

    mbedtls_aes_crypt_cbc(&aes_ctx, MBEDTLS_AES_ENCRYPT, 64, iv, plain, cipher);
    printf("After encryption:");
    for (i = 0; i<64; i++)
    {
        printf("%c", cipher[i] );            
    }
    printf(" -End\r\n");
       
    //设置解密秘银
    mbedtls_aes_setkey_dec(&aes_ctx, key, 128);
    for(i = 0; i < 16; i++)
    {
        iv[i] = 0x01;
    }
    mbedtls_aes_crypt_cbc(&aes_ctx, MBEDTLS_AES_DECRYPT, 64, iv, cipher, dec_plain);
    printf("After Decrypt %s\r\n", dec_plain);
    printf("\r\n");
    mbedtls_aes_free( &aes_ctx );
}


void test2() {
    std::string tmp("OGU1NTlmZDA2MmJmMWI5MGYwMjBmOTUzMTBiYWEyNDI");
    int count = 16;
    std::string key = tmp.substr(0,count);

    printf("key size : %d, key : %s\n", key.size(), key.c_str());
    //std::string plaintext("一个普通的山村穷小子，偶然之下，跨入到一个江湖小门派，虽然资质平庸，但依靠自身努力和合理算计最后修炼成仙的故事。");
    
    unsigned char plain[64] = "ZhangShiSan!!ZhangShiSan!!ZhangShiSan!!ZhangShiSan!!";
    std::string plaintext((char*)plain,64);
    std::string iv;
    std::string ciphertext;

    for(int i = 0; i < 16; i++)
    {
        iv.push_back(0x01);
    }

    printf("[1] plaintext size : %d, plaintext : %s\n", plaintext.size(), plaintext.c_str());
    int ret = aes_cbc_encrypt(plaintext, key, iv, ciphertext);
    if( ret != 0) {
        printf("aes_cbc_encrypt failed, ret=%d\n", ret);
        return ;
    }


    ret = aes_cbc_decrypt(ciphertext, key, iv, plaintext);
    if( ret != 0) {
        printf("aes_cbc_decrypt failed, ret=%d\n", ret);
        return ;
    }

    printf("[2] plaintext size : %d, plaintext : %s\n", plaintext.size(), plaintext.c_str());    
}


