//
//  crop_history.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-08-30.
//
//

#include "crop_history.hpp"

using namespace fcol::out;

FCOL_SINGLETON_INLINE_IMPLEMENTATION_CODE(CropHistory)

void CropHistory::setupParams(){
    parameters.setName("CropHistory");
    parameters.add(enabled.set("enabled", false));
    parameters.add(numberOfFrames.set("numberOfFrames", 6, 0, 20));
}

void CropHistory::setup(EyeCrop* eyeCrop){
    this->eyeCrop = eyeCrop ? eyeCrop : EyeCrop::instance();
    
    if(!this->eyeCrop->getFbo().isAllocated()){
        ofLogWarning() << "CropHistory::setup -- eyeCrop has not been setup yet.";
    }

    fbo.allocate(this->eyeCrop->getFbo().getWidth(), this->eyeCrop->getFbo().getHeight() * numberOfFrames);
    fbo2.allocate(fbo.getWidth(), fbo.getHeight()-this->eyeCrop->getFbo().getHeight());

    registerCallbacks();
}

void CropHistory::destroy(){
    registerCallbacks(false);
    this->eyeCrop = NULL;
    fbo.clear();
    fbo2.clear();
}

void CropHistory::draw(){
    // draw crop fbo
    ofSetColor(255);
    fbo.draw(0,0);
}

void CropHistory::registerCallbacks(bool _register){
    if(eyeCrop){
        if(_register){
            ofAddListener(eyeCrop->eyeCropEvent, this, &CropHistory::onEyeCrop);
        } else {
            ofRemoveListener(eyeCrop->eyeCropEvent, this, &CropHistory::onEyeCrop);
        }
    }
}

void CropHistory::onEyeCrop(ofFbo& eyeCropFbo){
    if(!enabled) return;

    // draw main fbo to secondary (pretty much like a backup)
    fbo2.begin();
    fbo.draw(0, 0);
    fbo2.end();

    // update main fbo
    fbo.begin();
    // redraw secondary to primary, but shift it down one frame
    fbo2.draw(0, eyeCropFbo.getHeight());
    // draw new frame at the top
    eyeCropFbo.draw(0,0);
    fbo.end();
}