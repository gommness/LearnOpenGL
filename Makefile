########################################################################################
#   PROJECT DIRECTORIES
########################################################################################
SRCD = src
INCLUDED = include
SHADERD = ${SRCD}/shaders
BUILD = build
OBJD = build
LIBD = lib

########################################################################################
#   COMPILER
########################################################################################
ifdef dev
	CC = python ~/.vim/bin/cc_args.py g++
else
	CC = g++
endif

CFLAGS = -Wall -g -std=c++17 -I ${INCLUDED}
LIBS = -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lGLEW -lGLU -lrt -lXxf86vm -lXinerama -lSOIL -lassimp

########################################################################################
#   CLASSES
########################################################################################
_SHADERS_OBJ = Shader.o VertexShader.o FragmentShader.o ShaderProgram.o Texture.o Image.o TextureSampler.o
_ASSIMP_OBJ = Mesh.o Model.o
_GAMEOBJECTS_OBJ = Camera.o
_DEBUG_OBJ = DebugTools.o

########################################################################################
#   SHADERS EN OPENGL
########################################################################################
_SHADER_CODE  = MyShader.vert MyShader.frag UniformExercise.frag InterpolationExercise.vert InterpolationExercise.frag
_SHADER_CODE += HOffset.vert UpsideDown.vert PositionalColor.vert PositionalColor.frag TextureExercise.vert
_SHADER_CODE += TextureExercise.frag TextureReverse.frag TextureSimple.vert TextureSimple.frag

SHADER_CODE = $(addprefix ${SHADERD}/,${_SHADER_CODE})

########################################################################################
#   PROJECT DEFINITIONS
########################################################################################

TRIANGLE_TARGET = HelloTriangle
_TRIANGLE_OBJ = HelloTriangle.o 
TRIANGLE_OBJ = $(addprefix ${OBJD}/,${_TRIANGLE_OBJ})
TRIANGLE_INCLUDE = 
TRIANGLE_REQUISITES = ${TRIANGLE_OBJ}

TRIFORCE_TARGET = HelloTriforce
_TRIFORCE_OBJ = HelloTriforce2.o ${_SHADERS_OBJ}
TRIFORCE_OBJ = $(addprefix ${OBJD}/,${_TRIFORCE_OBJ})
TRIFORCE_INCLUDE = 
TRIFORCE_REQUISITES = ${TRIFORCE_OBJ} ${SHADER_CODE}

RECTANGLE_TARGET = HelloRectangle
_RECTANGLE_OBJ = HelloRectangle.o
RECTANGLE_OBJ = $(addprefix ${OBJD}/,${_RECTANGLE_OBJ})
RECTANGLE_INCLUDE = 
RECTANGLE_REQUISITES = ${RECTANGLE_OBJ}

SHADER_EXERCISE_1_TARGET = ShaderExercise_1
_SHADER_EXERCISE_1_OBJ = ShaderExercise_1.o ${_SHADERS_OBJ}
SHADER_EXERCISE_1_OBJ = $(addprefix ${OBJD}/,${_SHADER_EXERCISE_1_OBJ})
SHADER_EXERCISE_1_INCLUDE = 
SHADER_EXERCISE_1_REQUISITES = ${SHADER_EXERCISE_1_OBJ} ${SHADER_CODE}

SHADER_EXERCISE_2_TARGET = ShaderExercise_2
_SHADER_EXERCISE_2_OBJ = ShaderExercise_2.o ${_SHADERS_OBJ}
SHADER_EXERCISE_2_OBJ = $(addprefix ${OBJD}/,${_SHADER_EXERCISE_2_OBJ})
SHADER_EXERCISE_2_INCLUDE = 
SHADER_EXERCISE_2_REQUISITES = ${SHADER_EXERCISE_2_OBJ} ${SHADER_CODE}

TEXTURE_EXERCISE_1_TARGET = TextureExercise_1
_TEXTURE_EXERCISE_1_OBJ = TextureExercise_1.o ${_SHADERS_OBJ}
TEXTURE_EXERCISE_1_OBJ = $(addprefix ${OBJD}/,${_TEXTURE_EXERCISE_1_OBJ})
TEXTURE_EXERCISE_1_INCLUDE = 
TEXTURE_EXERCISE_1_REQUISITES = ${TEXTURE_EXERCISE_1_OBJ} ${SHADER_CODE}

TEXTURE_EXERCISE_2_TARGET = TextureExercise_2
_TEXTURE_EXERCISE_2_OBJ = TextureExercise_2.o ${_SHADERS_OBJ}
TEXTURE_EXERCISE_2_OBJ = $(addprefix ${OBJD}/,${_TEXTURE_EXERCISE_2_OBJ})
TEXTURE_EXERCISE_2_INCLUDE = 
TEXTURE_EXERCISE_2_REQUISITES = ${TEXTURE_EXERCISE_2_OBJ} ${SHADER_CODE}

TEXTURE_EXERCISE_3_TARGET = TextureExercise_3
_TEXTURE_EXERCISE_3_OBJ = TextureExercise_3.o ${_SHADERS_OBJ}
TEXTURE_EXERCISE_3_OBJ = $(addprefix ${OBJD}/,${_TEXTURE_EXERCISE_3_OBJ})
TEXTURE_EXERCISE_3_INCLUDE = 
TEXTURE_EXERCISE_3_REQUISITES = ${TEXTURE_EXERCISE_3_OBJ} ${SHADER_CODE}

TEXTURE_EXERCISE_4_TARGET = TextureExercise_4
_TEXTURE_EXERCISE_4_OBJ = TextureExercise_4.o ${_SHADERS_OBJ}
TEXTURE_EXERCISE_4_OBJ = $(addprefix ${OBJD}/,${_TEXTURE_EXERCISE_4_OBJ})
TEXTURE_EXERCISE_4_INCLUDE = 
TEXTURE_EXERCISE_4_REQUISITES = ${TEXTURE_EXERCISE_4_OBJ} ${SHADER_CODE}

TRANSFORM_EXERCISE_1_TARGET = TransformationsExercise_1
_TRANSFORM_EXERCISE_1_OBJ = TransformationsExercise_1.o ${_SHADERS_OBJ}
TRANSFORM_EXERCISE_1_OBJ = $(addprefix ${OBJD}/,${_TRANSFORM_EXERCISE_1_OBJ})
TRANSFORM_EXERCISE_1_INCLUDE = 
TRANSFORM_EXERCISE_1_REQUISITES = ${TRANSFORM_EXERCISE_1_OBJ} ${SHADER_CODE}

TRANSFORM_EXERCISE_2_TARGET = TransformationsExercise_2
_TRANSFORM_EXERCISE_2_OBJ = TransformationsExercise_2.o ${_SHADERS_OBJ}
TRANSFORM_EXERCISE_2_OBJ = $(addprefix ${OBJD}/,${_TRANSFORM_EXERCISE_2_OBJ})
TRANSFORM_EXERCISE_2_INCLUDE = 
TRANSFORM_EXERCISE_2_REQUISITES = ${TRANSFORM_EXERCISE_2_OBJ} ${SHADER_CODE}

COORD_SYSTEMS_EXERCISE_1_TARGET = CoordSystemsExercise_1
_COORD_SYSTEMS_EXERCISE_1_OBJ = CoordSystemsExercise_1.o ${_SHADERS_OBJ}
COORD_SYSTEMS_EXERCISE_1_OBJ = $(addprefix ${OBJD}/,${_COORD_SYSTEMS_EXERCISE_1_OBJ})
COORD_SYSTEMS_EXERCISE_1_INCLUDE = 
COORD_SYSTEMS_EXERCISE_1_REQUISITES = ${COORD_SYSTEMS_EXERCISE_1_OBJ} ${SHADER_CODE}

CAMERA_EXERCISE_1_TARGET = CameraExercise_1
_CAMERA_EXERCISE_1_OBJ = CameraExercise_1.o ${_SHADERS_OBJ}
CAMERA_EXERCISE_1_OBJ = $(addprefix ${OBJD}/,${_CAMERA_EXERCISE_1_OBJ})
CAMERA_EXERCISE_1_INCLUDE = 
CAMERA_EXERCISE_1_REQUISITES = ${CAMERA_EXERCISE_1_OBJ} ${SHADER_CODE}

CAMERA_EXERCISE_2_TARGET = CameraExercise_2
_CAMERA_EXERCISE_2_OBJ = CameraExercise_2.o ${_SHADERS_OBJ}
CAMERA_EXERCISE_2_OBJ = $(addprefix ${OBJD}/,${_CAMERA_EXERCISE_2_OBJ})
CAMERA_EXERCISE_2_INCLUDE = 
CAMERA_EXERCISE_2_REQUISITES = ${CAMERA_EXERCISE_2_OBJ} ${SHADER_CODE}

CAMERA_EXERCISE_3_TARGET = CameraExercise_3
_CAMERA_EXERCISE_3_OBJ = CameraExercise_3.o ${_SHADERS_OBJ} ${_GAMEOBJECTS_OBJ} ${_DEBUG_OBJ}
CAMERA_EXERCISE_3_OBJ = $(addprefix ${OBJD}/,${_CAMERA_EXERCISE_3_OBJ})
CAMERA_EXERCISE_3_INCLUDE = 
CAMERA_EXERCISE_3_REQUISITES = ${CAMERA_EXERCISE_3_OBJ} ${SHADER_CODE}

LIGHT_EXERCISE_TARGET = colorsExercise
_LIGHT_EXERCISE_OBJ = ColorsExercise_1.o ${_SHADERS_OBJ} ${_GAMEOBJECTS_OBJ} ${_DEBUG_OBJ}
LIGHT_EXERCISE_OBJ = $(addprefix ${OBJD}/,${_LIGHT_EXERCISE_OBJ})
LIGHT_EXERCISE_INCLUDE = 
LIGHT_EXERCISE_REQUISITES = ${LIGHT_EXERCISE_OBJ} ${SHADER_CODE}

MATERIAL_EXERCISE_1_TARGET = MaterialsExercise_1
_MATERIAL_EXERCISE_1_OBJ = MaterialsExercise_1.o ${_SHADERS_OBJ} ${_GAMEOBJECTS_OBJ} ${_DEBUG_OBJ}
MATERIAL_EXERCISE_1_OBJ = $(addprefix ${OBJD}/,${_MATERIAL_EXERCISE_1_OBJ})
MATERIAL_EXERCISE_1_INCLUDE = 
MATERIAL_EXERCISE_1_REQUISITES = ${MATERIAL_EXERCISE_1_OBJ} ${SHADER_CODE}

MATERIAL_EXERCISE_2_TARGET = MaterialsExercise_2
_MATERIAL_EXERCISE_2_OBJ = MaterialsExercise_2.o ${_SHADERS_OBJ} ${_GAMEOBJECTS_OBJ} ${_DEBUG_OBJ}
MATERIAL_EXERCISE_2_OBJ = $(addprefix ${OBJD}/,${_MATERIAL_EXERCISE_2_OBJ})
MATERIAL_EXERCISE_2_INCLUDE = 
MATERIAL_EXERCISE_2_REQUISITES = ${MATERIAL_EXERCISE_2_OBJ} ${SHADER_CODE}

MATERIAL_EXERCISE_3_TARGET = MaterialsExercise_3
_MATERIAL_EXERCISE_3_OBJ = MaterialsExercise_3.o ${_SHADERS_OBJ} ${_GAMEOBJECTS_OBJ} ${_DEBUG_OBJ}
MATERIAL_EXERCISE_3_OBJ = $(addprefix ${OBJD}/,${_MATERIAL_EXERCISE_3_OBJ})
MATERIAL_EXERCISE_3_INCLUDE = 
MATERIAL_EXERCISE_3_REQUISITES = ${MATERIAL_EXERCISE_3_OBJ} ${SHADER_CODE}

LIGHT_DIRECTIONAL_EXERCISE_TARGET = LightDirectionalExercise
_LIGHT_DIRECTIONAL_EXERCISE_OBJ = LightDirectionalExercise.o ${_SHADERS_OBJ} ${_GAMEOBJECTS_OBJ} ${_DEBUG_OBJ}
LIGHT_DIRECTIONAL_EXERCISE_OBJ = $(addprefix ${OBJD}/,${_LIGHT_DIRECTIONAL_EXERCISE_OBJ})
LIGHT_DIRECTIONAL_EXERCISE_INCLUDE = 
LIGHT_DIRECTIONAL_EXERCISE_REQUISITES = ${LIGHT_DIRECTIONAL_EXERCISE_OBJ} ${SHADER_CODE}

LIGHT_ATTENUATION_EXERCISE_TARGET = LightAttenuationExercise
_LIGHT_ATTENUATION_EXERCISE_OBJ = LightAttenuationExercise.o ${_SHADERS_OBJ} ${_GAMEOBJECTS_OBJ} ${_DEBUG_OBJ}
LIGHT_ATTENUATION_EXERCISE_OBJ = $(addprefix ${OBJD}/,${_LIGHT_ATTENUATION_EXERCISE_OBJ})
LIGHT_ATTENUATION_EXERCISE_INCLUDE = 
LIGHT_ATTENUATION_EXERCISE_REQUISITES = ${LIGHT_ATTENUATION_EXERCISE_OBJ} ${SHADER_CODE}

LIGHT_FLASHLIGHT_EXERCISE_TARGET = LightFlashlightExercise
_LIGHT_FLASHLIGHT_EXERCISE_OBJ = LightFlashlightExercise.o ${_SHADERS_OBJ} ${_GAMEOBJECTS_OBJ} ${_DEBUG_OBJ}
LIGHT_FLASHLIGHT_EXERCISE_OBJ = $(addprefix ${OBJD}/,${_LIGHT_FLASHLIGHT_EXERCISE_OBJ})
LIGHT_FLASHLIGHT_EXERCISE_INCLUDE = 
LIGHT_FLASHLIGHT_EXERCISE_REQUISITES = ${LIGHT_FLASHLIGHT_EXERCISE_OBJ} ${SHADER_CODE}

LIGHT_MULTILIGHT_EXERCISE_TARGET = LightMultipleExercise
_LIGHT_MULTILIGHT_EXERCISE_OBJ = LightMultipleExercise.o ${_SHADERS_OBJ} ${_GAMEOBJECTS_OBJ} ${_DEBUG_OBJ}
LIGHT_MULTILIGHT_EXERCISE_OBJ = $(addprefix ${OBJD}/,${_LIGHT_MULTILIGHT_EXERCISE_OBJ})
LIGHT_MULTILIGHT_EXERCISE_INCLUDE = 
LIGHT_MULTILIGHT_EXERCISE_REQUISITES = ${LIGHT_MULTILIGHT_EXERCISE_OBJ} ${SHADER_CODE}

MODEL_EXERCISE_EXERCISE_TARGET = ModelExercise
_MODEL_EXERCISE_EXERCISE_OBJ = ModelExercise.o ${_SHADERS_OBJ} ${_GAMEOBJECTS_OBJ} ${_ASSIMP_OBJ} ${_DEBUG_OBJ}
MODEL_EXERCISE_EXERCISE_OBJ = $(addprefix ${OBJD}/,${_MODEL_EXERCISE_EXERCISE_OBJ})
MODEL_EXERCISE_EXERCISE_INCLUDE = 
MODEL_EXERCISE_EXERCISE_REQUISITES = ${MODEL_EXERCISE_EXERCISE_OBJ} ${SHADER_CODE}

########################################################################################
#   TARGET OF PROJECTS TO COMPILE
########################################################################################
ALL_TARGETS  = ${TRIANGLE_TARGET} ${RECTANGLE_TARGET} ${TRIFORCE_TARGET} ${SHADER_EXERCISE_1_TARGET} ${SHADER_EXERCISE_2_TARGET}
ALL_TARGETS += ${TEXTURE_EXERCISE_1_TARGET} ${TEXTURE_EXERCISE_2_TARGET} ${TEXTURE_EXERCISE_3_TARGET} ${TEXTURE_EXERCISE_4_TARGET}
ALL_TARGETS += ${TRANSFORM_EXERCISE_1_TARGET} ${TRANSFORM_EXERCISE_2_TARGET} ${COORD_SYSTEMS_EXERCISE_1_TARGET} ${CAMERA_EXERCISE_1_TARGET}
ALL_TARGETS += ${CAMERA_EXERCISE_2_TARGET} ${CAMERA_EXERCISE_3_TARGET} ${LIGHT_EXERCISE_TARGET} ${MATERIAL_EXERCISE_1_TARGET}
ALL_TARGETS += ${MATERIAL_EXERCISE_2_TARGET} ${MATERIAL_EXERCISE_3_TARGET} ${LIGHT_DIRECTIONAL_EXERCISE_TARGET} ${LIGHT_ATTENUATION_EXERCISE_TARGET}
ALL_TARGETS += ${LIGHT_FLASHLIGHT_EXERCISE_TARGET} ${LIGHT_MULTILIGHT_EXERCISE_TARGET} ${MODEL_EXERCISE_EXERCISE_TARGET}






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

${CAMERA_EXERCISE_1_TARGET}: ${CAMERA_EXERCISE_1_REQUISITES}
	${CC} ${CFLAGS} -o ${BUILD}/$@ ${CAMERA_EXERCISE_1_OBJ} ${LIBS}

${CAMERA_EXERCISE_2_TARGET}: ${CAMERA_EXERCISE_2_REQUISITES}
	${CC} ${CFLAGS} -o ${BUILD}/$@ ${CAMERA_EXERCISE_2_OBJ} ${LIBS}

${CAMERA_EXERCISE_3_TARGET}: ${CAMERA_EXERCISE_3_REQUISITES}
	${CC} ${CFLAGS} -o ${BUILD}/$@ ${CAMERA_EXERCISE_3_OBJ} ${LIBS}

${LIGHT_EXERCISE_TARGET}: ${LIGHT_EXERCISE_REQUISITES}
	${CC} ${CFLAGS} -o ${BUILD}/$@ ${LIGHT_EXERCISE_OBJ} ${LIBS}

${MATERIAL_EXERCISE_1_TARGET}: ${MATERIAL_EXERCISE_1_REQUISITES}
	${CC} ${CFLAGS} -o ${BUILD}/$@ ${MATERIAL_EXERCISE_1_OBJ} ${LIBS}

${MATERIAL_EXERCISE_2_TARGET}: ${MATERIAL_EXERCISE_2_REQUISITES}
	${CC} ${CFLAGS} -o ${BUILD}/$@ ${MATERIAL_EXERCISE_2_OBJ} ${LIBS}

${MATERIAL_EXERCISE_3_TARGET}: ${MATERIAL_EXERCISE_3_REQUISITES}
	${CC} ${CFLAGS} -o ${BUILD}/$@ ${MATERIAL_EXERCISE_3_OBJ} ${LIBS}

${LIGHT_DIRECTIONAL_EXERCISE_TARGET}: ${LIGHT_DIRECTIONAL_EXERCISE_REQUISITES}
	${CC} ${CFLAGS} -o ${BUILD}/$@ ${LIGHT_DIRECTIONAL_EXERCISE_OBJ} ${LIBS}

${LIGHT_ATTENUATION_EXERCISE_TARGET}: ${LIGHT_ATTENUATION_EXERCISE_REQUISITES}
	${CC} ${CFLAGS} -o ${BUILD}/$@ ${LIGHT_ATTENUATION_EXERCISE_OBJ} ${LIBS}

${LIGHT_FLASHLIGHT_EXERCISE_TARGET}: ${LIGHT_FLASHLIGHT_EXERCISE_REQUISITES}
	${CC} ${CFLAGS} -o ${BUILD}/$@ ${LIGHT_FLASHLIGHT_EXERCISE_OBJ} ${LIBS}

${LIGHT_MULTILIGHT_EXERCISE_TARGET}: ${LIGHT_MULTILIGHT_EXERCISE_REQUISITES}
	${CC} ${CFLAGS} -o ${BUILD}/$@ ${LIGHT_MULTILIGHT_EXERCISE_OBJ} ${LIBS}

${MODEL_EXERCISE_EXERCISE_TARGET}: ${MODEL_EXERCISE_EXERCISE_REQUISITES}
	${CC} ${CFLAGS} -o ${BUILD}/$@ ${MODEL_EXERCISE_EXERCISE_OBJ} ${LIBS}

${BUILD}/%.o: ${SRCD}/%.cpp
	${CC} ${CFLAGS} ${INCLUDES} -c $< -o $@
