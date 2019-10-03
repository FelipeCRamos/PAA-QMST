#ifndef _HELP_FN_H_
#define _HELP_FN_H_

#include <iostream>
#include <sstream>

std::string error(std::string errorName, std::string helpText = "") {
    std::stringstream ss;
    ss << "AGMQ: ";

    std::string defaultHelpText = "Para usar o programa, basta digitar:\n\n\t./agmq arquivoEntrada.txt";
    ss << errorName << ": " << (helpText.empty() ? "\n\n" + defaultHelpText : helpText) << "\n";
    return ss.str();
}

#endif
