#include "Conclusion.hpp"

#include <iostream>



int main() {
    ConfigReader cr;
    cr.readConfig();

    if (!cr.readConfig()) {
        std::wcout << cr.getErrorMessage() << std::endl;
        std::cin.get();
        return 1;
    }

    const std::vector<ServiceInfo> & requiredServStates = cr.getServicesInfo();
    const std::vector<TaskInfo> & requiredTaskStates = cr.getTaskInfo();

    std::vector<ServicesState> servState;

    {
        std::vector<std::string> servNames;
        for (const auto & s : requiredServStates) {
            // Service names must be in ASCII so this should work correctly
            servNames.push_back(std::string(s.name.begin(), s.name.end()));
        }
        servState = checkServices(servNames);
    }

    std::vector<TaskState> taskState;

    for (const auto & t : requiredTaskStates) {
        taskState.push_back(checkTask(t.taskPath, t.taskName));
    }

    Conclusion c;
    c.print(requiredServStates, servState, requiredTaskStates, taskState);

    std::cin.get();
    return 0;
}
