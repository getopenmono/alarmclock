
#ifndef set_time_controller_h
#define set_time_controller_h

#include <mono.h>
#include "number_select_view.h"
#include "scene.h"

using namespace mono::ui;

class SetTimeController : public Scene {
protected:
    
    IntegerSelectView hourSelect, minSelect;
    ButtonView clockBtn;
    mbed::FunctionPointer dismissHandler;
    bool clockIsSet;
    
public:
    
    SetTimeController();
    
    void didShow(const Scene &);
    void didHide(const Scene &);
    
    void requestDismiss();
    bool IsClockSet() const;
    
    template <typename Context>
    void setDismissCallback(Context *cnxt, void(Context::*memptr)(void))
    {
        dismissHandler.attach<Context>(cnxt, memptr);
    }
    
};

#endif /* set_time_controller_h */
