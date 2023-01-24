#include "media.hpp"

#include <fstream>

Media Media::fromBytes(std::string contents) {
    return Media(std::move(contents));
}
Media Media::fromFile(std::string_view fileName) {
    auto fileStream = std::ifstream(fileName.data());
    fileStream.seekg(0, std::ios::end);
    std::size_t size = fileStream.tellg();
    fileStream.seekg(0);

    auto contents = std::string(size, '\0');
    fileStream.read(contents.data(), size);

    return Media::fromBytes(std::move(contents));
}

std::string_view Media::contents() const {
    return contents_;
} 
