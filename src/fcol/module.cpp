//
//  module.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-08-22.
//
//

#include "module.hpp"
#include "video.hpp"
#include "collector.hpp"

using namespace fcol;

FCOL_SINGLETON_INLINE_IMPLEMENTATION_CODE(Module)

void Module::setupParams(){
    parameters.setName("fcol");
    parameters.add(autoSave.set("autoSave", true));

    // sub module params
    Collector::instance()->setupParams();
    parameters.add(Collector::instance()->parameters);

    Video::instance()->setupParams();
    parameters.add(Video::instance()->parameters);
}

void Module::setup(){
    // GUI
    gui.setup(parameters);
    gui.loadFromFile("params.xml");
    
    // load/setup
    // modules::Mask::instance()->setup();

    //cam.initGrabber(800, 600);

    Collector::instance()->setup();

    ofAddListener(Video::instance()->newFrameEvent, this, &Module::onNewVideoFrame);
}

void Module::destroy(){
    if(autoSave){
        gui.saveToFile("params.xml");
    }

    ofRemoveListener(Video::instance()->newFrameEvent, this, &Module::onNewVideoFrame);
    Video::delete_instance();
    Collector::delete_instance();
}

void Module::update(){
    Video::instance()->update();
    Collector::instance()->update();
}

void Module::draw(){
    ofPushMatrix();
    ofScale(0.5f, 0.5f);

    if(Collector::instance()->parDraw){
        Collector::instance()->draw();
        ofTranslate(Collector::instance()->getFbo().getWidth(), 0);
    }

    if(Video::instance()->parDraw){
        Video::instance()->draw();
        Collector::instance()->getTracker().getImageMesh().drawWireframe();
    }

    ofPopMatrix();
    gui.draw();
}

void Module::dragEvent(ofDragInfo dragInfo){
    ofLogVerbose() << "Got file: " << dragInfo.files[0];
    Video::instance()->load(dragInfo.files[0]);
}

void Module::onNewVideoFrame(ofVideoPlayer & player){
    Collector::instance()->addFrame(player);
}