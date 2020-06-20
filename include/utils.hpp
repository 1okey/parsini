#ifndef UTILS_H
#define UTILS_H

#include <string>

std::string inline rtrim(std::string& line) 
{
    auto it = line.begin();
    while(*it == ' ') {
        ++it;
    }
    return std::string(it, line.end());
}

std::pair<std::string, std::string> inline get_key_value(const std::string& line)
{
    std::string key;
    std::string value;
    size_t eq_idx = line.find_first_of('=');

    key = line.substr(0, eq_idx);
    for (char ch : line.substr(eq_idx + 1)) {
        if (ch == ';' || ch == '#') {
            break;
        }
        value.push_back(ch);
    }
    return { key, value };
}

#endif //UTILS_H