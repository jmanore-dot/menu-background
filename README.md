# Menu Background Picker (Geode mod)

Click a button on the main menu, pick an image, and switch between your saved backgrounds.

## Before building
1. Edit `mod.json`: change `id` (e.g. `yourname.menu-background`), `developer`, and `repository`.
2. Install the Geode CLI and SDK: https://docs.geode-sdk.org/getting-started/

## Build
```
geode build
```
The resulting `.geode` file is placed in `build/`. Copy it into
`Geometry Dash/geode/mods/` (or use `geode package install`).

## Build without a local setup
Push this folder to GitHub; the included workflow (`.github/workflows/build.yml`)
builds Windows, macOS, Android and iOS versions automatically.

## How it works
- `src/main.cpp` – hooks `MenuLayer`, adds the button, applies the saved background on load.
- `src/BGPopup.*` – the popup with Add / Prev / Next / Remove / Default.
- `src/BackgroundManager.*` – copies images into the mod save dir, tracks selection, draws the background sprite
  (scaled to cover the screen) and hides the default menu background.

Supported formats: PNG, JPG. Very large images (above your GPU's max texture size, usually 4096–8192 px) may fail to load.
