
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
_SHADERS_OBJ = shader.o vertexShader.o fragmentShader.o shaderProgram.o texture.o

########################################################################################
#   SHADERS EN OPENGL
########################################################################################
_SHADER_CODE  = myShader.vert myShader.frag uniformExercise.frag interpolationExercise.vert interpolationExercise.frag
_SHADER_CODE += hOffset.vert upsideDown.vert positionalColor.vert positionalColor.frag textureExercise.vert
_SHADER_CODE += textureExercise.frag textureReverse.frag textureSimple.vert textureSimple.frag

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

TEXTURE_EXERCISE_2_TARGET = textureExercise_2
_TEXTURE_EXERCISE_2_OBJ = textureExercise_2.o ${_SHADERS_OBJ}
TEXTURE_EXERCISE_2_OBJ = $(addprefix ${OBJD}/,${_TEXTURE_EXERCISE_2_OBJ})
TEXTURE_EXERCISE_2_INCLUDE = 
TEXTURE_EXERCISE_2_REQUISITES = ${TEXTURE_EXERCISE_2_OBJ} ${SHADER_CODE}

TEXTURE_EXERCISE_3_TARGET = textureExercise_3
_TEXTURE_EXERCISE_3_OBJ = textureExercise_3.o ${_SHADERS_OBJ}
TEXTURE_EXERCISE_3_OBJ = $(addprefix ${OBJD}/,${_TEXTURE_EXERCISE_3_OBJ})
TEXTURE_EXERCISE_3_INCLUDE = 
TEXTURE_EXERCISE_3_REQUISITES = ${TEXTURE_EXERCISE_3_OBJ} ${SHADER_CODE}

TEXTURE_EXERCISE_4_TARGET = textureExercise_4
_TEXTURE_EXERCISE_4_OBJ = textureExercise_4.o ${_SHADERS_OBJ}
TEXTURE_EXERCISE_4_OBJ = $(addprefix ${OBJD}/,${_TEXTURE_EXERCISE_4_OBJ})
TEXTURE_EXERCISE_4_INCLUDE = 
TEXTURE_EXERCISE_4_REQUISITES = ${TEXTURE_EXERCISE_4_OBJ} ${SHADER_CODE}

TRANSFORM_EXERCISE_1_TARGET = transformationsExercise_1
_TRANSFORM_EXERCISE_1_OBJ = transformationsExercise_1.o ${_SHADERS_OBJ}
TRANSFORM_EXERCISE_1_OBJ = $(addprefix ${OBJD}/,${_TRANSFORM_EXERCISE_1_OBJ})
TRANSFORM_EXERCISE_1_INCLUDE = 
TRANSFORM_EXERCISE_1_REQUISITES = ${TRANSFORM_EXERCISE_1_OBJ} ${SHADER_CODE}

TRANSFORM_EXERCISE_2_TARGET = transformationsExercise_2
_TRANSFORM_EXERCISE_2_OBJ = transformationsExercise_2.o ${_SHADERS_OBJ}
TRANSFORM_EXERCISE_2_OBJ = $(addprefix ${OBJD}/,${_TRANSFORM_EXERCISE_2_OBJ})
TRANSFORM_EXERCISE_2_INCLUDE = 
TRANSFORM_EXERCISE_2_REQUISITES = ${TRANSFORM_EXERCISE_2_OBJ} ${SHADER_CODE}

COORD_SYSTEMS_EXERCISE_1_TARGET = coordSystemsExercise_1
_COORD_SYSTEMS_EXERCISE_1_OBJ = coordSystemsExercise_1.o ${_SHADERS_OBJ}
COORD_SYSTEMS_EXERCISE_1_OBJ = $(addprefix ${OBJD}/,${_COORD_SYSTEMS_EXERCISE_1_OBJ})
COORD_SYSTEMS_EXERCISE_1_INCLUDE = 
COORD_SYSTEMS_EXERCISE_1_REQUISITES = ${COORD_SYSTEMS_EXERCISE_1_OBJ} ${SHADER_CODE}

########################################################################################
#   TARGET OF PROJECTS TO COMPILE
########################################################################################
ALL_TARGETS  = ${TRIANGLE_TARGET} ${RECTANGLE_TARGET} ${TRIFORCE_TARGET} ${SHADER_EXERCISE_1_TARGET} ${SHADER_EXERCISE_2_TARGET}
ALL_TARGETS += ${TEXTURE_EXERCISE_1_TARGET} ${TEXTURE_EXERCISE_2_TARGET} ${TEXTURE_EXERCISE_3_TARGET} ${TEXTURE_EXERCISE_4_TARGET}
ALL_TARGETS += ${TRANSFORM_EXERCISE_1_TARGET} ${TRANSFORM_EXERCISE_2_TARGET} ${COORD_SYSTEMS_EXERCISE_1_TARGET}






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

${TEXTURE_EXERCISE_2_TARGET}: ${TEXTURE_EXERCISE_2_REQUISITES}
	${CC} ${CFLAGS} -o ${BUILD}/$@ ${TEXTURE_EXERCISE_2_OBJ} ${LIBS}

${TEXTURE_EXERCISE_3_TARGET}: ${TEXTURE_EXERCISE_3_REQUISITES}
	${CC} ${CFLAGS} -o ${BUILD}/$@ ${TEXTURE_EXERCISE_3_OBJ} ${LIBS}

${TEXTURE_EXERCISE_4_TARGET}: ${TEXTURE_EXERCISE_4_REQUISITES}
	${CC} ${CFLAGS} -o ${BUILD}/$@ ${TEXTURE_EXERCISE_4_OBJ} ${LIBS}

${TRANSFORM_EXERCISE_1_TARGET}: ${TRANSFORM_EXERCISE_1_REQUISITES}
	${CC} ${CFLAGS} -o ${BUILD}/$@ ${TRANSFORM_EXERCISE_1_OBJ} ${LIBS}

${TRANSFORM_EXERCISE_2_TARGET}: ${TRANSFORM_EXERCISE_2_REQUISITES}
	${CC} ${CFLAGS} -o ${BUILD}/$@ ${TRANSFORM_EXERCISE_2_OBJ} ${LIBS}

${COORD_SYSTEMS_EXERCISE_1_TARGET}: ${COORD_SYSTEMS_EXERCISE_1_REQUISITES}
	${CC} ${CFLAGS} -o ${BUILD}/$@ ${COORD_SYSTEMS_EXERCISE_1_OBJ} ${LIBS}

${BUILD}/%.o: ${SRCD}/%.cpp
	${CC} ${CFLAGS} ${INCLUDES} -c $< -o $@
