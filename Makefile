
########################################################################################
#   COMPILER
########################################################################################
CC = g++
CFLAGS = -Wall -g
LIBS = -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lGLEW -lGLU -lrt -lXxf86vm -lXinerama -lSOIL

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
_SHADER_CODE  = myShader.vert myShader.frag uniformExercise.frag interpolationExercise.vert interpolationExercise.frag
_SHADER_CODE += hOffset.vert upsideDown.vert positionalColor.vert positionalColor.frag textureExercise.vert
_SHADER_CODE += textureExercise.frag
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

SHADER_EXERCISE_2_TARGET = shaderExercise_2
_SHADER_EXERCISE_2_OBJ = shaderExercise_2.o ${_SHADERS_OBJ}
SHADER_EXERCISE_2_OBJ = $(addprefix ${OBJD}/,${_SHADER_EXERCISE_2_OBJ})
SHADER_EXERCISE_2_INCLUDE = 
SHADER_EXERCISE_2_REQUISITES = ${SHADER_EXERCISE_2_OBJ} ${SHADER_CODE}

TEXTURE_EXERCISE_1_TARGET = textureExercise_1
_TEXTURE_EXERCISE_1_OBJ = textureExercise_1.o ${_SHADERS_OBJ}
TEXTURE_EXERCISE_1_OBJ = $(addprefix ${OBJD}/,${_TEXTURE_EXERCISE_1_OBJ})
TEXTURE_EXERCISE_1_INCLUDE = 
TEXTURE_EXERCISE_1_REQUISITES = ${TEXTURE_EXERCISE_1_OBJ} ${SHADER_CODE}

########################################################################################
#   TARGET OF PROJECTS TO COMPILE
########################################################################################
ALL_TARGETS  = ${TRIANGLE_TARGET} ${RECTANGLE_TARGET} ${TRIFORCE_TARGET} ${SHADER_EXERCISE_1_TARGET} ${SHADER_EXERCISE_2_TARGET}
ALL_TARGETS += ${TEXTURE_EXERCISE_1_TARGET}






########################################################################################
#   MAKEFILE RULES
########################################################################################

all: ${ALL_TARGETS}

clean:
	-rm -f ${BUILD}/*

${TRIANGLE_TARGET}: ${TRIANGLE_REQUISITES}
	${CC} ${CFLAGS} -o ${BUILD}/$@ ${TRIANGLE_OBJ} ${LIBS}

${RECTANGLE_TARGET}: ${RECTANGLE_REQUISITES}
	${CC} ${CFLAGS} -o ${BUILD}/$@ ${RECTANGLE_OBJ} ${LIBS}

${TRIFORCE_TARGET}: ${TRIFORCE_REQUISITES}
	${CC} ${CFLAGS} -o ${BUILD}/$@ ${TRIFORCE_OBJ} ${LIBS}

${SHADER_EXERCISE_1_TARGET}: ${SHADER_EXERCISE_1_REQUISITES}
	${CC} ${CFLAGS} -o ${BUILD}/$@ ${SHADER_EXERCISE_1_OBJ} ${LIBS}

${SHADER_EXERCISE_2_TARGET}: ${SHADER_EXERCISE_2_REQUISITES}
	${CC} ${CFLAGS} -o ${BUILD}/$@ ${SHADER_EXERCISE_2_OBJ} ${LIBS}

${TEXTURE_EXERCISE_1_TARGET}: ${TEXTURE_EXERCISE_1_REQUISITES}
	${CC} ${CFLAGS} -o ${BUILD}/$@ ${TEXTURE_EXERCISE_1_OBJ} ${LIBS}

${BUILD}/%.o: ${SRCD}/%.cpp
	${CC} ${CFLAGS} ${INCLUDES} -c $< -o $@
