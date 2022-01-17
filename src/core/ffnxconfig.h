/****************************************************************************
 ** FF8frPack FF8.fr Pack configurator and installer
 ** Copyright (C) 2022 Arzel Jérôme <myst6re@gmail.com>
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/
#pragma once

#define TOML_EXCEPTIONS 0
#include <toml++/toml.h>
#include <QString>

class FFNxConfig
{
public:
    enum Renderer {
        RendererAuto = 0,
        OpenGL,
        Direct3D9,
        Direct3D11,
        Direct3D12,
        Vulkan
    };

    FFNxConfig();
    explicit FFNxConfig(const QString &fileName);
    bool load();
    bool save();
    inline bool isLoaded() const {
        return _isLoaded;
    }
    inline QString fileName() const {
        return _fileName;
    }
    inline void setFileName(const QString &fileName) {
        _fileName = fileName;
    }
    inline QString errorString() const {
        return _lastError;
    }
    Renderer renderer(bool &ok) const;
    void setRenderer(Renderer renderer);
    bool isFullscreen() const;
    void setFullscreen(bool fullscreen);
    bool isExternalMusicEnabled() const;
    void setExternalMusicEnabled(bool enabled);
private:
    QString _fileName, _lastError;
    toml::table _config;
    bool _isLoaded;
};
