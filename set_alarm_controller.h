
#ifndef set_alarm_controller_h
#define set_alarm_controller_h

#include "scene.h"
#include "number_select_view.h"
#include <on_off_button_view.h>
#include <button_view.h>
#include <date_time.h>
#include <FunctionPointer.h>

using namespace mono;
using namespace mono::ui;

class SetAlarmController : public Scene {
protected:
    
    IntegerSelectView hourSel, minSel;
    OnOffButtonView activeBtn;
    ButtonView closeBtn;
    DateTime alarm;
    bool active;
    
    mbed::FunctionPointer dismissHandler;
    
public:
    
    SetAlarmController();
    
    bool IsAlarmSet() const;
    
    DateTime alarmTime() const;
    
    void setAlarmActive(bool act);
    
    void setAlarmTime(DateTime time);
    
    void didShowScene(const Scene &);
    
    void didHideScene(const Scene &);
    
    void requestDismiss();
    
    template <typename Context>
    void setDimissCallback(Context *cnxt, void(Context::*memptr)(void))
    {
        dismissHandler.attach<Context>(cnxt, memptr);
    }
};

#endif /* set_alarm_controller_h */
