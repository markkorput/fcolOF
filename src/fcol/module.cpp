//
//  module.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-08-22.
//
//

#include "module.hpp"
#include "ofxCv.h"
#include "video.hpp"

using namespace fcol;

FCOL_SINGLETON_INLINE_IMPLEMENTATION_CODE(Module)

void Module::setupParams(){
    parameters.setName("fcol");
    parameters.add(autoSave.set("autoSave", true));
    parameters.add(updateSource.set("updateSource", true));
    parameters.add(drawSource.set("drawSource", true));
    parameters.add(drawFbo.set("drawFbo", true));
    parameters.add(saveImages.set("saveImages", false));
    // parameters.add(eyeDistance.set("eyeDistance", 32.0f, 0.0f, 100.0f));
    parameters.add(leftEyePos.set("leftEyePos", ofVec2f(100.0f, 100.0f), ofVec2f(-100.0f, -100.0f), ofVec2f(700.0f, 300.0f)));
    parameters.add(rightEyePos.set("rightEyePos", ofVec2f(500.0f, 100.0f), ofVec2f(-100.0f, -100.0f), ofVec2f(700.0f, 300.0f)));
    parameters.add(drawEyeCircles.set("drawEyeCircles", true));

    // sub module params
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
    fbo.allocate(600, 200);

    tracker.setup();
    
    ofAddListener(Video::instance()->newFrameEvent, this, &Module::onNewVideoFrame);
}

void Module::destroy(){
    if(autoSave){
        gui.saveToFile("params.xml");
    }
    
    Video::delete_instance();
}

void Module::update(){
    Video::instance()->update();
    
//    if(updateSource){
//        // if(player.isPlaying()){
//        if(player.isLoaded()){
////            player.nextFrame();
//            // player.setFrame(player.getCurrentFrame()+ofRandom(3));
//            player.update();
//            if(player.isFrameNew()){
//                tracker.reset();
//                if(tracker.update(ofxCv::toCv(player))) {
//                    processTracker();
//                }
//            }
//             //player.setFrame(player.getCurrentFrame()+1);
//            
//        } else {
////            cam.update();
////            if(cam.isFrameNew()){
////                tracker.reset();
////                if(tracker.update(ofxCv::toCv(cam))) {
////                    processTracker();
////                }
////            }
//        }
//    }
    
}

void Module::processTracker(){
    fbo.begin();
    ofClear(0.0f, 0.0f);
    drawEyer();
    fbo.end();

    if(saveImages){
        ofPixels pix;
        fbo.readToPixels(pix);
        pix.setImageType(OF_IMAGE_GRAYSCALE);
        string filename;
        if(Video::instance()->isActive()){
            filename = Video::instance()->getMovieFilename()  + "-f" + ofToString(Video::instance()->getPlayer().getCurrentFrame());
        } else {
            filename = "f"+ofToString(ofGetFrameNum());
        }
        filename = "aligned/" + filename + "_" + ofToString(tracker.getOrientation()) + ".tiff";
        ofSaveImage(pix, filename);
    }

    if(drawEyeCircles){
        fbo.begin();
        ofNoFill();
        ofSetColor(255,0,0, 100.0f);
        ofDrawCircle(leftEyePos.get(), 30);
        ofDrawCircle(rightEyePos.get(), 30);
        fbo.end();
    }
}

void Module::drawEyer(){
    ofPushMatrix();
    // ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
    ofVec2f leftEye = tracker.getImageFeature(ofxFaceTracker::LEFT_EYE).getCentroid2D();
    ofVec2f rightEye = tracker.getImageFeature(ofxFaceTracker::RIGHT_EYE).getCentroid2D();
    float eyeDistance = leftEyePos->distance(rightEyePos);
    float curDistance = leftEye.distance(rightEye);
    float scale = eyeDistance / curDistance;
    float rotation = (rightEye - leftEye).angle(rightEyePos.get() - leftEyePos.get()); // (ofVec2f(1, 0));
    ofTranslate(leftEyePos.get() - leftEye * scale);
    //ofTranslate(-eyeDistance / 2, 0);
    ofScale(scale, scale);
    ofRotate(rotation);
    //ofTranslate(-leftEye.x, -leftEye.y);
    if(Video::instance()->isActive()){
        Video::instance()->getPlayer().draw(0,0);
    } else {
        //cam.draw(0,0);
    }
    ofPopMatrix();
}

void Module::draw(){
    ofPushMatrix();
    ofScale(0.5f, 0.5f);

    if(drawFbo){
        fbo.draw(0, 0);
        ofTranslate(fbo.getWidth(), 0);
    }

    Video::instance()->draw();
    tracker.getImageMesh().drawWireframe();

    ofPopMatrix();
    gui.draw();
}

//void Module::threadedFunction(){
//}

void Module::dragEvent(ofDragInfo dragInfo){
    ofLogVerbose() << "Got file: " << dragInfo.files[0];
    Video::instance()->load(dragInfo.files[0]);
}

void Module::onNewVideoFrame(ofVideoPlayer & player){
    tracker.reset();

    if(tracker.update(ofxCv::toCv(player))) {
        processTracker();
    }
}