#include "system/application_info.hpp"

namespace Eden::System
{
    ApplicationInfo::ApplicationInfo() = default;

    ApplicationInfo::ApplicationInfo(
        bool found,
        const Eden::Text::String& name,
        const Eden::IO::Path& path,
        const Eden::Text::String& source,
        bool isDirectory,
        const Eden::Text::String& displayName)
    {
        this->data()->m_Found = found;
        this->data()->m_Name = name;
        this->data()->m_DisplayName = displayName.empty() ? name : displayName;
        this->data()->m_Path = path;
        this->data()->m_Source = source;
        this->data()->m_IsDirectory = isDirectory;
    }

    bool ApplicationInfo::found() const
    {
        return this->data()->m_Found;
    }

    ApplicationInfo::operator bool() const
    {
        return found();
    }

    Eden::Text::String ApplicationInfo::name() const
    {
        return this->data()->m_Name;
    }

    Eden::Text::String ApplicationInfo::display_name() const
    {
        return this->data()->m_DisplayName;
    }

    Eden::Text::String ApplicationInfo::source() const
    {
        return this->data()->m_Source;
    }

    const Eden::IO::Path& ApplicationInfo::path() const
    {
        return this->data()->m_Path;
    }

    bool ApplicationInfo::is_directory() const
    {
        return this->data()->m_IsDirectory;
    }

    bool ApplicationInfo::is_file() const
    {
        return !this->data()->m_IsDirectory;
    }

    void ApplicationInfo::set_found(bool value)
    {
        this->data()->m_Found = value;
    }

    void ApplicationInfo::set_name(const Eden::Text::String& value)
    {
        this->data()->m_Name = value;
    }

    void ApplicationInfo::set_display_name(const Eden::Text::String& value)
    {
        this->data()->m_DisplayName = value;
    }

    void ApplicationInfo::set_path(const Eden::IO::Path& value)
    {
        this->data()->m_Path = value;
    }

    void ApplicationInfo::set_source(const Eden::Text::String& value)
    {
        this->data()->m_Source = value;
    }

    void ApplicationInfo::set_is_directory(bool value)
    {
        this->data()->m_IsDirectory = value;
    }

    ApplicationInfo ApplicationInfo::NotFound(const Eden::Text::String& name)
    {
        return ApplicationInfo(false, name, Eden::IO::Path(), "not_found", false, name);
    }
}
