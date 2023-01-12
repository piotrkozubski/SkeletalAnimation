SRC=src
LIB=-lGL -lGLU -lglut -Bdynamic
LIB_PARAMS= -Wl,--as-needed -g -Wl,--no-undefined -I$(FREEGLUT_INC_DIR) $(INC) -L$(FREEGLUT_LIB_DIR)
BIN_DIR=bin

FREEGLUT_INC_DIR=lib/freeglut/include
FREEGLUT_LIB_DIR=/usr/lib/i386-linux-gnu
INC=-I./inc -I./src/

CXX=g++
CPPFLAGS=-g -O0 -Wall -pedantic -fno-exceptions -fno-rtti -mno-ms-bitfields -DGLM_FORCE_CXX98 -DGL_GLEXT_PROTOTYPES $(INC) -I$(FREEGLUT_INC_DIR) 
CPP03FLAG= -std=gnu++03
CPP17FLAG= -std=gnu++17

srcfiles03 := main.cpp Logger.cpp OGLUtils.cpp Options.cpp OGLPolygonMesh.cpp OGLFunctions.cpp OGLProgram.cpp OGLShader.cpp GLSLPrograms.cpp OGLTexture.cpp Controls.cpp BMP.cpp OGLProgramUniformsSuite.cpp GridMesh.cpp Pmesh.cpp Pmap.cpp Bone.cpp BoneKeyFrames.cpp OGLBuffer.cpp ParseUtils.cpp BSPTree.cpp PolygonMesh.cpp Math3d.cpp
objects03  := $(patsubst %.cpp, %.o, $(srcfiles03))

srcfiles17 := Matrix.cpp TickCount.cpp
objects17  := $(patsubst %.cpp, %.o, $(srcfiles17))

objects := $(objects03) $(objects17)

all: bin demo

demo: $(objects)
	$(CXX) $(addprefix $(BIN_DIR)/,$^) $(LIB) $(LIB_PARAMS) -o $(BIN_DIR)/demo.linux
    
$(objects03): CXXFLAGS := $(CPP03FLAG)
$(objects17): CXXFLAGS := $(CPP17FLAG)
    
$(objects): %.o: $(SRC)/%.cpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $(BIN_DIR)/$@
	 	
bin:
	mkdir $(BIN_DIR)
	
clean:
	rm $(BIN_DIR) -rf
	
