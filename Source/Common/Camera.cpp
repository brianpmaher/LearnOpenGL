#include "Camera.hpp"

#include "Window.hpp"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(const Window& window, glm::vec3 position, float pitch)
	: window(window)
	, yaw(-90.0f)
	, pitch(pitch)
	, near(0.1f)
	, far(100.0f)
	, fovy(45.0f)
	, fovyMin(5.0f)
	, fovyMax(45.0f)
	, position(position)
	, front(0.0f, 0.0f, -1.0f)
	, up(0.0f, 1.0f, 0.0f)
	, right(glm::cross(front, up))
{
	UpdateMatrices();
}

Camera::~Camera()
{
}

void Camera::Update(float deltaTime)
{
	// Update keyboard movement
	{
		auto moveDirection = glm::vec3(0.0f);

		// Forward
		if (window.IsKeyDown(Input::Key::W))
		{
			moveDirection = front;
		}

		// Left
		if (window.IsKeyDown(Input::Key::A))
		{
			moveDirection -= right;
		}

		// Backward
		if (window.IsKeyDown(Input::Key::S))
		{
			moveDirection -= front;
		}

		// Right
		if (window.IsKeyDown(Input::Key::D))
		{
			moveDirection += right;
		}

		if (glm::length(moveDirection) > 0.0f)
		{
			constexpr float moveSpeed = 5.0f;
			position += glm::normalize(moveDirection) * moveSpeed * deltaTime;
		}
	}

	// Update mouse rotation
	{
		glm::vec2 cursorMovement = window.GetCursorMovement();
		constexpr float sensitivity = 0.1f;
		glm::vec2 deltaMovement = cursorMovement * sensitivity;

		yaw += deltaMovement.x;
		pitch -= deltaMovement.y;

		pitch = glm::clamp(pitch, -89.0f, 89.0f);

		glm::vec3 direction;
		auto yawRad = glm::radians(yaw);
		auto pitchRad = glm::radians(pitch);
		direction.x = cos(yawRad) * cos(pitchRad);
		direction.y = sin(pitchRad);
		direction.z = sin(yawRad) * cos(pitchRad);
		front = glm::normalize(direction);
		right = glm::normalize(glm::cross(front, up));
	}

	// Update scroll zoom
	{
		fovy -= window.GetScrollMovement().y;
		fovy = glm::clamp(fovy, 5.0f, 45.0f);
	}

	UpdateMatrices();
}

void Camera::UpdateMatrices()
{
	view = glm::lookAt(position, position + front, up);

	projection = glm::perspective(glm::radians(fovy), window.GetAspect(), near, far);
}
