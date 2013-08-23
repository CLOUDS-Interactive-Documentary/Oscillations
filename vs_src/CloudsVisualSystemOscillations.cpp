//
//  CloudsVisualSystemOscillations.cpp
//

#include "CloudsVisualSystemOscillations.h"

//#include "CloudsRGBDVideoPlayer.h"
//#ifdef AVF_PLAYER
//#include "ofxAVFVideoPlayer.h"
//#endif

//These methods let us add custom GUI parameters and respond to their events


#define NUMPOINTS 10000

void CloudsVisualSystemOscillations::selfSetupGui(){

	customGui = new ofxUISuperCanvas("Oscillations", gui);
	customGui->copyCanvasStyle(gui);
	customGui->copyCanvasProperties(gui);
	customGui->setName("Custom");
	customGui->setWidgetFontSize(OFX_UI_FONT_SMALL);
	
	customGui->addSlider("Precision", 1, 100, &precision);
	customGui->addSlider("CurveProgress", 0, 1, &curveProgress);
	customGui->addSlider("Height", 0, 10000, &curveHeight);
	customGui->addSlider("Width", 0, 10000, &curveWidth);
	customGui->addSlider("Depth", 0, 10000, &curveDepth);
	customGui->addSlider("Z Position", -1000, 1000, &curveZPos);
	customGui->addSlider("Speed", 0, 1, &speed);
    customGui->addSlider("Line Width", 0, 20, &lineWidth);
    
    
//    customGui = new ofxUISuperCanvas("Shader", gui);
//	customGui->copyCanvasStyle(gui);
//	customGui->copyCanvasProperties(gui);
//	customGui->setName("Lens Effects");
//	customGui->setWidgetFontSize(OFX_UI_FONT_SMALL);
//    
//    customGui->addSlider("Chromatic Abberation", 0, 1, &chromaAbbr);
//    customGui->addSlider("Chromatic Abberation", 0, 1, &chromaAbbr);

    
//	customGui->addButton("Custom Button", false);
//	customGui->addToggle("Custom Toggle", &customToggle);
	
	ofAddListener(customGui->newGUIEvent, this, &CloudsVisualSystemOscillations::selfGuiEvent);
	
	guis.push_back(customGui);
	guimap[customGui->getName()] = customGui;
}

void CloudsVisualSystemOscillations::selfGuiEvent(ofxUIEventArgs &e){
	if(e.widget->getName() == "Custom Button"){
		cout << "Button pressed!" << endl;
	}
}

//Use system gui for global or logical settings, for exmpl
void CloudsVisualSystemOscillations::selfSetupSystemGui(){
	
}

void CloudsVisualSystemOscillations::guiSystemEvent(ofxUIEventArgs &e){
	
}
//use render gui for display settings, like changing colors
void CloudsVisualSystemOscillations::selfSetupRenderGui(){

}

void CloudsVisualSystemOscillations::guiRenderEvent(ofxUIEventArgs &e){
	
}

// selfSetup is called when the visual system is first instantiated
// This will be called during a "loading" screen, so any big images or
// geometry should be loaded here
void CloudsVisualSystemOscillations::selfSetup(){
    
    
    ofEnableAlphaBlending();
//	loadTestVideo();
    
    ofFloatColor zero = ofFloatColor(0,0,0);
    for (int i = 0; i < NUMPOINTS ; i++){
        mesh.addColor(zero);
        mesh.addVertex(ofPoint(0,0,0));
    }
    
    
    //TODO: Find way to update on every resize
    
    offsetX = offsetY = 0;
    
    crtShader.load(getVisualSystemDataPath() +"shaders/oscillationsShader");
    
	
//	someImage.loadImage( getVisualSystemDataPath() + "images/someImage.png";
	
}

// selfPresetLoaded is called whenever a new preset is triggered
// it'll be called right before selfBegin() and you may wish to
// refresh anything that a preset may offset, such as stored colors or particles
void CloudsVisualSystemOscillations::selfPresetLoaded(string presetPath){
	
}

// selfBegin is called when the system is ready to be shown
// this is a good time to prepare for transitions
// but try to keep it light weight as to not cause stuttering
void CloudsVisualSystemOscillations::selfBegin(){
	
}

//do things like ofRotate/ofTranslate here
//any type of transformation that doesn't have to do with the camera
void CloudsVisualSystemOscillations::selfSceneTransformation(){
	
}

//normal update call
void CloudsVisualSystemOscillations::selfUpdate(){
    
    
    width = ofGetWidth();
    height = ofGetHeight();
    
    offsetX += speed;
    
    float px, py, pz;
    
    ofFloatColor zero = ofFloatColor(0,0,0,0);
    ofFloatColor c = ofFloatColor(1,1,1,0.2);
    
    for (int i = 0; i < NUMPOINTS;  i++){
        mesh.setColor(i, (i/(NUMPOINTS*1.0))>curveProgress ? zero : c);
        px = cos((0.7+ 0.09 * 0.3 / width) * i * precision + offsetX);
        py = sin(i * precision + offsetY);
        pz = abs((-NUMPOINTS/2.0)+i)/(NUMPOINTS/2.0);   
        mesh.setVertex(i, ofPoint( -curveWidth/2.0 + px * curveWidth, -curveHeight/2.0 + py * curveHeight, -curveDepth/2.0 + curveZPos + pz * curveDepth));
    }
    
}
// selfDraw draws in 3D using the default ofEasyCamera
// you can change the camera by returning getCameraRef()
void CloudsVisualSystemOscillations::selfDraw(){
    glDisable(GL_DEPTH_TEST);
    ofSetColor(255, 255, 255, 30);
    mesh.setMode(OF_PRIMITIVE_LINE_LOOP);
    ofSetLineWidth(lineWidth);
	mesh.drawWireframe();
}

// draw any debug stuff here
void CloudsVisualSystemOscillations::selfDrawDebug(){
	
}
// or you can use selfDrawBackground to do 2D drawings that don't use the 3D camera
void CloudsVisualSystemOscillations::selfDrawBackground(){

	//turn the background refresh off
	//bClearBackground = false;
	
}


//Overriding the superclass
void CloudsVisualSystemOscillations::selfPostDraw(){
    //DO NOT CHANGE THIS LINE< REZA MADE THIS
//    crtShader.setUniformTexture("screen", fbo.getTextureReference(), fbo.getTextureReference().getTextureData().textureID);
    crtShader.begin();
//    crtShader.setUniform1i("screen", GL_TEXTURE0);
    crtShader.setUniformTexture("screen", getSharedRenderTarget(), 0 );
    crtShader.setUniform2f("resolution", float( getSharedRenderTarget().getWidth() ), (float) getSharedRenderTarget().getHeight());
    getSharedRenderTarget().draw(0,CloudsVisualSystem::getSharedRenderTarget().getHeight(),
													 CloudsVisualSystem::getSharedRenderTarget().getWidth(),
													 -CloudsVisualSystem::getSharedRenderTarget().getHeight());
    crtShader.end();

}
// this is called when your system is no longer drawing.
// Right after this selfUpdate() and selfDraw() won't be called any more
void CloudsVisualSystemOscillations::selfEnd(){
	
}
// this is called when you should clear all the memory and delet anything you made in setup
void CloudsVisualSystemOscillations::selfExit(){
	
}

//events are called when the system is active
//Feel free to make things interactive for you, and for the user!
void CloudsVisualSystemOscillations::selfKeyPressed(ofKeyEventArgs & args){
	
}
void CloudsVisualSystemOscillations::selfKeyReleased(ofKeyEventArgs & args){
	
}

void CloudsVisualSystemOscillations::selfMouseDragged(ofMouseEventArgs& data){
	
}

void CloudsVisualSystemOscillations::selfMouseMoved(ofMouseEventArgs& data){
	
}

void CloudsVisualSystemOscillations::selfMousePressed(ofMouseEventArgs& data){
	
}

void CloudsVisualSystemOscillations::selfMouseReleased(ofMouseEventArgs& data){
	
}