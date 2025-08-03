#include "macho_loader.hpp"

#include <fstream>
#include <mach/vm_page_size.h>
#include <mach-o/loader.h>

auto MachoLoader::open(std::filesystem::path const& path) -> bool { 
    if (!std::filesystem::exists(path)) {
        return false;
    }

    auto file = std::ifstream(path, std::ios::binary);

    if (!file.is_open()) {
        return false;
    }

    buffer_ = std::vector<uint8_t>(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());

    return buffer_.empty() == false; 
}
auto MachoLoader::mach_header() const -> mach_header_64* { 
    return (mach_header_64*) buffer_.data(); 
}

auto MachoLoader::image_size() const -> size_t { 
    auto header = mach_header();

    size_t image_size = 0;

    load_command* cmd = (load_command*)(header + 1);

    for (auto i = 0; i < header->ncmds; i++) {
        if (cmd->cmd == LC_SEGMENT_64) {
            auto seg = (segment_command_64*) cmd;

            uint64_t seg_end = seg->vmaddr + seg->vmsize;
            if (seg_end > image_size) {
                image_size = seg_end;
            }
        }

        cmd = (load_command*)((uint8_t*) cmd + cmd->cmdsize);
    }

    image_size = (image_size + vm_page_size - 1) & ~(vm_page_size - 1);
    return image_size;
}

auto MachoLoader::get_section(const char* segment, const char* section) -> section_64* { 
    auto header = mach_header();

    load_command* cmd = (load_command*)(header + 1);

    for (auto i = 0; i < header->ncmds; i++) {
        if (cmd->cmd == LC_SEGMENT_64) {
            auto seg = (segment_command_64*) cmd;

            if (strcmp(seg->segname, segment) == 0) {
                section_64* sect = (section_64*)(seg + 1);

                for (auto j = 0; j < seg->nsects; j++) {
                    if (strcmp(sect->sectname, section) == 0) {
                        return sect;
                    }

                    sect++;
                }
            }
        }

        cmd = (load_command*)((uint8_t*) cmd + cmd->cmdsize);
    }

    return nullptr;
}

auto MachoLoader::for_each_segment(std::function<void(segment_command_64* segm)> callback) -> void {
    auto header = mach_header();

    load_command* cmd = (load_command*)(header + 1);

    for (auto i = 0; i < header->ncmds; i++) {
        if (cmd->cmd == LC_SEGMENT_64) {
            auto seg = (segment_command_64*) cmd;

            if (seg->nsects != 0) {
                callback(seg);
            }
        }

        cmd = (load_command*)((uint8_t*) cmd + cmd->cmdsize);
    }
}
