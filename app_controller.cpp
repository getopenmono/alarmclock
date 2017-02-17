#include "app_controller.h"
#include <Fonts/FreeSerifItalic18pt7b.h>
#include <project.h>

using namespace mono::geo;

const String clockNotSetText("Press to set clock!");
const String alarmNotSet("Press to set an alarm");
const display::Color PomegranateColor(192, 57, 43);
const int AppController::beepSequence[3][2] = {{200,100}, {200,100}, {0,400}};

AppController::AppController() :
    pwrsave(30000, 10000, 11, 100),
    clockView(Rect(10,30,156,50), "00:00"),
    helpLbl(Rect(10,220-45,156,35), "Alarm clock"),
    alarmView(Rect(10,80,156,35), "Set alarm"),
    dismissView(Rect(10,220-45,156,35),"Off"),
    snoozeView(Rect(10,110-17,156,35), "Snooze")
{
    PWM_WritePeriod(100);
    alarmSounding = false;
    //DateTime::setSystemDateTime(DateTime());
    
    clockIncrement.setTask<AppController>(this, &AppController::updateClock);

    clockView.setAlignment(TextLabelView::ALIGN_CENTER);
    clockView.setFont(FreeSerifItalic18pt7b);
    
    alarmView.setText(PomegranateColor);
    alarmView.setBorder(View::StandardBackgroundColor);
    alarmView.setClickCallback<AppController>(this, &AppController::showSetAlarm);
    
    helpLbl.setText(PomegranateColor);
    helpLbl.setBorder(View::StandardBackgroundColor);

    // Main Scene
    mainScene.setShowCallback<AppController>(this, &AppController::didShowScene);
    mainScene.setHideCallback<AppController>(this, &AppController::didHideScene);
    mainScene.addView(helpLbl);
    mainScene.addView(clockView);
    mainScene.addView(alarmView);
    mainScene.addView(dismissView);
    mainScene.addView(snoozeView);
    
    // Set clock scene
    setTimeCtrl.setDismissCallback<AppController>(this, &AppController::showMain);
    
    // set alarm scene
    setAlarmCtrl.setDimissCallback<AppController>(this, &AppController::showMain);
    
    dismissView.setClickCallback<AppController>(this, &AppController::dismissAlarm);
    snoozeView.setClickCallback<AppController>(this, &AppController::snoozeAlarm);
    
    alarmTask.setTask<AppController>(this, &AppController::fireAlarm);
    alarmTask.setRunInSleep(true);
    
    mainScene.show();
}

void AppController::updateClock()
{
    if (setTimeCtrl.IsClockSet())
    {
        DateTime now = DateTime::now();
        clockView.setText(display::AlizarinColor);
        clockView.setText(String::Format("%02d:%02d", now.Hours(), now.Minutes()));
        
        clockIncrement.reschedule(now.addSeconds(60-now.Seconds()));
    }
    else
    {
        if (clockView.TextColor() == display::AlizarinColor)
        {
            clockView.setText(View::StandardBackgroundColor);
            clockView.scheduleRepaint();
        }
        else
        {
            clockView.setText(display::AlizarinColor);
            clockView.scheduleRepaint();
        }
        
        clockIncrement.reschedule(DateTime::now().addSeconds(blinkIntervalSecs));
    }
}

void AppController::showSetClock()
{
    mainScene.hide();
    setTimeCtrl.show();
}

void AppController::showSetAlarm()
{
    mainScene.hide();
    setAlarmCtrl.show();
}

void AppController::showMain()
{
    setTimeCtrl.hide();
    setAlarmCtrl.hide();
    mainScene.show();
}

void AppController::didShowScene(const Scene &scene)
{
    if (&scene == &mainScene)
    {
        updateClock();
        
        if (!setTimeCtrl.IsClockSet())
        {
            helpLbl.show();
            helpLbl.setText(clockNotSetText);
            helpLbl.setClickCallback<AppController>(this, &AppController::showSetClock);
        }
        else
        {
            helpLbl.hide();
        }
        
        if (setAlarmCtrl.IsAlarmSet() && !alarmSounding)
        {
            DateTime alarm = setAlarmCtrl.alarmTime();
            String text = String::Format("* %02d:%02d", alarm.Hours(), alarm.Minutes());
            display::TextRender tr;
            geo::Size dim = tr.renderDimension(text, *TextLabelView::StandardGfxFont);
            //10,80,156,35
            alarmView.setRect(Rect(10+156-(dim.Width()+24), 80, dim.Width()+24, 35));
            alarmView.setText(text);
            
            alarmTask.reschedule(alarm);
        }
        else if (!alarmSounding)
        {
            alarmView.setRect(Rect(10, 80, 156, 35));
            alarmView.setText("-");
            alarmTask.reschedule(DateTime());
        }
        
        if (alarmSounding)
        {
            alarmView.hide();
            helpLbl.hide();
            
            dismissView.show();
            snoozeView.show();
        }
        else
        {
            dismissView.hide();
            snoozeView.hide();
        }
    }
}

void AppController::didHideScene(const mono::ui::Scene &scene)
{
    if (&scene == &setClockScene)
    {
        
    }
}

void AppController::fireAlarm()
{
    //we might be in sleep
    if (!mono::power::IPowerManagement::__shouldWakeUp)
    {
        //in sleep
        mono::power::IPowerManagement::__shouldWakeUp = true;
        async<AppController>(this, &AppController::fireAlarm); //come back again
    }
    
    pwrsave.undim();
    pwrsave.deactivate();
    beepIndex = 0;
    alarmSounding = true;
    mainScene.hide();
    mainScene.show();
    
    incrementBeep();
    
    maxAlarmTime = ScheduledTask(DateTime::now().addMinutes(maxAlarmMinutes));
    maxAlarmTime.setTask<AppController>(this, &AppController::snoozeAlarm);
}

void AppController::dismissAlarm()
{
    pwrsave.startDimTimer();
    maxAlarmTime = ScheduledTask();
    
    alarmSounding = false;
    IApplicationContext::Instance->Buzzer->buzzKill();
    
    setAlarmCtrl.setAlarmActive(false);
    
    mainScene.hide();
    mainScene.show();
}

void AppController::snoozeAlarm()
{
    pwrsave.dim();
    maxAlarmTime = ScheduledTask();
    
    alarmSounding = false;
    IApplicationContext::Instance->Buzzer->buzzKill();
    DateTime newAlarm = DateTime::now().addMinutes(snoozeTimeMinutes);
    setAlarmCtrl.setAlarmTime(newAlarm);
    setAlarmCtrl.setAlarmActive(true);
    
    mainScene.hide();
    mainScene.show();
}

void AppController::incrementBeep()
{
    if (!alarmSounding)
        return;
    
    if (beepIndex >= 3)
        beepIndex = 0;
    
    if (beepSequence[beepIndex][0] == 0)
        incrementBeepPause();
    else
    {
        IApplicationContext::Instance->Buzzer->
        buzzAsync<AppController>(beepSequence[beepIndex][0], this,
                                 &AppController::incrementBeepPause);
    }
}

void AppController::incrementBeepPause()
{
    IApplicationContext::Instance->Buzzer->buzzKill();
    Timer::callOnce<AppController>(beepSequence[beepIndex++][1], this, &AppController::incrementBeep);
}

// MARK: Power control interface

void AppController::monoWakeFromReset()
{
}

void AppController::monoWillGotoSleep()
{
    if (alarmSounding)
        snoozeAlarm();
}

void AppController::monoWakeFromSleep()
{
    setTimeCtrl.scheduleRepaint();
    setAlarmCtrl.scheduleRepaint();
    mainScene.scheduleRepaint();
    pwrsave.undim();
    
    //clockView.scheduleRepaint();
}
