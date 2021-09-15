#include <regex>
#include "TemplateShader.h"
#include <fstream>
#include <algorithm>
#include <utility>
#include "Utils.h"

std::string directive_insert(TemplateShader& sh, const std::string& body);
std::string directive_recursive_insert(TemplateShader& sh, const std::string& body);
std::string directive_include(TemplateShader& sh, const std::string& body);
std::string directive_recursive_include(TemplateShader& sh, const std::string& body);

const std::map<std::string, TemplateShader::Directive> TemplateShader::directives = {
    {"insert", directive_insert},
    {"recursive_insert", directive_recursive_insert},
    {"include", directive_include},
    {"recursive_include", directive_recursive_include}
};

std::string directive_include(TemplateShader& sh, const std::string& body) {
    std::string output;
    std::ifstream file(body);
    if (file) {
        std::string line;
        while(getline(file, line)) {
            output += line;
            output += "\n";
        }
        file.close();
    } else {
        output = "#error: [include] could not open file '"+body+"'";
    }
    return output;
}

std::string directive_recursive_include(TemplateShader& sh, const std::string& body) {
    std::ifstream file(body);
    if (file) {
        std::string line;
        while(getline(file, line)) {
            sh.preprocessLine(line);
        }
        file.close();
    } else {
        return "#error: [recursive_include] could not open file '"+body+"'";
    }
    return "";
}

std::string directive_insert(TemplateShader& sh, const std::string& body) {
    std::string output;
    if (sh.symbols.find(body) != sh.symbols.end()) {
        output = sh.symbols[body];
    } else {
        output = "#error: [insert] no symbol named '"+body+"' defined";
    }
    return output;
}

std::string directive_recursive_insert(TemplateShader& sh, const std::string& body) {
    std::string expandedText;
    if (sh.symbols.find(body) != sh.symbols.end()) {
        std::istringstream ss(sh.symbols[body]);
        std::string line;
        while (getline(ss, line)) {
            sh.preprocessLine(line);
        }
    } else {
        return "#error: [recursive_insert] no symbol named '"+body+"' defined";
    }
    return "";
}

TemplateShader::TemplateShader(const std::string& filename) : filename(filename) {}

void TemplateShader::addSymbol(const std::string& symbol, const std::string& body){
    symbols[symbol] = body;
}

std::string TemplateShader::getCode() const {
    return code;
}

std::string TemplateShader::getFileName() const {
    return filename;
}

void TemplateShader::preprocess() {
    std::ifstream file;
    file.open(filename, std::ios::in);
    if (file.is_open()) {
        std::string line;
        while(getline(file, line)) {
            preprocessLine(line);
        }
    }
    file.close();
}

void TemplateShader::preprocessLine(const std::string& line) {
    std::regex re("^\\s*#");
    if (std::regex_search(line, re)) {
        std::string prettyLine(std::regex_replace(line, re, ""));
        preprocessDirective(prettyLine);
    } else {
        code += std::move(line) + "\n";
    }
}

void TemplateShader::preprocessDirective(const std::string& line) {
    std::string directive;
    std::string body;

    size_t pos = line.find(" ");
    if (pos == std::string::npos) {
        return;
    }

    directive = line.substr(0, pos);
    body = line.substr(pos+1);
    if (TemplateShader::directives.find(directive) != TemplateShader::directives.end()) {
        code += TemplateShader::directives.at(directive)(*this, body) + "\n";
    } else {
        code += "#" + line + "\n";
    }
}
