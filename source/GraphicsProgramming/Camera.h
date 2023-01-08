#pragma once
#include "Vector3.h"
class Camera
{
public:
	Camera();
	~Camera();

	void Camera::update(float dt);

	void Camera::moveForward(float dt);
	void Camera::moveBackward(float dt);
	void Camera::moveLeft(float dt);
	void Camera::moveRight(float dt);
	void Camera::moveUp(float dt);
	void Camera::moveDown(float dt);

	void rotate(float x) { yaw += x; };
	void tilt(float x) { pitch += x; };

	void setX(float px) { position.x = px; };
	void setY(float py) { position.y = py; };
	void setZ(float pz) { position.z = pz; };
	void resetCamera() {
		position.z = -7.5;
		position.x = 7.5;
		position.y = 2;
	};
	Vector3 getPosition() { return position; };
	Vector3 getForward() { return forward; };
	Vector3 getLookAt() { return lookAt; };
	Vector3 getUp() { return up; };
	float getPitch() { return pitch; };

private:
	Vector3 lookAt;
	Vector3 position;
	Vector3 forward;
	Vector3 up;
	Vector3 right;
	
	float yaw = 0, pitch = 0, roll = 0;
	float speed = 0.1;
};

