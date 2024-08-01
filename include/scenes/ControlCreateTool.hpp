#pragma once
#include"ModuleBasicDefinition.hpp"

namespace OneKeyEngine
{
namespace CreateControl
{

template <typename T>
ShaPtr<T> create_control(Scenes::Scenes *scenes, Scenes::ScenesObject *parent, int width, int height)
{
    auto obj = scenes->create_object(typeid(T).name()); // 根据组件类型动态创建对象
    if (parent)
        obj->transform.set_parent(parent->transform);
    auto rect = obj->add_module<Scenes::Module::RectTransform>();
    rect->size = VEC::Vec2(width, height);
    return obj->add_module<T>();
}


static ShaPtr<Scenes::Module::Image> create_image(Scenes::Scenes *scenes, Scenes::ScenesObject *parent = nullptr, int width = 100, int height = 100)
{
    auto image = create_control<Scenes::Module::Image>(scenes, parent, width, height);
    return image;
}

static ShaPtr<Scenes::Module::Text> create_text(Scenes::Scenes *scenes, const std::string &text, Scenes::ScenesObject *parent = nullptr, int fontSize = 18)
{
    auto textObj = create_control<Scenes::Module::Text>(scenes, parent, text.size() * fontSize, 50);
    if (textObj)
    {
        auto textComponent = textObj->object->get_module<Scenes::Module::Text>();
        textComponent->showText = text;
        textComponent->textStyle.verticalAlign = TextAlign::Center;
        textComponent->textStyle.horizontalAlign = TextAlign::Left;
        textComponent->textStyle.font.size = fontSize;
    }
    return textObj;
}
static ShaPtr<Scenes::Module::Button> create_button(Scenes::Scenes *scenes, const std::string &text, Scenes::ScenesObject *parent = nullptr, int width = 100, int height = 50, int textSize = 18)
{
    
    auto button = create_control<Scenes::Module::Button>(scenes, parent, width, height);
    if (button)
    {
        auto bk= button->object->add_module<Scenes::Module::Image>();
        bk->create_image(1,1,VEC::Color(0.7f,0.7f,0.7f));
        button->drawAim=bk;
        auto textObj = create_text(scenes, text, button->object);
        if (textObj)
        {
            auto rect = textObj->object->get_module<Scenes::Module::RectTransform>();
            rect->size = VEC::Vec2(width, height);
            auto textComponent = textObj->object->get_module<Scenes::Module::Text>();
            textComponent->textStyle.verticalAlign = TextAlign::Center;
            textComponent->textStyle.horizontalAlign = TextAlign::Center;
            textComponent->textStyle.font.size = textSize;
        }

    }
    return button;
}

static ShaPtr<Scenes::Module::EditText> create_edit(Scenes::Scenes *scenes, const std::string &defaultText, Scenes::ScenesObject *parent = nullptr, int width = 200, int height = 70)
{
    using namespace Scenes::Module;
    auto editObj = scenes->create_object("EditText");
    if (parent)
        editObj->transform.set_parent(parent->transform);
    auto rect = editObj->add_module<RectTransform>();
    rect->size = VEC::Vec2(width, height);
    auto image = editObj->add_module<Image>();
    image->create_image(1, 1, VEC::Color(0.7, 0.7, 0.7));
    auto edit = editObj->add_module<EditText>();
    auto text = editObj->add_module<Text>();
    edit->rectTransform = rect;
    edit->drawAim = image;
    edit->text = text;
    text->showText = defaultText;
    text->textStyle.verticalAlign = TextAlign::Top;
    text->textStyle.horizontalAlign = TextAlign::Left;
    return edit;
}

// 纵向布局
static void vertical_layout(Scenes::Scenes *scenes, Scenes::ScenesObject *parent, const std::vector<Scenes::ScenesObject *> &items, int spacing = 10, TextAlign alignment = TextAlign::Left)
{
    if (!scenes || !parent)
        return;

    int totalHeight = 0;
    for (auto item : items)
    {
        auto rect = item->get_module<Scenes::Module::RectTransform>();
        totalHeight += rect->size.y + spacing;
    }

    int currentY = 0;
    switch (alignment)
    {
    case TextAlign::Left:
        currentY = 0;
        break;
    case TextAlign::Center:
        currentY = -totalHeight / 2;
        break;
    case TextAlign::Right:
        currentY = -totalHeight;
        break;
    }

    for (auto item : items)
    {
        auto rect = item->get_module<Scenes::Module::RectTransform>();
        rect->transform->position = VEC::Vec2(0, currentY);
        currentY -= (rect->size.y + spacing);
    }
}

// 横向布局
static void horizontal_layout(Scenes::Scenes *scenes, Scenes::ScenesObject *parent, const std::vector<Scenes::ScenesObject *> &items, int spacing = 10, TextAlign alignment = TextAlign::Left)
{
    if (!scenes || !parent)
        return;

    int totalWidth = 0;
    for (auto item : items)
    {
        auto rect = item->get_module<Scenes::Module::RectTransform>();
        totalWidth += rect->size.x + spacing;
    }

    int currentX = 0;
    switch (alignment)
    {
    case TextAlign::Left:
        currentX = 0;
        break;
    case TextAlign::Center:
        currentX = -totalWidth / 2;
        break;
    case TextAlign::Right:
        currentX = -totalWidth;
        break;
    }

    for (auto item : items)
    {
        auto rect = item->get_module<Scenes::Module::RectTransform>();
        rect->transform->position = VEC::Vec2(currentX, 0);
        currentX += (rect->size.x + spacing);
    }
}

}
}