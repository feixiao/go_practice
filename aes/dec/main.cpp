#include <iostream>
#include <string>
#include <zlib.h>
#include <fstream>

#include "aes.h"
#include "mbedtls/aes.h"

using namespace std;

void test();

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
    test();

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
    mbedtls_aes_setkey_enc( &aes_ctx, key, 128);
    for(i = 0; i < 16; i++)
    {
        iv[i] = 0x01;
    }
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