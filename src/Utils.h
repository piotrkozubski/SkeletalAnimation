/*
 * Utils.h
 *
 *  Created on: May 11, 2013
 *      Author: pkozubsk
 */

#ifndef UTILS_H_
#define UTILS_H_

/**
 * Utils name-space
 *
 * Utils name-space should contain non-member functions which implement
 * common operations.
 */
namespace Utils
{
    /**
     *  Increment operator for user-defined type.
     *
     *  @param  t       reference to 'this' object
     *  @return         object of 'Type' type
     */
    template <class Type, Type max> Type operator++(Type& t)
    {
        if (t >= max)
        {
            return t;
        }

        t = static_cast<Type>(t + 1);

        return t;
    }
}


#endif /* UTILS_H_ */
