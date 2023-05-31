#pragma once

#include <memory>
#include <string>
#include <functional>
#include <map>

namespace upy {
    std::string init();

    std::string serve(const std::string &address, int32_t port, const std::string &authkey,
                      const std::function<void(std::string, std::map<std::string, std::string>)> &callback);
}
