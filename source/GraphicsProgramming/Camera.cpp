#include "Camera.h"
#include <math.h>

Camera::Camera()
{
	//set initial camera position
	position.z = -7.5;
	position.x = 7.5;
	position.y = 2;
}

Camera::~Camera()
{
}

void Camera::update(float dt) {
	float cosR, cosP, cosY;
	float sinR, sinP, sinY;

	cosY = cosf(yaw*3.1415 / 180);
	cosP = cosf(pitch*3.1415 / 180);
	cosR = cosf(roll*3.1415 / 180);

	sinY = sinf(yaw*3.1415 / 180);
	sinP = sinf(pitch*3.1415 / 180);
	sinR = sinf(roll*3.1415 / 180);

	//calculate forward vector
	forward.x = sinY * cosP * dt;
	forward.y = sinP * dt;
	forward.z = cosP * cosY * -1 * dt;

	//set the look at position
	lookAt = position + forward;
	
	//calculate up vector
	up.x = (-cosY * sinR) - (sinY * sinP * cosR)  * dt;
	up.y = 1 * dt;
	up.z = (-sinY * sinR) - (sinP * cosR * -cosY)  * dt;

	//calculate right vector
	right.x = cosY * dt;
	right.y = 0 * dt;
	right.z = sinY * dt;
}

//Camera movement
void Camera::moveForward(float dt) { forward.y = 0;	position += forward; }
void Camera::moveBackward(float dt) { forward.y = 0; position -= forward; }
void Camera::moveLeft(float dt) { position -= right; }
void Camera::moveRight(float dt) { position += right; }
void Camera::moveUp(float dt) { position.y = position.y + 1 * dt; }
void Camera::moveDown(float dt) { position.y = position.y - 1 * dt; }