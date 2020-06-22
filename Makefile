SRCD = src
BUILD = build
OBJD = build
LIBD = lib

_OBJ = helloTriangle.o
OBJ = $(addprefix ${OBJD}/,${_OBJ})
CFLAG = -Wall -g
CC = g++
INCLUDE = 
LIBS = -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lGLEW -lGLU -lrt -lXxf86vm -lXinerama

helloTriangle: ${OBJ}
	${CC} ${CFLAGS} ${INCLUDES} -o ${BUILD}/$@ ${OBJ} ${LIBS}

clean:
	-rm -f ${BUILD}/*

${BUILD}/%.o: ${SRCD}/%.cpp
	${CC} ${CFLAGS} ${INCLUDES} -c $< -o $@
