#include "ConfigReader.hpp"

#include <fstream>
#include <wchar.h>



bool ConfigReader::readConfig(const std::string & fileName) {
    _servicesInfo.clear();
    _tasksInfo.clear();
    _curentSection = None;

    std::wifstream file;
    file.open(fileName);

    if (!file.is_open()) {
        return false;
    }

    while (!file.eof()) {
        std::wstring line = _readLine(file);
        if (line.empty()) { continue; }



        if (line == L"SERVICES") {
            _curentSection = Services;
            continue;
        }
        else if (line == L"TASKS") {
            _curentSection = Tasks;
            continue;
        }
        else if (_curentSection == None) {
            return false;
        }
        else if (_curentSection == Services) {
            std::wstring currentStateLine = _readLine(file);
            std::wstring startTypeLine = _readLine(file);

            if (currentStateLine.empty() || startTypeLine.empty()) {
                return false;
            }

            std::vector<ServicesState::CurrentState> currentState =
                _parseServicesCurrentState(currentStateLine);

            if (currentState.empty()) {
                return false;
            }

            std::vector<ServicesState::StartType> startType =
                _parseServicesStartType(startTypeLine);

            if (startType.empty()) {
                return false;
            }

            ServiceInfo info;
            info.name = std::move(line);
            info.requiredCurrentState = std::move(currentState);
            info.requiredStartType = std::move(startType);

            _servicesInfo.push_back(info);
        }
        else if (_curentSection == Tasks) {
            std::wstring taskName = _readLine(file);
            std::wstring stateLine = _readLine(file);

            if (taskName.empty() || stateLine.empty()) {
                return false;
            }

            std::vector<TaskState> states = _parseTaskState(stateLine);

            if (states.empty()) {
                return false;
            }

            TaskInfo info;
            info.taskPath = std::move(line);
            info.taskName = std::move(taskName);
            info.requiredState = std::move(states);

            _tasksInfo.push_back(info);
        }
    }

    return true;
}



const std::vector<ServiceInfo> & ConfigReader::getServicesInfo() const {
    return _servicesInfo;
}

const std::vector<TaskInfo> & ConfigReader::getTaskInfo() const {
    return _tasksInfo;
}



void ConfigReader::_removeSpaces(std::wstring & line) {
    size_t f = 0;
    size_t e = line.size() - 1;

    while ((line[f] == L' ' || line[f] == L'\t') && f < line.size()) { ++f; }
    while ((line[e] == L' ' || line[e] == L'\t') && e > f) { --e; }

    line = line.substr(f, e - f + 1);
}

std::wstring ConfigReader::_readLine(std::wifstream & file) {
    while (!file.eof()) {
        std::wstring line;
        std::getline(file, line);

        if (line.empty()) { continue; }

        const size_t pos = line.find_first_of(L"//");
        if (pos == 0) { continue; }

        if (pos != std::wstring::npos) {
            line = line.substr(0, pos);
        }

        _removeSpaces(line);
        if (line.empty()) { continue; }

        return line;
    }

    return std::wstring();
}

std::vector<TaskState> ConfigReader::_parseTaskState(std::wstring & line) {
    std::vector<TaskState> result;

    wchar_t * pwc = std::wcstok(&line[0], L"|");

    while (pwc != NULL) {
        std::wstring temp(pwc);
        _removeSpaces(temp);

        if (temp == L"ERROR") {
            result.push_back(TaskState::ERROR_TS);
        }
        else if (temp == L"DISABLED") {
            result.push_back(TaskState::DISABLED);
        }
        else if (temp == L"NOT_DISABLED") {
            result.push_back(TaskState::NOT_DISABLED);
        }
        else {
            return std::vector<TaskState>();
        }

        pwc = std::wcstok(NULL, L"|");
    }

    return result;
}

std::vector<ServicesState::CurrentState>
ConfigReader::_parseServicesCurrentState(std::wstring & line) {
    std::vector<ServicesState::CurrentState> result;

     wchar_t * pwc = std::wcstok(&line[0], L"|");

     while (pwc != NULL) {
        std::wstring temp(pwc);
        _removeSpaces(temp);

        if (temp == L"ERROR") {
            result.push_back(ServicesState::CurrentState::ERROR_CS);
        }
        else if (temp == L"STOPPED") {
            result.push_back(ServicesState::CurrentState::STOPPED);
        }
        else if (temp == L"RUNNING") {
            result.push_back(ServicesState::CurrentState::RUNNING);
        }
        else if (temp == L"PAUSED") {
            result.push_back(ServicesState::CurrentState::PAUSED);
        }
        else if (temp == L"START_PENDING") {
            result.push_back(ServicesState::CurrentState::START_PENDING);
        }
        else if (temp == L"STOP_PENDING") {
            result.push_back(ServicesState::CurrentState::STOP_PENDING);
        }
        else if (temp == L"CONTINUE_PENDING") {
            result.push_back(ServicesState::CurrentState::CONTINUE_PENDING);
        }
        else if (temp == L"PAUSE_PENDING") {
            result.push_back(ServicesState::CurrentState::PAUSE_PENDING);
        }
        else {
            return std::vector<ServicesState::CurrentState>();
        }

        pwc = std::wcstok(NULL, L"|");
     }

    return result;
}

std::vector<ServicesState::StartType>
ConfigReader::_parseServicesStartType(std::wstring & line) {
    std::vector<ServicesState::StartType> result;

     wchar_t * pwc = std::wcstok(&line[0], L"|");

     while (pwc != NULL) {
        std::wstring temp(pwc);
        _removeSpaces(temp);

        if (temp == L"ERROR") {
            result.push_back(ServicesState::StartType::ERROR_ST);
        }
        else if (temp == L"DISABLED") {
            result.push_back(ServicesState::StartType::DISABLED);
        }
        else if (temp == L"DEMAND_START") {
            result.push_back(ServicesState::StartType::DEMAND_START);
        }
        else if (temp == L"AUTO_START") {
            result.push_back(ServicesState::StartType::AUTO_START);
        }
        else if (temp == L"SYSTEM_START") {
            result.push_back(ServicesState::StartType::SYSTEM_START);
        }
        else if (temp == L"BOOT_START") {
            result.push_back(ServicesState::StartType::BOOT_START);
        }
        else {
            return std::vector<ServicesState::StartType>();
        }

        pwc = std::wcstok(NULL, L"|");
     }

    return result;
}
