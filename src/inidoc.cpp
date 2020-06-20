
#include "inidoc.hpp"

void inidoc::_copy(const inidoc& other) noexcept {
    if (*this != other) {
        this->sections = other.sections;
    }
}

inidoc::inidoc(){}

inidoc::inidoc(inidoc&& tmp_doc) {
    sections = tmp_doc.sections;
}

inidoc::inidoc(const inidoc& other) {
    _copy(other);
}

inidoc& inidoc::operator=(const inidoc& other) {
    _copy(other);
    return *this;
}

inidoc& inidoc::operator=(inidoc&& other) {
    _copy(other);
    return *this;
}

std::set<std::string> inidoc::GetSectionNames() {
    std::set<std::string> section_names;
    for(auto& [key, _]: sections) {
        section_names.insert(key);
    }
    return section_names;
}

inisection& inidoc::GetSection(const std::string& section_name) {
    auto section = sections.find(section_name);
    if (section == sections.end()) {
        throw new std::exception();
    }
    return section->second;
}

const inisection& inidoc::GetSection(const std::string& section_name) const {
    return const_cast<const inisection&>(GetSection(section_name));
}

iniparam& inidoc::GetParameter(
    const std::string& section_name, const std::string& param_name
) {
    auto section = sections.find(section_name);
    if (section == sections.end()) {
        throw new std::exception();
    }
    auto param = section->second.find(param_name);
    if (param == section->second.end()) {
        throw new std::exception();
    }
    iniparam& param_v = param->second;
    return param_v;    }

const iniparam& inidoc::GetParameter(
    const std::string& section_name, const std::string& param_name
) const {
    return const_cast<const iniparam&>(GetParameter(section_name, param_name));
}

inidoc& inidoc::AddParam(
    const std::string& section_name, const std::string& key, const std::string& value
) {
    auto section = sections.find(section_name);
    if (section == sections.end()) {
        throw new std::exception();
    }
    section->second.insert({key, value});
    return *this;
}
inidoc& inidoc::AddSection(const std::string& section_name) noexcept {
    auto section = sections.find(section_name);
    if (section == sections.end()) {
        sections.insert({section_name, inisection()});
    }
    return *this;
}
bool inidoc::operator==(const inidoc& other) noexcept {
    return sections == other.sections;
}
bool inidoc::operator!=(const inidoc& other) noexcept {
    return !(*this == other);
}


inidoc load_ini(std::ifstream& file_stream)
{
    std::string line;
    std::string cur_section;
    std::string key, value;
    size_t closing_idx;
    inidoc doc;
    while(!file_stream.eof()) {
        getline(file_stream, line);
        line = rtrim(line);
        
        if (isalpha(*line.begin()) && !cur_section.empty()) {
            auto pair = get_key_value(line);
            doc.AddParam(cur_section, pair.first, pair.second);
        } else {
            switch(*line.begin()) {
                case '[':
                    closing_idx = line.find_last_of(']');
                    if (closing_idx == std::string::npos) {
                        throw new std::exception();
                    }
                    cur_section = line.substr(1, closing_idx - 1);
                    doc.AddSection(cur_section);
                    break;
                case ';': case '#':
                    continue;
                case '\n':
                    cur_section.clear();
                    break;
            }
        }
    }
    return std::move(doc);
}

inidoc load_ini(const std::string& file_name)
{
    std::ifstream file;
    file.open(file_name, std::ios::in);
    inidoc doc;
    if(file.is_open()) {
        doc = load_ini(file);
        file.close();
    }
    return doc;
}
