#ifndef _SHADER_TEMPLATE_H
#define _SHADER_TEMPLATE_H
#include <string>
#include <map>
#include <functional>

class TemplateShader {
public:
    TemplateShader(const std::string& filename);
    void addSymbol(const std::string& symbol, const std::string& body);
    void preprocess();
    std::string getCode() const;
    std::string getFileName() const;
private:
    void preprocessLine(const std::string& line);
    void preprocessDirective(const std::string& line);

    using Directive = std::function<std::string(TemplateShader&, const std::string&)>;
    static const std::map<std::string, Directive> directives;

    std::map<std::string, std::string> symbols;
    std::string filename;
    std::string code;

    friend std::string directive_insert(TemplateShader& sh, const std::string& body);
    friend std::string directive_recursive_insert(TemplateShader& sh, const std::string& body);
    friend std::string directive_include(TemplateShader& sh, const std::string& body);
    friend std::string directive_recursive_include(TemplateShader& sh, const std::string& body);
    
};

#endif
