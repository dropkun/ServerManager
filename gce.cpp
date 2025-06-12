#include "gce.h"
#include <iostream>

InstanceController::InstanceController(std::string project, std::string zone, std::string instanceName) : _project(project), _zone(zone), _instanceName(instanceName)
{
}

InstanceController::~InstanceController()
{
}

std::string InstanceController::Start()
{
    auto options = google::cloud::Options{};
    auto client = gce::InstancesClient(google::cloud::compute_instances_v1::MakeInstancesConnectionRest(options));

    auto f = client.Start(_project, _zone, _instanceName);
    auto result = f.get(); // futureの完了を待つ
    if (!result)
    {
        return "Failed to launch: " + result.status().message();
    }

    return "A server launched.";
}

std::string InstanceController::Stop()
{
    auto options = google::cloud::Options{};
    auto client = gce::InstancesClient(google::cloud::compute_instances_v1::MakeInstancesConnectionRest(options));

    auto f = client.Stop(_project, _zone, _instanceName);
    auto result = f.get(); // futureの完了を待つ
    if (!result)
    {
        return "Failed to stop: " + result.status().message();
    }

    return "A server stopped.";
}

// 外部IP取得
std::string InstanceController::GetExternalIP()
{
    auto options = google::cloud::Options{};
    auto client = gce::InstancesClient(google::cloud::compute_instances_v1::MakeInstancesConnectionRest(options));

    auto instance_or = client.GetInstance(_project, _zone, _instanceName);
    if (!instance_or.ok())
    {
        return "Error: " + instance_or.status().message();
    }
    auto instance = *instance_or;
    if (instance.network_interfaces_size() == 0)
        return "No network interfaces";

    const auto &access_configs = instance.network_interfaces(0).access_configs();
    if (access_configs.size() == 0)
        return "No access configs";

    const auto &ip = access_configs[0].nat_ip();
    return ip.empty() ? "No external IP" : ip;
}