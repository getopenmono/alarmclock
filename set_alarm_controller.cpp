
#include "set_alarm_controller.h"

using namespace mono::ui;
using mono::geo::Rect;

//hourSelect(Rect(35, 30, 50, 70), 0, 23),
//minSelect(Rect(90, 30, 50, 70), 0, 59),

SetAlarmController::SetAlarmController() : Scene(),
    hourSel(Rect(35, 30, 50, 70), 0, 23),
    minSel(Rect(90, 30, 50, 70), 0, 59),
    activeBtn(Rect(35,105,105,35)),
    closeBtn(Rect(35,220-65, 105, 35), "Back")
{
    addView(hourSel);
    addView(minSel);
    addView(activeBtn);
    addView(closeBtn);
    
    setShowCallback<SetAlarmController>(this, &SetAlarmController::didShowScene);
    setHideCallback<SetAlarmController>(this, &SetAlarmController::didHideScene);
    closeBtn.setClickCallback<SetAlarmController>(this, &SetAlarmController::requestDismiss);
    
    active = false;
    activeBtn.setState(false);
}

void SetAlarmController::setAlarmActive(bool act)
{
    active = act;
    activeBtn.setState(act);
}

void SetAlarmController::setAlarmTime(DateTime time)
{
    alarm = time;
    
    if (DateTime::now() >= alarm)
        alarm = alarm = alarm.addDays(1);
}

bool SetAlarmController::IsAlarmSet() const
{
    return active;
}

DateTime SetAlarmController::alarmTime() const
{
    return alarm;
}

void SetAlarmController::didShowScene(const mono::ui::Scene &)
{
    hourSel.setValue(alarm.Hours());
    minSel.setValue(alarm.Minutes());
    activeBtn.setState(active);
}

void SetAlarmController::didHideScene(const mono::ui::Scene &)
{
    DateTime now = DateTime::now();
    alarm = DateTime(now.Year(), now.Month(), now.Days(), hourSel.Value(), minSel.Value(), 0);
    
    if (DateTime::now() >= alarm)
        alarm = alarm = alarm.addDays(1);
    
    active = activeBtn.isActive();
}

void SetAlarmController::requestDismiss()
{
    dismissHandler.call();
}
