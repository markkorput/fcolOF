//
//  module.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-08-22.
//
//

#ifndef module_hpp
#define module_hpp

#include "shared.h"
#include "ofxGui.h"
#include "ofxFaceTracker.h"

namespace fcol {
    class Module : public ofThread {

        FCOL_SINGLETON_INLINE_HEADER_CODE(Module)

    public:
        
        // Module();
        ~Module(){ destroy(); }
        
        void setupParams();
        void setup();
        void destroy();
        void update();
        void draw();

        void threadedFunction();

    private: // methods
        
        void drawEyer();

    private: // params
        
        ofParameterGroup parameters;
        ofParameter<bool> autoSave;
        ofParameter<bool> webcamUpdate;
        ofParameter<bool> webcamDraw;
        ofParameter<bool> webcamTracker;
        ofParameter<bool> drawFbo;
        ofParameter<bool> saveImages;
        ofParameter<bool> drawEyeCircles;

        ofParameter<ofVec2f> leftEyePos;
        ofParameter<ofVec2f> rightEyePos;
        
        
    private: // gui
        
        ofxPanel gui;

    private: // attributes

        ofVideoGrabber cam;
        ofxFaceTracker tracker;
        ofFbo fbo;
    };
}

#endif /* module_hpp */
