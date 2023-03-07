SRC=./src
LIB=-lGL -lGLU -lglut -Bdynamic
LIB_PARAMS= -Wl,--as-needed -g -Wl,--no-undefined -I$(FREEGLUT_INC_DIR) $(INC) -L$(FREEGLUT_LIB_DIR)
BIN_DIR=bin

FREEGLUT_INC_DIR=lib/freeglut/include
FREEGLUT_LIB_DIR=/usr/lib/i386-linux-gnu
INC=-I./inc -I./src/

CXX=g++
CPPFLAGS=-g -std=c++20 -O0 -Wall -pedantic -Wno-volatile -fno-exceptions -fno-rtti -mno-ms-bitfields -DGL_GLEXT_PROTOTYPES $(INC) -I$(FREEGLUT_INC_DIR)  

SRCFILES := main.cpp Logger.cpp OGLUtils.cpp Options.cpp OGLPolygonMesh.cpp OGLFunctions.cpp OGLProgram.cpp OGLShader.cpp GLSLPrograms.cpp OGLTexture.cpp Controls.cpp BMP.cpp OGLProgramUniformsSuite.cpp GridMesh.cpp Pmesh.cpp Pmap.cpp Bone.cpp BoneKeyFrames.cpp OGLBuffer.cpp ParseUtils.cpp BSPTree.cpp PolygonMesh.cpp Math3d.cpp Matrix.cpp TickCount.cpp
DEPFILES := $(addprefix $(BIN_DIR)/,$(patsubst %.cpp,%.d,$(SRCFILES))) 
OBJFILES := $(addprefix $(BIN_DIR)/,$(patsubst %.cpp,%.o,$(SRCFILES)))

all: bin $(BIN_DIR)/demo.linux

$(BIN_DIR)/demo.linux: $(OBJFILES)
	$(CXX) $(OBJFILES) $(LIB) $(LIB_PARAMS) -o $@
	
$(BIN_DIR)/%.o: $(addprefix $(SRC)/,%.cpp)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPFILES)
endif
		
.PHONY: all clean

bin:
	mkdir $(BIN_DIR)

clean:
	rm $(BIN_DIR) -rf
