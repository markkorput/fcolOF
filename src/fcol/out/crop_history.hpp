//
//  crop_history.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-08-30.
//
//

#ifndef crop_history_hpp
#define crop_history_hpp

#include "../shared.h"
#include "eye_crop.hpp"

namespace fcol {
    namespace out {
        
        class CropHistory {
            
            FCOL_SINGLETON_INLINE_HEADER_CODE(CropHistory)
            
        public: // common methods
            
            // CropHistory(){}
            ~CropHistory(){ destroy(); }

            void setupParams();
            void setup(EyeCrop* eyeCrop);
            void destroy();
            void draw();
            
        public: // getter methods
            
            inline ofFbo& getFbo(){ return fbo; }
            
        private: // methods
            
            void registerCallbacks(bool _register=true);
            void drawCrop(ofBaseVideoDraws &player, ofxFaceTracker &tracker);
            void applyTrackerEyesMatrix(ofxFaceTracker &tracker);
            
        private: // callbacks
            
            void onEyeCrop(ofFbo& eyeCropFbo);

        public: // params
            
            ofParameterGroup parameters;
            ofParameter<bool> enabled;
            ofParameter<int> numberOfFrames;
            
        private: // attributes
            
            EyeCrop* eyeCrop;
            ofFbo fbo, fbo2;

        }; // class EyeFile
        
    } // namespace out
} // namespace fcol

#endif /* crop_history_hpp */
