#pragma once
#include "Component.h"
#include "Globals.h"
#include "ImGuizmo/ImGuizmo.h"
#include <vector>
#include <string>

class GameObject
{
public:
	GameObject(std::string name, GameObject* parent);
	~GameObject();

	void Draw();
	void Update();

	const Component* FindComponent(component_type type);

	void AddChildren(GameObject* children);
	void AddComponent(Component* component);
	void DeleteComponentType(component_type type);
	void CreateHierarchy();

	const int GetVertices();
	const int GetIndices();
	const int GetTextureWidth();
	const int GetTextureHeight();

	void SetTextureDimensions(int w, int h);
	const float4x4 GetGlobalTransform(Component_Transform* trans);

public:
	std::string name;
	GameObject* parent;

private:
	std::vector<Component*> components;
	std::vector<GameObject*> childrens;
};

