//
//  webcam.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-08-23.
//
//

#ifndef webcam_hpp
#define webcam_hpp

#include "../shared.h"

namespace fcol {
    class Webcam {
        FCOL_SINGLETON_INLINE_HEADER_CODE(Webcam)
        
    public: // common methods
        
        // Webcam();
        ~Webcam(){ destroy(); }
        
        void setupParams();
        void setup();
        void destroy();
        void update();
        void draw();
        
    public: // getters
        inline bool isActive() const { return cam.isInitialized(); }
        inline ofVideoGrabber& getVideoGrabber(){ return cam; }

    public: // events
        ofEvent<ofVideoGrabber> newFrameEvent;
        
    public:
        ofParameterGroup parameters;
        ofParameter<bool> parUpdate;
        ofParameter<bool> parDraw;

    private: // attributes
        
        ofVideoGrabber cam;
    };
}

#endif /* webcam_hpp */
