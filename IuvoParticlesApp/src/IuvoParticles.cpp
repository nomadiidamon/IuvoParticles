#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"
#include "Walnut/Image.h"

class ExampleLayer : public Walnut::Layer
{
public:
	virtual void OnUIRender() override
	{
		ImGui::Begin("Hello");
		ImGui::Button("Button");
		ImGui::End();

		ImGui::ShowDemoWindow();
	}
};

struct Quaternion {
	union {
		float val[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		float x, y, z, w;
	};
};

struct Vector3 {
	union {
		float val[3] = { 0.0f, 0.0f, 0.0f };
		float x, y, z;
	};
};

struct Vector4 {
	union {
		float val[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		float x, y, z, w;
	};
};

class CameraLayer : public Walnut::Layer {
public:
	/// Layer properties
	Vector4 pos;
	Vector3 rot;
	Quaternion rot_q;
	float fov = 75.0f;
	float nearPlane = 0.01f;
	float farPlane = 300.0f;
	bool quat_rot = false;
	bool is_active = false;


	virtual void OnUIRender() override {
		ImGui::Begin("Camera Layer");

		ImVec2 max = ImVec2(ImGui::GetWindowContentRegionMax().x * 2.0f, ImGui::GetWindowContentRegionMax().y * 2.0f);
		is_active = ImGui::IsMouseHoveringRect(ImGui::GetWindowPos(), max);
		if (is_active) {
			ImGui::Checkbox("Quaternion Rotation", &quat_rot);
			ImGui::Separator();
		}

		ImGui::SliderFloat4("Position", pos.val, -25.0f, 25.0f);
		if (!quat_rot)
		{
			ImGui::SliderFloat3("Rotation", rot.val, -25.0f, 25.0f);
		}
		else {
			ImGui::SliderFloat4("Quaternion Rotation", rot_q.val, -25.0f, 25.0f);
		}
		/// TODO: convert and overwrite the inactive rot values to match selected mode

		ImGui::SliderFloat("FOV", &fov, 25.0f, 125.0f);
		ImGui::SliderFloat("Near Plane", &nearPlane, 0.005f, 10.0f);
		ImGui::SliderFloat("Near Plane", &farPlane, 10.0f, 500.0f);
		ImGui::Separator();
		ImGui::End();
	}
};

class DetailsLayer : public Walnut::Layer {
public:
	/// Layer properties
	bool is_active = false;

	/// Particle System Transform
	Vector4 PS_pos;
	Vector4 PS_rot;
	Quaternion PS_rot_q;
	Vector4 PS_scale;
	bool PS_quat_rot = false;


	/// Velocity
	Vector3 velocity;
	float movementSpeed = 5.0f;


	/// Bounds
	bool respectBounds = false;
	bool visualizeWireframeBounds = false;
	/// Bounds Shape
	bool bounds_sphere = false;
	bool bounds_cube = false;
	bool bounds_cylinder = false;
	bool bounds_cone = false;
	/// Bounds Transform
	Vector4 BS_pos;
	Vector4 BS_rot;
	Quaternion BS_rot_q;
	Vector4 BS_scale;
	bool BS_quat_rot = false;

	/// Particle Properties
	bool particle_3D = true;
	bool billboard = false;
	bool animated = false;

	virtual void OnUIRender() override {

		ImGui::Begin("Particle Properties");
		ImVec2 max = ImVec2(ImGui::GetWindowContentRegionMax().x * 2.0f, ImGui::GetWindowContentRegionMax().y * 2.0f);
		is_active = ImGui::IsMouseHoveringRect(ImGui::GetWindowPos(), max);
		if (is_active)
		{
			if (ImGui::Checkbox("3D Particles", &particle_3D)) {
				billboard = false;
			}
			ImGui::SameLine();
			if (ImGui::Checkbox("Billboard Particles", &billboard)) {
				particle_3D = false;
			}
			ImGui::SameLine();
			ImGui::Separator();
		}

		if (ImGui::TreeNode("Particle System Transform"))
		{
			if (is_active) {
				ImGui::Checkbox("Quaternion Rotation", &PS_quat_rot);
				ImGui::Separator();
			}

			ImGui::Text("Position:\t\tX: Horizontal\tY: Vertical\tZ: Depth");
			ImGui::Separator();
			ImGui::SliderFloat4("Position", PS_pos.val, -25.0f, 25.0f);
			if (!PS_quat_rot)
			{
				ImGui::SliderFloat3("Rotation", PS_rot.val, -25.0f, 25.0f);
			}
			else {
				ImGui::SliderFloat4("Quaternion Rotation", PS_rot_q.val, -25.0f, 25.0f);
			}

			/// TODO: restrict rotation for billbaord
			if (billboard) {

			}


			/// TODO: convert and overwrite the inactive rot values to match selected mode


			ImGui::SliderFloat4("Scale", PS_scale.val, -1.0f, 50.0f);
			ImGui::Separator();
			ImGui::TreePop();
			ImGui::Spacing();
		}

		if (ImGui::TreeNode("Velocity")) {

			ImGui::SliderFloat3("Velocity", velocity.val, -25.0f, 25.0f);


			ImGui::Separator();
			ImGui::TreePop();
			ImGui::Spacing();
		}

		if (ImGui::TreeNode("Bounds")) {

			if (is_active) {
				ImGui::Checkbox("Respect Bounds", &respectBounds);
				ImGui::SameLine();
				ImGui::Checkbox("Wire-Frame Bounds", &visualizeWireframeBounds);
				ImGui::SameLine();
				ImGui::Checkbox("Quaternion Rotation", &BS_quat_rot);
				ImGui::Separator();

				if (ImGui::Checkbox("Bounding Cone", &bounds_cone)) {
					bounds_cube = false;
					bounds_cylinder = false;
					bounds_sphere = false;
				}
				ImGui::SameLine();
				if (ImGui::Checkbox("Bounding Cube", &bounds_cube)) {
					bounds_cone = false;
					bounds_cylinder = false;
					bounds_sphere = false;
				}
				ImGui::SameLine();
				if (ImGui::Checkbox("Bounding Cylinder", &bounds_cylinder)) {
					bounds_cone = false;
					bounds_cube = false;
					bounds_sphere = false;
				}
				ImGui::SameLine();
				if (ImGui::Checkbox("Bounding Sphere", &bounds_sphere)) {
					bounds_cone = false;
					bounds_cube = false;
					bounds_cylinder = false;
				}
				ImGui::Separator();
			}


			if (ImGui::TreeNode("Bounds Transform"))
			{
				ImGui::Text("Position:\t\tX: Horizontal\tY: Vertical\tZ: Depth");
				ImGui::Separator();
				ImGui::SliderFloat4("Position", BS_pos.val, -25.0f, 25.0f);
				if (!BS_quat_rot)
				{
					ImGui::SliderFloat3("Rotation", BS_rot.val, -25.0f, 25.0f);
				}
				else {
					ImGui::SliderFloat4("Quaternion Rotation", BS_rot_q.val, -25.0f, 25.0f);
				}

				/// TODO: restrict rotation for billbaord
				if (billboard) {


				}


				/// TODO: convert and overwrite the inactive rot values to match selected mode


				ImGui::SliderFloat4("Scale", BS_scale.val, -1.0f, 50.0f);
				ImGui::Separator();
				ImGui::TreePop();
				ImGui::Spacing();
			}

			ImGui::TreePop();
			ImGui::Spacing();
		}

		if (particle_3D) {
			if (ImGui::TreeNode("3D Properties")) {

				if (ImGui::TreeNode("Model")) {

					ImGui::Text("Model properties");


					ImGui::TreePop();
					ImGui::Spacing();
				}

				if (animated) {
					if (ImGui::TreeNode("Animation")) {

						ImGui::Text("Animation properties");

						ImGui::TreePop();
						ImGui::Spacing();
					}

				}

				ImGui::TreePop();
				ImGui::Spacing();
			}
		}

		ImGui::End();
	}
};

class ParticleLayer : public Walnut::Layer {
public:
	/// Layer properties
	ImU32 start_left_color = ImGui::GetColorU32(IM_COL32(255, 0, 0, 255));
	ImU32 start_right_color = ImGui::GetColorU32(IM_COL32(0, 0, 255, 255));
	ImU32 end_left_color = ImGui::GetColorU32(IM_COL32(126, 255, 17, 255));
	ImU32 end_right_color = ImGui::GetColorU32(IM_COL32(126, 25, 175, 255));
	ImVec4 colf = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
	char buffer[255] = "*Untitled*";
	bool sceneIsSaved = false;
	bool is_active = false;
	bool opt_enable_grid = true;
	bool opt_enable_context_menu = true;

	DetailsLayer detailsLayer;
	CameraLayer cameraLayer;

	virtual void OnUIRender() override {
		detailsLayer.OnUIRender();
		cameraLayer.OnUIRender();

		ImGui::Begin("Scene");
		ImVec2 max = ImVec2(ImGui::GetWindowContentRegionMax().x * 2.0f, ImGui::GetWindowContentRegionMax().y * 2.0f);
		is_active = ImGui::IsMouseHoveringRect(ImGui::GetWindowPos(), max);
		if (is_active) {

			ImGui::Checkbox("Enable grid", &opt_enable_grid);
			ImGui::Checkbox("Enable context menu", &opt_enable_context_menu);
			ImGui::Text("Mouse Left: drag to add lines,\nMouse Right: drag to scroll, click for context menu.");
			ImGui::Separator();
		}

		ImGui::Separator();

		static ImVector<ImVec2> points;
		static ImVec2 scrolling(0.0f, 0.0f);

		static bool adding_line = false;

		ImGui::InputText("Scene Name", buffer, 255);





		// Typically you would use a BeginChild()/EndChild() pair to benefit from a clipping region + own scrolling.
		// Here we demonstrate that this can be replaced by simple offsetting + custom drawing + PushClipRect/PopClipRect() calls.
		// To use a child window instead we could use, e.g:
		//      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));      // Disable padding
		//      ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(50, 50, 50, 255));  // Set a background color
		//      ImGui::BeginChild("canvas", ImVec2(0.0f, 0.0f), ImGuiChildFlags_Borders, ImGuiWindowFlags_NoMove);
		//      ImGui::PopStyleColor();
		//      ImGui::PopStyleVar();
		//      [...]
		//      ImGui::EndChild();

		// Using InvisibleButton() as a convenience 1) it will advance the layout cursor and 2) allows us to use IsItemHovered()/IsItemActive()
		ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
		ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available
		if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
		if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
		ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

		// Draw border and background color
		ImGuiIO& io = ImGui::GetIO();
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(126, 25, 175, 25));
		draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 0, 0, 255));

		// This will catch our interactions
		ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
		const bool is_hovered = ImGui::IsItemHovered(); // Hovered
		const bool is_active = ImGui::IsItemActive();   // Held
		const ImVec2 origin(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y); // Lock scrolled origin
		const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);



		// Add first and second point
		if (is_hovered && !adding_line && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		{
			points.push_back(mouse_pos_in_canvas);
			points.push_back(mouse_pos_in_canvas);
			adding_line = true;
		}
		if (adding_line)
		{
			points.back() = mouse_pos_in_canvas;
			if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
				adding_line = false;
		}

		// Pan (we use a zero mouse threshold when there's no context menu)
		// You may decide to make that threshold dynamic based on whether the mouse is hovering something etc.
		const float mouse_threshold_for_pan = opt_enable_context_menu ? -1.0f : 0.0f;
		if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, mouse_threshold_for_pan))
		{
			scrolling.x += io.MouseDelta.x;
			scrolling.y += io.MouseDelta.y;
		}

		// Context menu (under default mouse threshold)
		ImVec2 drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
		if (opt_enable_context_menu && drag_delta.x == 0.0f && drag_delta.y == 0.0f)
			ImGui::OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
		if (ImGui::BeginPopup("context"))
		{
			if (adding_line)
				points.resize(points.size() - 2);
			adding_line = false;
			if (ImGui::MenuItem("Remove one", NULL, false, points.Size > 0)) { points.resize(points.size() - 2); }
			if (ImGui::MenuItem("Remove all", NULL, false, points.Size > 0)) { points.clear(); }
			ImGui::EndPopup();
		}

		// Draw grid + all lines in the canvas
		draw_list->PushClipRect(canvas_p0, canvas_p1, true);
		if (opt_enable_grid)
		{
			const float GRID_STEP = 64.0f;
			for (float x = fmodf(scrolling.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
				draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), IM_COL32(200, 200, 200, 40));
			for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
				draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), IM_COL32(200, 200, 200, 40));
		}
		for (int n = 0; n < points.Size; n += 2)
			draw_list->AddLine(ImVec2(origin.x + points[n].x, origin.y + points[n].y), ImVec2(origin.x + points[n + 1].x, origin.y + points[n + 1].y), IM_COL32(255, 255, 0, 255), 2.0f);
		draw_list->PopClipRect();

		ImGui::End();
	}



};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "IuvoParticles";
	spec.Width = 2160;
	spec.Height = 1400;

	Walnut::Application* app = new Walnut::Application(spec);
	//app->PushLayer<ExampleLayer>();
	app->PushLayer<ParticleLayer>();
	app->SetMenubarCallback([app]()
		{
			if (ImGui::BeginMenu("File"))
			{

				if (ImGui::MenuItem("New Scene", "CTRL+N"))
				{
					// TODO: Open New Particle Scene
				}

#pragma region Scene Management

#pragma region Saving and Loading Scenes
				if (ImGui::MenuItem("Save Scene", "CTRL+S"))
				{
					// TODO: Save Particle Scene
				}

				if (ImGui::MenuItem("Load Scene", "CTRL+L"))
				{
					// TODO: Load existing Particle Scene
				}
#pragma endregion

				if (ImGui::MenuItem("Undo", "CTRL+Z"))
				{
					// TODO: Add Undo Callback
				}

				if (ImGui::MenuItem("Redo", "CTRL+Y", false))
				{
					// TODO: Add Redo Callback
				}
#pragma endregion


				if (ImGui::MenuItem("Exit", "CTRL+X"))
				{
					app->Close();
				}

				ImGui::EndMenu();
			}
		});
	return app;
}