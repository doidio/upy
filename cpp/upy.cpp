#include "upy.h"
#include <iostream>
#include <pybind11/embed.h>
#include <pybind11/numpy.h>

namespace py = pybind11;
using namespace pybind11::literals;

std::string upy::init() {
    try {
        static py::scoped_interpreter guard{};
    }
    catch (const std::exception &e) {
        std::cerr << __FUNCTION__ << std::endl;
        std::cerr << e.what() << std::endl;
        return e.what();
    }
    return {};
}

std::string upy::serve(const std::string &address, int32_t port, const std::string &authkey,
                       const std::function<void(std::string, std::map<std::string, std::string>)> &callback) {
    try {
        py::globals()["address"] = address;
        py::globals()["port"] = port;
        py::globals()["authkey"] = py::bytes(authkey);

        py::exec(R"(
import threading
from multiprocessing import Process, Queue
from multiprocessing.managers import BaseManager


def serve(q: Queue, address: str, port: int, authkey: bytes):
    manager = BaseManager((address, port), authkey)
    manager.register('q', callable=lambda: q)
    manager.get_server().serve_forever()


queue = Queue()
p = threading.Thread(target=serve, args=(queue, address, port, authkey), daemon=True)
p.start()
    )");

        py::object queue = py::globals()["queue"];

        while (py::object event = queue.attr("get")()) {
            try {
                auto message = event["message"].cast<std::string>();

                std::map<std::string, std::string> payload;
                for (const auto &item: event["payload"].cast<py::dict>()) {
                    auto name = item.first.cast<std::string>();
                    auto data = item.second.cast<std::string>();
                    payload.insert({name, data});
                }

                if (callback) {
                    callback(message, payload);
                }
            }
            catch (const std::exception &e) {
                std::cerr << __FUNCTION__ << std::endl;
                std::cerr << e.what() << std::endl;
            }
        }
    }
    catch (const std::exception &e) {
        std::cerr << __FUNCTION__ << std::endl;
        std::cerr << e.what() << std::endl;
        return e.what();
    }
    return {};
}
