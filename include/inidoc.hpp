#ifndef INIDOC_H
#define INIDOC_H

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <set>
#include <variant>

#include "utils.hpp"

using iniparam = std::variant<std::string, std::vector<std::string>>;
using inisection = std::unordered_map<std::string, iniparam>;

class inidoc
{
private:
    std::unordered_map<std::string, inisection> sections;    
private:
    void _copy(const inidoc& other) noexcept;
public:
    inidoc();
    inidoc(inidoc&& tmp_doc);
    inidoc(const inidoc& other);

    inidoc& operator=(const inidoc& other);
    inidoc& operator=(inidoc&& other);

    std::set<std::string> GetSectionNames();
    
    inisection& GetSection(const std::string& section_name);
    iniparam GetParameter(const std::string& section_name, const std::string& param_name);

    const inisection& GetSection(const std::string& section_name) const;

    inidoc& AddParam(
        const std::string& section_name, 
        const std::string& key, 
        const std::string& value
    );

    inidoc& AddParam(
        const std::string& section_name, 
        const std::string& key, 
        const std::vector<std::string>& value
    );

    inidoc& AddSection(const std::string& section_name) noexcept;
    bool operator==(const inidoc& other) const noexcept;
    bool operator!=(const inidoc& other) const noexcept;
};

inidoc load(std::ifstream& file_stream);

inidoc load(const std::string& file_stream);

void save(const inidoc& document);

#endif //INIDOC_H