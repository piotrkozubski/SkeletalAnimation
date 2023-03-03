/*
 * pmap.h
 *
 *  Created on: 17.07.2019
 *      Author: piotr.kozubski
 */

#ifndef PMAP_H
#define PMAP_H

#include "Array.h"
#include <stdint.h>
#include <memory>
#include "PolygonMesh.h"

/**
 * The class provides support for polygon map files
 */
class Pmap
{
public:
	/**
	 *	Creates Pmap object from file
	 *
	 *	@param 	pFileName	file name of the pmap file
	 *	@return				pointer to pmap object
	 */
	static Pmap* createFromFile(const char* pFileName);

	/**
	 * Destructor
	 */
	~Pmap(void);

	/**
	 * Returns reference to vertex position list
	 */
	inline const Array<VertexPos>& getVertexPosList(void) const
	{
		return *mVertexPosList.get();
	}

	/**
	 * Returns reference to vertex UV list
	 */
	inline const Array<VertexUV>& getVertexUVList(void) const
	{
		return *mVertexUVList.get();
	}

	/**
	 * Returns reference to face list
	 */
	inline const Array< Face<3> >& getFaceList(void) const
	{
		return *mFaceList.get();
	}

	/**
	 * Returns reference to face list
	 */
	inline const Array<Normal>& getFaceNormalList(void) const
	{
		return *mFaceNormalList.get();
	}

	typedef std::unique_ptr<Pmap> Ptr;

private:
	Pmap(Array<VertexPos>::Ptr&  pVertexPosList,
		 Array<VertexUV>::Ptr&   pVertexUVList,
		 Array< Face<3> >::Ptr&     pFaceList,
		 Array<Normal>::Ptr& pFaceNormalList);

	const Array<VertexPos>::Ptr  mVertexPosList;
	const Array<VertexUV>::Ptr 	 mVertexUVList;
	const Array< Face<3> >::Ptr     mFaceList;
	const Array<Normal>::Ptr mFaceNormalList;
};

#endif /* PMAP_H */
