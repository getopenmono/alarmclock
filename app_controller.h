#ifndef app_controller_h
#define app_controller_h

#include <mono.h>
#include "number_select_view.h"
#include "scene.h"
#include "set_time_controller.h"
#include "set_alarm_controller.h"

using namespace mono;
using namespace mono::ui;


class AppController : public mono::IApplication {
public:

    static const int blinkIntervalSecs = 1;
    static const int maxAlarmMinutes = 3;
    static const int snoozeTimeMinutes = 9;

    PowerSaver pwrsave;
    DateTime alarmTime;
    TextLabelView clockView;
    ButtonView helpLbl, alarmView, dismissView, snoozeView;
    bool alarmSounding;
    static const int beepSequence[3][2];
    int beepIndex;
    
    Scene mainScene, setClockScene;
    SetAlarmController setAlarmCtrl;
    SetTimeController setTimeCtrl;

    ScheduledTask alarmTask, clockIncrement, maxAlarmTime;

    AppController();


    void updateClock();
    void showSetAlarm();
    void showSetClock();
    void showMain();
    
    void fireAlarm();
    void dismissAlarm();
    void snoozeAlarm();
    
    void incrementBeep();
    void incrementBeepPause();
    
    void didShowScene(const Scene&);
    void didHideScene(const Scene&);

    void monoWakeFromReset();
    void monoWillGotoSleep();
    void monoWakeFromSleep();

};

#endif /* app_controller_h */
