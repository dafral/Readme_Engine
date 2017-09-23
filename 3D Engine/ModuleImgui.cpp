#include "Globals.h"
#include "Application.h"
#include "ModuleImgui.h"
#include "ModuleWindow.h"
#include "Panel.h"
#include "PanelConsole.h"

#include "Imgui/imgui.h"
#include "imgui_impl_sdl.h"

ModuleImgui::ModuleImgui(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleImgui::~ModuleImgui()
{}

bool ModuleImgui::Init()
{
	ImGui_ImplSdl_Init(App->window->window);

	// Filling the vector with panels
	panels.push_back(console = new PanelConsole());

	return true;
}

update_status ModuleImgui::PreUpdate(float dt)
{
	ImGui_ImplSdl_NewFrame(App->window->window);

	return UPDATE_CONTINUE;
}

update_status ModuleImgui::Update(float dt)
{
	for (std::vector<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it)
	{
		Panel* panel = (*it);
		/*panel->Draw();*/
	}

	return UPDATE_CONTINUE;
}

bool ModuleImgui::CleanUp()
{
	for (std::vector<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it)
	{
		Panel* panel = (*it);

		if (panel != nullptr)
		{
			delete panel;
			panel = nullptr;
		}
	}

	panels.clear();

	return true;
}


