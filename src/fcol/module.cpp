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
#include "webcam.hpp"
#include "out/eye_crop.hpp"
#include "out/eye_file.hpp"

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
    
    Webcam::instance()->setupParams();
    parameters.add(Webcam::instance()->parameters);

    out::EyeCrop::instance()->setupParams();
    parameters.add(out::EyeCrop::instance()->parameters);

    out::EyeFile::instance()->setupParams();
    parameters.add(out::EyeFile::instance()->parameters);
}

void Module::setup(){
    // GUI
    gui.setup(parameters);
    gui.loadFromFile("params.xml");
    gui.add(videoSpeedResetButton.setup("reset video speed"));

    // setup submodules
    Collector::instance()->setup();
    Webcam::instance()->setup();
    out::EyeCrop::instance()->setup(Collector::instance());
    out::EyeFile::instance()->setup(Collector::instance(), out::EyeCrop::instance());

    // register callbacks
    ofAddListener(Video::instance()->newFrameEvent, this, &Module::onNewVideoFrame);
    ofAddListener(Webcam::instance()->newFrameEvent, this, &Module::onNewWebcamFrame);
    videoSpeedResetButton.addListener(this, &Module::onVideoSpeedResetButtonPressed);
}

void Module::destroy(){
    if(autoSave){
        gui.saveToFile("params.xml");
    }

    // unregister callbacks
    ofRemoveListener(Video::instance()->newFrameEvent, this, &Module::onNewVideoFrame);
    ofRemoveListener(Webcam::instance()->newFrameEvent, this, &Module::onNewWebcamFrame);

    // destroy submodules
    out::EyeFile::delete_instance();
    out::EyeCrop::delete_instance();

    Video::delete_instance();
    Webcam::delete_instance();
    Collector::delete_instance();
}

void Module::update(){
    Video::instance()->update();
    Webcam::instance()->update();
}

void Module::draw(){
    ofPushMatrix();
    ofScale(0.5f, 0.5f);

    out::EyeCrop::instance()->draw();
    ofTranslate(out::EyeCrop::instance()->getFbo().getWidth(), 0);

    if(Video::instance()->parDraw){
        Video::instance()->draw();
        Collector::instance()->getTracker().getImageMesh().drawWireframe();
        ofTranslate(Video::instance()->getPlayer().getWidth(), 0);
    }

    if(Webcam::instance()->parDraw){
        Webcam::instance()->draw();
        Collector::instance()->getTracker().getImageMesh().drawWireframe();
        ofTranslate(Webcam::instance()->getVideoGrabber().getWidth(),0);
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

void Module::onNewWebcamFrame(ofVideoGrabber & cam){
    Collector::instance()->addFrame(cam);
}

void Module::onVideoSpeedResetButtonPressed(){
    Video::instance()->parSpeed.set(1.0f);
}