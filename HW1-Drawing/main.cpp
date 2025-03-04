//#####################################################################
// Main
// CS3451 Computer Graphics Starter Code
// Contact: Bo Zhu (bo.zhu@gatech.edu)
//#####################################################################
#include <iostream>
#include <random>

#include "OpenGLMesh.h"
#include "OpenGLCommon.h"
#include "OpenGLWindow.h"
#include "OpenGLViewer.h"

/////////////////////////////////////////////////////////////////////
//// TODO: put your name in the string               
/////////////////////////////////////////////////////////////////////

const std::string author="MATTHEW KISTNER";

/////////////////////////////////////////////////////////////////////
//// These are helper functions we created to generate circles and triangles by testing whether a point is inside the shape or not.
//// They can be used in the paintGrid function as "if the pixel is inside, draw some color; else skip."
//// You may create your own functions to draw your own shapes

//// The paintGrid function is implemented as a GLSL fragment shader. 
//// The GLSL grammar is C-style, and if you are curious about its full picture (which we will start to learn the details in Week 3), 
//// you may find more information on https://www.khronos.org/files/opengl43-quick-reference-card.pdf (Page 6 - 7 would probably be helpful!)
//// You don't need advanced GLSL features for this assignment (reading the starter code should be enough).
//// You can also test it (copy the whole string) in Shadertoy: https://www.shadertoy.com/new    
/////////////////////////////////////////////////////////////////////

const std::string draw_pixels=To_String(
const float M_PI=3.1415926535; 

// The side length of the minimum unit (or the new "pixels")
const float PIXEL_SIZE=10.; 

// To check if a point is inside a circle
bool inCircle(vec2 p, vec2 center, float radius) {
	vec2 to_center=p - center;
	if (dot(to_center, to_center) < radius * radius) {
		return true;
	}
	return false;
}

// To check if a point is inside a triangle
bool inTriangle(vec2 p, vec2 p1, vec2 p2, vec2 p3) {
	if (dot(cross(vec3(p2 - p1, 0), vec3(p - p1, 0)), cross(vec3(p2 - p1, 0), vec3(p3 - p1, 0))) >= 0. &&
		dot(cross(vec3(p3 - p2, 0), vec3(p - p2, 0)), cross(vec3(p3 - p2, 0), vec3(p1 - p2, 0))) >= 0. &&
		dot(cross(vec3(p1 - p3, 0), vec3(p - p3, 0)), cross(vec3(p1 - p3, 0), vec3(p2 - p3, 0))) >= 0.) {
		return true;
	}
	return false;
}

//To check if a point is inside a rectangle
bool inRectangle(vec2 p, vec2 p1, vec2 p2, vec2 p3, vec2 p4) {
	if (dot(cross(vec3(p2 - p1, 0), vec3(p - p1, 0)), cross(vec3(p2 - p1, 0), vec3(p3 - p2, 0))) >= 0. &&
		dot(cross(vec3(p3 - p2, 0), vec3(p - p2, 0)), cross(vec3(p3 - p2, 0), vec3(p4 - p3, 0))) >= 0. &&
		dot(cross(vec3(p4 - p3, 0), vec3(p - p3, 0)), cross(vec3(p4 - p3, 0), vec3(p1 - p4, 0))) >= 0. &&
		dot(cross(vec3(p1 - p4, 0), vec3(p - p4, 0)), cross(vec3(p1 - p4, 0), vec3(p2 - p1, 0))) >= 0.) {
		return true;
	}
	return false;
}

// To convert from Polar Coordinates to Cartesian coordinates
vec2 polar2cart(float angle, float length) {
	return vec2(cos(angle) * length, sin(angle) * length);
}

/////////////////////////////////////////////////////////////////////////
// Feel free to add more functions if needed!                          
/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
// TODO: replace the code below with your own code                 //
// Useful variables:											   //
// iTime: the passed seconds from the start of the program         //
// iResolution: the size of the window (default: 1280*960)         //
/////////////////////////////////////////////////////////////////////

// Return the rgba color of the grid at position (x, y) 
vec4 paintGrid(float x, float y) {
	vec2 center=vec2(iResolution / PIXEL_SIZE / 2.); // window center

	bool inRoad = inRectangle(vec2(x, y), vec2(128, 0), vec2(128, 20), vec2(0, 20), vec2(0,0));

	bool inRoadLine1 = inRectangle(vec2(x, y), vec2(96, 8), vec2(96, 12), vec2(80, 12), vec2(80,8));
	bool inRoadLine2 = inRectangle(vec2(x, y), vec2(128, 8), vec2(128, 12), vec2(112, 12), vec2(112,8));
	bool inRoadLine3 = inRectangle(vec2(x, y), vec2(64, 8), vec2(64, 12), vec2(48, 12), vec2(48,8));
	bool inRoadLine4 = inRectangle(vec2(x, y), vec2(32, 8), vec2(32, 12), vec2(16, 12), vec2(16,8));

	bool inReckBody = inRectangle(vec2(x,y), vec2(96,25), vec2(96, 40), vec2(32, 40), vec2(32, 25));
	bool inReckBody1 = inRectangle(vec2(x, y), vec2(50, 40), vec2(50, 50), vec2(49, 50), vec2(49, 40));
	bool inReckTop = inRectangle(vec2(x, y), vec2(78, 40), vec2(78, 50), vec2(64, 50), vec2(64, 40));
	bool inReckTop2 = inRectangle(vec2(x,y), vec2(64, 49), vec2(64, 50), vec2(50, 50), vec2(50, 49));
	bool inRumbleSeat = inRectangle(vec2(x,y), vec2(93, 40), vec2(93, 47.5), vec2(92, 47.5), vec2(92, 40));
	bool inCushion = inRectangle(vec2(x,y), vec2(91, 40), vec2(91, 46.5), vec2(90, 46.5), vec2(90, 40));

	bool inWheel1 = inCircle(vec2(x, y), vec2(43, 22), 7.);
	bool inWheel1i = inCircle(vec2(x,y), vec2(43, 22), 5.);
	bool inWheel2 = inCircle(vec2(x, y), vec2(85, 22), 7.);
	bool inWheel2i = inCircle(vec2(x,y), vec2(85, 22), 5.);

	bool inFlagPole = inRectangle(vec2(x,y), vec2(32.5, 40), vec2(32.5, 51), vec2(32, 51), vec2(32, 40));
	bool inFlag = inTriangle(vec2(x,y), vec2(32.5, 46), vec2(44, 48.5), vec2(32.5, 51));

	bool inG1 = inRectangle(vec2(x,y), vec2(64, 37), vec2(64,38), vec2(58, 38), vec2(58, 37));
	bool inG2 = inRectangle(vec2(x,y), vec2(59,30), vec2(59, 37), vec2(58, 37), vec2(58, 30));
	bool inG3 = inRectangle(vec2(x,y), vec2(64, 30), vec2(64, 31), vec2(58, 31), vec2(58, 30));
	bool inG4 = inRectangle(vec2(x,y), vec2(64, 31), vec2(64, 33), vec2(63, 33), vec2(63, 31));
	bool inT1 = inRectangle(vec2(x,y), vec2(70, 33), vec2(70, 34), vec2(62, 34), vec2(62, 33));
	bool inT2 = inRectangle(vec2(x,y), vec2(67, 27), vec2(67, 33), vec2(66, 33), vec2(66, 27));

	if (inWheel1i || inWheel2i || inReckTop || inReckTop2 || inCushion || inG1 || inG2 || inG3 || inG4 || inT1 || inT2) {
		return vec4(vec3(255, 255, 255) / 255., 1.);
	}
	else if (inWheel1 || inWheel2) {
		return vec4(vec3(0, 0, 0) / 255., 1.);
	}
	else if (inReckBody || inReckBody1 || inRumbleSeat) {
		return vec4(vec3(179, 163, 105) / 255., 1.);
	}
	else if (inFlag) {
		return vec4(vec3(234, 170, 0) / 255., 1.);
	}
	else if (inFlagPole) {
		return vec4(vec3(192, 192, 192) / 255., 1.);
	}
	else if (inRoadLine1 || inRoadLine2 || inRoadLine3 || inRoadLine4) {
		return vec4(vec3(249, 215, 28) / 255., 1.);
	}
	else if (inRoad) {
		return vec4(vec3(0, 0, 0) / 255., 1.);
	}
	else {
		return vec4(vec3(0, 0, 165) / 255., 1.);
	}
}

// The function called in the fragment shader
void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
	// To divide the screen into the grids for painting!
	fragColor=paintGrid(floor(fragCoord.x / PIXEL_SIZE), floor(fragCoord.y / PIXEL_SIZE));
}

);

class ScreenDriver : public OpenGLViewer
{
	OpenGLScreenCover* screen_cover=nullptr;
	clock_t startTime=clock();

public:
	virtual void Initialize()
	{
		OpenGLViewer::Initialize();
	}

	//// Initialize the screen covering mesh and shaders
	virtual void Initialize_Data()
	{
		OpenGLShaderLibrary::Instance()->Create_Screen_Shader(draw_pixels, "shaderToy");
		screen_cover=Add_Interactive_Object<OpenGLScreenCover>();
		Set_Polygon_Mode(screen_cover, PolygonMode::Fill);
		Uniform_Update();

		screen_cover->Set_Data_Refreshed();
		screen_cover->Initialize();
		screen_cover->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("shaderToy"));
	}

	//// Update the uniformed variables used in shader
	void Uniform_Update()
	{
		screen_cover->setResolution((float)Win_Width(), (float)Win_Height());
		screen_cover->setTime(GLfloat(clock() - startTime) / CLOCKS_PER_SEC);
	}

	//// Go to next frame 
	virtual void Toggle_Next_Frame()
	{
		Uniform_Update();
		OpenGLViewer::Toggle_Next_Frame();
	}

	////Keyboard interaction
	virtual void Initialize_Common_Callback_Keys()
	{
		OpenGLViewer::Initialize_Common_Callback_Keys();
	}

	virtual void Run()
	{
		OpenGLViewer::Run();
	}
};

int main(int argc,char* argv[])
{
	if(author==""){std::cerr<<"***** The author name is not specified. Please put your name in the author string first. *****"<<std::endl;return 0;}
	else std::cout<<"Assignment 1 demo by "<<author<<" started"<<std::endl;
	
	ScreenDriver driver;
	driver.Initialize();
	driver.Run();	
}

