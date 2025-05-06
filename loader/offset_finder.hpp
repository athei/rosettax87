#pragma once

#include <filesystem>
#include <fstream>
#include <vector>
#include <iostream>
#include <cstdint>
#include <algorithm>

struct OffsetFinder {

    auto set_default_offsets() -> void;

    auto determine_offsets() -> void;

    std::uint64_t offset_loop_copy_func;
    std::uint64_t offset_svc_call_entry;
    std::uint64_t offset_svc_call_ret;
};