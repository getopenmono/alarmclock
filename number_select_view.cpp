
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

void IntegerSelectView::touchBegin(TouchEvent &event)
{
    touchStart = event.Position;
    touchInProgress = true;
    this->scheduleRepaint();
}

void IntegerSelectView::touchEnd(TouchEvent &)
{
    touchInProgress = false;
    this->scheduleRepaint();
}

void IntegerSelectView::touchMove(TouchEvent &event)
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
    painter.setBackgroundColor(display::MidnightBlueColor);
    painter.setForegroundColor(StandardBorderColor);
    
    geo::Rect sqRct = viewRect;
    sqRct.appendY(5);
    sqRct.setHeight(viewRect.Height()-10);
    
    int deStep = sqRct.Width() / 10;
    int wh = sqRct.Width() / 2;
    int c = sqRct.Center().X();
    for (int y=0; y<5; y++) {
        painter.drawHLine(c-wh, c+wh, sqRct.Y2()+y, true);
        painter.drawHLine(c-wh, c+wh, sqRct.Y()-y, true);
        wh -= deStep;
    }
    
    painter.drawFillRect(sqRct, true);
    //painter.drawRect(viewRect);
    
    display::Color textColor = StandardTextColor;
    if (touchInProgress)
        textColor = StandardHighlightColor;
    
    
    mono::display::TextRender tr(painter);
    tr.setAlignment(display::TextRender::ALIGN_CENTER);
    tr.setAlignment(display::TextRender::ALIGN_MIDDLE);
    
    tr.setForeground(painter.BackgroundColor());
    tr.drawInRect(viewRect, lastPaintedValue, *textFont, false);
    
    String newValue = String::Format("%02d",curValue);
    tr.setForeground(textColor);
    tr.drawInRect(viewRect, newValue, *textFont, false);
    
    lastPaintedValue = newValue;
}
