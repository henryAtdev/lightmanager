#ifndef _LIGHT_REQUEST_REGISTRY
#define _LIGHT_REQUEST_REGISTRY
#include <stdlib.h>
#include <set>

class LightRequestListener {
    public:
        virtual void stateChanged();
        virtual void loopChanged (const uint64_t);
};


class LightRequestRegistry{
    private:
        std::set<LightRequestListener*> registry_;
    protected:
        void fireStateChanged();
        void fireLoopChanged(uint64_t timeDiff);
    public: 
        void addLightRequestListener(LightRequestListener* listener){
            registry_.insert(listener);
        }
        void removeLightRequestListener(LightRequestListener* listener){
            registry_.erase(listener);
        }
};

#endif