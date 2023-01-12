/*
* main.cpp
 *
 *  Created on: 11 pa� 2014
 *      Author: zokp
 */
#include <stdint.h>
#include <GL/freeglut.h>
#include <cstring>
#include <stdio.h>
#include <unistd.h>
#include <auto_ptr.h>

#include "Logger.h"
#include "OGLUtils.h"
#include "Options.h"
#include "OGLPolygonMesh.h"
#include "GLSLPrograms.h"
#include "OGLProgramUniform.h"
#include "OGLProgramUniformsSuite.h"
#include "OGLTexture.h"
#include "Controls.h"
#include "Matrix.h"
#include "GridMesh.h"
#include "Pmesh.h"
#include "Pmap.h"
#include "OGLBuffer.h"
#include "BSPTree.h"
#include "TickCount.h"

// ********************** CONSTANTS ********************************************
static const float	  DEGREE	 = 0.01745329252f;
static const float	  DEGREE_90	 = 90 * DEGREE;
static const float	  TRANS_STEP = 0.07f;
static const float	  ROT_ANGLE	 = DEGREE;

static const char*	  APP_NAME      = "OpenGL demo";
static const uint32_t OGL_VER_MAJOR = 4;
static const uint32_t OGL_VER_MINOR = 4;

// ********************** VARIABLES ********************************************
static Pmesh::Ptr gsPmeshForLoad;
static OGLPolygonMesh* gsPolygonMesh;
static BoneKeyFrames*  gsBoneKeyFrames;
static std::auto_ptr<uint8_t> gsBonesFrameData;
static OGLPolygonMesh::Ptr gsGridMesh;

static Pmap::Ptr gsPmap;
static OGLPolygonMesh::Ptr 	 gsMapMesh;

static const OGLProgram::Ptr	*gsPrograms;
static OGLProgramUniformsSuite	*gsPhongUnfPtr;
static OGLProgramUniformsSuite	*gsGuardUnfPtr;
static OGLProgramUniformsSuite	*gsColorUnfPtr;
static OGLProgramUniformsSuite	*gsPhongBonesUnfPtr;

static const OGLProgram			*gsCurrProgPtr;
static OGLProgramUniformsSuite  *gsCurrProgUnfPtr;

static OGLTexture::Ptr gsTexture1;
static OGLTexture	  *gsTexture1Ptr;

static bool gsLeftMousePress = false;
static bool gsRightMousePress = false;
static bool gsCenterMousePress = false;

static Vector4f   gsMeshColor = { 1.0f, 1.0f, 1.0f, 1.0f };
static Vector4f   gsBSPColors[] =
{
		{ 1.0f, 0.0f, 0.0f, 1.0f },	// red
		{ 0.0f, 1.0f, 0.0f, 1.0f }, // green
		{ 0.0f, 0.0f, 1.0f, 1.0f }, // blue

		{ 1.0f, 0.0f, 1.0f, 1.0f }, // pink
		{ 0.5f, 0.0f, 1.0f, 1.0f }, // violet

		{ 0.0f, 0.5f, 1.0f, 1.0f }, // light blue
		{ 0.0f, 1.0f, 1.0f, 1.0f }, // sea

		{ 1.0f, 1.0f, 0.0f, 1.0f }, // yellow
		{ 1.0f, 0.5f, 0.0f, 1.0f },	// orange

		{ 0.0f, 0.0f, 0.0f, 1.0f },	// black
		{ 1.0f, 1.0f, 1.0f, 1.0f }, // white

		{ 0.5f, 0.0f, 0.0f, 1.0f },	// dark red
		{ 0.0f, 0.5f, 0.0f, 1.0f }, // dark green
		{ 0.0f, 0.0f, 0.5f, 1.0f }, // dark blue
};
static Matrix4x4f gsPerspMat;
static Matrix4x4f gsOrthoMat;
static Matrix4x4f *gsCurrProjMat;
static Matrix4x4f gsMDVProjMat;
static Matrix4x4f gsLightingModelMat;

Options::Options 	gsOptions;


//TEMP
static float camRotX = 0, camRotY = 0;
static float camTranX = 0.0f, camTranY = -10.0f, camTranZ = 0.0f;

static float objRotX = 0, objRotY = 0, objRotZ = 0;
static float gridTransY = 0.0f;

PolygonMeshList* gsBSPMeshList;
BSPTree* gsBSPTree;

// ********************** FUNCTIONS ********************************************
static void idle();
static void render();
static void keyboardFunc(unsigned char key, int x, int y);
static void specialInput(int key, int x, int y);
static void initUniforms(void);
static void setCurrentProgram(GLSLPrograms::GLSLProgName progName);
static void setPprojectionMatrix(Matrix4x4f& m);
static void motionFunc(int x, int y);
static void mouseFunc(int button, int state, int x, int y);
static void mouseWheelFunc(int button, int dir, int x, int y);
static Matrix4x4f& updateCamera(void);
static void handleParams(int argc, char** argv);

int main(int argc, char** argv)
{
    using namespace GLSLPrograms;
    LOG_INF(APP_NAME);

    handleParams(argc, argv);

    //Controls::init();

    glutInit(&argc, argv);
    glutInitContextVersion(OGL_VER_MAJOR, OGL_VER_MINOR);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(gsOptions.width, gsOptions.height);
    LOG_ASSERT(glutCreateWindow(APP_NAME), "Glut could not create window");

    if (gsOptions.fullScreen)
    {
        glutFullScreen();
    }
    glutReshapeFunc(OGLUtils::reshapeOGLWindow);
    glutIdleFunc(idle);
    glutDisplayFunc(render);
    glutKeyboardFunc(keyboardFunc);
    glutSpecialFunc(specialInput);
    glutMotionFunc(motionFunc);
    glutMouseFunc(mouseFunc);
    glutMouseWheelFunc(mouseWheelFunc);
    OGLUtils::initOGL();
    OGLUtils::reshapeOGLWindow(gsOptions.width, gsOptions.height);
    if (gsOptions.wireframe)
    {
        OGLUtils::enableWireframeMode(true);
    }

    if (gsOptions.meshFileName && ::strlen(gsOptions.meshFileName))
    {
    	// load Polygon Model
    	gsPmeshForLoad.reset(Pmesh::createFromFile(gsOptions.meshFileName));
    	LOG_ASSERT(gsPmeshForLoad.get(), "Cannot create PMESH object: " << gsOptions.meshFileName);
    	gsPolygonMesh = gsPmeshForLoad->getPolygonMesh();
    	if ((gsBoneKeyFrames = gsPmeshForLoad->getBonesFrame()))
    	{
    		gsBonesFrameData.reset(new uint8_t[gsBoneKeyFrames->getFrameSize()]);
    		LOG_INF("Size of bones frame data: " << gsBoneKeyFrames->getFrameSize());
    	}
    }
    if (gsOptions.mapFileName && ::strlen(gsOptions.mapFileName))
    {
    	gsPmap.reset(Pmap::createFromFile(gsOptions.mapFileName));
    	LOG_ASSERT(gsPmap.get(), "Cannot create PMAP object: " << gsOptions.mapFileName);

//    	gsMapMesh.reset(
//    			OGLPolygonMesh::create(
//    					&(gsPmap->getVertexPosList().mArray[0].pos[0]),
//    					NULL, NULL, 0, NULL, NULL,
//						gsPmap->getVertexPosList().mArraySize,
//						&(gsPmap->getFaceList().mArray[0].f[0]),
//						gsPmap->getFaceList().mArraySize));

    	gsBSPTree = BSPTree::create(*gsPmap.get());
    	gsBSPMeshList = gsBSPTree->getMeshList();
    }

    // load FLOOR MESH
    gsGridMesh.reset(GridMesh::create());
    LOG_ASSERT(gsGridMesh.get(), "Cannot create grid mesh.");

    // load GLSL programs
    gsPrograms = GLSLPrograms::loadGLSLPrograms();
    LOG_ASSERT(gsPrograms, "Couldn't load GLSL Program");

    // load textures
    if (gsOptions.text1FileName && ::strlen(gsOptions.text1FileName))
    {
    	gsTexture1.reset(OGLTexture::createFromBMP(gsOptions.text1FileName));
    	LOG_ASSERT(gsTexture1.get(), "Cannot create OGL Texture from file "
    			<< gsOptions.text1FileName);
    	gsTexture1Ptr = gsTexture1.get();
    	gsTexture1Ptr->use();
    }

//    tranMem[3][0] = 0.0f;
//    tranMem[3][1] = 0.0f;
//    tranMem[3][2] = -5.0f;

    initUniforms();
	setCurrentProgram(GLSLPR_GUARD_LIGHTING);
	setPprojectionMatrix(gsPerspMat);
	glutSetCursor(GLUT_CURSOR_FULL_CROSSHAIR);

    glutMainLoop();

    return 0;
}

static void handleParams(int argc, char** argv)
{
    gsOptions = Options::parseOptions(argc, argv);
    if ((!gsOptions.meshFileName || ::strlen(gsOptions.meshFileName) <= 0) &&
            (!gsOptions.mapFileName || ::strlen(gsOptions.mapFileName)<= 0))
        {
            printf("Usage: %s [options]\n", argv[0]);
            printf("options:\n");
            printf("\t-m,--mesh file name\tfile name of polygon mesh to be read\n");
            printf("\t--map file name\t\tfile name of polygon map to be read\n");
            printf("\t--t1 file name\t\tfile name of texture 1 used for mapping a mesh\n");
            printf("\t--t2 file name\t\tfile name of texture 2 used for mapping a mesh\n");
            printf("\t--t3 file name\t\tfile name of texture 3 used for mapping a mesh\n");
            printf("\t--t4 file name\t\tfile name of texture 4 used for mapping a mesh\n");
            printf("\t-f,--fullscreen\t\tfull screen\n");
            printf("\t-w,--width width\tscreen width\n");
            printf("\t-h,--height height\tscreen height\n");
            printf("\t-l,--wireframe\twire frame mode\n");
            printf("\t--light \"x,y,z[x,y,z]\"\tdefines light(s) at position x,y,z\n\n");
            printf("\t-s,speed of animation in milliseconds\n");
            exit(0);
        }

    if (gsOptions.meshFileName && gsOptions.mapFileName)
        {
            LOG_ERR("Cannot load mesh and map. Please load one file.");
            exit(0);
        }

}

static void idle()
{
	uint64_t updateTime = GetTickCount();
	float factor = (updateTime % gsOptions.speed) / (float)(gsOptions.speed);

	if (gsBoneKeyFrames)
	{
		gsBoneKeyFrames->getFrame(gsBonesFrameData.get(), factor);
	}
	glutPostRedisplay();
}

static void render()
{
	using namespace Matrix;
	using namespace Controls;
    OGLUtils::clearOGLBuffers();

    Matrix4x4f& camera = updateCamera();

    if (gCtrls[SHOW_GRID])
    {
    	Matrix4x4f transGrid(1.0f);
    	transGrid[3][1] = gridTransY;
    	Matrix4x4f gridMat = camera * transGrid;

    	Matrix::mulMatrix4x4f(gsMDVProjMat, gridMat, *gsCurrProjMat);
    	gsColorUnfPtr->set(UNFNAME_MESHCOLOR, gsMeshColor);
    	gsPrograms[GLSLPrograms::GLSLPR_COLOR].get()->use(*gsColorUnfPtr);
    	gsGridMesh->draw();
    }

    if (gsPolygonMesh)
    {
    	Matrix4x4f modelMat(1.0);
    	Matrix::rotMatrix4x4f(modelMat, -90 * DEGREE, ROT_X_FLAG);
    	Matrix::mulMatrix4x4f(gsMDVProjMat, camera * modelMat, *gsCurrProjMat);
    	gsCurrProgPtr->use(*gsCurrProgUnfPtr);
    	gsPolygonMesh->draw();
    }

    if (gsMapMesh.get())
    {
    	Matrix4x4f modelMat(1.0);
    	Matrix::rotMatrix4x4f(modelMat, -90 * DEGREE, ROT_X_FLAG);
    	Matrix::mulMatrix4x4f(modelMat, modelMat, camera);
    	Matrix::mulMatrix4x4f(gsMDVProjMat, modelMat, *gsCurrProjMat);
    	gsPrograms[GLSLPrograms::GLSLPR_COLOR].get()->use(*gsColorUnfPtr);
    	gsMapMesh->draw();
    }

    if (gsBSPMeshList)
    {
    	Matrix4x4f modelMat(1.0);
    	Matrix::rotMatrix4x4f(modelMat, -90 * DEGREE, ROT_X_FLAG);
    	Matrix::mulMatrix4x4f(modelMat, modelMat, camera);
    	Matrix::mulMatrix4x4f(gsMDVProjMat, modelMat, *gsCurrProjMat);

    	PolygonMeshList::iterator it = gsBSPMeshList->begin();
    	uint32_t colorIdx = 0;
    	while (it != gsBSPMeshList->end())
    	{
    		gsColorUnfPtr->set(UNFNAME_MESHCOLOR, gsBSPColors[colorIdx]);
        	gsPrograms[GLSLPrograms::GLSLPR_COLOR].get()->use(*gsColorUnfPtr);
    		(*it)->draw();
    		++it;
    		colorIdx = colorIdx % 14;
    	}
    }

    glutSwapBuffers();
}

static void keyboardFunc(unsigned char key, int x, int y)
{
	using namespace Controls;
	using namespace GLSLPrograms;
	switch (key)
	{
		case 'w':
			gCtrls[CTRLS_WIREFRAMED] = !gCtrls[CTRLS_WIREFRAMED];
			OGLUtils::enableWireframeMode(gCtrls[CTRLS_WIREFRAMED]);
			break;
		case 't':
			gCtrls[CTRLS_TEXTURED] = !gCtrls[CTRLS_TEXTURED];
			break;
		case 'l':
			gCtrls[CTRLS_LIGHTING] = !gCtrls[CTRLS_LIGHTING];
			break;
		case 'p':
		{
			static GLSLProgName glslProg = GLSLPR_GUARD_LIGHTING;
			if (glslProg < GLSLPR_PHONG_LIGHTING_BONE_ANIM) ++glslProg;
			else glslProg = GLSLPR_PHONG_LIGHTING;
			gCtrls[CTRLS_SHADER_PROG] = !gCtrls[CTRLS_SHADER_PROG];
			setCurrentProgram(glslProg);
		}
		break;
		case 'm':
			gCtrls[CTRLS_PROJ_MAT] = !gCtrls[CTRLS_PROJ_MAT];
			if (gCtrls[CTRLS_PROJ_MAT] == false)
				setPprojectionMatrix(gsPerspMat);
			else
				setPprojectionMatrix(gsOrthoMat);
			break;
		case 'g':
			gCtrls[SHOW_GRID] = !gCtrls[SHOW_GRID];
			break;
		case 27: 	// ESCAPE
			LOG_INF("Escape pressed. Terminating...");
			exit(0);
			break;
		case GLUT_KEY_DELETE:
			objRotZ -= ROT_ANGLE;
			LOG_INF("objRotZ: " << objRotZ);
			break;

		default:
			break;
	};
}

static void specialInput(int key, int x, int y)
{
	using namespace Controls;
	switch (key)
	{
		case GLUT_KEY_UP:
			gCtrls[CTRLS_OBJ_ROTUP] = !gCtrls[CTRLS_OBJ_ROTUP];
			camTranZ += TRANS_STEP;
			break;
		case GLUT_KEY_DOWN:
			gCtrls[CTRLS_OBJ_ROTDOWN] = !gCtrls[CTRLS_OBJ_ROTDOWN];
			camTranZ -= TRANS_STEP;
			break;
		case GLUT_KEY_LEFT:
			gCtrls[CTRLS_OBJ_ROTLEFT] = !gCtrls[CTRLS_OBJ_ROTLEFT];
			camTranX += TRANS_STEP;
			break;
		case GLUT_KEY_RIGHT:
			gCtrls[CTRLS_OBJ_ROTRIGHT] = !gCtrls[CTRLS_OBJ_ROTRIGHT];
			camTranX -= TRANS_STEP;
			break;
		case GLUT_KEY_HOME:
			objRotX += ROT_ANGLE;
			LOG_INF("objRotX: " << objRotX);
			break;
		case GLUT_KEY_END:
			objRotX -= ROT_ANGLE;
			LOG_INF("objRotX: " << objRotX);
			break;
		case GLUT_KEY_PAGE_UP:
			objRotY += ROT_ANGLE;
			LOG_INF("objRotY: " << objRotY);
			break;
		case GLUT_KEY_PAGE_DOWN:
			objRotY -= ROT_ANGLE;
			LOG_INF("objRotY: " << objRotY);
			break;
		case GLUT_KEY_INSERT:
			objRotZ += ROT_ANGLE;
			LOG_INF("objRotY: " << objRotY);
			break;
		default:
			break;
	}
}

void motionFunc(int x, int y)
{
	static int oldX;
	static int oldY;

	if (gsLeftMousePress)
	{
		if ((x - oldX) > 0)
		{
			camRotY += (2 * ROT_ANGLE);
		}
		else if ((x - oldX) < 0)
		{
			camRotY += -(2 * ROT_ANGLE);
		}
	}

	if (gsRightMousePress)
	{
		if ((y - oldY) > 0)
		{
			if (camRotX < DEGREE_90) {
				camRotX += (2 * ROT_ANGLE);
			}
		}
		else if ((y - oldY) < 0)
		{
			if (camRotX > -DEGREE_90) {
				camRotX += -(2 * ROT_ANGLE);
			}
		}
	}

	if (gsCenterMousePress )
	{
		gridTransY -= (y - oldY) * 0.1f;
		if (gridTransY < -10.0f) gridTransY = -10.0f;
		if (gridTransY > 10.0f) gridTransY = 10.0f;
	}

	oldX = x;
	oldY = y;
}

void mouseFunc(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		gsLeftMousePress = (state == GLUT_DOWN);
	}
	if (button == GLUT_RIGHT_BUTTON)
	{
		gsRightMousePress = (state == GLUT_DOWN);
	}
	if (button == GLUT_MIDDLE_BUTTON)
	{
		gsCenterMousePress = (state == GLUT_DOWN);
	}
}

void mouseWheelFunc(int button, int dir, int x, int y)
{
	if (dir > 0)
	{
		camTranZ += 2 * TRANS_STEP;
	}
	else
	{
		camTranZ -= 2 * TRANS_STEP;
	}
}

static void initUniforms(void)
{
	using namespace GLSLPrograms;
	using namespace Matrix;
	using namespace std;
	// shader's uniforms initialization

	setIdentityMatrix4x4f(gsMDVProjMat);
	float aspectRatio = float(gsOptions.width) / float(gsOptions.height);
	setPerspectiveMatrix4x4f(gsPerspMat, DEGREE * 65, aspectRatio, 1.0f, 100.0f);
	setOrthoMatrix4x4f(gsOrthoMat, aspectRatio, 1.0f, 100.0f);

	static GLint textureUnit = 0;

	// PHONG SHADER PROGRAM
	static auto_ptr<OGLProgramUniformsSuite>
		phongUnfAPTR(getUniformsSuite(GLSLPR_PHONG_LIGHTING));

	// GUARD SHADER PROGRAM
		static auto_ptr<OGLProgramUniformsSuite>
			guardUnfAPTR(getUniformsSuite(GLSLPR_GUARD_LIGHTING));

	// COLOR SHADER PROGRAM
	static auto_ptr<OGLProgramUniformsSuite>
		colorUnfAPTR(getUniformsSuite(GLSLPR_COLOR));

	// PHONG-BONES SHADER PROGRAM
	static auto_ptr<OGLProgramUniformsSuite>
		phongBonesUnfAPTR(getUniformsSuite(GLSLPR_PHONG_LIGHTING_BONE_ANIM));

	setIdentityMatrix4x4f(gsLightingModelMat);
	if (gsPolygonMesh)
	{
		phongUnfAPTR->set(UNFNAME_MDVPROJMAT, Matrix::getMatrix4x4Pointer(gsMDVProjMat));
		phongUnfAPTR->set(UNFNAME_MODELMAT, &gsLightingModelMat);
		phongUnfAPTR->set(UNFNAME_TEXTUNIT, &textureUnit);
		gsPhongUnfPtr = phongUnfAPTR.get();

		guardUnfAPTR->set(UNFNAME_MDVPROJMAT, Matrix::getMatrix4x4Pointer(gsMDVProjMat));
		guardUnfAPTR->set(UNFNAME_MODELMAT, &gsLightingModelMat);
		guardUnfAPTR->set(UNFNAME_TEXTUNIT, &textureUnit);
		gsGuardUnfPtr = guardUnfAPTR.get();

		phongBonesUnfAPTR->set(UNFNAME_MDVPROJMAT, Matrix::getMatrix4x4Pointer(gsMDVProjMat));
		phongBonesUnfAPTR->set(UNFNAME_MODELMAT, &gsLightingModelMat);
		phongBonesUnfAPTR->set(UNFNAME_TEXTUNIT, &textureUnit);
		phongBonesUnfAPTR->set(UNFNAME_BONES, gsBonesFrameData.get());
		gsPhongBonesUnfPtr = phongBonesUnfAPTR.get();

		validateOGLUniformSuite(*phongUnfAPTR.get(), "phongUnfAPTR");
		validateOGLUniformSuite(*guardUnfAPTR.get(), "guardUnfAPTR");
		validateOGLUniformSuite(*phongBonesUnfAPTR.get(), "phongBonesUnfAPTR");
	}

	colorUnfAPTR->set(UNFNAME_MDVPROJMAT, getMatrix4x4Pointer(gsMDVProjMat));
	colorUnfAPTR->set(UNFNAME_MESHCOLOR, gsMeshColor);
	gsColorUnfPtr = colorUnfAPTR.get();
	validateOGLUniformSuite(*colorUnfAPTR.get(), "colorUnfAPTR");

}

static void setCurrentProgram(GLSLPrograms::GLSLProgName progName)
{
	using namespace GLSLPrograms;
	switch (progName)
	{
		case GLSLPR_PHONG_LIGHTING:
		default:
			gsCurrProgPtr = gsPrograms[GLSLPR_PHONG_LIGHTING].get();
			gsCurrProgUnfPtr = gsPhongUnfPtr;
			LOG_INF("Current GLSL Program is: Model View Projection - Phong");
			break;

		case GLSLPR_GUARD_LIGHTING:
			gsCurrProgPtr = gsPrograms[GLSLPR_GUARD_LIGHTING].get();
			gsCurrProgUnfPtr = gsGuardUnfPtr;
			LOG_INF("Current GLSL Program is: Model View Projection - Guard");
			break;

		case GLSLPR_COLOR:
			gsCurrProgPtr = gsPrograms[GLSLPR_COLOR].get();
			gsCurrProgUnfPtr = gsColorUnfPtr;
			LOG_INF("Current GLSL Program is: Model View Projection - Color");
			break;

		case GLSLPR_PHONG_LIGHTING_BONE_ANIM:
			gsCurrProgPtr = gsPrograms[GLSLPR_PHONG_LIGHTING_BONE_ANIM].get();
			gsCurrProgUnfPtr = gsPhongBonesUnfPtr;
			LOG_INF("Current GLSL Program is: Model View Projection - Phong Bones");
			break;
	}
}

static void setPprojectionMatrix(Matrix4x4f& m)
{
	gsCurrProjMat = &m;
}

static Matrix4x4f& updateCamera(void)
{
	using namespace Matrix;
	static Matrix4x4f tranMem(1.0f);
	static Matrix4x4f result(1.0f);

    Matrix4x4f rotX(1.0f);
    Matrix4x4f rotY(1.0f);

    rotX = rotMatrix4x4f(rotX, camRotX, ROT_X_FLAG);
    rotY = rotMatrix4x4f(rotY, camRotY, ROT_Y_FLAG);

    Matrix4x4f tran(1.0f);
    tran[3][0] = camTranX;
    tran[3][1] = camTranY;
    tran[3][2] = camTranZ;
    camTranX = 0; camTranY = 0; camTranZ = 0;

    Matrix4x4f ROTX_TRANS_RESULT(1.0f);
    rotX[1][2] *= -1;
    rotX[2][1] *= -1;
    ROTX_TRANS_RESULT = rotX * tran;
    rotX[1][2] *= -1;
    rotX[2][1] *= -1;

    Matrix4x4f ROTY_TRANS_RESULT(1.0f);
    rotY[0][2] *= -1;
    rotY[2][0] *= -1;
    tran = rotY * ROTX_TRANS_RESULT;
    rotY[0][2] *= -1;
    rotY[2][0] *= -1;

    tranMem[3][0] += tran[3][0];
    tranMem[3][1] += tran[3][1];
    tranMem[3][2] += tran[3][2];

    result = rotX * rotY * tranMem;
    return result;
}
