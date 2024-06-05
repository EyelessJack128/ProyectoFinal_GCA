#define _USE_MATH_DEFINES
#include <cmath>
//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"

// Shader include
#include "Headers/Shader.h"

// Model geometric includes
#include "Headers/Sphere.h"
#include "Headers/Cylinder.h"
#include "Headers/Box.h"
#include "Headers/FirstPersonCamera.h"
#include "Headers/ThirdPersonCamera.h"

// Font rendering include
#include "Headers/FontTypeRendering.h"

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Headers/Texture.h"

// Include loader Model class
#include "Headers/Model.h"

// Include Terrain
#include "Headers/Terrain.h"

#include "Headers/AnimationUtils.h"

// Include Colision headers functions
#include "Headers/Colisiones.h"

// ShadowBox include
#include "Headers/ShadowBox.h"

// OpenAL include
#include <AL/alut.h>

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

int screenWidth;
int screenHeight;

const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

GLFWwindow *window;

Shader shader;
//Shader con skybox
Shader shaderSkybox;
//Shader con multiples luces
Shader shaderMulLighting;
//Shader para el terreno
Shader shaderTerrain;
// Shader para dibujar un objeto con solo textura
Shader shaderTexture;
//Shader para dibujar el buffer de profunidad
Shader shaderDepth;
// Shader para visualizar el buffer de profundidad
Shader shaderViewDepth;
//Shader para las particulas de fountain
/*Shader shaderParticlesFountain;*/

std::shared_ptr<Camera> camera(new ThirdPersonCamera());
float distanceFromTarget = 7.0;

//Carga de objetos basicos para el entorno del juego
Sphere skyboxSphere(20, 20);
Box boxCesped;
Box boxCollider;
Sphere sphereCollider(10, 10);
Box boxIntro;
Box boxViewDepth;

// Carga de modelos complejos

// Ejemplo
Model modelThrantaClass;

<<<<<<< HEAD
// Asteroid
Model modelAsteroid;
=======
// TIE Fighter
Model modelTIEFighter;

// TIE Interceptor
Model modelTIEInterceptor;

// A Wing
Model modelAWing;

// TIE Bomber
Model modelTIEBomber;
>>>>>>> eaa99d1444f7a6e9204fd224d6b45929e0a2a9e7

// Terrain model instance
Terrain terrain(-1, -1, 200, 8, "../Textures/heightmap.png");

ShadowBox * shadowBox;
GLuint textureTerrainBackgroundID;
GLuint textureTerrainRID, textureTerrainGID, textureTerrainBID, textureTerrainBlendMapID;
GLuint skyboxTextureID;
GLuint textureInit1ID, textureInit2ID, textureActivaID, textureScreenID;

//Variables de juego
bool iniciaPartida = false, presionarOpcion = false;
int selectedShip = 0;
bool enableShipShift = false;

// Modelo para el render del texto
FontTypeRendering::FontTypeRendering *modelText;

GLenum types[6] = {
GL_TEXTURE_CUBE_MAP_POSITIVE_X,
GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

std::string fileNames[6] = { "../Textures/mp_bloodvalley/blood-valley_ft.tga",
		"../Textures/mp_bloodvalley/blood-valley_bk.tga",
		"../Textures/mp_bloodvalley/blood-valley_up.tga",
		"../Textures/mp_bloodvalley/blood-valley_dn.tga",
		"../Textures/mp_bloodvalley/blood-valley_rt.tga",
		"../Textures/mp_bloodvalley/blood-valley_lf.tga" };

bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;

// Model matrix definitions
//Spaceship
glm::mat4 modelMatrixThrantaClass = glm::mat4(1.0f);
glm::mat4 modelMatrixTIEFighter = glm::mat4(1.0f);
glm::mat4 modelMatrixTIEBomber = glm::mat4(1.0f);
glm::mat4 modelMatrixTIEInterceptor = glm::mat4(1.0f);
glm::mat4 modelMatrixAWing = glm::mat4(1.0f);

// Asteroid
glm::mat4 modelMatrixAsteroid = glm::mat4(1.0f);

int modelSelected = 0;
bool enableCountSelected = true;

// Variables to animations keyframes
bool saveFrame = false, availableSave = true;
std::ofstream myfile;
std::string fileName = "";
bool record = false;

// Lamps position
std::vector<glm::vec3> lamp1Position = {
	glm::vec3(-7.03, 0, -19.14),
	glm::vec3(24.41, 0, -34.57),
	glm::vec3(-10.15, 0, -54.1)
};
std::vector<float> lamp1Orientation = {
	-17.0, -82.67, 23.70
};
std::vector<glm::vec3> lamp2Position = {
	glm::vec3(-36.52, 0, -23.24),
	glm::vec3(-52.73, 0, -3.90)
};
std::vector<float> lamp2Orientation = {
	21.37 + 90, -65.0 + 90
};

// Blending model unsorted
std::map<std::string, glm::vec3> blendingUnsorted = {
		{"aircraft", glm::vec3(10.0, 0.0, -17.5)},
		{"lambo", glm::vec3(23.0, 0.0, 0.0)},
		{"heli", glm::vec3(5.0, 10.0, -5.0)}
};

double deltaTime;
double currTime, lastTime;


// Jump variables
bool isJump = false;
float GRAVITY = 1.81;
double tmv = 0;
double startTimeJump = 0;

// Colliders
std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> > collidersOBB;
std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> > collidersSBB;

// OpenAL Defines
#define NUM_BUFFERS 3
#define NUM_SOURCES 3
#define NUM_ENVIRONMENTS 1
// Listener
ALfloat listenerPos[] = { 0.0, 0.0, 4.0 };
ALfloat listenerVel[] = { 0.0, 0.0, 0.0 };
ALfloat listenerOri[] = { 0.0, 0.0, 1.0, 0.0, 1.0, 0.0 };
// Source 0
ALfloat source0Pos[] = { -2.0, 0.0, 0.0 };
ALfloat source0Vel[] = { 0.0, 0.0, 0.0 };
// Source 1
ALfloat source1Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source1Vel[] = { 0.0, 0.0, 0.0 };
// Source 2
ALfloat source2Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source2Vel[] = { 0.0, 0.0, 0.0 };
// Buffers
ALuint buffer[NUM_BUFFERS];
ALuint source[NUM_SOURCES];
ALuint environment[NUM_ENVIRONMENTS];
// Configs
ALsizei size, freq;
ALenum format;
ALvoid *data;
int ch;
ALboolean loop;
std::vector<bool> sourcesPlay = {true, true, true};

// Framesbuffers
GLuint depthMap, depthMapFBO;

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow *window, int key, int scancode, int action,
		int mode);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void initParticleBuffers();
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroy();
bool processInput(bool continueApplication = true);

// Implementacion de todas las funciones.
void init(int width, int height, std::string strTitle, bool bFullScreen) {

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}

	screenWidth = width;
	screenHeight = height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen)
		window = glfwCreateWindow(width, height, strTitle.c_str(),
				glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
				nullptr);

	if (window == nullptr) {
		std::cerr
				<< "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
				<< std::endl;
		destroy();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Init glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to initialize glew" << std::endl;
		exit(-1);
	}

	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Inicialización de los shaders
	shader.initialize("../Shaders/colorShader.vs", "../Shaders/colorShader.fs");
	shaderSkybox.initialize("../Shaders/skyBox.vs", "../Shaders/skyBox_fog.fs");
	shaderMulLighting.initialize("../Shaders/iluminacion_textura_animation_shadow.vs", "../Shaders/multipleLights_shadow.fs");
	shaderTerrain.initialize("../Shaders/terrain_shadow.vs", "../Shaders/terrain_shadow.fs");
	shaderTexture.initialize("../Shaders/texturizado.vs", "../Shaders/texturizado.fs");
	shaderViewDepth.initialize("../Shaders/texturizado.vs", "../Shaders/texturizado_depth_view.fs");
	shaderDepth.initialize("../Shaders/shadow_mapping_depth.vs", "../Shaders/shadow_mapping_depth.fs");
	/*shaderParticlesFountain.initialize("../Shaders/particlesFountain.vs", "../Shaders/particlesFountain.fs");*/

	// Inicializacion de los objetos.
	skyboxSphere.init();
	skyboxSphere.setShader(&shaderSkybox);
	skyboxSphere.setScale(glm::vec3(20.0f, 20.0f, 20.0f));

	boxCollider.init();
	boxCollider.setShader(&shader);
	boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	sphereCollider.init();
	sphereCollider.setShader(&shader);
	sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	boxCesped.init();
	boxCesped.setShader(&shaderMulLighting);

	boxIntro.init();
	boxIntro.setShader(&shaderTexture);
	boxIntro.setScale(glm::vec3(2.0, 2.0, 1.0));

	boxViewDepth.init();
	boxViewDepth.setShader(&shaderViewDepth);

	// SpaceShip
	modelThrantaClass.loadModel("../models/TIE_LN/TIE_Fighter.fbx");
	modelThrantaClass.setShader(&shaderMulLighting);

<<<<<<< HEAD
	//Asteroid
	modelAsteroid.loadModel("../models/Asteroid/Asteroid.obj");
	modelAsteroid.setShader(&shaderMulLighting);
=======
	// TIE Fighter
	modelTIEFighter.loadModel("../models/TIE_LN/TIE_Fighter.fbx");
	modelTIEFighter.setShader(&shaderMulLighting);

	// TIE Bomber
	modelTIEBomber.loadModel("../models/TIE_sa/TIE_Bombarder.fbx");
	modelTIEBomber.setShader(&shaderMulLighting);

	// TIE Interceptor
	modelTIEInterceptor.loadModel("../models/TIE_IN/TIE_Interceptor.fbx");
	modelTIEInterceptor.setShader(&shaderMulLighting);

	// A Wing
	modelAWing.loadModel("../models/A_Wing/AWing.fbx");
	modelAWing.setShader(&shaderMulLighting);
>>>>>>> eaa99d1444f7a6e9204fd224d6b45929e0a2a9e7

	// Terreno
	terrain.init();
	terrain.setShader(&shaderTerrain);

	// Se inicializa el model de render text
	modelText = new FontTypeRendering::FontTypeRendering(screenWidth, screenHeight);
	modelText->Initialize();

	camera->setPosition(glm::vec3(0.0, 3.0, 4.0));
	camera->setDistanceFromTarget(distanceFromTarget);
	camera->setSensitivity(1.0);
	
	// Carga de texturas para el skybox
	Texture skyboxTexture = Texture("");
	glGenTextures(1, &skyboxTextureID);
	// Tipo de textura CUBE MAP
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(types); i++) {
		skyboxTexture = Texture(fileNames[i]);
		skyboxTexture.loadImage(true);
		if (skyboxTexture.getData()) {
			glTexImage2D(types[i], 0, skyboxTexture.getChannels() == 3 ? GL_RGB : GL_RGBA, skyboxTexture.getWidth(), skyboxTexture.getHeight(), 0,
			skyboxTexture.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, skyboxTexture.getData());
		} else
			std::cout << "Failed to load texture" << std::endl;
		skyboxTexture.freeImage();
	}

	Texture textureTerrainBackground("../Textures/grassy2.png");
	textureTerrainBackground.loadImage(); // Cargar la textura
	glGenTextures(1, &textureTerrainBackgroundID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureTerrainBackgroundID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureTerrainBackground.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureTerrainBackground.getChannels() == 3 ? GL_RGB : GL_RGBA, textureTerrainBackground.getWidth(), textureTerrainBackground.getHeight(), 0,
		textureTerrainBackground.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureTerrainBackground.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureTerrainBackground.freeImage(); // Liberamos memoria

	// Defininiendo texturas del mapa de mezclas
	// Definiendo la textura
	Texture textureR("../Textures/mud.png");
	textureR.loadImage(); // Cargar la textura
	glGenTextures(1, &textureTerrainRID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureTerrainRID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureR.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureR.getChannels() == 3 ? GL_RGB : GL_RGBA, textureR.getWidth(), textureR.getHeight(), 0,
		textureR.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureR.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureR.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureG("../Textures/grassFlowers.png");
	textureG.loadImage(); // Cargar la textura
	glGenTextures(1, &textureTerrainGID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureTerrainGID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureG.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureG.getChannels() == 3 ? GL_RGB : GL_RGBA, textureG.getWidth(), textureG.getHeight(), 0,
		textureG.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureG.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureG.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureB("../Textures/path.png");
	textureB.loadImage(); // Cargar la textura
	glGenTextures(1, &textureTerrainBID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureTerrainBID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureB.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureB.getChannels() == 3 ? GL_RGB : GL_RGBA, textureB.getWidth(), textureB.getHeight(), 0,
		textureB.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureB.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureB.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureBlendMap("../Textures/blendMap.png");
	textureBlendMap.loadImage(); // Cargar la textura
	glGenTextures(1, &textureTerrainBlendMapID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureBlendMap.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureBlendMap.getChannels() == 3 ? GL_RGB : GL_RGBA, textureBlendMap.getWidth(), textureBlendMap.getHeight(), 0,
		textureBlendMap.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureBlendMap.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureBlendMap.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureIntro1("../Textures/Intro1.png");
	textureIntro1.loadImage(); // Cargar la textura
	glGenTextures(1, &textureInit1ID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureInit1ID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureIntro1.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureIntro1.getChannels() == 3 ? GL_RGB : GL_RGBA, textureIntro1.getWidth(), textureIntro1.getHeight(), 0,
		textureIntro1.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureIntro1.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureIntro1.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureIntro2("../Textures/Intro2.png");
	textureIntro2.loadImage(); // Cargar la textura
	glGenTextures(1, &textureInit2ID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureInit2ID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureIntro2.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureIntro2.getChannels() == 3 ? GL_RGB : GL_RGBA, textureIntro2.getWidth(), textureIntro2.getHeight(), 0,
		textureIntro2.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureIntro2.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureIntro2.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureScreen("../Textures/Screen.png");
	textureScreen.loadImage(); // Cargar la textura
	glGenTextures(1, &textureScreenID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureScreenID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureScreen.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureScreen.getChannels() == 3 ? GL_RGB : GL_RGBA, textureScreen.getWidth(), textureScreen.getHeight(), 0,
		textureScreen.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureScreen.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureScreen.freeImage(); // Liberamos memoria

	/*******************************************
	 * OpenAL init
	 *******************************************/
	alutInit(0, nullptr);
	alListenerfv(AL_POSITION, listenerPos);
	alListenerfv(AL_VELOCITY, listenerVel);
	alListenerfv(AL_ORIENTATION, listenerOri);
	alGetError(); // clear any error messages
	if (alGetError() != AL_NO_ERROR) {
		printf("- Error creating buffers !!\n");
		exit(1);
	}
	else {
		printf("init() - No errors yet.");
	}
	// Generate buffers, or else no sound will happen!
	alGenBuffers(NUM_BUFFERS, buffer);
	buffer[0] = alutCreateBufferFromFile("../sounds/fountain.wav");
	buffer[1] = alutCreateBufferFromFile("../sounds/fire.wav");
	buffer[2] = alutCreateBufferFromFile("../sounds/darth_vader.wav");
	int errorAlut = alutGetError();
	if (errorAlut != ALUT_ERROR_NO_ERROR){
		printf("- Error open files with alut %d !!\n", errorAlut);
		exit(2);
	}

	alGetError(); /* clear error */
	alGenSources(NUM_SOURCES, source);

	if (alGetError() != AL_NO_ERROR) {
		printf("- Error creating sources !!\n");
		exit(2);
	}
	else {
		printf("init - no errors after alGenSources\n");
	}
	alSourcef(source[0], AL_PITCH, 1.0f);
	alSourcef(source[0], AL_GAIN, 3.0f);
	alSourcefv(source[0], AL_POSITION, source0Pos);
	alSourcefv(source[0], AL_VELOCITY, source0Vel);
	alSourcei(source[0], AL_BUFFER, buffer[0]);
	alSourcei(source[0], AL_LOOPING, AL_TRUE);
	alSourcef(source[0], AL_MAX_DISTANCE, 2000);

	alSourcef(source[1], AL_PITCH, 1.0f);
	alSourcef(source[1], AL_GAIN, 0.5f);
	alSourcefv(source[1], AL_POSITION, source1Pos);
	alSourcefv(source[1], AL_VELOCITY, source1Vel);
	alSourcei(source[1], AL_BUFFER, buffer[1]);
	alSourcei(source[1], AL_LOOPING, AL_TRUE);
	alSourcef(source[1], AL_MAX_DISTANCE, 1000);

	alSourcef(source[2], AL_PITCH, 1.0f);
	alSourcef(source[2], AL_GAIN, 0.3f);
	alSourcefv(source[2], AL_POSITION, source2Pos);
	alSourcefv(source[2], AL_VELOCITY, source2Vel);
	alSourcei(source[2], AL_BUFFER, buffer[2]);
	alSourcei(source[2], AL_LOOPING, AL_TRUE);
	alSourcef(source[2], AL_MAX_DISTANCE, 2000);

	/*******************************************
	 * Inicializacion del framebuffer para
	 * almacenar el buffer de profunidadad
	 *******************************************/
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
				 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
	// --------- IMPORTANTE ----------
	// Eliminar los shader y buffers creados.

	// Shaders Delete
	shader.destroy();
	shaderMulLighting.destroy();
	shaderSkybox.destroy();
	shaderTerrain.destroy();
	/*shaderParticlesFountain.destroy();*/

	// Basic objects Delete
	skyboxSphere.destroy();
	boxCesped.destroy();
	boxCollider.destroy();
	sphereCollider.destroy();
	boxIntro.destroy();
	boxViewDepth.destroy();

	// Custom objects Delete
	//Spaceship
	modelThrantaClass.destroy();
	modelTIEFighter.destroy();
	modelTIEBomber.destroy();
	modelTIEInterceptor.destroy();
	modelAWing.destroy();

	// Asteroid
	modelAsteroid.destroy();

	// Terrains objects Delete
	terrain.destroy();

	// Textures Delete
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &textureTerrainBackgroundID);
	glDeleteTextures(1, &textureTerrainBID);
	glDeleteTextures(1, &textureTerrainGID);
	glDeleteTextures(1, &textureTerrainRID);
	glDeleteTextures(1, &textureTerrainBlendMapID);
	glDeleteTextures(1, &textureInit1ID);
	glDeleteTextures(1, &textureInit2ID);
	glDeleteTextures(1, &textureScreenID);

	// Cube Maps Delete
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glDeleteTextures(1, &skyboxTextureID);
}

void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
		int mode) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			exitApp = true;
			break;
		}
	}
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
	offsetX = xpos - lastMousePosX;
	offsetY = ypos - lastMousePosY;
	lastMousePosX = xpos;
	lastMousePosY = ypos;
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset){
	distanceFromTarget -= yoffset;
	camera->setDistanceFromTarget(distanceFromTarget);
}

void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod) {
	if (state == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_RIGHT:
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_LEFT:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		}
	}
}

bool processInput(bool continueApplication) {
	if (exitApp || glfwWindowShouldClose(window) != 0) {
		return false;
	}

	if(!iniciaPartida){
		bool presionarEnter = glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS;
		if(textureActivaID == textureInit1ID && presionarEnter){
			iniciaPartida = true;
			textureActivaID = textureScreenID;
		}
		else if(!presionarOpcion && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
			presionarOpcion = true;
			if(textureActivaID == textureInit1ID)
				textureActivaID = textureInit2ID;
			else if(textureActivaID == textureInit2ID)
				textureActivaID = textureInit1ID;
		}
		else if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
			presionarOpcion = false;
	}

	if (glfwJoystickPresent(GLFW_JOYSTICK_1) == GL_TRUE) {
		std::cout << "Esta presente el joystick" << std::endl;
		int axesCount, buttonCount;
		const float * axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
		std::cout << "Número de ejes disponibles :=>" << axesCount << std::endl;
		std::cout << "Left Stick X axis: " << axes[0] << std::endl;
		std::cout << "Left Stick Y axis: " << axes[1] << std::endl;
		std::cout << "Left Trigger/L2: " << axes[2] << std::endl;
		std::cout << "Right Stick X axis: " << axes[3] << std::endl;
		std::cout << "Right Stick Y axis: " << axes[4] << std::endl;
		std::cout << "Right Trigger/R2: " << axes[5] << std::endl;

		if(fabs(axes[1]) > 0.2){
			modelMatrixThrantaClass = glm::translate(modelMatrixThrantaClass, glm::vec3(0, 0, -axes[1] * 0.1));
		}if(fabs(axes[0]) > 0.2){
			modelMatrixThrantaClass = glm::rotate(modelMatrixThrantaClass, glm::radians(-axes[0] * 0.5f), glm::vec3(0, 1, 0));
		}

		if(fabs(axes[3]) > 0.2){
			camera->mouseMoveCamera(axes[3], 0.0, deltaTime);
		}if(fabs(axes[4]) > 0.2){
			camera->mouseMoveCamera(0.0, axes[4], deltaTime);
		}

		const unsigned char * buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);
		std::cout << "Número de botones disponibles :=>" << buttonCount << std::endl;
		if(buttons[0] == GLFW_PRESS)
			std::cout << "Se presiona x" << std::endl;

		if(!isJump && buttons[0] == GLFW_PRESS){
			isJump = true;
			startTimeJump = currTime;
			tmv = 0;
		}
	}

	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		camera->mouseMoveCamera(offsetX, 0.0, deltaTime);
	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		camera->mouseMoveCamera(0.0, offsetY, deltaTime);

	offsetX = 0;
	offsetY = 0;

	// Enable Ship shift
	if(glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		enableShipShift = !enableShipShift;
	
	// Ship Shifting
	if (enableShipShift && glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS){
		enableShipShift = false;
		selectedShip++;
		if (selectedShip > 3)
			selectedShip = 0;
		std::cout << "Ship selected:" << selectedShip << std::endl;
	} else if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE)
		enableShipShift = true;
	

	// Seleccionar modelo
	if (enableCountSelected && glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS){
		enableCountSelected = false;
		modelSelected++;
		if(modelSelected > 4)
			modelSelected = 0;
		if(modelSelected == 1)
			fileName = "../animaciones/animation_dart_joints.txt";
		if (modelSelected == 2)
			fileName = "../animaciones/animation_dart.txt";
		if(modelSelected == 3)
			fileName = "../animaciones/animation_buzz_joints.txt";
		if (modelSelected == 4)
			fileName = "../animaciones/animation_buzz.txt";
		std::cout << "modelSelected:" << modelSelected << std::endl;
	}
	else if(glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE)
		enableCountSelected = true;

	// Guardar key frames
	if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS
			&& glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
		record = true;
		if(myfile.is_open())
			myfile.close();
		myfile.open(fileName);
	}
	if(availableSave && glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS){
		saveFrame = true;
		availableSave = false;
	}if(glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE)
		availableSave = true;

	// Controles de mayow
	if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
		modelMatrixThrantaClass = glm::rotate(modelMatrixThrantaClass, 0.02f, glm::vec3(0, 1, 0));
	} else if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
		modelMatrixThrantaClass = glm::rotate(modelMatrixThrantaClass, -0.02f, glm::vec3(0, 1, 0));
	}
	if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
		modelMatrixThrantaClass = glm::translate(modelMatrixThrantaClass, glm::vec3(0.0, 0.0, 0.02));
	}
	else if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
		modelMatrixThrantaClass = glm::translate(modelMatrixThrantaClass, glm::vec3(0.0, 0.0, -0.02));
	}

	bool keySpaceStatus = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
	if(!isJump && keySpaceStatus){
		isJump = true;
		startTimeJump = currTime;
		tmv = 0;
	}

	glfwPollEvents();
	return continueApplication;
}

void prepareScene(){

	terrain.setShader(&shaderTerrain);

	//Grass
	//modelGrass.setShader(&shaderMulLighting);

	// SpaceShip
	modelThrantaClass.setShader(&shaderMulLighting);

<<<<<<< HEAD
	//Asteroid
	modelAsteroid.setShader(&shaderMulLighting);
=======
	// TIE Fighter
	modelTIEFighter.setShader(&shaderMulLighting);

	// TIE Bomber
	modelTIEBomber.setShader(&shaderMulLighting);

	// TIE Interceptor
	modelTIEInterceptor.setShader(&shaderMulLighting);

	// A Wing
	modelAWing.setShader(&shaderMulLighting);
>>>>>>> eaa99d1444f7a6e9204fd224d6b45929e0a2a9e7
}

void prepareDepthScene(){

	terrain.setShader(&shaderDepth);

	//Space Ship
	modelThrantaClass.setShader(&shaderDepth);

<<<<<<< HEAD
	//Asteroid
	modelAsteroid.setShader(&shaderDepth);
=======
	// TIE Fighter
	modelTIEFighter.setShader(&shaderDepth);

	// TIE Bomber
	modelTIEBomber.setShader(&shaderDepth);

	// TIE Interceptor
	modelTIEInterceptor.setShader(&shaderDepth);

	// A Wing
	modelAWing.setShader(&shaderDepth);
>>>>>>> eaa99d1444f7a6e9204fd224d6b45929e0a2a9e7
}

void renderSolidScene(){
	/*******************************************
	 * Terrain Cesped
	 *******************************************/
	// Se activa la textura del agua
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBackgroundID);
	shaderTerrain.setInt("backgroundTexture", 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureTerrainRID);
	shaderTerrain.setInt("rTexture", 1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, textureTerrainGID);
	shaderTerrain.setInt("gTexture", 2);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBID);
	shaderTerrain.setInt("bTexture", 3);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID);
	shaderTerrain.setInt("blendMapTexture", 4);
	shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(80, 80)));
	terrain.setPosition(glm::vec3(100, 0, 100));
	terrain.render();
	shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
	glBindTexture(GL_TEXTURE_2D, 0);

	/*******************************************
	 * Custom objects obj
	 *******************************************/
	// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
	glActiveTexture(GL_TEXTURE0);
	// Render Ship
	modelMatrixThrantaClass[3][1] = terrain.getHeightTerrain(modelMatrixThrantaClass[3][0] , modelMatrixThrantaClass[3][2]) + 2.0;
	glm::mat4 modelMatrixThrantaClassCopy = glm::mat4(modelMatrixThrantaClass);
	switch (selectedShip){
	case 0:
		modelMatrixThrantaClassCopy = glm::scale(modelMatrixThrantaClassCopy, glm::vec3(0.2f, 0.2f, 0.2f));
		modelMatrixThrantaClassCopy = glm::rotate(modelMatrixThrantaClassCopy, glm::radians(-90.0f),glm::vec3(1, 0, 0));
		modelTIEFighter.render(modelMatrixThrantaClassCopy);
		break;
	case 1:
		modelMatrixThrantaClassCopy = glm::scale(modelMatrixThrantaClassCopy, glm::vec3(0.2f, 0.2f, 0.2f));
		modelMatrixThrantaClassCopy = glm::rotate(modelMatrixThrantaClassCopy, glm::radians(-90.0f),glm::vec3(1, 0, 0));
		modelTIEBomber.render(modelMatrixThrantaClassCopy);
		break;
	case 2:
		modelMatrixThrantaClassCopy = glm::scale(modelMatrixThrantaClassCopy, glm::vec3(0.2f, 0.2f, 0.2f));
		modelMatrixThrantaClassCopy = glm::rotate(modelMatrixThrantaClassCopy, glm::radians(-90.0f),glm::vec3(1, 0, 0));
		modelTIEInterceptor.render(modelMatrixThrantaClassCopy);
		break;
	case 3:
		modelMatrixThrantaClassCopy = glm::scale(modelMatrixThrantaClassCopy, glm::vec3(0.002f, 0.002f, 0.002f));
		modelAWing.render(modelMatrixThrantaClassCopy);
		break;
	default:
		modelMatrixThrantaClassCopy = glm::scale(modelMatrixThrantaClassCopy, glm::vec3(0.2f, 0.2f, 0.2f));
		modelMatrixThrantaClassCopy = glm::rotate(modelMatrixThrantaClassCopy, glm::radians(-90.0f),glm::vec3(1, 0, 0));
		modelTIEFighter.render(modelMatrixThrantaClassCopy);
		break;
	}

	modelMatrixTIEFighter[3][1] = terrain.getHeightTerrain(modelMatrixTIEFighter[3][0] , modelMatrixTIEFighter[3][2]) + 2.0;
	glm::mat4 modelMatrixTIEFighterCopy = glm::scale(modelMatrixTIEFighter, glm::vec3(0.2f, 0.2f, 0.2f));
	modelMatrixTIEFighterCopy = glm::rotate(modelMatrixTIEFighterCopy, glm::radians(-90.0f),glm::vec3(1, 0, 0));
	modelTIEFighter.render(modelMatrixTIEFighterCopy);

	modelMatrixTIEBomber[3][1] = terrain.getHeightTerrain(modelMatrixTIEBomber[3][0] , modelMatrixTIEBomber[3][2]) + 2.0;
	glm::mat4 modelMatrixTIEBomberCopy = glm::scale(modelMatrixTIEBomber, glm::vec3(0.2f, 0.2f, 0.2f));
	modelMatrixTIEBomberCopy = glm::rotate(modelMatrixTIEBomberCopy, glm::radians(-90.0f),glm::vec3(1, 0, 0));
	modelTIEBomber.render(modelMatrixTIEBomberCopy);

	modelMatrixTIEInterceptor[3][1] = terrain.getHeightTerrain(modelMatrixTIEInterceptor[3][0] , modelMatrixTIEInterceptor[3][2]) + 2.0;
	glm::mat4 modelMatrixTIEInterceptorCopy = glm::scale(modelMatrixTIEInterceptor, glm::vec3(0.2f, 0.2f, 0.2f));
	modelMatrixTIEInterceptorCopy = glm::rotate(modelMatrixTIEInterceptorCopy, glm::radians(-90.0f),glm::vec3(1, 0, 0));
	modelTIEInterceptor.render(modelMatrixTIEInterceptorCopy);

	modelMatrixAWing[3][1] = terrain.getHeightTerrain(modelMatrixAWing[3][0] , modelMatrixAWing[3][2]) + 2.0;
	glm::mat4 modelMatrixAWingCopy = glm::scale(modelMatrixAWing, glm::vec3(0.002f, 0.002f, 0.002f));
	modelMatrixAWingCopy = glm::rotate(modelMatrixAWingCopy, glm::radians(-90.0f),glm::vec3(0, 1, 0));
	modelAWing.render(modelMatrixAWingCopy);



	// Render Asteroid
	modelMatrixAsteroid[3][1] = terrain.getHeightTerrain(modelMatrixAsteroid[3][0] , modelMatrixAsteroid[3][2]) + 2.0;
	glm::mat4 modelMatrixAsteroidBody = glm::scale(modelMatrixAsteroid, glm::vec3(1.3, 1.3, 1.3));
	modelMatrixAsteroidBody = glm::rotate(modelMatrixAsteroidBody, glm::radians(-90.0f),glm::vec3(0, 1, 0));
	modelAsteroid.render(modelMatrixAsteroidBody);
	glActiveTexture(GL_TEXTURE0);

	/*******************************************
	 * Skybox
	 *******************************************/
	GLint oldCullFaceMode;
	GLint oldDepthFuncMode;
	// deshabilita el modo del recorte de caras ocultas para ver las esfera desde adentro
	glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
	glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);
	shaderSkybox.setFloat("skybox", 0);
	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);
	glActiveTexture(GL_TEXTURE0);
	skyboxSphere.render();
	glCullFace(oldCullFaceMode);
	glDepthFunc(oldDepthFuncMode);
}

void renderAlphaScene(bool render = true){
	/**********
	 * Update the position with alpha objects
	 */

	/**********
	 * Sorter with alpha objects
	 */
	std::map<float, std::pair<std::string, glm::vec3>> blendingSorted;
	std::map<std::string, glm::vec3>::iterator itblend;
	for(itblend = blendingUnsorted.begin(); itblend != blendingUnsorted.end(); itblend++){
		float distanceFromView = glm::length(camera->getPosition() - itblend->second);
		blendingSorted[distanceFromView] = std::make_pair(itblend->first, itblend->second);
	}

	/**********
	 * Render de las transparencias
	 */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	for(std::map<float, std::pair<std::string, glm::vec3> >::reverse_iterator it = blendingSorted.rbegin(); it != blendingSorted.rend(); it++){
	}
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	if(render){
		/************Render de imagen de frente**************/
		shaderTexture.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
		shaderTexture.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureActivaID);
		shaderTexture.setInt("outTexture", 0);
		glEnable(GL_BLEND);
		boxIntro.render();
		glDisable(GL_BLEND);

		modelText->render("Texto en OpenGL", -1, 0);
	}
}

void renderScene(){
	renderSolidScene();
	renderAlphaScene(false);
}

void applicationLoop() {
	bool psi = true;

	glm::vec3 axis;
	glm::vec3 target;
	float angleTarget;

	// Transformaciones iniciales para el acomodo de los modelos
	//SpaceShip
	modelMatrixThrantaClass = glm::translate(modelMatrixThrantaClass, glm::vec3(5.0, 0.0, -40.0));

<<<<<<< HEAD
	//Asteroid
	modelMatrixAsteroid = glm::translate(modelMatrixAsteroid, glm::vec3(7.0, 5.0, -10.0));
=======
	modelMatrixTIEFighter = glm::translate(modelMatrixTIEFighter, glm::vec3(5.0, 0.0, -30));

	modelMatrixTIEBomber = glm::translate(modelMatrixTIEBomber, glm::vec3(5.0, 0.0, -20));

	modelMatrixTIEFighter = glm::translate(modelMatrixTIEFighter, glm::vec3(5.0, 0.0, -10));

	modelMatrixAWing = glm::translate(modelMatrixAWing, glm::vec3(5.0, 0.0, 0.0));
>>>>>>> eaa99d1444f7a6e9204fd224d6b45929e0a2a9e7

	lastTime = TimeManager::Instance().GetTime();

	textureActivaID = textureInit1ID;

	glm::vec3 lightPos = glm::vec3(10.0, 10.0, -10.0);

	shadowBox = new ShadowBox(-lightPos, camera.get(), 15.0f, 0.1f, 45.0f);

	while (psi) {
		currTime = TimeManager::Instance().GetTime();
		if(currTime - lastTime < 0.016666667){
			glfwPollEvents();
			continue;
		}
		lastTime = currTime;
		TimeManager::Instance().CalculateFrameRate(true);
		deltaTime = TimeManager::Instance().DeltaTime;
		psi = processInput(true);

		std::map<std::string, bool> collisionDetection;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
				(float) screenWidth / (float) screenHeight, 0.01f, 100.0f);

		if(modelSelected == 1){
			axis = glm::axis(glm::quat_cast(modelMatrixThrantaClass));
			angleTarget = glm::angle(glm::quat_cast(modelMatrixThrantaClass));
			target = modelMatrixThrantaClass[3];
		}
		else{
			axis = glm::axis(glm::quat_cast(modelMatrixThrantaClass));
			angleTarget = glm::angle(glm::quat_cast(modelMatrixThrantaClass));
			target = modelMatrixThrantaClass[3];
		}

		if(std::isnan(angleTarget))
			angleTarget = 0.0;
		if(axis.y < 0)
			angleTarget = -angleTarget;
		if(modelSelected == 1)
			angleTarget -= glm::radians(90.0f);
		camera->setCameraTarget(target);
		camera->setAngleTarget(angleTarget);
		camera->updateCamera();
		glm::mat4 view = camera->getViewMatrix();

		shadowBox->update(screenWidth, screenHeight);
		glm::vec3 centerBox = shadowBox->getCenter();

		// Projection light shadow mapping
		glm::mat4 lightProjection = glm::mat4(1.0f), lightView = glm::mat4(1.0f);
		glm::mat4 lightSpaceMatrix;
		lightProjection[0][0] = 2.0f / shadowBox->getWidth();
		lightProjection[1][1] = 2.0f / shadowBox->getHeight();
		lightProjection[2][2] = -2.0f / shadowBox->getLength();
		lightProjection[3][3] = 1.0f;
		lightView = glm::lookAt(centerBox, centerBox + glm::normalize(-lightPos), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
		shaderDepth.setMatrix4("lightSpaceMatrix", 1, false, glm::value_ptr(lightSpaceMatrix));

		// Settea la matriz de vista y projection al shader con solo color
		shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shader.setMatrix4("view", 1, false, glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con skybox
		shaderSkybox.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderSkybox.setMatrix4("view", 1, false,
				glm::value_ptr(glm::mat4(glm::mat3(view))));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderMulLighting.setMatrix4("projection", 1, false,
					glm::value_ptr(projection));
		shaderMulLighting.setMatrix4("view", 1, false,
				glm::value_ptr(view));
		shaderMulLighting.setMatrix4("lightSpaceMatrix", 1, false,
				glm::value_ptr(lightSpaceMatrix));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderTerrain.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderTerrain.setMatrix4("view", 1, false,
				glm::value_ptr(view));
		shaderTerrain.setMatrix4("lightSpaceMatrix", 1, false,
				glm::value_ptr(lightSpaceMatrix));
		// Settea la matriz de vista y projection al shader para el fountain
		/*shaderParticlesFountain.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderParticlesFountain.setMatrix4("view", 1, false,
				glm::value_ptr(view));*/

		/*******************************************
		 * Propiedades de neblina
		 *******************************************/
		shaderMulLighting.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.5, 0.5, 0.4)));
		shaderTerrain.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.5, 0.5, 0.4)));
		shaderSkybox.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.5, 0.5, 0.4)));

		/*******************************************
		 * Propiedades Luz direccional
		 *******************************************/
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMulLighting.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-0.707106781, -0.707106781, 0.0)));

		shaderTerrain.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderTerrain.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderTerrain.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
		shaderTerrain.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderTerrain.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-0.707106781, -0.707106781, 0.0)));

		/*******************************************
		 * Propiedades SpotLights
		 *******************************************/
		shaderMulLighting.setInt("spotLightCount", 0);
		shaderTerrain.setInt("spotLightCount", 0);

		/*******************************************
		 * Propiedades PointLights
		 *******************************************/
		shaderMulLighting.setInt("pointLightCount", lamp1Position.size() + lamp2Position.size());
		shaderTerrain.setInt("pointLightCount", lamp1Position.size() + lamp2Position.size());
		for(int i = 0; i < lamp1Position.size(); i++){
			glm::mat4 matrixAdjustLamp = glm::mat4(1.0);
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, lamp1Position[i]);
			matrixAdjustLamp = glm::rotate(matrixAdjustLamp, glm::radians(lamp1Orientation[i]), glm::vec3(0, 1, 0));
			matrixAdjustLamp = glm::scale(matrixAdjustLamp, glm::vec3(0.5));
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, glm::vec3(0.0, 10.35, 0));
			glm::vec3 lampPosition = glm::vec3(matrixAdjustLamp[3]);
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].position", glm::value_ptr(lampPosition));
			shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.02);
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].position", glm::value_ptr(lampPosition));
			shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0);
			shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09);
			shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.02);
		}
		for(int i = 0; i < lamp2Position.size(); i++){
			glm::mat4 matrixAdjustLamp = glm::mat4(1.0);
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, lamp2Position[i]);
			matrixAdjustLamp = glm::rotate(matrixAdjustLamp, glm::radians(lamp2Orientation[i]), glm::vec3(0, 1, 0));
			matrixAdjustLamp = glm::scale(matrixAdjustLamp, glm::vec3(1.0));
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, glm::vec3(0.75, 5.0, 0));
			glm::vec3 lampPosition = glm::vec3(matrixAdjustLamp[3]);
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].position", glm::value_ptr(lampPosition));
			shaderMulLighting.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].constant", 1.0);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].linear", 0.09);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].quadratic", 0.02);
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].position", glm::value_ptr(lampPosition));
			shaderTerrain.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].constant", 1.0);
			shaderTerrain.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].linear", 0.09);
			shaderTerrain.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].quadratic", 0.02);
		}

		/************Render de imagen de frente**************/
		if(!iniciaPartida){
			shaderTexture.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
			shaderTexture.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureActivaID);
			shaderTexture.setInt("outTexture", 0);
			boxIntro.render();
			glfwSwapBuffers(window);
			continue;
		}

		/*******************************************
		 * 1.- We render the depth buffer
		 *******************************************/
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// render scene from light's point of view
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glCullFace(GL_FRONT);
		prepareDepthScene();
		renderScene();
		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		/*******************************************
		 * Debug to view the texture view map
		 *******************************************/
		// reset viewport
		/*glViewport(0, 0, screenWidth, screenHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// render Depth map to quad for visual debugging
		shaderViewDepth.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
		shaderViewDepth.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));
		shaderViewDepth.setFloat("near_plane", near_plane);
		shaderViewDepth.setFloat("far_plane", far_plane);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		boxViewDepth.setScale(glm::vec3(2.0, 2.0, 1.0));
		boxViewDepth.render();*/

		/*******************************************
		 * 2.- We render the normal objects
		 *******************************************/
		glViewport(0, 0, screenWidth, screenHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		prepareScene();
		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		shaderMulLighting.setInt("shadowMap", 10);
		shaderTerrain.setInt("shadowMap", 10);
		renderSolidScene();

		/*******************************************
		 * Creacion de colliders
		 * IMPORTANT do this before interpolations
		 *******************************************/

		/*Ejemplo de collider para objeto no movible
		// Collider del aricraft
		glm::mat4 modelMatrixColliderAircraft = glm::mat4(modelMatrixAircraft);
		AbstractModel::OBB aircraftCollider;
		// Set the orientation of collider before doing the scale
		aircraftCollider.u = glm::quat_cast(modelMatrixAircraft);
		modelMatrixColliderAircraft = glm::scale(modelMatrixColliderAircraft,
				glm::vec3(1.0, 1.0, 1.0));
		modelMatrixColliderAircraft = glm::translate(
				modelMatrixColliderAircraft, modelAircraft.getObb().c);
		aircraftCollider.c = glm::vec3(modelMatrixColliderAircraft[3]);
		aircraftCollider.e = modelAircraft.getObb().e * glm::vec3(1.0, 1.0, 1.0);
		addOrUpdateColliders(collidersOBB, "aircraft", aircraftCollider, modelMatrixAircraft);*/

		/* Ejemplo de collider tipo SSB
		//Collider del la rock
		AbstractModel::SBB rockCollider;
		glm::mat4 modelMatrixColliderRock= glm::mat4(matrixModelRock);
		modelMatrixColliderRock = glm::scale(modelMatrixColliderRock, glm::vec3(1.0, 1.0, 1.0));
		modelMatrixColliderRock = glm::translate(modelMatrixColliderRock, modelRock.getSbb().c);
		rockCollider.c = glm::vec3(modelMatrixColliderRock[3]);
		rockCollider.ratio = modelRock.getSbb().ratio * 1.0;
		addOrUpdateColliders(collidersSBB, "rock", rockCollider, matrixModelRock);*/

		// Lamps1 colliders
		/*for (int i = 0; i < lamp1Position.size(); i++){
			AbstractModel::OBB lampCollider;
			glm::mat4 modelMatrixColliderLamp = glm::mat4(1.0);
			modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp, lamp1Position[i]);
			modelMatrixColliderLamp = glm::rotate(modelMatrixColliderLamp, glm::radians(lamp1Orientation[i]),
					glm::vec3(0, 1, 0));
			addOrUpdateColliders(collidersOBB, "lamp1-" + std::to_string(i), lampCollider, modelMatrixColliderLamp);
			// Set the orientation of collider before doing the scale
			lampCollider.u = glm::quat_cast(modelMatrixColliderLamp);
			modelMatrixColliderLamp = glm::scale(modelMatrixColliderLamp, glm::vec3(0.5, 0.5, 0.5));
			modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp, modelLamp1.getObb().c);
			lampCollider.c = glm::vec3(modelMatrixColliderLamp[3]);
			lampCollider.e = modelLamp1.getObb().e * glm::vec3(0.5, 0.5, 0.5);
			std::get<0>(collidersOBB.find("lamp1-" + std::to_string(i))->second) = lampCollider;
		}*/

		/*
		// Collider de TIE Fighter, Ejemplo de OBB con objeto movible
		AbstractModel::OBB ThrantaClassCollider;
		glm::mat4 modelMatrixThrantaClassCopy = glm::rotate(modelMatrixThrantaClass, glm::radians(-90.0f), glm::vec3(0, 1, 0));
		glm::mat4 modelmatrixColliderThrantaClass = glm::mat4(modelMatrixThrantaClassCopy);
		modelmatrixColliderThrantaClass = glm::rotate(modelMatrixThrantaClassCopy,
				glm::radians(-90.0f), glm::vec3(1, 0, 0));
		// Set the orientation of collider before doing the scale
		ThrantaClassCollider.u = glm::quat_cast(modelmatrixColliderThrantaClass);
		modelmatrixColliderThrantaClass = glm::scale(modelmatrixColliderThrantaClass, glm::vec3(0.2, 0.2, 0.2));
		modelmatrixColliderThrantaClass = glm::translate(modelmatrixColliderThrantaClass,
				glm::vec3(modelThrantaClass.getObb().c.x,
						modelThrantaClass.getObb().c.y,
						modelThrantaClass.getObb().c.z));
		ThrantaClassCollider.e = modelThrantaClass.getObb().e * glm::vec3(0.021, 0.021, 0.021) * glm::vec3(0.787401574, 0.787401574, 0.787401574);
		ThrantaClassCollider.c = glm::vec3(modelmatrixColliderThrantaClass[3]);
		addOrUpdateColliders(collidersOBB, "ThrantaClass", ThrantaClassCollider, modelMatrixThrantaClass);*/
	glm::mat4 modelMatrixThrantaClassCopy = glm::mat4(modelMatrixThrantaClass);
	glm::mat4 modelmatrixColliderThrantaClass;
	AbstractModel::OBB ThrantaClassCollider;
	switch (selectedShip){
	case 0:
		modelMatrixThrantaClassCopy = glm::rotate(modelMatrixThrantaClassCopy, glm::radians(-90.0f), glm::vec3(0, 1, 0));
		modelmatrixColliderThrantaClass = glm::mat4(modelMatrixThrantaClassCopy);
		modelmatrixColliderThrantaClass = glm::rotate(modelMatrixThrantaClassCopy,glm::radians(-90.0f), glm::vec3(1, 0, 0));
		// Set the orientation of collider before doing the scale
		ThrantaClassCollider.u = glm::quat_cast(modelmatrixColliderThrantaClass);
		modelmatrixColliderThrantaClass = glm::scale(modelmatrixColliderThrantaClass, glm::vec3(0.22, 0.22, 0.22));
		modelmatrixColliderThrantaClass = glm::translate(modelmatrixColliderThrantaClass,
				glm::vec3(modelTIEFighter.getObb().c.x,
						modelTIEFighter.getObb().c.y,
						modelTIEFighter.getObb().c.z));
		ThrantaClassCollider.e = modelThrantaClass.getObb().e * glm::vec3(0.22, 0.22, 0.22) * glm::vec3(0.787401574, 0.787401574, 0.787401574);
		ThrantaClassCollider.c = glm::vec3(modelmatrixColliderThrantaClass[3]);
		addOrUpdateColliders(collidersOBB, "ThrantaClass", ThrantaClassCollider, modelMatrixThrantaClass);
		break;
	case 1:
		modelMatrixThrantaClassCopy = glm::rotate(modelMatrixThrantaClassCopy, glm::radians(-90.0f), glm::vec3(0, 1, 0));
		modelmatrixColliderThrantaClass = glm::mat4(modelMatrixThrantaClassCopy);
		//modelmatrixColliderThrantaClass = glm::rotate(modelMatrixThrantaClassCopy,glm::radians(-90.0f), glm::vec3(1, 0, 0));
		// Set the orientation of collider before doing the scale
		ThrantaClassCollider.u = glm::quat_cast(modelmatrixColliderThrantaClass);
		modelmatrixColliderThrantaClass = glm::scale(modelmatrixColliderThrantaClass, glm::vec3(0.22, 0.22, 0.22));
		modelmatrixColliderThrantaClass = glm::translate(modelmatrixColliderThrantaClass,
				glm::vec3(modelTIEBomber.getObb().c.x,
						modelTIEBomber.getObb().c.y,
						modelTIEBomber.getObb().c.z));
		ThrantaClassCollider.e = modelThrantaClass.getObb().e * glm::vec3(0.22, 0.22, 0.22) * glm::vec3(0.787401574, 0.787401574, 0.787401574);
		ThrantaClassCollider.c = glm::vec3(modelmatrixColliderThrantaClass[3]);
		addOrUpdateColliders(collidersOBB, "ThrantaClass", ThrantaClassCollider, modelMatrixThrantaClass);
		break;
	case 2:
		modelMatrixThrantaClassCopy = glm::rotate(modelMatrixThrantaClassCopy, glm::radians(-90.0f), glm::vec3(0, 1, 0));
		modelmatrixColliderThrantaClass = glm::mat4(modelMatrixThrantaClassCopy);
		modelmatrixColliderThrantaClass = glm::rotate(modelMatrixThrantaClassCopy, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		// Set the orientation of collider before doing the scale
		ThrantaClassCollider.u = glm::quat_cast(modelmatrixColliderThrantaClass);
		modelmatrixColliderThrantaClass = glm::scale(modelmatrixColliderThrantaClass, glm::vec3(0.22, 0.22, 0.22));
		modelmatrixColliderThrantaClass = glm::translate(modelmatrixColliderThrantaClass,
				glm::vec3(modelTIEInterceptor.getObb().c.x,
						modelTIEInterceptor.getObb().c.y,
						modelTIEInterceptor.getObb().c.z));
		ThrantaClassCollider.e = modelThrantaClass.getObb().e * glm::vec3(0.22, 0.22, 0.22) * glm::vec3(0.787401574, 0.787401574, 0.787401574);
		ThrantaClassCollider.c = glm::vec3(modelmatrixColliderThrantaClass[3]);
		addOrUpdateColliders(collidersOBB, "ThrantaClass", ThrantaClassCollider, modelMatrixThrantaClass);
		break;
	case 3:
		modelMatrixThrantaClassCopy = glm::rotate(modelMatrixThrantaClassCopy, glm::radians(-90.0f), glm::vec3(0, 1, 0));
		modelmatrixColliderThrantaClass = glm::mat4(modelMatrixThrantaClassCopy);
		modelmatrixColliderThrantaClass = glm::rotate(modelMatrixThrantaClassCopy, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		// Set the orientation of collider before doing the scale
		ThrantaClassCollider.u = glm::quat_cast(modelmatrixColliderThrantaClass);
		modelmatrixColliderThrantaClass = glm::scale(modelmatrixColliderThrantaClass, glm::vec3(0.0022, 0.0022, 0.0022));
		modelmatrixColliderThrantaClass = glm::translate(modelmatrixColliderThrantaClass,
				glm::vec3(modelAWing.getObb().c.x,
						modelAWing.getObb().c.y,
						modelAWing.getObb().c.z));
		ThrantaClassCollider.e = modelThrantaClass.getObb().e * glm::vec3(0.0022, 0.0022, 0.0022) * glm::vec3(0.787401574, 0.787401574, 0.787401574);
		ThrantaClassCollider.c = glm::vec3(modelmatrixColliderThrantaClass[3]);
		addOrUpdateColliders(collidersOBB, "ThrantaClass", ThrantaClassCollider, modelMatrixThrantaClass);
		break;
	default:
		modelMatrixThrantaClassCopy = glm::rotate(modelMatrixThrantaClassCopy, glm::radians(-90.0f), glm::vec3(0, 1, 0));
		modelmatrixColliderThrantaClass = glm::mat4(modelMatrixThrantaClassCopy);
		modelmatrixColliderThrantaClass = glm::rotate(modelMatrixThrantaClassCopy, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		// Set the orientation of collider before doing the scale
		ThrantaClassCollider.u = glm::quat_cast(modelmatrixColliderThrantaClass);
		modelmatrixColliderThrantaClass = glm::scale(modelmatrixColliderThrantaClass, glm::vec3(0.22, 0.22, 0.22));
		modelmatrixColliderThrantaClass = glm::translate(modelmatrixColliderThrantaClass,
				glm::vec3(modelTIEFighter.getObb().c.x,
						modelTIEFighter.getObb().c.y,
						modelTIEFighter.getObb().c.z));
		ThrantaClassCollider.e = modelThrantaClass.getObb().e * glm::vec3(0.22, 0.22, 0.22) * glm::vec3(0.787401574, 0.787401574, 0.787401574);
		ThrantaClassCollider.c = glm::vec3(modelmatrixColliderThrantaClass[3]);
		addOrUpdateColliders(collidersOBB, "ThrantaClass", ThrantaClassCollider, modelMatrixThrantaClass);
		break;
	}

		// Collider de Asteroid, Ejemplo de OBB con objeto movible
		AbstractModel::OBB AsteroidCollider;
		glm::mat4 modelMatrixAsteroidBody = glm::rotate(modelMatrixAsteroid, glm::radians(-90.0f), glm::vec3(0, 1, 0));
		glm::mat4 modelmatrixColliderAsteroid = glm::mat4(modelMatrixAsteroidBody);
		modelmatrixColliderAsteroid = glm::rotate(modelMatrixAsteroidBody,
				glm::radians(-90.0f), glm::vec3(1, 0, 0));
		// Set the orientation of collider before doing the scale
		AsteroidCollider.u = glm::quat_cast(modelmatrixColliderAsteroid);
		modelmatrixColliderAsteroid = glm::scale(modelmatrixColliderAsteroid, glm::vec3(0.021, 0.021, 0.021));
		modelmatrixColliderAsteroid = glm::translate(modelmatrixColliderAsteroid,
				glm::vec3(modelAsteroid.getObb().c.x,
						modelAsteroid.getObb().c.y,
						modelAsteroid.getObb().c.z));
		AsteroidCollider.e = modelAsteroid.getObb().e * glm::vec3(0.021, 0.021, 0.021) * glm::vec3(0.787401574, 0.787401574, 0.787401574);
		AsteroidCollider.c = glm::vec3(modelmatrixColliderAsteroid[3]);
		addOrUpdateColliders(collidersOBB, "Asteroid", AsteroidCollider, modelMatrixAsteroid);

		/*******************************************
		 * Render de colliders
		 *******************************************/
		for (std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator it =
				collidersOBB.begin(); it != collidersOBB.end(); it++) {
			glm::mat4 matrixCollider = glm::mat4(1.0);
			matrixCollider = glm::translate(matrixCollider, std::get<0>(it->second).c);
			matrixCollider = matrixCollider * glm::mat4(std::get<0>(it->second).u);
			matrixCollider = glm::scale(matrixCollider, std::get<0>(it->second).e * 2.0f);
			boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
			boxCollider.enableWireMode();
			boxCollider.render(matrixCollider);
		}

		for (std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
				collidersSBB.begin(); it != collidersSBB.end(); it++) {
			glm::mat4 matrixCollider = glm::mat4(1.0);
			matrixCollider = glm::translate(matrixCollider, std::get<0>(it->second).c);
			matrixCollider = glm::scale(matrixCollider, glm::vec3(std::get<0>(it->second).ratio * 2.0f));
			sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
			sphereCollider.enableWireMode();
			sphereCollider.render(matrixCollider);
		}

		/**********Render de transparencias***************/
		renderAlphaScene();

		/*********************Prueba de colisiones****************************/
		for (std::map<std::string,
			std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>>::iterator it =
			collidersSBB.begin(); it != collidersSBB.end(); it++) {
			bool isCollision = false;
			for (std::map<std::string,
				std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>>::iterator jt =
				collidersSBB.begin(); jt != collidersSBB.end(); jt++) {
				if (it != jt && testSphereSphereIntersection(
					std::get<0>(it->second), std::get<0>(jt->second))) {
					std::cout << "Hay collision entre " << it->first <<
						" y el modelo " << jt->first << std::endl;
					isCollision = true;
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		for (std::map<std::string,
			std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::iterator it =
			collidersOBB.begin(); it != collidersOBB.end(); it++) {
			bool isColision = false;
			for (std::map<std::string,
				std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::iterator jt =
				collidersOBB.begin(); jt != collidersOBB.end(); jt++) {
				if (it != jt && 
					testOBBOBB(std::get<0>(it->second), std::get<0>(jt->second))) {
					std::cout << "Hay colision entre " << it->first << " y el modelo" <<
						jt->first << std::endl;
					isColision = true;
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isColision);
		}

		for (std::map<std::string,
			std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>>::iterator it =
			collidersSBB.begin(); it != collidersSBB.end(); it++) {
			bool isCollision = false;
			for (std::map<std::string,
				std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::iterator jt =
				collidersOBB.begin(); jt != collidersOBB.end(); jt++) {
				if (testSphereOBox(std::get<0>(it->second), std::get<0>(jt->second))) {
					std::cout << "Hay colision del " << it->first << " y el modelo" <<
						jt->first << std::endl;
					isCollision = true;
					addOrUpdateCollisionDetection(collisionDetection, jt->first, true);
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		std::map<std::string, bool>::iterator itCollision;
		for (itCollision = collisionDetection.begin(); 
			itCollision != collisionDetection.end(); itCollision++) {
			std::map<std::string, std::tuple<AbstractModel::SBB, 
				glm::mat4, glm::mat4>>::iterator sbbBuscado = 
				collidersSBB.find(itCollision->first);
			std::map<std::string, std::tuple<AbstractModel::OBB,
				glm::mat4, glm::mat4>>::iterator obbBuscado =
				collidersOBB.find(itCollision->first);
			if (sbbBuscado != collidersSBB.end()) {
				if (!itCollision->second) 
					addOrUpdateColliders(collidersSBB, itCollision->first);
			}
			if (obbBuscado != collidersOBB.end()) {
				if (!itCollision->second) 
					addOrUpdateColliders(collidersOBB, itCollision->first);
				else {
					if (itCollision->first.compare("mayow") == 0)
						modelMatrixThrantaClass = std::get<1>(obbBuscado->second);
					if (itCollision->first.compare("dart") == 0)
						modelMatrixThrantaClass = std::get<1>(obbBuscado->second);
				}
			}
		}

		/* Ejemplo del rayo de MayOW
		glm::mat4 modelMatrixRayMay = glm::mat4(modelMatrixThrantaClass);
		modelMatrixRayMay = glm::translate(modelMatrixRayMay, glm::vec3(0, 1, 0));
		float maxDistanceRay = 10.0;
		glm::vec3 rayDirection = modelMatrixRayMay[2];
		glm::vec3 ori = modelMatrixRayMay[3];
		glm::vec3 rmd = ori + rayDirection * (maxDistanceRay / 2.0f);
		glm::vec3 targetRay = ori + rayDirection * maxDistanceRay;
		modelMatrixRayMay[3] = glm::vec4(rmd, 1.0);
		modelMatrixRayMay = glm::rotate(modelMatrixRayMay, glm::radians(90.0f), 
			glm::vec3(1, 0, 0));
		modelMatrixRayMay = glm::scale(modelMatrixRayMay, 
			glm::vec3(0.05, maxDistanceRay, 0.05));
		rayModel.render(modelMatrixRayMay);*/

		/*std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>>::
			iterator itSBB;
		for (itSBB = collidersSBB.begin(); itSBB != collidersSBB.end(); itSBB++) {
			float tRint;
			if (raySphereIntersect(ori, targetRay, rayDirection,
				std::get<0>(itSBB->second), tRint)) {
				std::cout << "Collision del rayo con el modelo " << itSBB->first 
				<< std::endl;
			}
		}*/

		/*std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::
			iterator itOBB;
		for (itOBB = collidersOBB.begin(); itOBB != collidersOBB.end(); itOBB++) {
			if (testRayOBB(ori, targetRay, std::get<0>(itOBB->second))) {
				std::cout << "Collision del rayo con el modelo " << itOBB->first
					<< std::endl;
			}
		}*/

		// Esto es para ilustrar la transformacion inversa de los coliders
		/*glm::vec3 cinv = glm::inverse(mayowCollider.u) * glm::vec4(rockCollider.c, 1.0);
		glm::mat4 invColliderS = glm::mat4(1.0);
		invColliderS = glm::translate(invColliderS, cinv);
		invColliderS =  invColliderS * glm::mat4(mayowCollider.u);
		invColliderS = glm::scale(invColliderS, glm::vec3(rockCollider.ratio * 2.0, rockCollider.ratio * 2.0, rockCollider.ratio * 2.0));
		sphereCollider.setColor(glm::vec4(1.0, 1.0, 0.0, 1.0));
		sphereCollider.enableWireMode();
		sphereCollider.render(invColliderS);
		glm::vec3 cinv2 = glm::inverse(mayowCollider.u) * glm::vec4(mayowCollider.c, 1.0);
		glm::mat4 invColliderB = glm::mat4(1.0);
		invColliderB = glm::translate(invColliderB, cinv2);
		invColliderB = glm::scale(invColliderB, mayowCollider.e * 2.0f);
		boxCollider.setColor(glm::vec4(1.0, 1.0, 0.0, 1.0));
		boxCollider.enableWireMode();
		boxCollider.render(invColliderB);
		// Se regresa el color blanco
		sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
		boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));*/

		glfwSwapBuffers(window);

		/****************************+
		 * Open AL sound data
		 */
		/*source0Pos[0] = modelMatrixFountain[3].x;
		source0Pos[1] = modelMatrixFountain[3].y;
		source0Pos[2] = modelMatrixFountain[3].z;
		alSourcefv(source[0], AL_POSITION, source0Pos);

		source1Pos[0] = modelMatrixGuardian[3].x;
		source1Pos[1] = modelMatrixGuardian[3].y;
		source1Pos[2] = modelMatrixGuardian[3].z;
		alSourcefv(source[1], AL_POSITION, source1Pos);
		
		source2Pos[0] = modelMatrixDart[3].x;
		source2Pos[1] = modelMatrixDart[3].y;
		source2Pos[2] = modelMatrixDart[3].z;
		alSourcefv(source[2], AL_POSITION, source2Pos);

		// Listener for the Thris person camera
		listenerPos[0] = modelMatrixMayow[3].x;
		listenerPos[1] = modelMatrixMayow[3].y;
		listenerPos[2] = modelMatrixMayow[3].z;
		alListenerfv(AL_POSITION, listenerPos);

		glm::vec3 upModel = glm::normalize(modelMatrixMayow[1]);
		glm::vec3 frontModel = glm::normalize(modelMatrixMayow[2]);

		listenerOri[0] = frontModel.x;
		listenerOri[1] = frontModel.y;
		listenerOri[2] = frontModel.z;
		listenerOri[3] = upModel.x;
		listenerOri[4] = upModel.y;
		listenerOri[5] = upModel.z;

		// Listener for the First person camera
		// listenerPos[0] = camera->getPosition().x;
		// listenerPos[1] = camera->getPosition().y;
		// listenerPos[2] = camera->getPosition().z;
		// alListenerfv(AL_POSITION, listenerPos);
		// listenerOri[0] = camera->getFront().x;
		// listenerOri[1] = camera->getFront().y;
		// listenerOri[2] = camera->getFront().z;
		// listenerOri[3] = camera->getUp().x;
		// listenerOri[4] = camera->getUp().y;
		// listenerOri[5] = camera->getUp().z;
		alListenerfv(AL_ORIENTATION, listenerOri);

		for(unsigned int i = 0; i < sourcesPlay.size(); i++){
			if(sourcesPlay[i]){
				sourcesPlay[i] = false;
				alSourcePlay(source[i]);
			}
		}
		*/
	}
}

int main(int argc, char **argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}