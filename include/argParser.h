#ifndef _ARG_PARSER_H_
#define _ARG_PARSER_H_

#include <iostream>
#include <string>
#include <utility>

std::string parseFilename(int argc, char **argv) {
    if(argc == 2) {
        // correct number of args 
        std::string filename = argv[1];
        return filename;
    } else {
        // args are incorrect
        return ""; 
    }
}

std::pair<std::string, std::string> parseParam(std::string rawParam) {
    std::string paramKey = "";      /// Param name
    std::string paramValue = "";    /// Param value

    for( auto &it : rawParam ) {
        std::cout << "thing: '" << it << "'" << std::endl;
    }

    return std::make_pair(paramKey, paramValue);
}

#endif
