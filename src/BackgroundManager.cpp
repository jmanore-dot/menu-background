#include "BackgroundManager.hpp"

#include <chrono>
#include <system_error>

using namespace geode::prelude;

namespace {
    constexpr auto KEY_IMAGES = "images";
    constexpr auto KEY_INDEX = "current-index";

    std::string s_lastTextureKey;

    void saveImages(std::vector<std::string> const& images) {
        Mod::get()->setSavedValue<std::vector<std::string>>(KEY_IMAGES, images);
    }

    std::string lowerExt(std::filesystem::path const& p) {
        auto ext = geode::utils::string::pathToString(p.extension());
        for (auto& c : ext) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        return ext;
    }
}

namespace bgm {

std::filesystem::path imagesDir() {
    return Mod::get()->getSaveDir() / "backgrounds";
}

std::vector<std::string> getImages() {
    return Mod::get()->getSavedValue<std::vector<std::string>>(KEY_IMAGES, {});
}

int getIndex() {
    return static_cast<int>(Mod::get()->getSavedValue<int64_t>(KEY_INDEX, -1));
}

void setIndex(int index) {
    Mod::get()->setSavedValue<int64_t>(KEY_INDEX, index);
}

Result<> addImage(std::filesystem::path const& source) {
    auto ext = lowerExt(source);
    if (ext != ".png" && ext != ".jpg" && ext != ".jpeg") {
        return Err("Only PNG and JPG images are supported.");
    }

    std::error_code ec;
    std::filesystem::create_directories(imagesDir(), ec);
    if (ec) {
        return Err("Could not create the backgrounds folder: " + ec.message());
    }

    auto stamp = std::chrono::system_clock::now().time_since_epoch().count();
    auto name = std::to_string(stamp) + ext;
    auto dest = imagesDir() / name;

    std::filesystem::copy_file(source, dest, std::filesystem::copy_options::overwrite_existing, ec);
    if (ec) {
        return Err("Could not copy the image: " + ec.message());
    }

    auto images = getImages();
    images.push_back(name);
    saveImages(images);
    setIndex(static_cast<int>(images.size()) - 1);
    return Ok();
}

void next() {
    int n = static_cast<int>(getImages().size());
    int idx = getIndex() + 1;
    if (idx > n - 1) idx = -1;
    setIndex(idx);
}

void prev() {
    int n = static_cast<int>(getImages().size());
    int idx = getIndex() - 1;
    if (idx < -1) idx = n - 1;
    setIndex(idx);
}

bool removeCurrent() {
    auto images = getImages();
    int idx = getIndex();
    if (idx < 0 || idx >= static_cast<int>(images.size())) return false;

    std::error_code ec;
    std::filesystem::remove(imagesDir() / images[idx], ec);

    images.erase(images.begin() + idx);
    saveImages(images);
    setIndex(-1);
    return true;
}

void apply(MenuLayer* layer) {
    if (!layer) return;

    if (auto old = layer->getChildByID("bg-image"_spr)) {
        old->removeFromParent();
    }

    // The default GD menu background lives inside MenuGameLayer.
    CCSprite* defaultBG = nullptr;
    if (layer->m_menuGameLayer) {
        defaultBG = layer->m_menuGameLayer->m_backgroundSprite;
    }

    auto images = getImages();
    int idx = getIndex();

    bool custom = false;
    if (idx >= 0 && idx < static_cast<int>(images.size())) {
        auto path = imagesDir() / images[idx];
        std::error_code ec;
        if (std::filesystem::exists(path, ec)) {
            auto key = geode::utils::string::pathToString(path);
            auto tex = CCTextureCache::get()->addImage(key.c_str(), false);
            if (tex) {
                auto spr = CCSprite::createWithTexture(tex);
                auto win = CCDirector::get()->getWinSize();
                auto size = spr->getContentSize();
                float scale = std::max(win.width / size.width, win.height / size.height);
                spr->setScale(scale);
                spr->setPosition(win / 2);
                spr->setID("bg-image"_spr);
                layer->addChild(spr, -100);
                custom = true;

                // free the previously used texture
                if (!s_lastTextureKey.empty() && s_lastTextureKey != key) {
                    CCTextureCache::get()->removeTextureForKey(s_lastTextureKey.c_str());
                }
                s_lastTextureKey = key;
            }
            else {
                log::warn("Failed to load background image {}", key);
            }
        }
    }

    if (defaultBG) {
        defaultBG->setVisible(!custom);
    }
}

void applyToScene() {
    auto scene = CCDirector::get()->getRunningScene();
    if (!scene) return;
    if (auto layer = scene->getChildByType<MenuLayer>(0)) {
        apply(layer);
    }
}

}
