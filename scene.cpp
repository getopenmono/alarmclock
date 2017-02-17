
#include "scene.h"

using namespace mono::ui;

Scene::Scene() : View()
{
    viewRect = geo::Rect(geo::Point(0,0), DisplaySize());
    backColor = StandardBackgroundColor;
}

Scene::Scene(const geo::Rect &rect) : View(rect)
{
    backColor = StandardBackgroundColor;
}

void Scene::addView(const View &child)
{
    childviewList.push_back((View*)&child);
}

void Scene::removeView(const View &child)
{
    childviewList.remove((View*)&child);
}

void Scene::show()
{
    if (Visible())
        return;
    
    View::show();
    for(std::list<View*>::iterator it = childviewList.begin(); it != childviewList.end(); ++it)
    {
        (*it)->show();
    }
    
    showHandler.call(*this);
}

void Scene::hide()
{
    if (!Visible())
        return;
    
    View::hide();
    for(std::list<View*>::iterator it = childviewList.begin(); it != childviewList.end(); ++it)
    {
        (*it)->hide();
    }
    
    hideHandler.call(*this);
}

void Scene::repaint()
{
    //paint it back
    painter.setBackgroundColor(backColor);
    painter.drawFillRect(viewRect, true);
    
    for(std::list<View*>::iterator it = childviewList.begin(); it != childviewList.end(); ++it)
    {
        (*it)->scheduleRepaint();
    }
}

mono::display::Color Scene::BackgroundColor() const
{
    return backColor;
}

void Scene::setBackground(display::Color color)
{
    backColor = color;
}
