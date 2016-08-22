//
//  module.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-08-22.
//
//

#include "module.hpp"

using namespace fcol;

FCOL_SINGLETON_INLINE_IMPLEMENTATION_CODE(Module)

void Module::setupParams(){
    parameters.setName("fcol");
    parameters.add(autoSave.set("autoSave", true));
    parameters.add(webcam.set("webcam", true));
    
    // modules::Mask::instance()->setupParams();
    //parameters.add(modules::Mask::instance()->parameters);
}

void Module::setup(){
    // GUI
    gui.setup(parameters);
    gui.loadFromFile("params.xml");
    
    // load/setup
    // modules::Mask::instance()->setup();

    if(webcam)
        vidGrabber.initGrabber(640, 480);
}

void Module::destroy(){
    if(autoSave){
        gui.saveToFile("params.xml");
    }
}

void Module::update(){
    if(webcam)
        vidGrabber.update();
    
    // if(vidGrabber.isFrameNew())

}

void Module::draw(){
    if(webcam)
        vidGrabber.draw(0.0f, 0.0f);
    
    gui.draw();
}

void Module::threadedFunction(){
}