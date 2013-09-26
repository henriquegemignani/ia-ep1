#ifndef GOLDMINE_ERRORS_H_
#define GOLDMINE_ERRORS_H_

#include <exception>
#include <string>

class input_error : public std::exception {
  public:
    input_error(const std::string& message) throw()
        : message_(message) {}
        
    virtual const char* what() const throw() {
        return message_.c_str();
    }
    
  private:
    std::string message_;
    
};

#endif // GOLDMINE_ERRORS_H_