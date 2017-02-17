
#include "number_select_view.h"
#include <display/text_render.h>
#include <text_label_view.h>

using namespace mono::ui;

IntegerSelectView::IntegerSelectView(const geo::Rect &rect, int minimum,
                                     int maximum, int stepSize) :
    mono::ui::ResponderView(rect)
{
    this->textFont = TextLabelView::StandardGfxFont;
    this->minimum = minimum;
    this->maximum = maximum;
    this->stepSize = stepSize;
    curValue = minimum;
    incrementDistance = textFont->glyph[textFont->first].height;
    wrapAround = true;
}

void IntegerSelectView::TouchBegin(TouchEvent &event)
{
    touchStart = event.Position;
    touchInProgress = true;
    this->scheduleRepaint();
}

void IntegerSelectView::TouchEnd(TouchEvent &)
{
    touchInProgress = false;
    this->scheduleRepaint();
}

void IntegerSelectView::TouchMove(TouchEvent &event)
{
    int distance = event.Position.Y() - touchStart.Y();
    if (distance > incrementDistance)
    {
        touchStart = event.Position;
        if (wrapAround && curValue-stepSize < minimum)
            setValue(maximum);
        else
            setValue(curValue-stepSize);
    }
    else if (distance < -incrementDistance)
    {
        touchStart = event.Position;
        if (wrapAround && curValue+stepSize > maximum)
            setValue(minimum);
        else
            setValue(curValue+stepSize);
    }
}

void IntegerSelectView::setValue(int val)
{
    if (val < minimum)
        val = minimum;
    if (val > maximum)
        val = maximum;
    
    bool changed = false;
    if (curValue != val)
        changed = true;
    
    curValue = val;
    
    if (changed)
    {
        scheduleRepaint();
        changeHandler.call();
    }
}

int IntegerSelectView::Value() const
{
    return curValue;
}

void IntegerSelectView::setTouchIncrementThreshold(uint8_t threshold)
{
    incrementDistance = threshold;
}

uint8_t IntegerSelectView::TouchIncrementThreshold() const
{
    return incrementDistance;
}


void IntegerSelectView::setWrapAround(bool wrap)
{
    wrapAround = wrap;
}

bool IntegerSelectView::WrapAround() const
{
    return wrapAround;
}

void IntegerSelectView::repaint()
{
    //draw border
    painter.setBackgroundColor(StandardBackgroundColor);
    painter.setForegroundColor(StandardBorderColor);
    painter.drawRect(viewRect);
    
    display::Color textColor = StandardTextColor;
    if (touchInProgress)
        textColor = StandardHighlightColor;
    
    
    mono::display::TextRender tr(painter);
    tr.setAlignment(display::TextRender::ALIGN_CENTER);
    tr.setAlignment(display::TextRender::ALIGN_MIDDLE);
    
    tr.setForeground(StandardBackgroundColor);
    tr.drawInRect(viewRect, lastPaintedValue, *textFont, false);
    
    String newValue = String::Format("%02d",curValue);
    tr.setForeground(textColor);
    tr.drawInRect(viewRect, newValue, *textFont, false);
    
    lastPaintedValue = newValue;
}
