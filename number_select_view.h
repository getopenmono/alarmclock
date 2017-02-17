
#ifndef number_select_view_h
#define number_select_view_h

#include <responder_view.h>
#include <mbed.h>
#include <display/gfxfont.h>

using mono::geo::Point;

namespace mono { namespace ui {
    
    class IntegerSelectView : public ResponderView {
    protected:
        
        mbed::FunctionPointer changeHandler;
        int curValue;
        String lastPaintedValue;
        bool touchInProgress;
        bool wrapAround;
        Point touchStart;
        int incrementDistance, minimum, maximum, stepSize;
        const GFXfont *textFont;
        
        
    public:
        
        IntegerSelectView(const geo::Rect &rect, int minimum = 0, int maximum = 100, int stepSize = 1);
        
        template <typename Context>
        void setChangeCallback(const Context *cnxt, void(Context::*memptr)(void))
        {
            changeHandler.attach<Context>(cnxt, memptr);
        }
        
        virtual void TouchBegin(TouchEvent &event);
        virtual void TouchEnd(TouchEvent &event);
        virtual void TouchMove(TouchEvent &event);
        
        void setValue(int val);
        int Value() const;
        
        void setTouchIncrementThreshold(uint8_t threshold);
        uint8_t TouchIncrementThreshold() const;
        
        void setWrapAround(bool wrap);
        bool WrapAround() const;
        
        void repaint();
    };
} }

#endif /* number_select_view_h */
