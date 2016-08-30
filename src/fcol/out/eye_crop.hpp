//
//  eye_crop.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-08-30.
//
//

#ifndef eye_crop_hpp
#define eye_crop_hpp

#include "../shared.h"
#include "../collector.hpp"

namespace fcol {
namespace out {
    
    class EyeCrop {
        
        FCOL_SINGLETON_INLINE_HEADER_CODE(EyeCrop)
        
    public: // sub-types

        typedef struct {
            ofBaseVideoDraws* draws;
            ofFbo* fbo;
        } BaseVideoDrawsEyeCrop;
        
        typedef struct {
            ofVideoPlayer* player;
            ofFbo* fbo;
        } VideoFrameEyeCrop;

    public: // common methods
        
        // EyeCrop(){}
        ~EyeCrop(){ destroy(); }
        
        void setupParams();
        void setup(Collector* collector);
        void destroy();
        void draw();
    
    public: // getter methods
        
        inline ofFbo& getFbo(){ return fbo; }

    private: // methods
        
        void registerCallbacks(bool _register=true);
        void drawCrop(ofBaseVideoDraws &player, ofxFaceTracker &tracker);
        void applyTrackerEyesMatrix(ofxFaceTracker &tracker);

    private: // callbacks

        void onBaseVideoDrawsTracker(BaseVideoDrawsTracker& baseVideoDrawsTracker);
        void onVideoFrameTrack(VideoFrameTracker& videoFrameTracker);

    public: // events

        ofEvent<BaseVideoDrawsEyeCrop> baseVideoDrawsEyeCropEvent;
        ofEvent<VideoFrameEyeCrop> videoEyeCropEvent;

    public: // params
        
        ofParameterGroup parameters;
        ofParameter<bool> enabled;        
        ofParameter<int> imageWidth;
        ofParameter<int> imageHeight;
        ofParameter<ofVec2f> leftEyePos;
        ofParameter<ofVec2f> rightEyePos;

    private: // attributes
        
        Collector* collector;
        ofFbo fbo;
        
    }; // class EyeFile
    
} // namespace out
} // namespace fcol


#endif /* eye_crop_hpp */
