#ifndef _local_statusbar_scene_h
#define _local_statusbar_scene_h

#include <mono.h>
#include <icons/wifi-medium-signal-symbol-16.h>
#include "battery_view.h"

using namespace mono::ui;
using namespace mono::geo;

class StatusbarScene : public SceneController, public mono::power::IPowerAware
{
    BatteryView battery;
    IconView wifiOn;

public:
    
    StatusbarScene(const Point &pos);
    
    void setBatteryLevel(uint8_t percent);
    void setWifiOn(bool on);

    void updateBattery();

    // MARK: Power Aware Methods
    
    void onSystemPowerOnReset();
    void onSystemWakeFromSleep();
};

#endif
