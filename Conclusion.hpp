#ifndef CONCLUSION_HPP
#define CONCLUSION_HPP

#include "CheckServices.hpp"
#include "CheckTasks.hpp"
#include "ConfigReader.hpp"



class Conclusion {
public:
    Conclusion();

    void print(const std::vector<ServiceInfo> & requiredServStates,
               const std::vector<ServicesState> & currentServStates,
               const std::vector<TaskInfo> & requiredTaskStates,
               const std::vector<TaskState> & currentTaskStates);

private:
    static bool _stateIn(ServicesState::CurrentState current,
                         const std::vector<ServicesState::CurrentState> & required);
    static bool _stateIn(ServicesState::StartType current,
                         const std::vector<ServicesState::StartType> & required);
    static bool _stateIn(TaskState current,
                         const std::vector<TaskState> & required);

    void _setRedColor();
    void _setGreenColor();
    void _setDefaultColor();

    static const wchar_t * _toStr(ServicesState::CurrentState v);
    static const wchar_t * _toStr(ServicesState::StartType v);
    static const wchar_t * _toStr(TaskState v);

private:
    void * _outHandle;
    bool _allServicesOkey = false;
    bool _allTasksOkey = false;
};



#endif // CONCLUSION_HPP
