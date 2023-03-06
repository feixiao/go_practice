#include <iostream>
#include <string>
#include <zlib.h>
#include <fstream>

#include "gzip.h"

using namespace std;
using namespace ladder;

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


    std::string data("12345678900987654321");

    int ret = 0;
    std::string c1;
    ret = ladder::GZipper::Inflate(data, c1);
    if (ret != Z_OK) {
        printf("Inflate  failed : %d \n",ret);
        return ret;
    }

    std::string c2;
    ret = ladder::GZipper::Deflate(c1, c2);
    if (ret != Z_OK) {
          printf("Deflate  failed : %d \n",ret);
        return ret;
    }

    printf("data : %s , c2 : %s \n", data.c_str(), c2.c_str());

    printf("file : %s \n", file.c_str());

    std::string out = file + ".txt";
    std::ofstream outFile(out, ios::out | ios::binary );
     if(!outFile) {
        std::cout << "outFile create failed" << std::endl;
        return 0;
    }

    std::string  dst;
    ret  = ladder::GZipper::DeflateFile(file, dst);
    if( ret != Z_OK) {
      printf("DeflateFile failed, ret=%d\n", ret);
      return -1;
    }
    printf("dst size : %ld \n", dst.size());

    outFile.write(dst.c_str(), dst.size());
  
    return 0;
}




