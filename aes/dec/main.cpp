#include <iostream>
#include <string>
#include <zlib.h>
#include <fstream>


using namespace std;
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

    printf("data : %s , c2 : %s \n", data.c_str(), c2.c_str());

    printf("file : %s \n", file.c_str());

    std::string out = file + ".txt";
    std::ofstream outFile(out, ios::out | ios::binary );
     if(!outFile) {
        std::cout << "outFile create failed" << std::endl;
        return 0;
    }
   
    printf("dst size : %ld \n", dst.size());

    outFile.write(dst.c_str(), dst.size());
  
    return 0;
}




