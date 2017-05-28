#define NOMINMAX


// will be created by the pre_built step, see cmake file or project config
#include "strcmprs_out.h" 
#include "../decompress.h" //helper header

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <fstream>
#include <cstring> //strlen, strcmp

TEST_CASE("compare decompressed file", "[decompress]")
{
    read();
    CHECK(strlen(&strout[0]) == strdecmprSize);

    //test_text is saved with LF and not with CRLF
    std::ifstream ifile(std::string(SOURCE_DIR) + "/test_text.txt");
    std::string cmpstring;
    ifile.seekg(0, std::ios::end);
    cmpstring.resize(ifile.tellg());
    ifile.seekg(0, std::ios::beg);
    ifile.read(&cmpstring[0], cmpstring.size());
    ifile.close();

    CHECK(strcmp(&strout[0], &cmpstring[0]) == 0);
}