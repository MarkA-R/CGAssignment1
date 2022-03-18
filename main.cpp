#include "NOU/App.h" 
#include "NOU/Input.h" 
#include "NOU/Entity.h" 
#include "NOU/CCamera.h" 
#include "NOU/CMeshRenderer.h" 
#include "NOU/Shader.h" 
#include "NOU/GLTFLoader.h" 
#include "Tex3D.h"


//all above inclusions are from NOU
//i am using this in order to create an aesthetically pleasing scene with entities, easier
#include "imgui.h" 
using namespace nou;
void LoadDefaultResources();
//make one shader to use
std::unique_ptr<ShaderProgram> shader;

int shaderToggleInfo = 0;//the data to tell the shader what to toggle 

// Templated LERP function 
template<typename T>
T Lerp(const T& p0, const T& p1, float t)
{
	return (1.0f - t) * p0 + t * p1;
}
std::vector<Entity*> renderingEntities;
int main() { 
	App::Init("CG Assignment 1 - 100782747", 1280, 720);
	App::SetClearColor(glm::vec4(0.611, 0.850, 1,1.f));
	// Initialize ImGui 
	App::InitImgui();

	// Load in our model/texture resources 
	LoadDefaultResources();


	// Create and set up camera 
	Entity cameraEntity = Entity::Create();
	CCamera& cam = cameraEntity.Add<CCamera>(cameraEntity);
	cam.Perspective(60.0f, (float)1280 / 720, 0.001f, 100.0f);
	cameraEntity.transform.m_pos = glm::vec3(-0.7f, -1.2f, -10.7f);
	cameraEntity.transform.m_rotation = glm::angleAxis(glm::radians(0.f), glm::vec3(0.0f, 1.0f, 0.0f));
	


	std::unique_ptr<Texture2D> LUTTexture = std::make_unique<Texture2D>("LUT1.png");//warm
	std::unique_ptr<ShaderProgram> LUTShader = std::make_unique<ShaderProgram>(*shader);
	std::unique_ptr<Material> LUTMat = std::make_unique<Material>(*LUTShader);

	LUTMat->AddTexture("LUT", *LUTTexture);
	
	std::unique_ptr<Texture2D> LUT2Texture = std::make_unique<Texture2D>("LUT2.png");//cool
	std::unique_ptr<ShaderProgram> LUT2Shader = std::make_unique<ShaderProgram>(*shader);
	std::unique_ptr<Material> LUT2Mat = std::make_unique<Material>(*LUT2Shader);
	
	LUT2Mat->AddTexture("LUT2", *LUT2Texture);

	std::unique_ptr<Texture2D> LUT3Texture = std::make_unique<Texture2D>("LUT3.png");//custom
	std::unique_ptr<ShaderProgram> LUT3Shader = std::make_unique<ShaderProgram>(*shader);
	std::unique_ptr<Material> LUT3Mat = std::make_unique<Material>(*LUT3Shader);

	LUT3Mat->AddTexture("LUT3", *LUT3Texture);


	std::unique_ptr<Texture2D> tableTexture = std::make_unique<Texture2D>("table.png");
	std::unique_ptr<ShaderProgram> tableShader = std::make_unique<ShaderProgram>(*shader);
	std::unique_ptr<Material> tableMat = std::make_unique<Material>(*tableShader);

	tableMat->AddTexture("table", *tableTexture);

	
	std::unique_ptr<Mesh> backgroundMesh = std::make_unique<Mesh>();

	GLTF::LoadMesh("background.gltf", *backgroundMesh);
	std::unique_ptr<Texture2D> backgroundTexture = std::make_unique<Texture2D>("background.png");
	std::unique_ptr<ShaderProgram> backgroundShader = std::make_unique<ShaderProgram>(*shader);
	std::unique_ptr<Material> backgroundMat = std::make_unique<Material>(*backgroundShader);

	backgroundMat->AddTexture("albedo", *backgroundTexture);
	backgroundMat->AddTexture("LUT", *LUTTexture.get());
	backgroundMat->AddTexture("LUT2", *LUT2Texture.get());
	backgroundMat->AddTexture("LUT3", *LUT3Texture.get());
	backgroundMat->AddTexture("table", *tableTexture.get());

	std::unique_ptr<Mesh> mountainMesh = std::make_unique<Mesh>();

	GLTF::LoadMesh("mountain.gltf", *mountainMesh);
	std::unique_ptr<Texture2D> mountainTexture = std::make_unique<Texture2D>("mountain.png");
	std::unique_ptr<ShaderProgram> mountainShader = std::make_unique<ShaderProgram>(*shader);
	std::unique_ptr<Material> mountainMat = std::make_unique<Material>(*mountainShader);

	mountainMat->AddTexture("albedo", *mountainTexture);
	mountainMat->AddTexture("LUT", *LUTTexture.get());
	mountainMat->AddTexture("LUT2", *LUT2Texture.get());
	mountainMat->AddTexture("LUT3", *LUT3Texture.get());
	mountainMat->AddTexture("table", *tableTexture.get());
	
	
	std::unique_ptr<Mesh> chickMesh = std::make_unique<Mesh>();
	
	GLTF::LoadMesh("chicken.gltf", *chickMesh);
	std::unique_ptr<Texture2D> chickTexture = std::make_unique<Texture2D>("chick.png");
	std::unique_ptr<ShaderProgram> chickShader = std::make_unique<ShaderProgram>(*shader);
	std::unique_ptr<Material> chickMat = std::make_unique<Material>(*chickShader);
	
	chickMat->AddTexture("albedo", *chickTexture);
	chickMat->AddTexture("LUT", *LUTTexture.get());
	chickMat->AddTexture("LUT2", *LUT2Texture.get());
	chickMat->AddTexture("LUT3", *LUT3Texture.get());
	chickMat->AddTexture("table", *tableTexture.get());

	
	
	//make duck entity
	Entity chick = Entity::Create();
	chick.Add<CMeshRenderer>(chick, *chickMesh, *chickMat);
	chick.transform.m_scale = glm::vec3(0.105);
	chick.transform.m_rotation = glm::angleAxis(glm::radians(90.f), glm::vec3(0.0f, 1.0f, 0.0f))
		* glm::angleAxis(glm::radians(90.f), glm::vec3(1.0f, 0.0f, 0.0f))
		* glm::angleAxis(glm::radians(0.f), glm::vec3(0.0f, 0.0f, 1.0f));
	chick.transform.m_pos = glm::vec3(1.450, -1.599, -12.464);
	chick.transform.m_pos = glm::vec3(-2.760, -1.599, -12.464);
	renderingEntities.push_back(&chick);

	
	
	
	
	//make background entity
	Entity background = Entity::Create();
	background.Add<CMeshRenderer>(background, *backgroundMesh, *backgroundMat);
	background.transform.m_scale = glm::vec3(0.469);
	background.transform.m_rotation = glm::angleAxis(glm::radians(0.f), glm::vec3(0.0f, 1.0f, 0.0f))
		* glm::angleAxis(glm::radians(0.f), glm::vec3(1.0f, 0.0f, 0.0f))
		* glm::angleAxis(glm::radians(0.f), glm::vec3(0.0f, 0.0f, 1.0f));
	background.transform.m_pos = glm::vec3(-0.700, -2.239, -12.895);
	renderingEntities.push_back(&background);

	//make mountain entity
	Entity mountain = Entity::Create();
	mountain.Add<CMeshRenderer>(mountain, *mountainMesh, *mountainMat);
	mountain.transform.m_scale = glm::vec3(0.990);
	mountain.transform.m_rotation = glm::angleAxis(glm::radians(0.f), glm::vec3(0.0f, 1.0f, 0.0f))
		* glm::angleAxis(glm::radians(0.f), glm::vec3(1.0f, 0.0f, 0.0f))
		* glm::angleAxis(glm::radians(0.f), glm::vec3(0.0f, 0.0f, 1.0f));
	mountain.transform.m_pos = glm::vec3(-0.700, -2.429, -22.575);
	renderingEntities.push_back(&mountain);

	
	App::Tick();
	
	
	float t = 0;
	int mode = 0;
	bool isDiffuseRampOn = false;
	bool isSpecularRampOn = false;
	bool isLUT1On = false;
	bool isLUT2On = false;
	bool isLUT3On = false;
	std::string modeNames[6] = {"Normal","No lighting","Ambient only","Specular only","Ambient + Specular","Toon shading"};
	
	while (!App::IsClosing() && (!Input::GetKeyDown(GLFW_KEY_BACKSPACE) && !Input::GetKeyDown(GLFW_KEY_ESCAPE)))
	{
		App::FrameStart();
		float deltaTime = App::GetDeltaTime();
		cameraEntity.Get<CCamera>().Update();
		cameraEntity.transform.RecomputeGlobal();
		t += deltaTime/5;
		if (t > 1) {
			t = 0;
		}
		
		//bind shader and pass in mode of operation parameters
		shader.get()->Bind();
		shader.get()->SetUniform("mode", mode);
		shader.get()->SetUniform("diffuseRamp", (int)isDiffuseRampOn);
		shader.get()->SetUniform("specularRamp", (int)isSpecularRampOn);
		shader.get()->SetUniform("includeLUT1", (int)isLUT1On);
		shader.get()->SetUniform("includeLUT2", (int)isLUT2On);
		shader.get()->SetUniform("includeLUT3", (int)isLUT3On);
		//pass in camera and light position parameters
		shader.get()->SetUniform("cameraPos", cameraEntity.transform.m_pos);
		shader.get()->SetUniform("lightPos", Lerp(glm::vec3(4.450, -1.599, -12.464), glm::vec3(-5.760, -1.599, -12.464), 1-t));
		

		
		//draw all entities
		for each(Entity* e in renderingEntities) {
			
			
			e->transform.RecomputeGlobal();
			e->Get<CMeshRenderer>().Draw();
		}
		
		std::string name = "Mode: " + modeNames[mode];
		//ImGui GUI
		App::StartImgui();
		ImGui::SetNextWindowPos(ImVec2(0, 800), ImGuiCond_FirstUseEver);
		
		ImGui::Text(name.c_str());
		ImGui::SliderInt("", &(mode),0,5);	
		ImGui::Checkbox("Diffuse ramp: ", &isDiffuseRampOn);
		ImGui::Checkbox("Specular ramp: ", &isSpecularRampOn);
		ImGui::Checkbox("Warm colour LUT: ", &isLUT1On);
		ImGui::Checkbox("Cool colour LUT: ", &isLUT2On);
		ImGui::Checkbox("Custom colour LUT: ", &isLUT3On);
		
		App::EndImgui();
		//LERP the duck
		chick.transform.m_pos = Lerp(glm::vec3(1.450, -1.599, -12.464), glm::vec3(-2.760, -1.599, -12.464), t);
		
		App::SwapBuffers();
	}
	App::Cleanup();

	return 0;
} 

void LoadDefaultResources() {
	// Load in the shaders we will need and activate them 

	// Textured lit shader 
	std::unique_ptr vs_Shader = std::make_unique<Shader>("shaderVS.vert", GL_VERTEX_SHADER);
	std::unique_ptr fs_Shader = std::make_unique<Shader>("shaderFS.glsl", GL_FRAGMENT_SHADER);
	std::vector<Shader*> texLit = { vs_Shader.get(), fs_Shader.get() };
	shader = std::make_unique<ShaderProgram>(texLit);
}
