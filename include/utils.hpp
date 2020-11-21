#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>


std::string inline rtrim(std::string& line) 
{
    if (line.size() == 0) {
        return std::string();
    }
    auto it = line.begin();
    while(*it == ' ') {
        ++it;
    }
    return std::string(it, line.end());
}

using var_t = std::variant<std::string, std::vector<std::string>>;

std::pair<std::string, var_t> inline get_key_value(const std::string& line)
{
    std::string key;
    std::string value;
    std::vector<std::string> value_list;
    size_t eq_idx = line.find_first_of('=');

    key = line.substr(0, eq_idx);
    for (char ch : line.substr(eq_idx + 1)) {
        if (ch == ' ') {
            continue;
        }
        if (ch == ';' || ch == '#') {
            break;
        }
        if (ch == ',') {
            value_list.push_back(value);
            value.clear();
            continue;
        }
        value.push_back(ch);
    }

    if (value_list.size() > 0) {        
        if (value.size() > 0) {
            value_list.push_back(value);
        }    
        return { key, var_t{value_list} };
    } else {
        return { key, var_t{value} };
    }
}


#endif //UTILS_H