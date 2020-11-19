#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "debugTools.h"

#define CAMERA_DEFAULT_YAW -90.0f
#define CAMERA_DEFAULT_PITCH 0.0f
#define CAMERA_DEFAULT_ROLL 0.0f
#define CAMERA_DEFAULT_SPEED 3.0f
#define CAMERA_DEFAULT_SENSITIVITY 0.05f
#define CAMERA_DEFAULT_ZOOM 45.0f

class Camera {
public:

    enum Movement{
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    GLfloat yaw;
    GLfloat pitch;
    GLfloat roll;

    GLfloat movementSpeed;
    GLfloat sensitivity;
    GLfloat zoomAmmount;

    Camera(glm::vec3 position = glm::vec3(0,0,0), glm::vec3 up = glm::vec3(0,1,0), GLfloat yaw = CAMERA_DEFAULT_YAW, GLfloat pitch = CAMERA_DEFAULT_PITCH, GLfloat roll = CAMERA_DEFAULT_ROLL);
    Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch, GLfloat roll);
    glm::mat4 getViewMatrix();

    void translate(Camera::Movement direction, GLfloat deltaTime);
    void translate(glm::vec3 direction);
    void rotate(GLfloat yaw, GLfloat pitch, GLfloat roll = 0, GLboolean constrainPitch = true); //TODO quaternions for better rotation
    void zoom(GLfloat ammount);

private:
    void updateCameraVectors();

};

#endif
