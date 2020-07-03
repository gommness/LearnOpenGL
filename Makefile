
########################################################################################
#   COMPILER
########################################################################################
CC = g++
CFLAG = -Wall -g
LIBS = -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lGLEW -lGLU -lrt -lXxf86vm -lXinerama

########################################################################################
#   PROJECT DIRECTORIES
########################################################################################
SRCD = src
SHADERD = ${SRCD}/shaders
BUILD = build
OBJD = build
LIBD = lib



########################################################################################
#   CLASSES
########################################################################################
_SHADERS_OBJ = shader.o vertexShader.o fragmentShader.o shaderProgram.o

########################################################################################
#   SHADERS EN OPENGL
########################################################################################
_SHADER_CODE = myShader.vert myShader.frag
SHADER_CODE = $(addprefix ${SHADERD}/,${_SHADER_CODE})

########################################################################################
#   PROJECT DEFINITIONS
########################################################################################

TRIANGLE_TARGET = helloTriangle
_TRIANGLE_OBJ = helloTriangle.o 
TRIANGLE_OBJ = $(addprefix ${OBJD}/,${_TRIANGLE_OBJ})
TRIANGLE_INCLUDE = 
TRIANGLE_REQUISITES = ${TRIANGLE_OBJ}

TRIFORCE_TARGET = helloTriforce
_TRIFORCE_OBJ = helloTriforce2.o ${_SHADERS_OBJ}
TRIFORCE_OBJ = $(addprefix ${OBJD}/,${_TRIFORCE_OBJ})
TRIFORCE_INCLUDE = 
TRIFORCE_REQUISITES = ${TRIFORCE_OBJ} ${SHADER_CODE}

RECTANGLE_TARGET = helloRectangle
_RECTANGLE_OBJ = helloRectangle.o
RECTANGLE_OBJ = $(addprefix ${OBJD}/,${_RECTANGLE_OBJ})
RECTANGLE_INCLUDE = 
RECTANGLE_REQUISITES = ${RECTANGLE_OBJ}

SHADER_EXERCISE_1_TARGET = shaderExercise_1
_SHADER_EXERCISE_1_OBJ = shaderExercise_1.o ${_SHADERS_OBJ}
SHADER_EXERCISE_1_OBJ = $(addprefix ${OBJD}/,${_SHADER_EXERCISE_1_OBJ})
SHADER_EXERCISE_1_INCLUDE = 
SHADER_EXERCISE_1_REQUISITES = ${SHADER_EXERCISE_1_OBJ} ${SHADER_CODE}


########################################################################################
#   TARGET OF PROJECTS TO COMPILE
########################################################################################
ALL_TARGETS = ${TRIANGLE_TARGET} ${RECTANGLE_TARGET} ${TRIFORCE_TARGET} ${SHADER_EXERCISE_1_TARGET}






########################################################################################
#   MAKEFILE RULES
########################################################################################

all: ${ALL_TARGETS}

clean:
	-rm -f ${BUILD}/*

${TRIANGLE_TARGET}: ${TRIANGLE_REQUISITES}
	${CC} ${CFLAGS} ${INCLUDES} -o ${BUILD}/$@ ${TRIANGLE_OBJ} ${LIBS}

${RECTANGLE_TARGET}: ${RECTANGLE_REQUISITES}
	${CC} ${CFLAGS} ${INCLUDES} -o ${BUILD}/$@ ${RECTANGLE_OBJ} ${LIBS}

${TRIFORCE_TARGET}: ${TRIFORCE_REQUISITES}
	${CC} ${CFLAGS} ${INCLUDES} -o ${BUILD}/$@ ${TRIFORCE_OBJ} ${LIBS}

${SHADER_EXERCISE_1_TARGET}: ${SHADER_EXERCISE_1_REQUISITES}
	${CC} ${CFLAGS} ${INCLUDES} -o ${BUILD}/$@ ${SHADER_EXERCISE_1_OBJ} ${LIBS}

${BUILD}/%.o: ${SRCD}/%.cpp
	${CC} ${CFLAGS} ${INCLUDES} -c $< -o $@
