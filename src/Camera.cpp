#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch, GLfloat roll) :
    front(glm::vec3(0,0,-1)),
    yaw(yaw),
    pitch(pitch),
    roll(roll),
    movementSpeed(CAMERA_DEFAULT_SPEED),
    sensitivity(CAMERA_DEFAULT_SENSITIVITY),
    zoomAmmount(CAMERA_DEFAULT_ZOOM)
{
    this->position = position;
    this->worldUp = up;
    this->updateCameraVectors();
}

Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch, GLfloat roll) : 
    Camera(glm::vec3(posX, posY, posZ), glm::vec3(upX, upY, upZ), yaw, pitch, roll)
{}

glm::mat4 Camera::getViewMatrix(){
    return glm::lookAt(position, position+front, up);
}

void Camera::translate(Camera::Movement direction, GLfloat deltaTime){
    GLfloat velocity = this->movementSpeed * deltaTime;
    if (direction == Camera::Movement::FORWARD)
        this->position += this->front * velocity;
    if (direction == Camera::Movement::BACKWARD)
        this->position -= this->front * velocity;
    if (direction == Camera::Movement::LEFT)
        this->position -= this->right * velocity;
    if (direction == Camera::Movement::RIGHT)
        this->position += this->right * velocity;
    if(direction == Camera::Movement::UP)
        this->position += this->up * velocity;
    if(direction == Camera::Movement::DOWN)
        this->position -= this->up * velocity;
}

void Camera::translate(glm::vec3 direction){
    this->position += direction;
}

void Camera::rotate(GLfloat yaw, GLfloat pitch, GLfloat roll, GLboolean constrainPitch){
    this->yaw += yaw * this->sensitivity;
    this->pitch += pitch * this->sensitivity;
    this->roll += roll * this->sensitivity;

    if(constrainPitch){
        if (this->pitch > 89.0f)
            this->pitch = 89.0f;
        if (this->pitch < -89.0f)
            this->pitch = -89.0f;
    }

    this->updateCameraVectors();
}

void Camera::zoom(GLfloat ammount){
    if (this->zoomAmmount >= 1.0f && this->zoomAmmount <= 45.0f)
        this->zoomAmmount -= ammount;
    if (this->zoomAmmount <= 1.0f)
        this->zoomAmmount = 1.0f;
    if (this->zoomAmmount >= 45.0f)
        this->zoomAmmount = 45.0f;
}

void Camera::updateCameraVectors(){
    glm::vec3 front;
    front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    front.y = sin(glm::radians(this->pitch));
    front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    this->front = glm::normalize(front);
    // Also re-calculate the Right and Up vector
    this->right = glm::normalize(glm::cross(this->front, this->worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    this->up    = glm::normalize(glm::cross(this->right, this->front));
}

