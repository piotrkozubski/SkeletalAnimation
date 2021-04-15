/*
 * GLSLPrograms.cpp
 *
 *  Created on: Apr 30, 2013
 *      Author: pkozubsk
 */
#include <memory>
#include <vector>
#include <fstream>
#include <istream>
#include <string>

#include "GLSLPrograms.h"
#include "OGLShader.h"
#include "OGLProgram.h"
#include "OGLProgramAttrib.h"
#include "Utils.h"
#include "Logger.h"

using namespace GLSLPrograms;

/**
 * GLSL Programs name-space
 *
 * GLSLPrograms name-space should contain non-member functions which implements
 * creation of common GLSL shading programs.
 */
namespace GLSLPrograms
{
    /**
     *  Attributes' flags indicating which attribute is used in the program.
     */
    enum AttribFlag
    {
        ATTRIBFLAG_POS  	= 0x0001,
        ATTRIBFLAG_NOR  	= 0x0002,
        ATTRIBFLAG_COL  	= 0x0004,
        ATTRIBFLAG_TEXCOORD = 0x0008,
		ATTRIBFLAG_BONEIDX  = 0x0010,
		ATTRIBFLAG_BONEWGHT = 0x0020
    };

    /**
     *  Uniforms' flags indicating which uniform is used in the program.
     */
    enum UniformFlag
    {
        UNFFLAG_MDVPROJMAT = 0x0001,
		UNFFLAG_MODELMAT   = 0x0002,
		UNFFLAG_TEXTUNIT   = 0x0004,
		UNFFLAG_MESHCOLOR  = 0x0008,
		UNFFLAG_BONES	   = 0x0010
    };

    /**
     *  Structure describing shading program.
     */
    struct ProgInfo
    {
        const char* vertShaderFileName;
        const char* fragShaderFileName;
        uint16_t    attribFlags;
        uint16_t    uniformFlags;
    };

    /**
     *  Declaration of common GLSL shading programs.
     */
    OGLProgram::Ptr glslProgs[GLSLPR_NUM];

    /**
     *  Definition of GLSL programs info.
     */
    static const ProgInfo progInfoArray[GLSLPR_NUM] =
    {
		{ "res/shaders/vert_mdvproj_phong.glsl", 			 "res/shaders/frag_phong_texture.glsl", ATTRIBFLAG_POS | ATTRIBFLAG_NOR | ATTRIBFLAG_TEXCOORD, 											UNFFLAG_MDVPROJMAT | UNFFLAG_MODELMAT | UNFFLAG_TEXTUNIT},  // GLSLPR_PHONG_LIGHTING
		{ "res/shaders/vert_mdvproj_guard.glsl", 		     "res/shaders/frag_guard_texture.glsl", ATTRIBFLAG_POS | ATTRIBFLAG_NOR | ATTRIBFLAG_TEXCOORD, 											UNFFLAG_MDVPROJMAT | UNFFLAG_MODELMAT | UNFFLAG_TEXTUNIT},  // GLSLPR_GUARD_LIGHTING
		{ "res/shaders/vert_mdvproj.glsl", 	   			 "res/shaders/frag_color.glsl", 	   	  ATTRIBFLAG_POS,                                        										    UNFFLAG_MDVPROJMAT | UNFFLAG_MESHCOLOR                  },  // GLSLPR_COLOR
		{ "res/shaders/vert_mdvproj_phong_bone_anim.glsl", "res/shaders/frag_phong_texture.glsl", ATTRIBFLAG_POS | ATTRIBFLAG_NOR | ATTRIBFLAG_TEXCOORD | ATTRIBFLAG_BONEIDX | ATTRIBFLAG_BONEWGHT, UNFFLAG_MDVPROJMAT | UNFFLAG_MODELMAT | UNFFLAG_TEXTUNIT | UNFFLAG_BONES},  // GLSLPR_PHONG_LIGHTING_BONE_ANIM
    };

    /**
     *  Loads GLSL program into OpenGL.
     *
     *  @param  verShdFName		file name of the vertex shader
     *  @param  fragShdFName	file name of the fragment shader
     *  @param  attribFlags     indicates which attributes are used
     *  @return                 pointer to the OGLProgram
     */
    static OGLProgram* loadProgram(const char* verShdFName,
        						   const char* fragShdFName,
        						   uint16_t    attribFlags);

    /**
     *  Sets attrib structure with attributes' locations.
     *
     *  @param  attribFlags     indicates which attributes are used
     *  @param  attrib          pointer to the first attribute in array of size
     *                          ATTRIBIDX_NUM
     *  @param  attribCount     count of used attributes
     */
    static void setProgAttribs(uint8_t            attribFlags,
                               OGLProgramAttrib*  attrib,
                               uint32_t&          attribCount);

    /**
     *  Sets uniform structure with uniforms' locations.
     *
     *  @param  uniformFlags     indicates which uniforms are used
     *  @param  uniform          pointer to the first uniform in array of size
     	 	 	 	 	 	 	 OGLProgramUniform::UNFNAME_NUM
     */
    static void setProgUniforms(uint8_t            uniformFlags,
    						    OGLProgramUniform* uniform);
}

GLSLPrograms::GLSLProgName
GLSLPrograms::operator++(GLSLPrograms::GLSLProgName& t)
{
    return Utils::operator++<GLSLProgName, GLSLPR_NUM>(t);
}

static void GLSLPrograms::setProgAttribs(uint8_t            attribFlags,
                                         OGLProgramAttrib*  attrib,
                                         uint32_t&          attribCount)
{
    attribCount = 0;

    if (attribFlags & ATTRIBFLAG_POS)
    {
        attrib[attribCount].name = "vPositionAttr";
        attrib[attribCount++].location = OGLProgramAttrib::ATTRIBIDX_VERTPOS;
    }
    if (attribFlags & ATTRIBFLAG_COL)
    {
        attrib[attribCount].name = "vColor";
        attrib[attribCount++].location = OGLProgramAttrib::ATTRIBIDX_VERTCOL;
    }
	if (attribFlags & ATTRIBFLAG_NOR)
    {
        attrib[attribCount].name = "vNormalAttr";
        attrib[attribCount++].location = OGLProgramAttrib::ATTRIBIDX_VERTNORM;
    }
    if (attribFlags & ATTRIBFLAG_TEXCOORD)
    {
        attrib[attribCount].name = "vTexCoordAttr";
        attrib[attribCount++].location = OGLProgramAttrib::ATTRIBIDX_VERTTEXCOORD;
    }
    if (attribFlags & ATTRIBFLAG_BONEIDX)
    {
    	attrib[attribCount].name = "vBoneIndexAttr";
    	attrib[attribCount++].location = OGLProgramAttrib::ATTRIBIDX_BONEIDXS;
    }
    if (attribFlags & ATTRIBFLAG_BONEWGHT)
    {
    	attrib[attribCount].name = "vBoneWeightAttr";
    	attrib[attribCount++].location = OGLProgramAttrib::ATTRIBIDX_BONEWEIGHTS;
    }
}

static void GLSLPrograms::setProgUniforms(uint8_t            uniformFlags,
   						    			  OGLProgramUniform* uniform)
{
    uint32_t uniformCount = 0;

    if (uniformFlags & UNFFLAG_MDVPROJMAT)
    {
    	uniform[uniformCount].name = "mMdvProjMatUnf";
    	uniform[uniformCount].enumName = UNFNAME_MDVPROJMAT;
    	uniform[uniformCount].count = 1;
    	uniform[uniformCount++].type = OGLProgramUniform::UNIFORMTYPE_MAT4F;
    }
    if (uniformFlags & UNFFLAG_MODELMAT)
    {
    	uniform[uniformCount].name = "mModelMatUnf";
    	uniform[uniformCount].enumName = UNFNAME_MODELMAT;
    	uniform[uniformCount].count = 1;
    	uniform[uniformCount++].type = OGLProgramUniform::UNIFORMTYPE_MAT4F;
    }
    if (uniformFlags & UNFFLAG_TEXTUNIT)
    {
    	uniform[uniformCount].name = "iTexUnitUnf";
    	uniform[uniformCount].enumName = UNFNAME_TEXTUNIT;
    	uniform[uniformCount].count = 1;
    	uniform[uniformCount++].type = OGLProgramUniform::UNIFORMTYPE_INT;
    }
    if (uniformFlags & UNFFLAG_MESHCOLOR)
    {
    	uniform[uniformCount].name = "vMeshColorUnf";
    	uniform[uniformCount].enumName = UNFNAME_MESHCOLOR;
    	uniform[uniformCount].count = 1;
    	uniform[uniformCount++].type = OGLProgramUniform::UNIFORMTYPE_VEC4F;
    }
    if (uniformFlags & UNFFLAG_BONES)
    {
    	uniform[uniformCount].name = "mBonesMatUnf";
    	uniform[uniformCount].enumName = UNFNAME_BONES;
    	uniform[uniformCount].count = 63;
    	uniform[uniformCount++].type = OGLProgramUniform::UNIFORMTYPE_MAT4F;
    }
}

static OGLProgram* GLSLPrograms::loadProgram(const char* verShdFName,
    						   	   	   	   	 const char* fragShdFName,
											 uint16_t    attribFlags)
{
	ASSERT(verShdFName);
	ASSERT(fragShdFName);

	// vertex shader
	LOG_INF("opening " << verShdFName);
	std::ifstream file(verShdFName);
	LOG_ASSERT(!file.bad(), "Error opening file: " << verShdFName);
	std::string shaderStr((std::istreambuf_iterator<char>(file)),
						   std::istreambuf_iterator<char>());
    OGLShader::Ptr vSh(OGLShader::create(shaderStr.c_str(),
    									 shaderStr.length(),
                                         OGLShader::SHADER_VERTEX));
    LOG_ASSERT(vSh.get(), "creating vertex shader failed");

    // fragment shader
    file.close();
    LOG_INF("opening " << fragShdFName);
    file.open(fragShdFName);
    LOG_ASSERT(!file.bad(), "Error opening file: " << fragShdFName);
    shaderStr.assign((std::istreambuf_iterator<char>(file)),
				      std::istreambuf_iterator<char>());
    OGLShader::Ptr fSh(OGLShader::create(shaderStr.c_str(),
    									 shaderStr.length(),
                                         OGLShader::SHADER_FRAGMENT));
    LOG_ASSERT(fSh.get(), "creating fragment shader failed");

    OGLProgramAttrib progAttribs[OGLProgramAttrib::ATTRIBIDX_NUM];
    uint32_t attrCount = 0;
    setProgAttribs(attribFlags, progAttribs, attrCount);

    OGLProgram *programPtr =
            OGLProgram::create(*vSh, *fSh, attrCount, &progAttribs[0]);
    LOG_ASSERT(programPtr, "creating program failed");

    return programPtr;
}

const OGLProgram::Ptr* GLSLPrograms::loadGLSLPrograms(void)
{
    LOG_INF("loading programs...");

    for (GLSLProgName i = GLSLPR_FIRST; i != GLSLPR_NUM; ++i)
    {
        const ProgInfo& info = progInfoArray[i];
        glslProgs[i].reset(loadProgram(info.vertShaderFileName,
                                       info.fragShaderFileName,
                                       info.attribFlags));
        LOG_ASSERT(glslProgs[i].get(),
        		"Couldn't load program with index " << i);
    }

    LOG_INF("programs loaded successful");

    return glslProgs;
}

OGLProgramUniformsSuite* GLSLPrograms::getUniformsSuite(const GLSLProgName pProgName)
{
	LOG_INF("getting uniform suite for program: " << pProgName);

    if (glslProgs[pProgName].get() == NULL)
    {
    	LOG_ERR("Error getting uniform suite for program: " << pProgName
    			<< ". OpenGL program not exist.");
    	return NULL;
    }


    const ProgInfo& info = progInfoArray[pProgName];
    uint32_t unfCount = 0;
    uint8_t bitNum = 0;

    // count number of uniforms in program
    do
    {
    	if ((info.uniformFlags >> bitNum) & 0x0001)
    		++unfCount;
    }
    while (++bitNum < 16);

    OGLProgramUniformsSuite *suite = new OGLProgramUniformsSuite(unfCount);
    setProgUniforms(info.uniformFlags, suite->mArray);

    while (unfCount--)
    {
    	LOG_DET("getting location for: " << suite->mArray[unfCount].name);

    	suite->mArray[unfCount].data = NULL;
    	glslProgs[pProgName]->getUniformLocation(suite->mArray[unfCount]);

    	LOG_ASSERT((suite->mArray[unfCount].location > -1),
    			"error getting location for uniform: " << suite->mArray[unfCount].name);
    }

	return suite;
}

