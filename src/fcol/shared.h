//
//  shared.h
//  emptyExample
//
//  Created by Mark van de Korput on 16-08-22.
//
//

#ifndef shared_h
#define shared_h

#include "ofMain.h"

#define FCOL_SINGLETON_INLINE_HEADER_CODE(x) \
    public: \
        inline static x* instance(){\
            if(!singleton){\
                ofLogVerbose() << "Creating singleton of class " << #x;\
                singleton = new x();\
            }\
            return singleton;\
        }\
        inline static void delete_instance(){ \
            if(singleton){\
                delete singleton;\
                singleton = NULL;\
            }\
        }\
    private:\
        static x* singleton;

#define FCOL_SINGLETON_INLINE_IMPLEMENTATION_CODE(x) \
    x* x::singleton = NULL;

#endif /* shared_h */
