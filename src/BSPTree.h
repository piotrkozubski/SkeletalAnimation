/*
 * BSPTree.h
 *
 *  Created on: 19.07.2019
 *      Author: piotr.kozubski
 */

#ifndef BSPTREE_H
#define BSPTREE_H

#include <vector>

class Pmap;
class OGLPolygonMesh;

typedef std::vector<OGLPolygonMesh*> PolygonMeshList;

/**
 *
 */
class BSPTree
{
public:

	/**
	 *
	 */
	~BSPTree();

	/**
	 *
	 */
	static BSPTree* create(const Pmap& pPmap);

	/**
	 *
	 */
	PolygonMeshList* getMeshList(void)
	{
		return const_cast<PolygonMeshList*>(&mMeshList);
	}

private:
	/**
	 *
	 */
	BSPTree(PolygonMeshList pMeshList);

	const PolygonMeshList mMeshList;
};

#endif /* BSPTREE_H */
