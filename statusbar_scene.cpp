#include "statusbar_scene.h"

StatusbarScene::StatusbarScene(const Point &pos) : SceneController(Rect(pos.X(), pos.Y(), 176,20)),
    battery(Rect(viewRect.X2()-23, pos.Y()+5, 18, 10)),
    wifiOn(Point(viewRect.X2() - 25 - 20, 2), mono::display::wifiMediumSignalSymbol16)
{
    addView(battery);
    addView(wifiOn);
    
    battery.setBlinkWhenCritical(true);
    battery.setBatteryPercent(100);
    
    wifiOn.setForeground(BackgroundColor());
    
    IApplicationContext *cnxt = IApplicationContext::Instance;
    if (cnxt != 0)
        cnxt->PowerManager->AppendToPowerAwareQueue(this);
}


void StatusbarScene::setBatteryLevel(uint8_t percent)
{
    battery.setBatteryPercent(percent);
}

void StatusbarScene::setWifiOn(bool on)
{
    if (on)
        wifiOn.setForeground(View::StandardBorderColor);
    else
    {
        wifiOn.setForeground(BackgroundColor());
    }
    scheduleRepaint();
}

void StatusbarScene::updateBattery()
{
#ifndef EMUNO
    setBatteryLevel(mono::power::MonoBattery::ReadPercentage());
#else
    setBatteryLevel(75);
#endif
}

void StatusbarScene::onSystemPowerOnReset()
{
    onSystemWakeFromSleep();
}

void StatusbarScene::onSystemWakeFromSleep()
{
    mono::async(this, &StatusbarScene::updateBattery);
}
