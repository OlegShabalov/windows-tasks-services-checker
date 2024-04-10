#include "Conclusion.hpp"

#include <windows.h>
#include <iostream>



Conclusion::Conclusion() {
    _outHandle = GetStdHandle(STD_OUTPUT_HANDLE);
}



void Conclusion::print(const std::vector<ServiceInfo> & requiredServStates,
                       const std::vector<ServicesState> & currentServStates,
                       const std::vector<TaskInfo> & requiredTaskStates,
                       const std::vector<TaskState> & currentTaskStates)
{
    _allServicesOkey = true;
    _allTasksOkey = true;

    std::wcout << L"SERVICES:\n\n";

    if (requiredServStates.empty()) {
        std::wcout << L"No services\n\n";
    }

    for (int i=0; i<requiredServStates.size(); ++i) {
        std::wcout << requiredServStates[i].name << L'\n';

        if (_stateIn(currentServStates[i].currentState,
                     requiredServStates[i].requiredCurrentState))
        {
            _setGreenColor();
            std::wcout << L"OK:  ";
            _setDefaultColor();
        }
        else {
            _setRedColor();
            std::wcout << L"BAD: ";
            _setDefaultColor();
            _allServicesOkey = false;
        }

        std::wcout << _toStr(currentServStates[i].currentState) << L" [" <<
                      _toStr(requiredServStates[i].requiredCurrentState[0]);
        for (int j=1; j<requiredServStates[i].requiredCurrentState.size(); ++j) {
            std::wcout << L"|" << _toStr(requiredServStates[i].requiredCurrentState[j]);
        }
        std::wcout << L"]\n";

        if (_stateIn(currentServStates[i].startType,
                     requiredServStates[i].requiredStartType))
        {
            _setGreenColor();
            std::wcout << L"OK:  ";
            _setDefaultColor();
        }
        else {
            _setRedColor();
            std::wcout << L"BAD: ";
            _setDefaultColor();
            _allServicesOkey = false;
        }

        std::wcout << _toStr(currentServStates[i].startType) << L" [" <<
                      _toStr(requiredServStates[i].requiredStartType[0]);
        for (int j=1; j<requiredServStates[i].requiredStartType.size(); ++j) {
            std::wcout << L"|" << _toStr(requiredServStates[i].requiredStartType[j]);
        }
        std::wcout << L"]\n\n";
    }



    std::wcout << L"\n\nTASKS:\n\n";

    if (requiredTaskStates.empty()) {
        std::wcout << L"No tasks\n\n";
    }

    for (int i=0; i<requiredTaskStates.size(); ++i) {
        std::wcout << requiredTaskStates[i].taskName << L" | " <<
                      requiredTaskStates[i].taskPath << L'\n';

        if (_stateIn(currentTaskStates[i], requiredTaskStates[i].requiredState)) {
            _setGreenColor();
            std::wcout << L"OK:  ";
            _setDefaultColor();
        }
        else {
            _setRedColor();
            std::wcout << L"BAD: ";
            _setDefaultColor();
            _allTasksOkey = false;
        }

        std::wcout << _toStr(currentTaskStates[i]) << L" [" <<
                      _toStr(requiredTaskStates[i].requiredState[0]);
        for (int j=1; j<requiredTaskStates[i].requiredState.size(); ++j) {
            std::wcout << L"|" << _toStr(requiredTaskStates[i].requiredState[j]);
        }
        std::wcout << L"]\n\n";
    }



    std::wcout << L"\n\nSUMMARY:\n";

    std::wcout << L"Services: ";
    if (_allServicesOkey) {
        _setGreenColor();
        std::wcout << L"OK\n";
        _setDefaultColor();
    }
    else {
        _setRedColor();
        std::wcout << L"BAD\n";
        _setDefaultColor();
    }

    std::wcout << L"Tasks:    ";
    if (_allTasksOkey) {
        _setGreenColor();
        std::wcout << L"OK\n";
        _setDefaultColor();
    }
    else {
        _setRedColor();
        std::wcout << L"BAD\n";
        _setDefaultColor();
    }

    std::wcout << "\n\n";
}



bool Conclusion::_stateIn(
    ServicesState::CurrentState current,
    const std::vector<ServicesState::CurrentState> & required)
{
    for (int i=0; i<required.size(); ++i) {
        if (required[i] == current) { return true; }
    }
    return false;
}

bool Conclusion::_stateIn(
    ServicesState::StartType current,
    const std::vector<ServicesState::StartType> & required)
{
    for (int i=0; i<required.size(); ++i) {
        if (required[i] == current) { return true; }
    }
    return false;
}

bool Conclusion::_stateIn(
    TaskState current,
    const std::vector<TaskState> & required)
{
    for (int i=0; i<required.size(); ++i) {
        if (required[i] == current) { return true; }
    }
    return false;
}



void Conclusion::_setRedColor() {
    SetConsoleTextAttribute(_outHandle, FOREGROUND_RED | FOREGROUND_INTENSITY);
}

void Conclusion::_setGreenColor() {
    SetConsoleTextAttribute(_outHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
}

void Conclusion::_setDefaultColor() {
    SetConsoleTextAttribute(_outHandle, FOREGROUND_BLUE | FOREGROUND_RED |
                                        FOREGROUND_GREEN);
}



const wchar_t * Conclusion::_toStr(ServicesState::CurrentState v) {
    if (v == ServicesState::STOPPED) {
        return L"STOPPED";
    }
    else if (v == ServicesState::RUNNING) {
        return L"RUNNING";
    }
    else if (v == ServicesState::PAUSED) {
        return L"PAUSED";
    }
    else if (v == ServicesState::START_PENDING) {
        return L"START_PENDING";
    }
    else if (v == ServicesState::STOP_PENDING) {
        return L"STOP_PENDING";
    }
    else if (v == ServicesState::CONTINUE_PENDING) {
        return L"CONTINUE_PENDING";
    }
    else if (v == ServicesState::PAUSE_PENDING) {
        return L"PAUSE_PENDING";
    }
    else {
        return L"ERROR";
    }
}

const wchar_t * Conclusion::_toStr(ServicesState::StartType v) {
    if (v == ServicesState::DISABLED) {
        return L"DISABLED";
    }
    else if (v == ServicesState::DEMAND_START) {
        return L"DEMAND_START";
    }
    else if (v == ServicesState::AUTO_START) {
        return L"AUTO_START";
    }
    else if (v == ServicesState::SYSTEM_START) {
        return L"SYSTEM_START";
    }
    else if (v == ServicesState::BOOT_START) {
        return L"BOOT_START";
    }
    else {
        return L"ERROR";
    }
}

const wchar_t * Conclusion::_toStr(TaskState v) {
    if (v == TaskState::DISABLED) {
        return L"DISABLED";
    }
    if (v == TaskState::NOT_DISABLED) {
        return L"NOT_DISABLED";
    }
    else {
        return L"ERROR";
    }
}
