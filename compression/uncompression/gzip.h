#ifndef LADDER_GZIP_H
#define LADDER_GZIP_H

#include <string>
#include <iostream>

namespace ladder {

class  GZipper {
public:
  static int  DeflateFile(const std::string& filename, std::string& dst);

  static int  Deflate(const std::string& buf, std::string& out);
  static int  Inflate(const std::string& buf, std::string& out);
};

}  // namespace ladder

#endif