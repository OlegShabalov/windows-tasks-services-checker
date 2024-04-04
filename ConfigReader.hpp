#ifndef CONFIGREADER_HPP
#define CONFIGREADER_HPP

#include "CheckServices.hpp"
#include "CheckTasks.hpp"

#include <string>



struct ServiceInfo {
    std::wstring name;
    std::vector<ServicesState::CurrentState> requiredCurrentState;
    std::vector<ServicesState::StartType> requiredStartType;
};

struct TaskInfo {
    std::wstring taskPath;
    std::wstring taskName;
    std::vector<TaskState> requiredState;
};



class ConfigReader {
public:
    bool readConfig(const std::string & fileName = "config.txt");

    const std::vector<ServiceInfo> & getServicesInfo() const;
    const std::vector<TaskInfo> & getTaskInfo() const;

private:
    static void _removeSpaces(std::wstring & line);
    static std::wstring _readLine(std::wifstream & file);
    static std::vector<TaskState> _parseTaskState(std::wstring & line);

    static std::vector<ServicesState::CurrentState>
    _parseServicesCurrentState(std::wstring & line);

    static std::vector<ServicesState::StartType>
    _parseServicesStartType(std::wstring & line);

private:
    enum _Section : char {
        None,
        Tasks,
        Services,
    };

private:
    std::vector<ServiceInfo> _servicesInfo;
    std::vector<TaskInfo> _tasksInfo;
    _Section _curentSection;
};



#endif // CONFIGREADER_HPP
