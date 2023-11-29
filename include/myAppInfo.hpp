//
// Created by gotoo77 on 05/08/2023.
//
#include <string>
#include <ostream>

class myAppInfo {
private:
public:
    myAppInfo(std::string name, std::string version) : _name(std::move(name)), _version(std::move(version)) {}

    const std::string &getFullName() {
        _fullname = _name + "_v" + _version;
        return _fullname;
    }

    virtual ~myAppInfo() = default;

    friend std::ostream &operator<<(std::ostream &os, const myAppInfo &info) {
        os << "_name: " << info._name << " _version: " << info._version;
        return os;
    }


private:
    std::string _name;
    std::string _version;
    std::string _fullname;

};