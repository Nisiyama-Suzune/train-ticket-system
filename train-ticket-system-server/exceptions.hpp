#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

class exception {
protected:
    const std::string variant = "";
    std::string detail = "";
public:
    exception() {}
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

class container_is_empty : public exception {

};

#endif
