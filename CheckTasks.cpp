#include "CheckTasks.hpp"

#include <iostream>
#include <taskschd.h>
#include <comutil.h>



TaskState checkTask(const std::wstring & path, const std::wstring & name) {
    CoInitializeEx(NULL, COINIT_MULTITHREADED);

    ITaskService * taskService = NULL;
    ITaskFolder * taskFolder = NULL;
    IRegisteredTask * registeredTask = NULL;
    ITaskDefinition * taskDefinition = NULL;
    ITaskSettings * taskSettings = NULL;
    VARIANT_BOOL enabled;

    HRESULT hr = CoCreateInstance(CLSID_TaskScheduler, NULL, CLSCTX_INPROC_SERVER, IID_ITaskService, (void **)&taskService);
    if (FAILED(hr)) {
        CoUninitialize();
        return TaskState::ERROR_TS;
    }

    hr = taskService->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t());
    if (FAILED(hr)) {
        taskService->Release();
        CoUninitialize();
        return TaskState::ERROR_TS;
    }

    hr = taskService->GetFolder(_bstr_t(path.c_str()), &taskFolder);
    if (FAILED(hr)) {
        taskService->Release();
        CoUninitialize();
        return TaskState::ERROR_TS;
    }

    hr = taskFolder->GetTask(_bstr_t(name.c_str()), &registeredTask);
    if (FAILED(hr)) {
        taskFolder->Release();
        taskService->Release();
        CoUninitialize();
        return TaskState::ERROR_TS;
    }

    hr = registeredTask->get_Definition(&taskDefinition);
    if (FAILED(hr)) {
        registeredTask->Release();
        taskFolder->Release();
        taskService->Release();
        CoUninitialize();
        return TaskState::ERROR_TS;
    }

    hr = taskDefinition->get_Settings(&taskSettings);
    if (FAILED(hr)) {
        taskDefinition->Release();
        registeredTask->Release();
        taskFolder->Release();
        taskService->Release();
        CoUninitialize();
        return TaskState::ERROR_TS;
    }

    hr = taskSettings->get_Enabled(&enabled);
    if (FAILED(hr)) {
        taskSettings->Release();
        taskDefinition->Release();
        registeredTask->Release();
        taskFolder->Release();
        taskService->Release();
        CoUninitialize();
        return TaskState::ERROR_TS;
    }

    taskSettings->Release();
    taskDefinition->Release();
    registeredTask->Release();
    taskFolder->Release();
    taskService->Release();
    CoUninitialize();

    if (enabled) {
        return TaskState::NOT_DISABLED;
    }
    else {
        return TaskState::DISABLED;
    }
}
