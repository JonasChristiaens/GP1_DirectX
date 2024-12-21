#include "pch.h"
#include "Camera.h"

namespace dae
{
	Camera::Camera(const Vector3& origin, float fovAngle, float aspectRatio, float nearPlane, float farPlane)
		: m_Origin{ origin }
		, m_FovAngle{ fovAngle }
		, m_AspectRatio{ aspectRatio }
		, m_NearPlane{ nearPlane }
		, m_FarPlane{ farPlane }
	{
		m_FOV = tanf((m_FovAngle * TO_RADIANS) / 2.0f);
	}

	void Camera::CalculateViewMatrix()
	{
		// Method 1
		//ONB -> invViewMatrix
		m_Right = Vector3::Cross(Vector3::UnitY, m_Forward).Normalized();
		m_Up = Vector3::Cross(m_Forward, m_Right).Normalized();
		m_InvViewMatrix = { {m_Right, 0}, {m_Up, 0}, {m_Forward, 0}, {m_Origin, 1} };

		//Inverse(ONB) => ViewMatrix
		m_ViewMatrix = m_InvViewMatrix.Inverse();

		// Method 2 - used in DirectX => DOES NOT WORK YET
		/*m_ViewMatrix = Matrix::CreateLookAtLH(m_Origin, m_Forward, m_Up, m_Right);
		m_InvViewMatrix = Matrix::Inverse(m_ViewMatrix);*/
	}
	void Camera::CalculateProjectionMatrix()
	{
		Matrix projection{ Matrix::CreatePerspectiveFovLH(m_FOV, m_AspectRatio, .1f, 100.f) };

		// combine all space transformation matrix into one matrix
		m_ProjectionMatrix = m_WorldMatrix * m_ViewMatrix * projection;
	}

	void Camera::Update(const Timer* pTimer)
	{
		//Camera Update Logic
		const float deltaTime = pTimer->GetElapsed();

		//Keyboard Input
		const uint8_t* pKeyboardState = SDL_GetKeyboardState(nullptr);
		MoveCamera(pKeyboardState, deltaTime);
		RotateCamera(deltaTime);

		//Update Matrices
		CalculateViewMatrix();
		CalculateProjectionMatrix();
	}

	Matrix Camera::GetViewMatrix() const
	{
		return m_ViewMatrix;
	}
	Matrix Camera::GetProjectionMatrix() const
	{
		return m_ProjectionMatrix;
	}

	void Camera::MoveCamera(const uint8_t* pKeyboardState, float deltaTime)
	{
		//Transforming camera's origin ( Movement )
		if (pKeyboardState[SDL_SCANCODE_A])
		{
			m_Origin -= m_Right * m_MovementSpeed * deltaTime;
		}
		if (pKeyboardState[SDL_SCANCODE_D])
		{
			m_Origin += m_Right * m_MovementSpeed * deltaTime;
		}
		if (pKeyboardState[SDL_SCANCODE_W])
		{
			m_Origin += m_Forward * m_MovementSpeed * deltaTime;
		}
		if (pKeyboardState[SDL_SCANCODE_S])
		{
			m_Origin -= m_Forward * m_MovementSpeed * deltaTime;
		}
	}
	void Camera::RotateCamera(float deltaTime)
	{
		//Mouse Input
		int mouseX{}, mouseY{};
		const uint32_t mouseState = SDL_GetRelativeMouseState(&mouseX, &mouseY);

		//Transforming camera's forward vector ( Rotation )
		if (mouseState == 5)
		{
			m_Origin += m_Up * static_cast<float>(mouseY * -1) * m_MovementSpeed * deltaTime;
		}
		else if (mouseState == 1)
		{
			m_Origin += m_Forward * static_cast<float>(mouseY * -1) * m_MovementSpeed * deltaTime;
			m_TotalYaw += static_cast<float>(mouseX) * m_RotationSpeed * deltaTime;
		}
		else if (mouseState == 4)
		{
			m_TotalYaw += static_cast<float>(mouseX) * m_RotationSpeed * deltaTime;
			m_TotalPitch += static_cast<float>(mouseY * -1) * m_RotationSpeed * deltaTime;
			
		}
		if (mouseX || mouseY)
		{
			Matrix rotationMatrix{ Matrix::CreateRotationX(m_TotalPitch * TO_RADIANS) * Matrix::CreateRotationY(m_TotalYaw * TO_RADIANS) };

			m_Forward = rotationMatrix.TransformVector(Vector3::UnitZ);
			m_Forward.Normalize();
		}
	}
}