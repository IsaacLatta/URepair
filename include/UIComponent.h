/*
#ifndef UICOMPONENT_H
#define UICOMPONENT_H

#include <string>
#include <functional>


template <class T>
class UIComponent
{
public:
    virtual ~UIComponent() = default;
    virtual void render() = 0;
    virtual void add(std::string label = "", std::function<void()> callback = nullptr) = 0;
protected:
    std::vector<T> options;
};

struct MenuOption {
    std::string label;
    std::function<void()> callback;
    MenuOption (std::string label, std::function<void()> callback) : label(label), callback(callback) {}
};

class Menu: public UIComponent<MenuOption>
{
public:
    void render() override;
    void add(std::string label = "", std::function<void()> callback = nullptr) override;
};


#endif
*/