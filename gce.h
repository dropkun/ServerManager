#include <string>
#include <google/cloud/compute/instances/v1/instances_client.h>

namespace gce = google::cloud::compute_instances_v1;

class InstanceController
{
private:
    std::string _project;
    std::string _zone;
    std::string _instanceName;

public:
    InstanceController(std::string project, std::string zone, std::string instanceName);
    ~InstanceController();
    std::string Start();
    std::string Stop();
    std::string GetExternalIP();
};
