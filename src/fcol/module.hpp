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

    private: // params
        
        ofParameterGroup parameters;
        ofParameter<bool> autoSave;
        ofParameter<bool> webcam;
        
    private: // gui
        
        ofxPanel gui;

    private: // attributes

        ofVideoGrabber vidGrabber;

    };
}

#endif /* module_hpp */
