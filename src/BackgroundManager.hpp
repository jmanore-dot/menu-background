#pragma once

#include <Geode/Geode.hpp>
#include <filesystem>
#include <string>
#include <vector>

// Handles storing, selecting and applying custom menu backgrounds.
namespace bgm {
    // Folder where imported images are copied to.
    std::filesystem::path imagesDir();

    // File names (inside imagesDir) of all imported images.
    std::vector<std::string> getImages();

    // Current selection: -1 = default GD background, 0..n-1 = imported image.
    int getIndex();
    void setIndex(int index);

    // Copies an image into the mod's folder, selects it. Returns an error message on failure.
    geode::Result<> addImage(std::filesystem::path const& source);

    // Cycle selection (includes the default background).
    void next();
    void prev();

    // Removes the currently selected image from the list and deletes the copy.
    bool removeCurrent();

    // Apply selection to a MenuLayer / to the MenuLayer in the running scene.
    void apply(MenuLayer* layer);
    void applyToScene();
}
