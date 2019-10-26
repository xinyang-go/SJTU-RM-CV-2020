//
// Created by xinyang on 2019/10/25.
//

#ifndef _FSM_HPP_
#define _FSM_HPP_

#include <map>
#include <functional>
#include <type_traits>

template<typename StateType, typename OnStateFuncType=std::function<void()>>
class FSM{
private:
    using OnStateFunc = OnStateFuncType;
    using OnEnterFunc = std::function<void()>;
    using OnExitFunc = std::function<void()>;

    using OnStateMap = std::map<StateType, OnStateFunc>;
    using OnEnterMap = std::map<StateType, std::vector<OnEnterFunc>>;
    using OnExitMap = std::map<StateType, std::vector<OnExitFunc>>;

    OnStateMap onState;
    OnEnterMap onEnter;
    OnExitMap onExit;

    const StateType &state;
    StateType lastState;
public:
    explicit FSM(const StateType &state) : state(state), lastState(state){
    }

    void registerOnStateFunc(const StateType &s, const OnStateFunc &func){
        onState.insert(std::pair(s, func));
    }

    void registerOnEnterFunc(const StateType &s, const OnEnterFunc &func){
        onEnter[s].emplace_back(func);
    }

    void registerOnExitFunc(const StateType &s, const OnExitFunc &func){
        onExit[s].emplace_back(func);
    }

    template <typename ...Params>
    typename std::result_of<OnStateFuncType(Params...)>::type step(Params ...params){
        auto tmpLast = lastState;
        auto tmpCurr = state;
        lastState = tmpCurr;
        if(tmpCurr != tmpLast){
            for(auto func:onExit[tmpLast]){
                func();
            }
            for(auto func:onEnter[tmpCurr]){
                func();
            }
        }
        if(onState.find(state) != onState.end()){
            return onState[state](params...);
        }
        return typename std::result_of<OnStateFuncType(Params...)>::type();
    }
};


#endif /* _FSM_HPP_ */
