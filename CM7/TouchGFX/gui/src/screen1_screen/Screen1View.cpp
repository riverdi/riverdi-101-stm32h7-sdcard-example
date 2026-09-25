#include <gui/screen1_screen/Screen1View.hpp>
#include <touchgfx/Color.hpp>

Screen1View::Screen1View()
{

}

void Screen1View::setupScreen()
{
    Screen1ViewBase::setupScreen();
}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

void Screen1View::setSdStatus(uint8_t status)
{
    switch (status)
    {
    case 1:
        box1.setColor(touchgfx::Color::getColorFromRGB(0, 200, 0));
        break;
    case 2:
        box1.setColor(touchgfx::Color::getColorFromRGB(220, 50, 50));
        break;
    default:
        box1.setColor(touchgfx::Color::getColorFromRGB(192, 192, 192));
        break;
    }
    box1.invalidate();
}
