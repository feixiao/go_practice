#include <iostream>
#include <zlib.h>
#include <fstream>

using namespace std;


int gzipInflate( const std::string& compressedBytes, std::string& uncompressedBytes );

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


    printf("file : %s \n", file.c_str());

    std::ifstream inFile(file,ios::in|ios::binary); 
    if(!inFile) {
        std::cout << "inFile create failed" << std::endl;
        return 0;
    }


    std::string out = file + ".ucmpr";
    std::ofstream outFile(out, ios::out | ios::binary );
     if(!outFile) {
        std::cout << "outFile create failed" << std::endl;
        return 0;
    }

    char buffer[1024] = {"\0"};
    int err = 0;
    uLong bufSize = 1024 * 32;

    // 一直读到文件结束
    while(inFile.read(buffer, 1024)){ 
        // 来取得实际读取的字符数；
        uLong size = inFile.gcount();
       
        std::string src(buffer, size);
        std::string dst;
        // 先解压缩
        if( !gzipInflate(src,dst) ) {
            printf("uncompress faied, ret=%d\n", err);
            return -1;
        }

        printf("read size : %d, dst size : %d \n", size, dst.size());
        outFile.write(dst.c_str(), dst.size());
    }
    return 0;
}




int gzipInflate( const std::string& compressedBytes, std::string& uncompressedBytes ) {  
  if ( compressedBytes.size() == 0 ) {  
    uncompressedBytes = compressedBytes ;  
    return true ;  
  }  
  
  uncompressedBytes.clear() ;  
  
  unsigned full_length = compressedBytes.size() ;  
  unsigned half_length = compressedBytes.size() / 2;  
  
  unsigned uncompLength = full_length ;  
  char* uncomp = (char*) calloc( sizeof(char), uncompLength );  
  
  z_stream strm;  
  strm.next_in = (Bytef *) compressedBytes.c_str();  
  strm.avail_in = compressedBytes.size() ;  
  strm.total_out = 0;  
  strm.zalloc = Z_NULL;  
  strm.zfree = Z_NULL;  
  
  bool done = false ;  
  
  int ret = Z_OK;
  ret =inflateInit2(&strm, (16+MAX_WBITS));
  if (ret != Z_OK) {  
    free( uncomp );  
    return ret;  
  }  
  
  while (!done) {  
    // If our output buffer is too small  
    if (strm.total_out >= uncompLength ) {  
      // Increase size of output buffer  
      char* uncomp2 = (char*) calloc( sizeof(char), uncompLength + half_length );  
      memcpy( uncomp2, uncomp, uncompLength );  
      uncompLength += half_length ;  
      free( uncomp );  
      uncomp = uncomp2 ;  
    }  
  
    strm.next_out = (Bytef *) (uncomp + strm.total_out);  
    strm.avail_out = uncompLength - strm.total_out;  
  
    // Inflate another chunk.  
    int err = inflate (&strm, Z_SYNC_FLUSH);  
    if (err == Z_STREAM_END) done = true;  
    else if (err != Z_OK)  {  
      return err;  
    }  
  }  
  
  ret  = inflateEnd (&strm);
  if ( ret != Z_OK) {  
    free( uncomp );  
    return ret;  
  }  

  for ( size_t i=0; i< strm.total_out; ++i ) {  
    uncompressedBytes += uncomp[ i ];  
  }  
  free( uncomp );  
  return Z_OK ;  
}  