#pragma once
#include<functional>
#include<any>
#include<string>
#include<unordered_map>
#include <memory>



namespace OneKeyEngine
{
    



// 基础事件类的非模板基类
class EventBaseBase
{
public:
    virtual ~EventBaseBase() = default;
};

// 基础事件类，增加了模板，允许返回值
template<typename Return>
class EventBase : public EventBaseBase
{
public:
    virtual Return call(const std::vector<std::any>& args = {}) = 0;
    virtual ~EventBase() = default;
};

// 具体事件信息类，继承自 EventBase
template<typename Return, typename... Args>
class EventInformation : public EventBase<Return>
{
public:
    std::function<Return(Args...)> callback;
    std::vector<std::any> defaultArgs;

    // 重写 call 方法，允许传入新参数覆盖默认参数
    Return call(const std::vector<std::any>& args = {}) override
    {
        if (args.empty())
        {
            return callImpl(defaultArgs, std::index_sequence_for<Args...>{});
        }
        else
        {
            return callImpl(args, std::index_sequence_for<Args...>{});
        }
    }

private:
    // 使用 index_sequence 将参数解包并调用 callback
    template<std::size_t... I>
    Return callImpl(const std::vector<std::any>& args, std::index_sequence<I...>)
    {
        return callback(std::any_cast<Args>(args[I])...);
    }
};

class EventManager {
public:
    // 注册带参数的事件回调 (函数指针)
    template <typename Return, typename... DefaultArgs>
    void register_event(const std::string& eventName, Return(*callback)(DefaultArgs...), DefaultArgs... defaultArgs)
    {
        // 将函数指针转换为 std::function
        std::function<Return(DefaultArgs...)> func = callback;

        // 调用原有的 register_event 方法
        register_event_f(eventName, std::move(func), std::forward<DefaultArgs>(defaultArgs)...);
    }
    // 注册带参数的事件回调
    template <typename Return, typename... DefaultArgs>
    void register_event_f(const std::string& eventName, std::function<Return(DefaultArgs...)> callback, DefaultArgs... defaultArgs)
    {
        auto eventInfo = std::make_unique<EventInformation<Return, DefaultArgs...>>();
        eventInfo->callback = std::move(callback);

        // 将 defaultArgs 存储到 defaultArgs vector 中
        eventInfo->defaultArgs = { std::make_any<DefaultArgs>(defaultArgs)... };

        // 存储事件信息
        eventListeners[eventName] = std::move(eventInfo);
    }

    // 调用单个事件，并返回其结果
    template<typename Return, typename... Args>
    Return call(const std::string& name, Args... args)
    {
        auto fun = eventListeners.find(name);
        if (fun == eventListeners.end())
            throw std::runtime_error("Event not found!");

        auto eventInfo = dynamic_cast<EventBase<Return>*>(fun->second.get());
        if (!eventInfo)
            throw std::runtime_error("Invalid return type!");

        // 将新参数打包为 std::vector<std::any>
        std::vector<std::any> newArgs = { std::make_any<Args>(args)... };

        return eventInfo->call(newArgs);
    }

    // 无返回值的事件调用重载
    template<typename... Args>
    void call_void(const std::string& name, Args... args)
    {
        call<void, Args...>(name, args...);
    }

    // 调用所有已注册事件并返回结果
    template <typename Return, typename... Args>
    std::vector<Return> call_all(Args... args)
    {
        std::vector<Return> results;
        std::vector<std::any> newArgs = {std::make_any<Args>(args)...};

        for (auto &[name, eventBase] : eventListeners)
        {
            auto eventInfo = dynamic_cast<EventBase<Return> *>(eventBase.get());
            if (eventInfo)
            {
                results.push_back(eventInfo->call(newArgs));
            }
        }
        return results;
    }

    // 无返回值的所有事件调用重载
    template <typename... Args>
    void call_all_void(Args... args)
    {
        for (auto &[name, eventBase] : eventListeners)
        {
            auto eventInfo = dynamic_cast<EventBase<void> *>(eventBase.get());
            if (eventInfo)
            {
                eventInfo->call({std::make_any<Args>(args)...});
            }
        }
    }
    /// @brief 删除指定名称的函数
    /// @param name 当name为空时清空所有
    bool clear(const std::string& name)
    {
        if(name.empty())
        {
            eventListeners.clear();
            return true;
        }
        auto it=eventListeners.find(name);
        if(it==eventListeners.end())
        {
            return false;
        }
        eventListeners.erase(it);
        return true;
    }
private:
    std::unordered_map<std::string, std::unique_ptr<EventBaseBase>> eventListeners;
};

} // namespace OneKeyEngine