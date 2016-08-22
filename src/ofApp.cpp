#include "ofApp.h"

#include "fcol/module.hpp"

//--------------------------------------------------------------
void ofApp::setup(){
    ofLogVerbose() << "Redirect logging to log.txt";
    ofLogToFile("log.txt", true);
    
    ofSetFrameRate(60);
    dt = 1.0f / ofGetFrameRate();
    
    fcol::Module::instance()->setupParams();    
    fcol::Module::instance()->setup();
}

//--------------------------------------------------------------
void ofApp::update(){
    fcol::Module::instance()->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    fcol::Module::instance()->draw();
}

//--------------------------------------------------------------
void ofApp::exit(ofEventArgs &args){
    fcol::Module::delete_instance();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}