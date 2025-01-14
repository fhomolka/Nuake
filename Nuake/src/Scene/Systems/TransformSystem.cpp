#include "TransformSystem.h"

#include "src/Core/Maths.h"

#include "src/Scene/Scene.h"
#include <src/Scene/Components/TransformComponent.h>
#include <src/Scene/Components/CameraComponent.h>
#include <src/Scene/Components/ParentComponent.h>

namespace Nuake {
	TransformSystem::TransformSystem(Scene* scene)
	{
		m_Scene = scene;
	}

	bool TransformSystem::Init()
	{
		UpdateTransform();
		return true;
	}

	void TransformSystem::Update(Timestep ts)
	{
		UpdateTransform();
	}

	void TransformSystem::FixedUpdate(Timestep ts)
	{

	}

	void TransformSystem::Exit()
	{

	}

	void TransformSystem::UpdateTransform()
	{
		auto camView = m_Scene->m_Registry.view<TransformComponent, CameraComponent>();
		for (auto e : camView)
		{
			auto [transform, camera] = camView.get<TransformComponent, CameraComponent>(e);
			Matrix4 cameraTransform = camera.CameraInstance->GetTransformRotation();
		}

		// Calculate all local transforms
		auto localTransformView = m_Scene->m_Registry.view<TransformComponent>();
		for (auto tv : localTransformView)
		{
			TransformComponent& transform = localTransformView.get<TransformComponent>(tv);

			Matrix4 localTransform = Matrix4(1.0f);
			localTransform = glm::translate(localTransform, transform.Translation);
			localTransform = glm::rotate(localTransform, glm::radians(transform.Rotation.x), Vector3(1, 0, 0));
			localTransform = glm::rotate(localTransform, glm::radians(transform.Rotation.y), Vector3(0, 1, 0));
			localTransform = glm::rotate(localTransform, glm::radians(transform.Rotation.z), Vector3(0, 0, 1));
			localTransform = glm::scale(localTransform, transform.Scale);

			transform.LocalTransform = localTransform;
		}

		// Calculate all global transforms
		auto transformView = m_Scene->m_Registry.view<ParentComponent, TransformComponent>();
		for (auto e : transformView) 
		{
			auto [parent, transform] = transformView.get<ParentComponent, TransformComponent>(e);

			if (!parent.HasParent)
			{
				// If no parents, then globalTransform is local transform.
				transform.GlobalTransform = transform.LocalTransform;
			}

			Entity currentParent = Entity((entt::entity)e, m_Scene);

			Matrix4 globalTransform = transform.LocalTransform;
			Vector3 globalPosition = transform.Translation;
			Vector3 globalRotation = transform.Rotation;
			Vector3 globalScale = transform.Scale;

			ParentComponent parentComponent = currentParent.GetComponent<ParentComponent>();
			while (parentComponent.HasParent)
			{
				TransformComponent& transformComponent = parentComponent.Parent.GetComponent<TransformComponent>();

				globalTransform = transformComponent.LocalTransform * globalTransform;

				globalPosition += transformComponent.Translation;
				globalRotation += transformComponent.Rotation;
				globalScale *= transformComponent.Scale;
		
				parentComponent = parentComponent.Parent.GetComponent<ParentComponent>();
			}
				
				
			transform.GlobalTranslation = globalPosition;
			transform.GlobalRotation = globalRotation;
			transform.GlobalScale = globalScale;

			transform.GlobalTransform = globalTransform;
		}
	}
}
