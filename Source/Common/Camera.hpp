#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Window;

class Camera
{
public:
	Camera(const Window& window, glm::vec3 position = {0.0f, 0.0f, 0.0f}, float pitch = 0.0f);

	~Camera();

	void Update(float deltaTime);

	inline glm::mat4 GetView() const
	{
		return view;
	}

	inline glm::mat4 GetProjection() const
	{
		return projection;
	}

private:
	const Window& window;

	float yaw;
	float pitch;
	float near;
	float far;
	float fovy;
	float fovyMin;
	float fovyMax;
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::mat4 view;
	glm::mat4 projection;

	void UpdateMatrices();
};
