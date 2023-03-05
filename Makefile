SRC=src
LIB=-lGL -lGLU -lglut -Bdynamic
LIB_PARAMS= -Wl,--as-needed -g -Wl,--no-undefined -I$(FREEGLUT_INC_DIR) $(INC) -L$(FREEGLUT_LIB_DIR)
BIN_DIR=bin

FREEGLUT_INC_DIR=lib/freeglut/include
FREEGLUT_LIB_DIR=/usr/lib/i386-linux-gnu
INC=-I./inc -I./src/

CXX=g++
CPPFLAGS=-g -O0 -Wall -pedantic -Wno-volatile -fno-exceptions -fno-rtti -mno-ms-bitfields -DGL_GLEXT_PROTOTYPES $(INC) -I$(FREEGLUT_INC_DIR) 
CPP20FLAG= -std=c++20

srcfiles20 := main.cpp Logger.cpp OGLUtils.cpp Options.cpp OGLPolygonMesh.cpp OGLFunctions.cpp OGLProgram.cpp OGLShader.cpp GLSLPrograms.cpp OGLTexture.cpp Controls.cpp BMP.cpp OGLProgramUniformsSuite.cpp GridMesh.cpp Pmesh.cpp Pmap.cpp Bone.cpp BoneKeyFrames.cpp OGLBuffer.cpp ParseUtils.cpp BSPTree.cpp PolygonMesh.cpp Math3d.cpp Matrix.cpp TickCount.cpp
objects20  := $(patsubst %.cpp, %.o, $(srcfiles20))

objects := $(objects20)

all: bin demo

demo: $(objects)
	$(CXX) $(addprefix $(BIN_DIR)/,$^) $(LIB) $(LIB_PARAMS) -o $(BIN_DIR)/demo.linux
    
$(objects20): CXXFLAGS := $(CPP20FLAG)
    
$(objects): %.o: $(SRC)/%.cpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $(BIN_DIR)/$@
	 	
bin:
	mkdir $(BIN_DIR)
	
clean:
	rm $(BIN_DIR) -rf
	
