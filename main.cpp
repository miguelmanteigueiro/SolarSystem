// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <Windows.h>
using namespace std;

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
using namespace glm;

bool jogo = false;
#include "common/shader.hpp"
#include "common/texture.hpp" 
#include "common/controls.hpp" 
#include "common/objloader.hpp"
#include "common/shader.cpp"
#include "common/texture.cpp" 
#include "common/controls.cpp" 
#include "common/objloader.cpp"


//Struct de translação
struct Astro
{
	float transX;
	float transY;
	float transZ;

	float posX; //Posição no eixo dos x
	float posY; //Posição no eixo dos y
	float posZ; //Posição no eixo dos z  

	float rotationSpeed; //Velocidade de rotação/
	float currentRotation; //Ângulo atual
	float selfRotationZ; //Rotação específica de alguns planetas no eixo do z
	float selfRotationX; //Rotação específica de alguns planetas no eixo do x
	float translationSpeed; //Velocidade de translação

	float radius; //Distância ao Sol

	float scale; // size of the planet
};

//Struct e variáveis de Rotação
struct Rotation
{
	int rotX;
	int rotY;
	int rotZ;
};
float ang = 0.0f;

//Variáveis de escala
float UA = 25.0f;//Variável padrão da distância ao Sol(Unidade Astronomica)
float TP = 0.5f;//Tamanho padrão dos planetas para a escala;
float DB = 2.0f;//Distância bonita com mercurio como base 
float TB = 0.5f / 3;//Tamanho padrão dos planetas para a escala;
float RP = 1.0f / 10; //Velocidade padrão de rotação dos planetas (Terra como padrão)
float VP = 0.1f; // Velocidade padrão dos planetas(Terra como padrão)

//booleans
bool EscalaReal = false;
bool DevemMover = false;

#include "include/transferDataToGPU.hpp"
#include "include/draw.hpp"


//Buffers to draw the sphere object. They can be the same to all spherical objects
GLuint VertexArrayID;
GLuint MatrixID;
GLuint uvbuffer;
GLuint rings_uvbuffer;
GLuint vertexbuffer;
GLuint rings_vertexbuffer;
// PANEL
GLuint panel_vertexbuffer;
GLuint panel_uvbuffer;
// MONKE
GLuint monke_vertexbuffer;
GLuint monke_uvbuffer;

std::vector<glm::vec3> vertices;
std::vector<glm::vec2> uvs;
std::vector<glm::vec3> normals;
std::vector<glm::vec3> rings_vertices;
std::vector<glm::vec2> rings_uvs;
std::vector<glm::vec3> rings_normals;
// PANEL
std::vector<glm::vec3> panel_vertices;
std::vector<glm::vec2> panel_uvs;
std::vector<glm::vec3> panel_normals;
// MONKE
std::vector<glm::vec3> monke_vertices;
std::vector<glm::vec2> monke_uvs;
std::vector<glm::vec3> monke_normals;



//Buffers to apply the different textures to the objects, they need to be different! Or else it will only draw one object the last texture
GLuint SunTexture;
GLuint SunTextureID;
GLuint EarthTexture;
GLuint EarthTextureID;
GLuint MercuryTexture;
GLuint MercuryTextureID;
GLuint VenusTexture;
GLuint VenusTextureID;
GLuint MarsTexture;
GLuint MarsTextureID;
GLuint JupiterTexture;
GLuint JupiterTextureID;
GLuint SaturnTexture;
GLuint SaturnTextureID;
GLuint UranusTexture;
GLuint UranusTextureID;
GLuint NeptuneTexture;
GLuint NeptuneTextureID;
GLuint MoonTexture;
GLuint MoonTextureID;
GLuint HUDTextureID;
GLuint HUDTexture;
GLuint MenuTextureID;
GLuint MenuTexture;

GLuint SpaceTexture;
GLuint SpaceTextureID;
GLuint SaturnRingsTexture;
GLuint SaturnRingsTextureID;
GLuint UranusRingsTexture;
GLuint UranusRingsTextureID;
// PANEL
GLuint PanelSunTexture;
GLuint PanelSunTextureID;
GLuint PanelMercuryTexture;
GLuint PanelMercuryTextureID;
GLuint PanelVenusTexture;
GLuint PanelVenusTextureID;
GLuint PanelEarthTexture;
GLuint PanelEarthTextureID;
GLuint PanelMarsTexture;
GLuint PanelMarsTextureID;
GLuint PanelJupiterTexture;
GLuint PanelJupiterTextureID;
GLuint PanelSaturnTexture;
GLuint PanelSaturnTextureID;
GLuint PanelUranusTexture;
GLuint PanelUranusTextureID;
GLuint PanelNeptuneTexture;
GLuint PanelNeptuneTextureID;
GLuint MonkeTexture;
GLuint MonkeTextureID;

//Creates the textures
const char* Sun_texture = "resources/textures/sun.DDS";
const char* Earth_texture = "resources/textures/earth_texture.DDS";
const char* Mercury_texture = "resources/textures/mercury_texture.DDS";
const char* Venus_texture = "resources/textures/venus_texture.DDS";
const char* Mars_texture = "resources/textures/mars_texture.DDS";
const char* Jupiter_texture = "resources/textures/jupiter_texture.DDS";
const char* Saturn_texture = "resources/textures/saturn_texture.DDS";
const char* Uranus_texture = "resources/textures/uranus_texture.DDS";
const char* Neptune_texture = "resources/textures/neptune_texture.DDS";
const char* Space_texture = "resources/textures/stars_milky_way.DDS";
const char* Moon_texture = "resources/textures/moon_texture.DDS";
const char* HUD_texture = "resources/textures/HUD.DDS";
const char* Menu_texture = "resources/textures/Menu.DDS";
const char* Saturn_Rings_texture = "resources/textures/saturn_rings.DDS";
const char* Uranus_Rings_texture = "resources/textures/uranus_rings.DDS";
// PANEL
const char* PanelSun_texture = "resources/textures/Menus/PlaneSun.DDS";
const char* PanelMercury_texture = "resources/textures/Menus/PlaneMercury.DDS";
const char* PanelVenus_texture = "resources/textures/Menus/PlaneVenus.DDS";
const char* PanelEarth_texture = "resources/textures/Menus/PlaneEarth.DDS";
const char* PanelMars_texture = "resources/textures/Menus/PlaneMars.DDS";
const char* PanelJupiter_texture = "resources/textures/Menus/PlaneJupiter.DDS";
const char* PanelSaturn_texture = "resources/textures/Menus/PlaneSaturn.DDS";
const char* PanelUranus_texture = "resources/textures/Menus/PlaneUranus.DDS";
const char* PanelNeptune_texture = "resources/textures/Menus/PlaneNeptune.DDS";
const char* monke_texture = "resources/textures/Menu.DDS";

//Creates the objects
const char* object = "resources/objects/sphere.obj";
const char* object2 = "resources/objects/sphere_inverted.obj";
const char* rings_obj = "resources/objects/ring.obj";
// PANEL
const char* panel_obj = "resources/objects/panel.obj";
const char* monke_obj = "resources/objects/monkey.obj";


//Define as translações
Astro Space;
Astro Sun;
Astro Earth;
Astro EarthPlane;
Astro Mercury;
Astro Venus;
Astro Mars;
Astro Saturn;
Astro Jupiter;
Astro Uranus;
Astro Neptune;
Astro Moon;
Astro HUD;
Astro Monke;
Astro Saturn_Rings;
Astro Uranus_Rings;
// PANEL
Astro PanelSun;
Astro PanelMercury;
Astro PanelVenus;
Astro PanelEarth;
Astro PanelMars;
Astro PanelJupiter;
Astro PanelSaturn;
Astro PanelUranus;
Astro PanelNeptune;


//Variáveis de rotações
Rotation Rpl1;

//HUD buffers
GLuint HUD_VertexArrayID2D;
GLuint HUD_vertexbuffer2D;
GLuint HUD_colorbuffer2D;
GLuint HUD_2DTexture, HUD_2DTextureID;

//Menu buffers
GLuint Menu_VertexArrayID2D;
GLuint Menu_vertexbuffer2D;
GLuint Menu_colorbuffer2D;
GLuint Menu_2DTexture, Menu_2DTextureID;


//Cabeçalho das funções
void definirPropriedadesReais();
void rotacoes();
void posicaoPlanetas();
void definirPropriedadesBonitas();
void definirPropsIniciais();
void translacoesPlanetas();


int main(void)
{
	//initialization-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1024 * 1.2, 768 * 1.2, "The Solar System", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//Centra a janela---------------------------------------------------------------------------------------------------------
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	int monitorX, monitorY;
	glfwGetMonitorPos(glfwGetPrimaryMonitor(), &monitorX, &monitorY);
	//get window size
	int windowWidth, windowHeight;
	glfwGetWindowSize(window, &windowWidth, &windowHeight);

	//center window
	glfwSetWindowPos(window, monitorX + (mode->width - windowWidth) / 2, monitorY + (mode->height - windowHeight) / 2);
	//------------------------------------------------------------------------------------------------------------------------

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Hide the mouse and enable unlimited mouvement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set the mouse at the center of the screen
	glfwPollEvents();
	glfwSetCursorPos(window, 1024 / 2, 768 / 2);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	// Create and compile our GLSL program from the shaders
	GLuint programID2D = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");
	transferDataToGPU2D_HUD(&HUD_VertexArrayID2D, programID2D, &HUD_vertexbuffer2D, &HUD_colorbuffer2D, &HUD_2DTexture, &HUD_2DTextureID, HUD_texture);
	transferDataToGPU2D_Menu(&Menu_VertexArrayID2D, programID2D, &Menu_vertexbuffer2D, &Menu_colorbuffer2D, &Menu_2DTexture, &Menu_2DTextureID, Menu_texture);

	GLuint programID = LoadShaders("shaders/TransformVertexShader.vertexshader", "shaders/TextureFragmentShader.fragmentshader");


	// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


	//Transfer data to gpu
	transferDataToGPU(&VertexArrayID, programID, Space_texture, object2, MatrixID, &SpaceTexture, &SpaceTextureID, &vertexbuffer, &uvbuffer, &vertices, &uvs, &normals);
	transferDataToGPU(&VertexArrayID, programID, Sun_texture, object, MatrixID, &SunTexture, &SunTextureID, &vertexbuffer, &uvbuffer, &vertices, &uvs, &normals);
	transferDataToGPU(&VertexArrayID, programID, Earth_texture, object, MatrixID, &EarthTexture, &EarthTextureID, &vertexbuffer, &uvbuffer, &vertices, &uvs, &normals);
	transferDataToGPU(&VertexArrayID, programID, Mercury_texture, object, MatrixID, &MercuryTexture, &MercuryTextureID, &vertexbuffer, &uvbuffer, &vertices, &uvs, &normals);
	transferDataToGPU(&VertexArrayID, programID, Venus_texture, object, MatrixID, &VenusTexture, &VenusTextureID, &vertexbuffer, &uvbuffer, &vertices, &uvs, &normals);
	transferDataToGPU(&VertexArrayID, programID, Mars_texture, object, MatrixID, &MarsTexture, &MarsTextureID, &vertexbuffer, &uvbuffer, &vertices, &uvs, &normals);
	transferDataToGPU(&VertexArrayID, programID, Jupiter_texture, object, MatrixID, &JupiterTexture, &JupiterTextureID, &vertexbuffer, &uvbuffer, &vertices, &uvs, &normals);
	transferDataToGPU(&VertexArrayID, programID, Saturn_texture, object, MatrixID, &SaturnTexture, &SaturnTextureID, &vertexbuffer, &uvbuffer, &vertices, &uvs, &normals);
	transferDataToGPU(&VertexArrayID, programID, Uranus_texture, object, MatrixID, &UranusTexture, &UranusTextureID, &vertexbuffer, &uvbuffer, &vertices, &uvs, &normals);
	transferDataToGPU(&VertexArrayID, programID, Neptune_texture, object, MatrixID, &NeptuneTexture, &NeptuneTextureID, &vertexbuffer, &uvbuffer, &vertices, &uvs, &normals);
	transferDataToGPU(&VertexArrayID, programID, Moon_texture, object, MatrixID, &MoonTexture, &MoonTextureID, &vertexbuffer, &uvbuffer, &vertices, &uvs, &normals);
	transferDataToGPU(&VertexArrayID, programID, Saturn_Rings_texture, rings_obj, MatrixID, &SaturnRingsTexture, &SaturnRingsTextureID, &rings_vertexbuffer, &rings_uvbuffer, &rings_vertices, &rings_uvs, &rings_normals);
	transferDataToGPU(&VertexArrayID, programID, Uranus_Rings_texture, rings_obj, MatrixID, &UranusRingsTexture, &UranusRingsTextureID, &rings_vertexbuffer, &rings_uvbuffer, &rings_vertices, &rings_uvs, &rings_normals);

	transferDataToGPU(&VertexArrayID, programID, PanelSun_texture, panel_obj, MatrixID, &PanelSunTexture, &PanelSunTextureID, &panel_vertexbuffer, &panel_uvbuffer, &panel_vertices, &panel_uvs, &panel_normals);
	transferDataToGPU(&VertexArrayID, programID, PanelMercury_texture, panel_obj, MatrixID, &PanelMercuryTexture, &PanelMercuryTextureID, &panel_vertexbuffer, &panel_uvbuffer, &panel_vertices, &panel_uvs, &panel_normals);
	transferDataToGPU(&VertexArrayID, programID, PanelVenus_texture, panel_obj, MatrixID, &PanelVenusTexture, &PanelVenusTextureID, &panel_vertexbuffer, &panel_uvbuffer, &panel_vertices, &panel_uvs, &panel_normals);
	transferDataToGPU(&VertexArrayID, programID, PanelEarth_texture, panel_obj, MatrixID, &PanelEarthTexture, &PanelEarthTextureID, &panel_vertexbuffer, &panel_uvbuffer, &panel_vertices, &panel_uvs, &panel_normals);
	transferDataToGPU(&VertexArrayID, programID, PanelMars_texture, panel_obj, MatrixID, &PanelMarsTexture, &PanelMarsTextureID, &panel_vertexbuffer, &panel_uvbuffer, &panel_vertices, &panel_uvs, &panel_normals);
	transferDataToGPU(&VertexArrayID, programID, PanelJupiter_texture, panel_obj, MatrixID, &PanelJupiterTexture, &PanelJupiterTextureID, &panel_vertexbuffer, &panel_uvbuffer, &panel_vertices, &panel_uvs, &panel_normals);
	transferDataToGPU(&VertexArrayID, programID, PanelSaturn_texture, panel_obj, MatrixID, &PanelSaturnTexture, &PanelSaturnTextureID, &panel_vertexbuffer, &panel_uvbuffer, &panel_vertices, &panel_uvs, &panel_normals);
	transferDataToGPU(&VertexArrayID, programID, PanelUranus_texture, panel_obj, MatrixID, &PanelUranusTexture, &PanelUranusTextureID, &panel_vertexbuffer, &panel_uvbuffer, &panel_vertices, &panel_uvs, &panel_normals);
	transferDataToGPU(&VertexArrayID, programID, PanelNeptune_texture, panel_obj, MatrixID, &PanelNeptuneTexture, &PanelNeptuneTextureID, &panel_vertexbuffer, &panel_uvbuffer, &panel_vertices, &panel_uvs, &panel_normals);
	transferDataToGPU(&VertexArrayID, programID, monke_texture, monke_obj, MatrixID, &MonkeTexture, &MonkeTextureID, &monke_vertexbuffer, &monke_uvbuffer, &monke_vertices, &monke_uvs, &monke_normals);

	transferDataToGPU(&VertexArrayID, programID, HUD_texture, object, MatrixID, &HUDTexture, &HUDTextureID, &vertexbuffer, &uvbuffer, &vertices, &uvs, &normals);
	transferDataToGPU(&VertexArrayID, programID, Menu_texture, object, MatrixID, &MenuTexture, &MenuTextureID, &vertexbuffer, &uvbuffer, &vertices, &uvs, &normals);


	//Eixos de rotação normais (Maior parte dos planetas roda assim)
	Rpl1.rotX = 0;
	Rpl1.rotY = 1;
	Rpl1.rotZ = 0;

	definirPropsIniciais();

	int menuSongShouldPlay = 0;
	int gameSongShouldPlay = 0;
	bool monke = false;

	do {
		if (!jogo) {
			// Music on the background
			if (menuSongShouldPlay == 0) {
				bool played = PlaySound(TEXT("resources/sounds/MenuSong.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
				cout << "Success or Not:" << played << endl;
				menuSongShouldPlay++;
			}

			//Menu principal---------------------------------------------------------------------------------------------------------------------
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//Desenha o menu
			draw2D(Menu_vertexbuffer2D, Menu_colorbuffer2D, programID2D);
			draw(programID, MatrixID, MenuTexture, MenuTextureID, &vertexbuffer, &uvbuffer, GL_TRIANGLES, &vertices, HUD, Rpl1);

			//Define as propriedades dos corpos celestes----------------------------------------------------------------------
			if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE) {
				jogo = !jogo;
			}

			//Disable mouse movement
			float speed = 0.0f; // 3 units / second
			float mouseSpeed = 0.000f;

			// Swap buffers
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
		else {
			// Music on the background
			if (gameSongShouldPlay == 0){
				bool played = PlaySound(TEXT("resources/sounds/GameSong.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
				cout << "Success or Not:" << played << endl;
				gameSongShouldPlay++;
			}
			//Dentro do sistema solar-------------------------------------------------------------------------------------------------------------
			// Clear the screen
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//Enable mouse movement
			float speed = 3.0f; // 3 units / second
			float mouseSpeed = 0.005f;

			//Define as propriedades dos corpos celestes----------------------------------------------------------------------
			if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE) {
				EscalaReal = !EscalaReal;
			}

			if (EscalaReal) {

				definirPropriedadesReais();
			}
			else {
				definirPropriedadesBonitas();
			}
			//----------------------------------------------------------------------------------------------------------------



			//translação dos planetas

			posicaoPlanetas();
			if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
				DevemMover = !DevemMover;
			}
			if (DevemMover) {
				translacoesPlanetas();
			}

			if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE) {
				DevemMover = false;
				definirPropsIniciais();
			}

			

			//d2d();
			draw2D(HUD_vertexbuffer2D, HUD_colorbuffer2D, programID2D);
			//Universo
			draw(programID, MatrixID, SpaceTexture, SpaceTextureID, &vertexbuffer, &uvbuffer, GL_TRIANGLES, &vertices, Space, Rpl1);

			//Sol
			draw(programID, MatrixID, SunTexture, SunTextureID, &vertexbuffer, &uvbuffer, GL_TRIANGLES, &vertices, Sun, Rpl1);
			if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
				EscalaReal = false;
				DevemMover = false;
				definirPropsIniciais();
				// PANEL SUN
				draw(programID, MatrixID, PanelSunTexture, PanelSunTextureID, &panel_vertexbuffer, &panel_uvbuffer, GL_TRIANGLES, &panel_vertices, PanelSun, Rpl1);
			}

			//Mercúrio-----------------------------------------------------------------------------------------------------------------------------------------
			draw(programID, MatrixID, MercuryTexture, MercuryTextureID, &vertexbuffer, &uvbuffer, GL_TRIANGLES, &vertices, Mercury, Rpl1);
			if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
				EscalaReal = false;
				DevemMover = false;
				definirPropsIniciais();
				// PANEL MERCURY
				draw(programID, MatrixID, PanelMercuryTexture, PanelMercuryTextureID, &panel_vertexbuffer, &panel_uvbuffer, GL_TRIANGLES, &panel_vertices, PanelMercury, Rpl1);
			}

			//Vénus-----------------------------------------------------------------------------------------------------------------------------------------
			draw(programID, MatrixID, VenusTexture, VenusTextureID, &vertexbuffer, &uvbuffer, GL_TRIANGLES, &vertices, Venus, Rpl1);
			if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
				EscalaReal = false;
				DevemMover = false;
				definirPropsIniciais();
				// PANEL VENUS
				draw(programID, MatrixID, PanelVenusTexture, PanelVenusTextureID, &panel_vertexbuffer, &panel_uvbuffer, GL_TRIANGLES, &panel_vertices, PanelVenus, Rpl1);
			}

			//Terra-----------------------------------------------------------------------------------------------------------------------------------------
			draw(programID, MatrixID, EarthTexture, EarthTextureID, &vertexbuffer, &uvbuffer, GL_TRIANGLES, &vertices, Earth, Rpl1);
			if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
				EscalaReal = false;
				DevemMover = false;
				definirPropsIniciais();
				// PANEL EARTH
				draw(programID, MatrixID, PanelEarthTexture, PanelEarthTextureID, &panel_vertexbuffer, &panel_uvbuffer, GL_TRIANGLES, &panel_vertices, PanelEarth, Rpl1);
			}
			//Easter egg
			draw(programID, MatrixID, EarthTexture, EarthTextureID, &panel_vertexbuffer, &panel_uvbuffer, GL_TRIANGLES, &panel_vertices, EarthPlane, Rpl1);

			//Moon-----------------------------------------------------------------------------------------------------------------------------------------
			draw(programID, MatrixID, MoonTexture, MoonTextureID, &vertexbuffer, &uvbuffer, GL_TRIANGLES, &vertices, Moon, Rpl1);

			//Marte-----------------------------------------------------------------------------------------------------------------------------------------
			draw(programID, MatrixID, MarsTexture, MarsTextureID, &vertexbuffer, &uvbuffer, GL_TRIANGLES, &vertices, Mars, Rpl1);
			if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
				EscalaReal = false;
				DevemMover = false;
				definirPropsIniciais();
				// PANEL MARS
				draw(programID, MatrixID, PanelMarsTexture, PanelMarsTextureID, &panel_vertexbuffer, &panel_uvbuffer, GL_TRIANGLES, &panel_vertices, PanelMars, Rpl1);
			}
			

			//Jupiter-----------------------------------------------------------------------------------------------------------------------------------------
			draw(programID, MatrixID, JupiterTexture, JupiterTextureID, &vertexbuffer, &uvbuffer, GL_TRIANGLES, &vertices, Jupiter, Rpl1);
			if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
				EscalaReal = false;
				DevemMover = false;
				definirPropsIniciais();
				// PANEL JUPITER
				draw(programID, MatrixID, PanelJupiterTexture, PanelJupiterTextureID, &panel_vertexbuffer, &panel_uvbuffer, GL_TRIANGLES, &panel_vertices, PanelJupiter, Rpl1);
			}

			//????
			if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_M) == GLFW_RELEASE) {
				monke = !monke;
			}
			if (monke) {
				draw(programID, MatrixID, MonkeTexture, MonkeTextureID, &monke_vertexbuffer, &monke_uvbuffer, GL_TRIANGLES, &monke_vertices, Monke, Rpl1);
			}

			//Saturno-----------------------------------------------------------------------------------------------------------------------------------------
			draw(programID, MatrixID, SaturnTexture, SaturnTextureID, &vertexbuffer, &uvbuffer, GL_TRIANGLES, &vertices, Saturn, Rpl1);
			//Anéis de Saturno
			draw(programID, MatrixID, SaturnRingsTexture, SaturnRingsTextureID, &rings_vertexbuffer, &rings_uvbuffer, GL_TRIANGLES, &rings_vertices, Saturn_Rings, Rpl1);
			if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
				EscalaReal = false;
				DevemMover = false;
				definirPropsIniciais();
				// PANEL SATURN
				draw(programID, MatrixID, PanelSaturnTexture, PanelSaturnTextureID, &panel_vertexbuffer, &panel_uvbuffer, GL_TRIANGLES, &panel_vertices, PanelSaturn, Rpl1);
			}
			

			//Urano-----------------------------------------------------------------------------------------------------------------------------------------
			draw(programID, MatrixID, UranusTexture, UranusTextureID, &vertexbuffer, &uvbuffer, GL_TRIANGLES, &vertices, Uranus, Rpl1);
			//Anéis de Urano
			draw(programID, MatrixID, UranusRingsTexture, UranusRingsTextureID, &rings_vertexbuffer, &rings_uvbuffer, GL_TRIANGLES, &rings_vertices, Uranus_Rings, Rpl1);
			if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) {
				EscalaReal = false;
				DevemMover = false;
				definirPropsIniciais();
				// PANEL URANUS
				draw(programID, MatrixID, PanelUranusTexture, PanelUranusTextureID, &panel_vertexbuffer, &panel_uvbuffer, GL_TRIANGLES, &panel_vertices, PanelUranus, Rpl1);
			}
			

			//Neptuno-----------------------------------------------------------------------------------------------------------------------------------------
			draw(programID, MatrixID, NeptuneTexture, NeptuneTextureID, &vertexbuffer, &uvbuffer, GL_TRIANGLES, &vertices, Neptune, Rpl1);
			if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) {
				EscalaReal = false;
				DevemMover = false;
				definirPropsIniciais();
				// PANEL NEPTUNE
				draw(programID, MatrixID, PanelNeptuneTexture, PanelNeptuneTextureID, &panel_vertexbuffer, &panel_uvbuffer, GL_TRIANGLES, &panel_vertices, PanelNeptune, Rpl1);
			}
			

			//HUD-----------------------------------------------------------------------------------------------------------------------------------------
			draw(programID, MatrixID, HUDTexture, HUDTextureID, &vertexbuffer, &uvbuffer, GL_TRIANGLES, &vertices, HUD, Rpl1);



			//Define as velocidades das rotações em torno do seu eixo
			rotacoes();
			//----------------------------------------------------------------------------------------------------------------------------------------------

			// Swap buffers
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

//Vai inicializar cada um dos structs dos planetas com as suas respetivas propriedades
void definirPropriedadesReais() {

	//Proprieades do sol

	Sun.rotationSpeed = 0.01f;
	Sun.radius = 0.0f;
	Sun.scale = TP * 15.0f;


	//Propriedades de Mercurio

	Mercury.rotationSpeed = RP * 0.01705f;
	Mercury.radius = UA * 0.38f;
	Mercury.scale = TP * 0.382;
	Mercury.translationSpeed = VP * 4.1477f;

	//Propriedades de Vénus

	Venus.rotationSpeed = RP * 0.00412f;
	Venus.radius = UA * 0.72f;
	Venus.scale = TP * 0.948;
	Venus.translationSpeed = VP * 1.622f;

	//Propriedades da Terra

	Earth.rotationSpeed = RP;
	Earth.radius = UA;
	Earth.scale = TP;
	Earth.translationSpeed = VP;

	EarthPlane.rotationSpeed = RP;
	EarthPlane.radius = UA;
	EarthPlane.scale = TP * 0.35f;
	EarthPlane.translationSpeed = VP;

	//Propriedades da Lua

	Moon.rotationSpeed = RP;
	Moon.radius = UA * 0.2f * 0.2f;
	Moon.scale = TP * 0.27;
	Moon.translationSpeed = VP * 2;

	//Propriedades de Marte

	Mars.rotationSpeed = RP * 0.96f;
	Mars.radius = UA * 1.52f;
	Mars.scale = TP * 0.532;
	Mars.translationSpeed = VP * 0.5313f;

	//Propriedades de Júpiter

	Jupiter.rotationSpeed = RP * 2.4f;
	Jupiter.radius = UA * 5.21f;
	Jupiter.scale = TP * 11.208;
	Jupiter.translationSpeed = VP * 0.0842f;

	//Propriedades de Saturno

	Saturn.rotationSpeed = RP * 2.182f;
	Saturn.radius = UA * 9.54f;
	Saturn.scale = TP * 9.449;
	Saturn.translationSpeed = VP * 0.034f;

	//Propriedades dos anéis de Saturno

	Saturn_Rings.rotationSpeed = RP * 2.182f;
	Saturn_Rings.radius = UA * 9.54f;
	Saturn_Rings.scale = TP * 13.449;
	Saturn_Rings.translationSpeed = VP * 0.034f;

	//Propriedades de Urano

	Uranus.rotationSpeed = RP * 1.4117f;
	Uranus.radius = UA * 19.18f;
	Uranus.scale = TP * 4.007;
	Uranus.translationSpeed = VP * 0.012f;

	//Propriedades dos anéis Urano

	Uranus_Rings.rotationSpeed = RP * 1.4117f;
	Uranus_Rings.radius = UA * 19.18f;
	Uranus_Rings.scale = TP * 7.007;
	Uranus_Rings.translationSpeed = VP * 0.012f;

	//Propriedades de Neptuno

	Neptune.rotationSpeed = RP * 1.5f;
	Neptune.radius = UA * 28.11f;
	Neptune.scale = TP * 3.882;
	Neptune.translationSpeed = VP * 0.0061f;

	// Space

	Space.transX = 0.0f;
	Space.transY = 0.0f;
	Space.transZ = 0.0f;
	Space.posX;
	Space.posY = -2.0f;
	Space.posZ;
	Space.rotationSpeed = RP * 0.01f;
	Space.radius = UA * 30.11f;
	Space.scale = TP * 1500.0f;

	// HUD

	HUD.transX = 0.0f;
	HUD.transY = 0.0f;
	HUD.transZ = 0.0f;
	HUD.posX;
	HUD.posY = -2.0f;
	HUD.posZ;
	HUD.rotationSpeed = RP * 0.01f;
	HUD.radius = UA * 0.11f;
	HUD.scale = TP * 0.0001f;

	// ???

	Monke.transX = 0.0f;
	Monke.transY = 0.0f;
	Monke.transZ = 0.0f;
	Monke.posX;
	Monke.posY = 8.0f;
	Monke.posZ;
	Monke.rotationSpeed = RP * 0.00f;
	Monke.radius = UA * 0.00f;
	Monke.scale = TP * 0.91f;
}

void definirPropriedadesBonitas() {
	//Proprieades do sol

	Sun.rotationSpeed = 0.01f;
	Sun.radius = 0.0f;
	Sun.scale = TB * 8;


	//Propriedades de Mercurio

	Mercury.rotationSpeed = RP * 0.01705f * 20;
	Mercury.radius = DB;
	Mercury.scale = TB * 0.382;
	Mercury.translationSpeed = VP * 4.1477f;

	//Propriedades de Vénus

	Venus.rotationSpeed = RP * 0.00412f * 20;
	Venus.radius = DB * 1.86f;
	Venus.scale = TB * 0.948f;
	Venus.translationSpeed = VP * 1.622f;

	//Propriedades da Terra

	Earth.rotationSpeed = RP;
	Earth.radius = DB * 2.586f;
	Earth.scale = TB;
	Earth.translationSpeed = VP;

	EarthPlane.rotationSpeed = RP;
	EarthPlane.radius = UA;
	EarthPlane.scale = TB * 0.35f;
	EarthPlane.translationSpeed = VP;

	//Propriedades da Lua

	Moon.rotationSpeed = RP;
	Moon.radius = DB * 0.2f;
	Moon.scale = TB * 0.27;
	Moon.translationSpeed = VP * 2;

	//Propriedades de Marte

	Mars.rotationSpeed = RP * 0.96f;
	Mars.radius = DB * 3.931f;
	Mars.scale = TB * 0.532f;
	Mars.translationSpeed = VP * 0.5313f;

	//Propriedades de Júpiter

	Jupiter.rotationSpeed = RP * 2.4f;
	Jupiter.radius = DB * 7.414f;
	Jupiter.scale = TB * 4.208f;
	Jupiter.translationSpeed = VP * 5 * 0.0842f;

	//Propriedades de Saturno

	Saturn.rotationSpeed = RP * 2.182f;
	Saturn.radius = DB * 9.638f;
	Saturn.scale = TB * 3.449f;
	Saturn.translationSpeed = VP * 6 * 0.034f;

	//Propriedades dos anéis de Saturno

	Saturn_Rings.rotationSpeed = RP * 2.182f;
	Saturn_Rings.radius = DB * 9.638f;
	Saturn_Rings.scale = TB * 5.449f;
	Saturn_Rings.translationSpeed = VP * 6 * 0.034f;

	//Propriedades de Urano

	Uranus.rotationSpeed = RP * 1.4117f;
	Uranus.radius = DB * 11.5f;
	Uranus.scale = TB * 2.407f;
	Uranus.translationSpeed = VP * 8 * 0.012f;

	//Propriedades dos anéis de Urano

	Uranus_Rings.rotationSpeed = RP * 1.4117f;
	Uranus_Rings.radius = DB * 11.5f;
	Uranus_Rings.scale = TB * 4.407f;
	Uranus_Rings.translationSpeed = VP * 8 * 0.012f;

	//Propriedades de Neptuno

	Neptune.rotationSpeed = RP * 1.5f;
	Neptune.radius = DB * 13.9655f;
	Neptune.scale = TB * 1.582f;
	Neptune.translationSpeed = VP * 10 * 0.0061f;

	// Space

	Space.transX = 0.0f;
	Space.transY = 0.0f;
	Space.transZ = 0.0f;
	Space.posX;
	Space.posY = -2.0f;
	Space.posZ;
	Space.rotationSpeed = RP * 0.01f;
	Space.radius = UA * 30.11f;
	Space.scale = TP * 1500.0f;

	// HUD

	HUD.transX = 0.0f;
	HUD.transY = 0.0f;
	HUD.transZ = 0.0f;
	HUD.posX;
	HUD.posY = -2.0f;
	HUD.posZ;
	HUD.rotationSpeed = RP * 0.01f;
	HUD.radius = UA * 0.11f;
	HUD.scale = TP * 0.0000f;

	// PANEL SUN

	PanelSun.transX = 0.0f;
	PanelSun.transY = 0.0f;
	PanelSun.transZ = 0.0f;
	PanelSun.posX = -2.8f; // comprimento
	PanelSun.posY = -2.0f; // altura
	PanelSun.posZ = 0.0f; // largura
	PanelSun.rotationSpeed = RP * 0.01f;
	PanelSun.radius = 0.0f;
	PanelSun.scale = 0.5f;

	// PANEL MERCURY

	PanelMercury.transX = 0.0f;
	PanelMercury.transY = 0.0f;
	PanelMercury.transZ = 0.0f;
	PanelMercury.posX = 1.7f; // comprimento
	PanelMercury.posY = -2.0f; // altura
	PanelMercury.posZ = 0.0f; // largura
	PanelMercury.rotationSpeed = RP * 0.01f;
	PanelMercury.radius = 0.0f;
	PanelMercury.scale = 0.1f;

	// PANEL VENUS

	PanelVenus.transX = 0.0f;
	PanelVenus.transY = 0.0f;
	PanelVenus.transZ = 0.0f;
	PanelVenus.posX = 3.0f; // comprimento
	PanelVenus.posY = -2.0f; // altura
	PanelVenus.posZ = 0.0f; // largura
	PanelVenus.rotationSpeed = RP * 0.01f;
	PanelVenus.radius = 0.0f;
	PanelVenus.scale = 0.2f;

	// PANEL EARTH

	PanelEarth.transX = 0.0f;
	PanelEarth.transY = 0.0f;
	PanelEarth.transZ = 0.0f;
	PanelEarth.posX = 4.4f; // comprimento
	PanelEarth.posY = -2.0f; // altura
	PanelEarth.posZ = 0.0f; // largura
	PanelEarth.rotationSpeed = RP * 0.01f;
	PanelEarth.radius = 0.0f;
	PanelEarth.scale = 0.2f;

	// PANEL MARS

	PanelMars.transX = 0.0f;
	PanelMars.transY = 0.0f;
	PanelMars.transZ = 0.0f;
	PanelMars.posX = 7.2f; // comprimento
	PanelMars.posY = -2.0f; // altura
	PanelMars.posZ = 0.0f; // largura
	PanelMars.rotationSpeed = RP * 0.01f;
	PanelMars.radius = 0.0f;
	PanelMars.scale = 0.2f;

	// PANEL JUPITER

	PanelJupiter.transX = 0.0f;
	PanelJupiter.transY = 0.0f;
	PanelJupiter.transZ = 0.0f;
	PanelJupiter.posX = 12.6f; // comprimento
	PanelJupiter.posY = -2.0f; // altura
	PanelJupiter.posZ = 0.0f; // largura
	PanelJupiter.rotationSpeed = RP * 0.01f;
	PanelJupiter.radius = 0.0f;
	PanelJupiter.scale = 0.5f;

	// PANEL SATURN

	PanelSaturn.transX = 0.0f;
	PanelSaturn.transY = 0.0f;
	PanelSaturn.transZ = 0.0f;
	PanelSaturn.posX = 17.1f; // comprimento
	PanelSaturn.posY = -2.0f; // altura
	PanelSaturn.posZ = 0.0f; // largura
	PanelSaturn.rotationSpeed = RP * 0.01f;
	PanelSaturn.radius = 0.0f;
	PanelSaturn.scale = 0.5f;

	// PANEL URANUS

	PanelUranus.transX = 0.0f;
	PanelUranus.transY = 0.0f;
	PanelUranus.transZ = 0.0f;
	PanelUranus.posX = 21.1f; // comprimento
	PanelUranus.posY = -2.0f; // altura
	PanelUranus.posZ = 0.0f; // largura
	PanelUranus.rotationSpeed = RP * 0.01f;
	PanelUranus.radius = 0.0f;
	PanelUranus.scale = 0.5f;

	// PANEL NEPTUNE

	PanelNeptune.transX = 0.0f;
	PanelNeptune.transY = 0.0f;
	PanelNeptune.transZ = 0.0f;
	PanelNeptune.posX = 26.1f; // comprimento
	PanelNeptune.posY = -2.0f; // altura
	PanelNeptune.posZ = 0.0f; // largura
	PanelNeptune.rotationSpeed = RP * 0.01f;
	PanelNeptune.radius = 0.0f;
	PanelNeptune.scale = 0.5f;

	// ???

	Monke.transX = 0.0f;
	Monke.transY = 0.0f;
	Monke.transZ = 0.0f;
	Monke.posX;
	Monke.posY = 3.0f;
	Monke.posZ;
	Monke.rotationSpeed = RP * 0.00f;
	Monke.radius = UA * 0.00f;
	Monke.scale = TP * 0.91f;
}

void rotacoes() {

	Space.currentRotation += Space.rotationSpeed;
	Sun.currentRotation += Sun.rotationSpeed;
	Mercury.currentRotation += Mercury.rotationSpeed;
	Venus.currentRotation -= Venus.rotationSpeed;
	Earth.currentRotation += Earth.rotationSpeed;
	Moon.currentRotation += Moon.rotationSpeed;
	Mars.currentRotation += Mars.rotationSpeed;
	Jupiter.currentRotation += Jupiter.rotationSpeed;
	Saturn.currentRotation += Saturn.rotationSpeed;
	Saturn_Rings.currentRotation += Saturn_Rings.rotationSpeed;
	Uranus.currentRotation += Uranus.rotationSpeed;
	Uranus_Rings.currentRotation += Uranus_Rings.rotationSpeed;
	Neptune.currentRotation += Neptune.rotationSpeed;
}

void posicaoPlanetas() {
	//Desenha os planetas
	Earth.posX = Earth.radius * cos(Earth.transX * M_PI / 180);
	Earth.posZ = Earth.radius * sin(Earth.transZ * M_PI / 180);

	EarthPlane.posX = Earth.radius * cos(Earth.transX * M_PI / 180);
	EarthPlane.posZ = Earth.radius * sin(Earth.transZ * M_PI / 180);

	Moon.posX = Earth.posX + Moon.radius * cos(Moon.transX * M_PI / 180);
	Moon.posZ = Earth.posZ + Moon.radius * sin(Moon.transZ * M_PI / 180);

	Mercury.posX = Mercury.radius * cos(Mercury.transX * M_PI / 180);
	Mercury.posZ = Mercury.radius * sin(Mercury.transZ * M_PI / 180);

	Venus.posX = Venus.radius * cos(Venus.transX * M_PI / 180);
	Venus.posZ = Venus.radius * sin(Venus.transZ * M_PI / 180);

	Mars.posX = Mars.radius * cos(Mars.transX * M_PI / 180);
	Mars.posZ = Mars.radius * sin(Mars.transZ * M_PI / 180);

	Jupiter.posX = Jupiter.radius * cos(Jupiter.transX * M_PI / 180);
	Jupiter.posZ = Jupiter.radius * sin(Jupiter.transZ * M_PI / 180);

	Saturn.posX = Saturn.radius * cos(Saturn.transX * M_PI / 180);
	Saturn.posZ = Saturn.radius * sin(Saturn.transZ * M_PI / 180);

	Saturn_Rings.posX = Saturn_Rings.radius * cos(Saturn_Rings.transX * M_PI / 180);
	Saturn_Rings.posZ = Saturn_Rings.radius * sin(Saturn_Rings.transZ * M_PI / 180);

	Uranus.posX = Uranus.radius * cos(Uranus.transX * M_PI / 180);
	Uranus.posZ = Uranus.radius * sin(Uranus.transZ * M_PI / 180);

	Uranus_Rings.posX = Uranus_Rings.radius * cos(Uranus_Rings.transX * M_PI / 180);
	Uranus_Rings.posZ = Uranus_Rings.radius * sin(Uranus_Rings.transZ * M_PI / 180);

	Neptune.posX = Neptune.radius * cos(Neptune.transX * M_PI / 180);
	Neptune.posZ = Neptune.radius * sin(Neptune.transZ * M_PI / 180);
}

void definirPropsIniciais() {
	Space.currentRotation = 0.0f;

	Sun.currentRotation = 0.0f;
	Sun.posX = 0.0f;
	Sun.posY = -2.0f;
	Sun.posZ = 0.0f;
	Sun.selfRotationZ = 0.0f;
	Sun.selfRotationX = 0.0f;

	Mercury.currentRotation = 0.0f;
	Mercury.posX;
	Mercury.posY = -2.0f;
	Mercury.posZ;
	Mercury.transX = 0.0f;
	Mercury.transY = 0.0f;
	Mercury.transZ = 0.0f;
	Mercury.selfRotationZ = 0.0f;
	Mercury.selfRotationX = 0.0f;

	Venus.currentRotation = 0.0f;
	Venus.posX;
	Venus.posY = -2.0f;
	Venus.posZ;
	Venus.transX = 0.0f;
	Venus.transY = 0.0f;
	Venus.transZ = 0.0f;
	Venus.selfRotationZ = 0.0f;
	Venus.selfRotationX = 0.0f;

	Earth.currentRotation = 0.0f;
	Earth.posX;
	Earth.posY = -2.0f;
	Earth.posZ;
	Earth.transX = 0.0f;
	Earth.transY = 0.0f;
	Earth.transZ = 0.0f;
	Earth.selfRotationZ = 0.0f;
	Earth.selfRotationX = 0.0f;
	
	EarthPlane.currentRotation = 0.0f;
	EarthPlane.posX;
	EarthPlane.posY = -2.0f;
	EarthPlane.posZ;
	EarthPlane.transX = 0.0f;
	EarthPlane.transY = 0.0f;
	EarthPlane.transZ = 0.0f;
	EarthPlane.selfRotationZ = 0.0f;
	EarthPlane.selfRotationX = 0.0f;

	Moon.currentRotation = 0.0f;
	Moon.posX;
	Moon.posY = -2.0f;
	Moon.posZ;
	Moon.transX = 0.0f;
	Moon.transY = 0.0f;
	Moon.transZ = 0.0f;
	Moon.selfRotationZ = 0.0f;
	Moon.selfRotationX = 0.0f;

	Mars.currentRotation = 0.0f;
	Mars.posX;
	Mars.posY = -2.0f;
	Mars.posZ;
	Mars.transX = 0.0f;
	Mars.transY = 0.0f;
	Mars.transZ = 0.0f;
	Mars.selfRotationZ = 0.0f;
	Mars.selfRotationX = 0.0f;

	Jupiter.currentRotation = 0.0f;
	Jupiter.posX;
	Jupiter.posY = -2.0f;
	Jupiter.posZ;
	Jupiter.transX = 0.0f;
	Jupiter.transY = 0.0f;
	Jupiter.transZ = 0.0f;
	Jupiter.selfRotationZ = 0.0f;
	Jupiter.selfRotationX = 0.0f;

	Saturn.currentRotation = 45.0f;
	Saturn.posX;
	Saturn.posY = -2.0f;
	Saturn.posZ;
	Saturn.transX = 0.0f;
	Saturn.transY = 0.0f;
	Saturn.transZ = 0.0f;
	Saturn.selfRotationZ = 45.0f;
	Saturn.selfRotationX = 0.0f;


	Saturn_Rings.currentRotation = 0.0f;
	Saturn_Rings.posX;
	Saturn_Rings.posY = -2.0f;
	Saturn_Rings.posZ;
	Saturn_Rings.transX = 0.0f;
	Saturn_Rings.transY = 0.0f;
	Saturn_Rings.transZ = 0.0f;
	Saturn_Rings.selfRotationZ = 45.0f;
	Saturn_Rings.selfRotationX = 0.0f;

	Uranus.currentRotation = 0.0f;
	Uranus.posX;
	Uranus.posY = -2.0f;
	Uranus.posZ;
	Uranus.transX = 0.0f;
	Uranus.transY = 0.0f;
	Uranus.transZ = 0.0f;
	Uranus.selfRotationZ = 0.0f;
	Uranus.selfRotationX = 90.0f;

	Uranus_Rings.currentRotation = 45.0f;
	Uranus_Rings.posX;
	Uranus_Rings.posY = -2.0f;
	Uranus_Rings.posZ;
	Uranus_Rings.transX = 0.0f;
	Uranus_Rings.transY = 0.0f;
	Uranus_Rings.transZ = 0.0f;
	Uranus_Rings.selfRotationZ = 0.0f;
	Uranus_Rings.selfRotationX = 90.0f;

	Neptune.currentRotation = 0.0f;
	Neptune.posX;
	Neptune.posY = -2.0f;
	Neptune.posZ;
	Neptune.transX = 0.0f;
	Neptune.transY = 0.0f;
	Neptune.transZ = 0.0f;
	Neptune.selfRotationZ = -30.0f;
	Neptune.selfRotationX = 0.0f;

	// ???

	Monke.transX = 0.0f;
	Monke.transY = 0.0f;
	Monke.transZ = 0.0f;
	Monke.posX;
	Monke.posY = 3.0f;
	Monke.posZ;
	Monke.rotationSpeed = RP * 0.00f;
	Monke.radius = UA * 0.00f;
	Monke.scale = TP * 0.91f;
	Monke.currentRotation = 90.0f;

}

void translacoesPlanetas() {
	Mercury.transX += Mercury.translationSpeed;
	Mercury.transZ += Mercury.translationSpeed;

	Venus.transX += Venus.translationSpeed;
	Venus.transZ += Venus.translationSpeed;

	Earth.transX += Earth.translationSpeed;
	Earth.transZ += Earth.translationSpeed;

	EarthPlane.transX += Earth.translationSpeed;
	EarthPlane.transZ += Earth.translationSpeed;

	Moon.transX += Moon.translationSpeed;
	Moon.transZ += Moon.translationSpeed;


	Mars.transX += Mars.translationSpeed;
	Mars.transZ += Mars.translationSpeed;

	Jupiter.transX += Jupiter.translationSpeed;
	Jupiter.transZ += Jupiter.translationSpeed;

	Saturn.transX += Saturn.translationSpeed;
	Saturn.transZ += Saturn.translationSpeed;

	Saturn_Rings.transX += Saturn.translationSpeed;
	Saturn_Rings.transZ += Saturn.translationSpeed;

	Uranus.transX += Uranus.translationSpeed;
	Uranus.transZ += Uranus.translationSpeed;

	Uranus_Rings.transX += Uranus_Rings.translationSpeed;
	Uranus_Rings.transZ += Uranus_Rings.translationSpeed;

	Neptune.transX += Neptune.translationSpeed;
	Neptune.transZ += Neptune.translationSpeed;
}