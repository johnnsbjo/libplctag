#include <pybind11/pybind11.h>
#include <stdio.h>
#include "../lib/libplctag.h"

namespace py = pybind11;

int get_lib_version(int major, int minor, int patch) {
  int response_code = plc_tag_check_lib_version(major, minor, patch);
  return response_code;
}

PYBIND11_MODULE(pybind11_example, m) {
  m.doc() = "a pybind11 example for libplctag";
  m.def("get_lib_version", &get_lib_version, "Test the libplctag version");
}