/*
 * Controls.h
 *
 *  Created on: 08.03.2017
 *      Author: zokp
 */

#ifndef CONTROLS_H_
#define CONTROLS_H_

#include <stdint.h>
#include <vector>

/**
 * Controls namespace contains functions/data related to application controls.
 */
namespace Controls
{
	enum
	{
		CTRLS_WIREFRAMED = 0,
		CTRLS_TEXTURED,
		CTRLS_LIGHTING,
		CTRLS_SHADER_PROG,
		CTRLS_OBJ_ROTUP,
		CTRLS_OBJ_ROTDOWN,
		CTRLS_OBJ_ROTLEFT,
		CTRLS_OBJ_ROTRIGHT,
		CTRLS_OBJ_ZOOMIN,
		CTRLS_OBJ_ZOOMOUT,
		CTRLS_PROJ_MAT,
		SHOW_GRID,
		CTRLS_SIZE
	};

	struct CtrlCallback
	{
		void (*callback)(void);
		uint8_t control;
	};

	extern bool gCtrls[CTRLS_SIZE];

	void init(std::vector<CtrlCallback> ctrlCallbacks);
}

#endif /* CONTROLS_H_ */
