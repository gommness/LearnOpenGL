SRCD = src
BUILD = build
OBJD = build
LIBD = lib

CFLAGS= -g -Wall

TRIANGLE_TARGET = helloTriangle
_TRIANGLE_OBJ = helloTriangle.o
TRIANGLE_OBJ = $(addprefix ${OBJD}/,${_TRIANGLE_OBJ})
TRIANGLE_INCLUDE = 

TRIFORCE_TARGET = helloTriforce
_TRIFORCE_OBJ = helloTriforce2.o
TRIFORCE_OBJ = $(addprefix ${OBJD}/,${_TRIFORCE_OBJ})
TRIFORCE_INCLUDE = 

RECTANGLE_TARGET = helloRectangle
_RECTANGLE_OBJ = helloRectangle.o
RECTANGLE_OBJ = $(addprefix ${OBJD}/,${_RECTANGLE_OBJ})
RECTANGLE_INCLUDE = 

ALL_TARGETS = ${TRIANGLE_TARGET} ${RECTANGLE_TARGET} ${TRIFORCE_TARGET}

CFLAG = -Wall -g
CC = g++
LIBS = -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lGLEW -lGLU -lrt -lXxf86vm -lXinerama

all: ${ALL_TARGETS}

clean:
	-rm -f ${BUILD}/*

${TRIANGLE_TARGET}: ${TRIANGLE_OBJ}
	${CC} ${CFLAGS} ${INCLUDES} -o ${BUILD}/$@ ${TRIANGLE_OBJ} ${LIBS}

${RECTANGLE_TARGET}: ${RECTANGLE_OBJ}
	${CC} ${CFLAGS} ${INCLUDES} -o ${BUILD}/$@ ${RECTANGLE_OBJ} ${LIBS}

${TRIFORCE_TARGET}: ${TRIFORCE_OBJ}
	${CC} ${CFLAGS} ${INCLUDES} -o ${BUILD}/$@ ${TRIFORCE_OBJ} ${LIBS}

${BUILD}/%.o: ${SRCD}/%.cpp
	${CC} ${CFLAGS} ${INCLUDES} -c $< -o $@
