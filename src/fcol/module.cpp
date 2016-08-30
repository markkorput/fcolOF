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
#include "out/crop_history.hpp"

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

    out::CropHistory::instance()->setupParams();
    parameters.add(out::CropHistory::instance()->parameters);
}

void Module::setup(){
    // GUI
    gui.setup(parameters);
    gui.loadFromFile("params.xml");
    gui.add(videoSpeedResetButton.setup("reset video speed"));

    // setup submodules
    Collector::instance()->setup();
    Webcam::instance()->setup(Collector::instance());
    Video::instance()->setup(Collector::instance());
    out::EyeCrop::instance()->setup(Collector::instance());
    out::EyeFile::instance()->setup(Collector::instance(), out::EyeCrop::instance());
    out::CropHistory::instance()->setup(out::EyeCrop::instance());

    videoSpeedResetButton.addListener(this, &Module::onVideoSpeedResetButtonPressed);
}

void Module::destroy(){
    if(autoSave){
        gui.saveToFile("params.xml");
    }

    // unregister callbacks
    videoSpeedResetButton.removeListener(this, &Module::onVideoSpeedResetButtonPressed);

    // destroy submodules
    Video::delete_instance();
    Webcam::delete_instance();
    out::EyeFile::delete_instance();
    out::CropHistory::delete_instance();
    out::EyeCrop::delete_instance();
    Collector::delete_instance();
}

void Module::update(){
    Video::instance()->update();
    Webcam::instance()->update();
}

void Module::draw(){
    ofPushMatrix();
    ofScale(0.5f, 0.5f);

    // draw cropper
    out::EyeCrop::instance()->draw();

    // draw crop history below
    if(out::CropHistory::instance()->enabled){
        ofPushMatrix();
        ofTranslate(0, out::EyeCrop::instance()->getFbo().getHeight());
        out::CropHistory::instance()->draw();
        ofPopMatrix();
    }

    // move to the right
    ofTranslate(out::EyeCrop::instance()->getFbo().getWidth(), 0);

    // draw video?
    if(Video::instance()->parDraw){
        Video::instance()->draw();
        Collector::instance()->getTracker().getImageMesh().drawWireframe();
        ofTranslate(Video::instance()->getPlayer().getWidth(), 0);
    }

    // draw webcam?
    if(Webcam::instance()->parDraw){
        Webcam::instance()->draw();
        Collector::instance()->getTracker().getImageMesh().drawWireframe();
        ofTranslate(Webcam::instance()->getVideoGrabber().getWidth(),0);
    }

    ofPopMatrix();

    // draw gui
    gui.draw();
}

void Module::dragEvent(ofDragInfo dragInfo){
    ofLogVerbose() << "Got file: " << dragInfo.files[0];
    Video::instance()->load(dragInfo.files[0]);
}

void Module::onVideoSpeedResetButtonPressed(){
    Video::instance()->parSpeed.set(1.0f);
}