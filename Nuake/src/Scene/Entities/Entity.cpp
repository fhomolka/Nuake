#include "../Components/ParentComponent.h"
#include "Entity.h"
#include "../Components/NameComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/CameraComponent.h"
#include "../Components/QuakeMap.h"
#include "../Components/LightComponent.h"
#include "../Components/QuakeMap.h"
#include "../Components/WrenScriptComponent.h"
#include "../Components/CharacterControllerComponent.h"
#include <src/Scene/Components/MeshComponent.h>

namespace Nuake
{
	void Entity::AddChild(Entity ent)
	{
		if ((int)m_EntityHandle != ent.GetHandle())
		{
			ent.GetComponent<ParentComponent>().HasParent = true;
			ent.GetComponent<ParentComponent>().Parent = *this;

			GetComponent<ParentComponent>().Children.push_back(ent);
		}
	}

	json Entity::Serialize()
	{
		BEGIN_SERIALIZE();
		SERIALIZE_OBJECT_REF_LBL("NameComponent", GetComponent<NameComponent>());
		SERIALIZE_OBJECT_REF_LBL("ParentComponent", GetComponent<ParentComponent>());
		SERIALIZE_OBJECT_REF_LBL("TransformComponent", GetComponent<TransformComponent>());
		if (HasComponent<CameraComponent>())
			SERIALIZE_OBJECT_REF_LBL("CameraComponent", GetComponent<CameraComponent>());
		if (HasComponent<QuakeMapComponent>())
			SERIALIZE_OBJECT_REF_LBL("QuakeMapComponent", GetComponent<QuakeMapComponent>());
		if (HasComponent<LightComponent>())
			SERIALIZE_OBJECT_REF_LBL("LightComponent", GetComponent<LightComponent>());
		if (HasComponent<WrenScriptComponent>())
			SERIALIZE_OBJECT_REF_LBL("WrenScriptComponent", GetComponent<WrenScriptComponent>());
		if (HasComponent<CharacterControllerComponent>())
			SERIALIZE_OBJECT_REF_LBL("CharacterControllerComponent", GetComponent<CharacterControllerComponent>());
		if (HasComponent<MeshComponent>())
			SERIALIZE_OBJECT_REF_LBL("MeshComponent", GetComponent<MeshComponent>());
		END_SERIALIZE();
	}

	bool Entity::Deserialize(const std::string& str)
	{
		BEGIN_DESERIALIZE();
		DESERIALIZE_COMPONENT(TransformComponent);
		DESERIALIZE_COMPONENT(NameComponent);
		DESERIALIZE_COMPONENT(ParentComponent);
		DESERIALIZE_COMPONENT(CameraComponent);
		DESERIALIZE_COMPONENT(QuakeMapComponent);
		DESERIALIZE_COMPONENT(LightComponent);
		DESERIALIZE_COMPONENT(MeshComponent);
		DESERIALIZE_COMPONENT(WrenScriptComponent);
		DESERIALIZE_COMPONENT(CharacterControllerComponent);
		return true;
	}

	Entity::Entity(entt::entity handle, Scene* scene)
	{
		m_EntityHandle = handle;
		m_Scene = scene;
	}

	Entity::Entity(const Entity& ent)
	{
		this->m_EntityHandle = ent.m_EntityHandle;
		this->m_Scene = ent.m_Scene;
	}


	Entity::Entity() 
	{
		this->m_EntityHandle = (entt::entity)-1;
	}
}
