
#include "set_time_controller.h"

using namespace mono;
using mono::geo::Rect;

SetTimeController::SetTimeController() : Scene(),
    hourSelect(Rect(35, 30, 50, 70), 0, 23),
    minSelect(Rect(90, 30, 50, 70), 0, 59),
    clockBtn(Rect(35,220-65,105,35), "Set time")
{
    addView(hourSelect);
    addView(minSelect);
    addView(clockBtn);
    
    setShowCallback<SetTimeController>(this, &SetTimeController::didShow);
    setHideCallback<SetTimeController>(this, &SetTimeController::didHide);
    
    minSelect.setTouchIncrementThreshold(6);
    clockBtn.setClickCallback<SetTimeController>(this, &SetTimeController::requestDismiss);
    
    clockIsSet = false;
}

void SetTimeController::requestDismiss()
{
    dismissHandler.call();
}

bool SetTimeController::IsClockSet() const
{
    return clockIsSet;
}

void SetTimeController::didShow(const Scene &)
{
    DateTime now = DateTime::now();
    hourSelect.setValue(now.Hours());
    minSelect.setValue(now.Minutes());
}

void SetTimeController::didHide(const mono::ui::Scene &)
{
    DateTime now = DateTime::now();
    DateTime newTime(now.Year(), now.Month(), now.Days(),
                     hourSelect.Value(), minSelect.Value(), 0);
    DateTime::setSystemDateTime(newTime);
    clockIsSet = true;
}
