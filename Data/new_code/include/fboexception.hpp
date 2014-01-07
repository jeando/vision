/*
 * FBOException.h
 *
 *  Created on: 28 Mar 2013
 *      Author: arnaud
 */

#ifndef FBOEXCEPTION_H_
#define FBOEXCEPTION_H_

// standard exceptions
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

/**
 * @brief Describes FBO exception.
 */
class FBOException : public std::runtime_error {
public:
    /**
     * @brief A human readable description of FBO's errors.
     *
     * @param message
     */
    FBOException(const std::string& message)
        : std::runtime_error(message) { };
};



#endif /* FBOEXCEPTION_H_ */
