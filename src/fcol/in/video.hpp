//
//  video.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-08-23.
//
//

#ifndef video_hpp
#define video_hpp

#include "../shared.h"

namespace fcol {
    class Video {
        FCOL_SINGLETON_INLINE_HEADER_CODE(Video)

    public: // common methods

        // Video();
        ~Video(){ destroy(); }

        void setupParams();
        // void setup();
        bool load(const string &path);
        void destroy();
        void update();
        void draw();

    public: // getters
        inline bool isActive() const { return player.isLoaded(); }
        inline ofVideoPlayer& getPlayer(){ return player; }

    private: // callbacks
        void onSpeedChange(float & speed);

    public: // events
        ofEvent<ofVideoPlayer> newFrameEvent;

    public:
        ofParameterGroup parameters;
        ofParameter<bool> parUpdate;
        ofParameter<bool> parDraw;
        ofParameter<float> parSpeed;

    private:
        ofVideoPlayer player;
    };
}

#endif /* video_hpp */
