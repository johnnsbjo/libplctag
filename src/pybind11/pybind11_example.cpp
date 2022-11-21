#include <pybind11/pybind11.h>
#include <stdio.h>
#include "../lib/libplctag.h"

namespace py = pybind11;

std::string decode_error(int error_code)
{
  std::string text = plc_tag_decode_error(error_code);
  return text;
};

void set_debug_level(int debug_level)
{plc_tag_set_debug_level(debug_level);
};

std::string check_lib_version(int major, int minor, int patch)
{
  int response_code = plc_tag_check_lib_version(major, minor, patch);
  return plc_tag_decode_error(response_code);
};

int32_t create_tag(const char *tag_string, int timeout)
{
  return plc_tag_create(tag_string, timeout);
};

void shutdown(void)
{
  plc_tag_shutdown();
};

std::string destroy_tag(int32_t tag_id) {
  int response_code = plc_tag_destroy(tag_id);
  return plc_tag_decode_error(response_code);
};

std::string read_tag(int32_t tag_id, int timeout) {
  int response_code = plc_tag_read(tag_id, timeout);
  return plc_tag_decode_error(response_code);
};

std::string read_tag_status(int32_t tag_id) {
  int response_code = plc_tag_status(tag_id);
  return plc_tag_decode_error(response_code);
}

std::string write_tag(int32_t tag_id, int timeout) {
  int response_code = plc_tag_read(tag_id, timeout);
  return plc_tag_decode_error(response_code);
};

PYBIND11_MODULE(pybind11_example, m)
{
  m.doc() = "a pybind11 example for libplctag";
  m.def("decode_error", &decode_error, "Decode an integer error into a text representation");
  m.def("set_debug_level", &set_debug_level, "change the debug level in the lib");
  m.def("check_lib_version", &check_lib_version, "Checks to see if the lib is comparitible with the passed arguments (X.Y.Z format)");
  m.def("create_tag", &create_tag, "register a tag with the internal library tag listing");
  m.def("shutdown", &shutdown, "closes the library and destroys any objects within");
  m.def("destroy_tag", &destroy_tag, "removes a tag from the tag table");
  m.def("read_tag", &read_tag, "initiate a tag read event, doesn't return a value but does return a response string");
  m.def("read_tag_status", &read_tag_status, "read the status of the given tag");
  m.def("write_tag", &write_tag, "write a value to a tag, doesn't return a value but does return a response string");
}