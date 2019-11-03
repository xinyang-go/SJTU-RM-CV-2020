//
// Created by xinyang on 2019/11/2.
//

#ifndef _MAINFSM_HPP_
#define _MAINFSM_HPP_

#include <FSM/FSM.hpp>
#include <Algorithm>
#include <map>

template<typename StateType>
class MainFSM : public FSM<StateType, std::function<bool(cv::Mat &, GimbalControlData &)>> {
private:
    using Base = FSM<StateType, std::function<bool(cv::Mat &, GimbalControlData &)>>;
    using AlgMap = std::map<StateType, std::shared_ptr<Algorithm>>;

    AlgMap algs;
public:
    MainFSM(StateType &state, const AlgMap &algs) : Base(state), algs(algs) {
        for (auto[s, a] : algs) {
            Base::registerOnStateFunc(s, [=](cv::Mat &src, GimbalControlData &data) { return a->run(src, data); });
            Base::registerOnEnterFunc(s, [=]() { return a->onEnter(); });
            Base::registerOnExitFunc(s, [=]() { return a->onExit(); });
        }
    }
};

#endif /* _MAINFSM_HPP_ */
