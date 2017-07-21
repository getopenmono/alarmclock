#include "statusbar_scene.h"

StatusbarScene::StatusbarScene(const Point &pos) : SceneController(Rect(pos.X(), pos.Y(), 176,20)),
    battery(Rect(viewRect.X2()-23, pos.Y()+5, 18, 10)),
    wifiOn(Point(viewRect.X2() - 25 - 20, 2), mono::display::wifiMediumSignalSymbol16),
    chargingTimer(1000, false)
{
    addView(battery);
    addView(wifiOn);
    
    battery.setBlinkWhenCritical(true);
    battery.setBatteryPercent(100);
    
    wifiOn.setForeground(BackgroundColor());
    
    IApplicationContext *cnxt = IApplicationContext::Instance;
    if (cnxt != 0)
        cnxt->PowerManager->AppendToPowerAwareQueue(this);

    chargingTimer.setCallback(this, &StatusbarScene::updateBattery);

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
    bool charging = true;

#ifndef EMUNO
    charging = IApplicationContext::Instance->PowerManager->PowerSystem->IsUSBCharging();
#endif

    if (!charging) {
        battery.setCriticalPercentAmount(25);
        battery.setBorder(View::StandardBorderColor);
#ifndef EMUNO
        int percent = mono::power::MonoBattery::ReadPercentage();
        if (percent == 0)
        {
            onSystemWakeFromSleep();
        }
        else
        {
            setBatteryLevel(percent);
        }
#endif
    }
    else {
        if (!chargingTimer.Running())
        {
            chargingTimer.start();
        }

        battery.setCriticalPercentAmount(0);
        battery.setBorder(GreenColor);
        uint8_t newPercent = battery.batteryPercent()+20;
        setBatteryLevel(newPercent > 100 ? 20 : newPercent);
    }
}

void StatusbarScene::onSystemPowerOnReset()
{
    onSystemWakeFromSleep();
}

void StatusbarScene::onSystemWakeFromSleep()
{
    mono::Timer::callOnce(500, this, &StatusbarScene::updateBattery);
}
