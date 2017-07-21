
#ifndef set_time_controller_h
#define set_time_controller_h

#include <mono.h>
#include "number_select_view.h"
#include <scene_controller.h>
#include "icon_view.h"

using namespace mono::ui;

class SetTimeController : public SceneController {
protected:
    
    IntegerSelectView hourSelect, minSelect;
    ButtonView clockBtn;
    IconView clockView;
    mbed::FunctionPointer dismissHandler;
    bool clockIsSet;
    
public:
    
    SetTimeController(const mono::geo::Rect &rct);
    
    void didShow(const SceneController &);
    void didHide(const SceneController &);
    
    void requestDismiss();
    bool IsClockSet() const;
    
    template <typename Context>
    void setDismissCallback(Context *cnxt, void(Context::*memptr)(void))
    {
        dismissHandler.attach<Context>(cnxt, memptr);
    }
    
};

#endif /* set_time_controller_h */
