#pragma once

#include <filesystem>

namespace Storage {

class FileSystemStorage {
public:
    void load(const std::filesystem::path& path);
};

} // namespace Storage