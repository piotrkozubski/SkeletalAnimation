/*
 * Array.h
 *
 *  Created on: 16.07.2017
 *      Author: Piotr Kozubski
 */

#ifndef ARRAY_H_
#define ARRAY_H_

#include <stdint.h>
#include <memory>
#include "Logger.h"

/**
 * Class template for arrays implementation
 */
template <class T> class Array
{
private:
	/**
	 * Disabled constructors
	 */
	Array();
	Array(const Array& a);

	/**
	 * Disabled operator
	 */
	Array<T>& operator=(const Array<T>& other);

public:

	/**
	 * Constructor. Allocates memory for array
	 *
	 * @param 	size of array
	 */
	Array(uint32_t pSize);

	/**
	 * Destructor. Deallocates array memory
	 */
	~Array();

public:
	/**
	 * Generic array pointer
	 */
	T	    	   * const mArray;

	/**
	 * array size
	 */
	const uint32_t  mArraySize;

	/**
	 * typedef of smart pointer to BonesFrame
	 */
	typedef std::unique_ptr<Array<T> > Ptr;
};

template <class T> Array<T>::Array(uint32_t pSize) :
		mArray(new T[pSize]),
		mArraySize(pSize)
{
}

template <class T> Array<T>::~Array()
{
	if (mArray)
	{
		delete[] mArray;
	}
}

//template <class T> void Array<T>::reset(uint32_t pSize)
//{
//	if (mArray)
//	{
//		delete[] mArray;
//	}
//	mArraySize = pSize;
//	mArray = new T[pSize];
//}

#endif /* ARRAY_H_ */
