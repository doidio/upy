#include <iostream>
#include "upy.h"

int main() {
    upy::init();
    upy::serve("", 50000, "",
               [](const std::string &message, const std::map<std::string, std::string> &payload) {
                   std::cout << message << std::endl;
                   for (const auto &item: payload) {
                       std::cout << item.first << " " << item.second.size() << std::endl;
                       auto num_floats = item.second.size() / sizeof(float);
                       auto *float_ptr = reinterpret_cast<const float *>(item.second.c_str());
                       for (auto i = 0; i < num_floats; i++) {
                           std::cout << float_ptr[i] << std::endl;
                       }
                   }
               });
    return EXIT_SUCCESS;
}
