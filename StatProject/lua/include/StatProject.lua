-- Load the binary module
local path = "../../bin/debug/statproject.so";
local f = assert(package.loadlib(path, "__lua_main"));
StatProject = f();