#pragma once
#include <src/Core/Timestep.h>
#include "src/Core/Core.h"

class Scene;
class System {
public:
	Scene* m_Scene;

	virtual void Init() = 0;

	virtual void Draw() = 0;

	virtual void Update(Timestep ts) = 0;
	virtual void FixedUpdate(Timestep ts) = 0;

	virtual void Exit() = 0;
};