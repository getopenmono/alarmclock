
#ifndef scene_h
#define scene_h

#include <view.h>
#include <list>
#include <FunctionPointer.h>

namespace mono { namespace ui {
    
    class Scene : public View
    {
    protected:
        
        std::list<View*> childviewList;
        display::Color backColor;
        mbed::FunctionPointerArg1<void, const Scene&> showHandler, hideHandler;
        
    public:
        
        Scene();
        
        Scene(const geo::Rect &rect);
        
        virtual void addView(const View &child);
        
        virtual void removeView(const View &child);
        
        virtual void show();
        
        virtual void hide();
        
        virtual void repaint();
        
        display::Color BackgroundColor() const;
        void setBackground(display::Color color);
        
        template <typename Context>
        void setShowCallback(Context *cnxt, void(Context::*memptr)(const Scene&))
        {
            showHandler.attach(cnxt, memptr);
        }
        
        template <typename Context>
        void setHideCallback(Context *cnxt, void(Context::*memptr)(const Scene&))
        {
            hideHandler.attach(cnxt, memptr);
        }
    };
} }

#endif /* scene_h */
