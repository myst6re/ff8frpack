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
#include "ffnxconfig.h"

#include <QCoreApplication>
#include <QFile>
#include <sstream>

constexpr const char *CFG_FULLSCREEN = "fullscreen";
constexpr const char *CFG_USE_EXTERNAL_MUSIC = "use_external_music";
constexpr const char *CFG_RENDERER_BACKEND = "renderer_backend";

FFNxConfig::FFNxConfig() :
    _isLoaded(false)
{
}

FFNxConfig::FFNxConfig(const QString &fileName) :
    _fileName(fileName), _isLoaded(false)
{
}

bool FFNxConfig::load()
{
    _isLoaded = false;

    if (! QFile::exists(_fileName)) {
        _lastError = qApp->translate("FFNxConfig", "The file \"%1\" does not exist")
                .arg(_fileName);
        return false;
    }

    const toml::parse_result result = toml::parse_file(_fileName.toStdString());

    if (! result) {
        _lastError = QString::fromStdString(std::string{result.error().description()});
        return false;
    }

    _config = std::move(result).table();
    _isLoaded = true;

    return true;
}

bool FFNxConfig::save()
{
    QFile f(_fileName);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        _lastError = f.errorString();
        return false;
    }

    std::stringstream buffer;

    buffer << _config;

    const std::string str = buffer.str();
    const qint64 size = qint64(str.length());

    if (!f.write(str.c_str(), size)) {
        _lastError = f.errorString();
        f.close();
        return false;
    }

    f.close();

    return false;
}

bool FFNxConfig::isFullscreen() const
{
    return _config[CFG_FULLSCREEN].value_or(false);
}

void FFNxConfig::setFullscreen(bool fullscreen)
{
    _config[CFG_FULLSCREEN].visit([fullscreen](auto&& el) noexcept {
        if constexpr (toml::is_boolean<decltype(el)>) {
            el = fullscreen;
        }
    });
}

bool FFNxConfig::isExternalMusicEnabled() const
{
    return _config[CFG_USE_EXTERNAL_MUSIC].value_or(false);
}

void FFNxConfig::setExternalMusicEnabled(bool enabled)
{
    _config[CFG_USE_EXTERNAL_MUSIC].visit([enabled](auto&& el) noexcept {
        if constexpr (toml::is_boolean<decltype(el)>) {
            el = enabled;
        }
    });
}

FFNxConfig::Renderer FFNxConfig::renderer(bool &ok) const
{
    int renderer = _config[CFG_RENDERER_BACKEND].value_or(int(RendererAuto));

    ok = renderer >= RendererAuto && renderer <= Vulkan;

    return Renderer(renderer);
}

void FFNxConfig::setRenderer(Renderer renderer)
{
    _config[CFG_RENDERER_BACKEND].visit([renderer](auto&& el) noexcept {
        if constexpr (toml::is_integer<decltype(el)>) {
            el = renderer;
        }
    });
}
