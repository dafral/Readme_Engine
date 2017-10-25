#include "Application.h"
#include "ModuleImgui.h"
#include "PanelInspector.h"

#include "GameObject.h"
#include "glew/include/GL/glew.h"
#include "Imgui/imgui.h"

GameObject::GameObject(std::string name, GameObject* parent) : name(name), parent(parent)
{}

GameObject::~GameObject()
{}

void GameObject::AddChildren(GameObject* children)
{
	childrens.push_back(children);
}

void GameObject::AddComponent(Component* component)
{
	components.push_back(component);
}

void GameObject::Update()
{
	// Doing stuff
	Draw();

	// Calling childrens Update()
	if (!childrens.empty())
	{
		for (int i = 0; i < childrens.size(); i++)
		{
			childrens[i]->Update();
		}
	}
}

void GameObject::Draw()
{
	Component_Material* material = (Component_Material*)FindComponent(COMPONENT_MATERIAL); // Right now we can only have one and it works!

	for (int i = 0; i < components.size(); i++)
	{	
		if (components[i]->type == COMPONENT_MESH)
		{
			Component_Mesh* mesh = (Component_Mesh*)components[i];

			glEnableClientState(GL_VERTEX_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertices);
			glVertexPointer(3, GL_FLOAT, 0, NULL);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices);

			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->id_uvs);
			glTexCoordPointer(3, GL_FLOAT, 0, NULL);

			if (material != nullptr) glBindTexture(GL_TEXTURE_2D, (GLuint)material->id_texture);
			glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, NULL);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			glBindTexture(GL_TEXTURE_2D, 0);
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
	}
}

void GameObject::CreateHierarchy()
{
	uint flag = 0; // Deafult

	// Setting flags
	if (childrens.empty())
		flag |= ImGuiTreeNodeFlags_Leaf;

	if (App->imgui->inspector->selected == this) 
		flag |= ImGuiTreeNodeFlags_Selected;

	// Creating tree
	if (ImGui::TreeNodeEx(name.c_str(), flag))
	{
		// Know if a GO is selected
		if (ImGui::IsItemClicked())
			App->imgui->inspector->selected = this;

		// Recursion
		for (int i = 0; i < childrens.size(); i++)
			childrens[i]->CreateHierarchy();

		ImGui::TreePop();
	}
}

Component* GameObject::FindComponent(component_type type)
{
	for (int i = 0; i < components.size(); i++)
	{
		if (components[i]->type == type) return components[i];
	}
	return nullptr;
}

// This function deletes all components of that type
void GameObject::DeleteComponentType(component_type type)
{
	for (int i = 0; i < components.size(); )
	{
		if (components[i]->type == type)
		{
			delete components[i];
			components.erase(components.begin() + i);
		}
		else i++;
	}
}

// ============================================================================

const int GameObject::GetVertices()
{
	int t_vertices = 0;

	for (int i = 0; i < components.size(); i++)
	{
		if (components[i]->type == COMPONENT_MESH)
		{
			Component_Mesh* aux = (Component_Mesh*)components[i];
			t_vertices += aux->num_vertices;
		}
	}
	return t_vertices;
}

const int GameObject::GetIndices()
{
	int t_indices = 0;

	for (int i = 0; i < components.size(); i++)
	{
		if (components[i]->type == COMPONENT_MESH)
		{
			Component_Mesh* aux = (Component_Mesh*)components[i];
			t_indices += aux->num_indices;
		}
	}

	return t_indices;
}

const int GameObject::GetTextureWidth()
{
	Component_Material* aux = (Component_Material*)FindComponent(COMPONENT_MATERIAL);
	return aux->width;
}

const int GameObject::GetTextureHeight()
{
	Component_Material* aux = (Component_Material*)FindComponent(COMPONENT_MATERIAL);
	return aux->height;
}

void GameObject::SetTextureDimensions(int w, int h)
{
	Component_Material* aux = (Component_Material*)FindComponent(COMPONENT_MATERIAL);
	aux->width = w;
	aux->height = h;
}