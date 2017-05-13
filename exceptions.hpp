#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <iostream>

namespace sjtu {

class exception {
protected:
    const std::string variant = "";
    std::string detail = "";
public:
    exception() {}
    exception(const std::string var, const std::string det) : variant(var), detail(det) {}
    exception(const exception &ec) : variant(ec.variant), detail(ec.detail) {}

    virtual std::string what() {
        return variant + " " + detail;
    }
};

class no_permission : public exception {

};

class index_out_of_bound : public exception {

};

class invalid_iterator : public exception {

};

class ticket_error : public exception {

};

class line_error : public exception {

};

class container_is_empty : public exception {

};

}

#endif
