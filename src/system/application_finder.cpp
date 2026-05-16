#include "system/application_finder.hpp"

#include "system/linux/linux_application_finder.hpp"
#include "system/windows/windows_application_finder.hpp"

namespace Eden::System
{
    ApplicationFinder::ApplicationFinder()
    {
        this->data()->m_PlatformFinder = CreatePlatformFinder();
    }

    ApplicationFinder::ApplicationFinder(std::shared_ptr<IApplicationFinder> platformFinder)
    {
        this->data()->m_PlatformFinder = platformFinder ? platformFinder : CreatePlatformFinder();
    }

    ApplicationInfo ApplicationFinder::find_local(const Eden::Text::String& applicationName) const
    {
        Collections::List<ApplicationInfo> all = find_all_local(applicationName);

        if (all.count() <= 0)
        {
            return ApplicationInfo::NotFound(applicationName);
        }

        return all[0];
    }

    Collections::List<ApplicationInfo> ApplicationFinder::find_all_local(const Eden::Text::String& applicationName) const
    {
        if (!this->data()->m_PlatformFinder)
        {
            return Collections::List<ApplicationInfo>();
        }

        return this->data()->m_PlatformFinder->find_all(
            applicationName,
            this->data()->m_ExtraSearchPaths,
            this->data()->m_Options);
    }

    bool ApplicationFinder::exists_local(const Eden::Text::String& applicationName) const
    {
        return find_local(applicationName).found();
    }

    void ApplicationFinder::add_search_path(const Eden::IO::Path& path)
    {
        this->data()->m_ExtraSearchPaths.add(path);
    }

    void ApplicationFinder::clear_search_paths()
    {
        this->data()->m_ExtraSearchPaths.clear();
    }

    Collections::List<Eden::IO::Path> ApplicationFinder::search_paths() const
    {
        return this->data()->m_ExtraSearchPaths;
    }

    Collections::List<Eden::IO::Path> ApplicationFinder::get_search_roots() const
    {
        if (!this->data()->m_PlatformFinder)
        {
            return Collections::List<Eden::IO::Path>();
        }

        return this->data()->m_PlatformFinder->get_search_roots(
            this->data()->m_ExtraSearchPaths,
            this->data()->m_Options);
    }

    ApplicationSearchOptions ApplicationFinder::options() const
    {
        return this->data()->m_Options;
    }

    void ApplicationFinder::set_options(const ApplicationSearchOptions& options)
    {
        this->data()->m_Options = options;
    }

    void ApplicationFinder::set_scan_path(bool value)
    {
        this->data()->m_Options.scanPath = value;
    }

    void ApplicationFinder::set_scan_system_locations(bool value)
    {
        this->data()->m_Options.scanSystemLocations = value;
    }

    void ApplicationFinder::set_scan_all_drives(bool value)
    {
        this->data()->m_Options.scanAllDrives = value;
    }

    void ApplicationFinder::set_deep_drive_scan(bool value)
    {
        this->data()->m_Options.deepDriveScan = value;
    }

    void ApplicationFinder::set_max_drive_scan_depth(int value)
    {
        this->data()->m_Options.maxDriveScanDepth = value;
    }

    void ApplicationFinder::set_allow_contains_match(bool value)
    {
        this->data()->m_Options.allowContainsMatch = value;
    }

    ApplicationInfo ApplicationFinder::Find(const Eden::Text::String& applicationName)
    {
        return ApplicationFinder().find_local(applicationName);
    }

    Collections::List<ApplicationInfo> ApplicationFinder::FindAll(const Eden::Text::String& applicationName)
    {
        return ApplicationFinder().find_all_local(applicationName);
    }

    bool ApplicationFinder::Exists(const Eden::Text::String& applicationName)
    {
        return ApplicationFinder().exists_local(applicationName);
    }

    std::shared_ptr<IApplicationFinder> ApplicationFinder::CreatePlatformFinder()
    {
#ifdef _WIN32
        return std::make_shared<WindowsApplicationFinder>();
#else
        return std::make_shared<LinuxApplicationFinder>();
#endif
    }
}
