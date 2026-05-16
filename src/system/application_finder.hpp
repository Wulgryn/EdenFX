#ifndef EDEN_SYSTEM_APPLICATION_FINDER_HPP
#define EDEN_SYSTEM_APPLICATION_FINDER_HPP

#include "core/object/managed_object.hpp"
#include "collections/list.hpp"
#include "io/path.hpp"
#include "system/application_info.hpp"
#include "system/i_application_finder.hpp"
#include "text/string.hpp"

#include <memory>

namespace Eden::System
{
    namespace Managed
    {
        class ApplicationFinderData : public virtual OBJECT
        {
        public:
            std::shared_ptr<IApplicationFinder> m_PlatformFinder;
            Collections::List<Eden::IO::Path> m_ExtraSearchPaths;
            ApplicationSearchOptions m_Options;
        };
    }

    class ApplicationFinder : public MANAGEDOBJECT<ApplicationFinder, Managed::ApplicationFinderData>
    {
    public:
        ApplicationFinder();
        explicit ApplicationFinder(std::shared_ptr<IApplicationFinder> platformFinder);

        ApplicationInfo find_local(const Eden::Text::String& applicationName) const;
        Collections::List<ApplicationInfo> find_all_local(const Eden::Text::String& applicationName) const;
        bool exists_local(const Eden::Text::String& applicationName) const;

        void add_search_path(const Eden::IO::Path& path);
        void clear_search_paths();
        Collections::List<Eden::IO::Path> search_paths() const;
        Collections::List<Eden::IO::Path> get_search_roots() const;

        ApplicationSearchOptions options() const;
        void set_options(const ApplicationSearchOptions& options);

        void set_scan_path(bool value);
        void set_scan_system_locations(bool value);
        void set_scan_all_drives(bool value);
        void set_deep_drive_scan(bool value);
        void set_max_drive_scan_depth(int value);
        void set_allow_contains_match(bool value);

        static ApplicationInfo Find(const Eden::Text::String& applicationName);
        static Collections::List<ApplicationInfo> FindAll(const Eden::Text::String& applicationName);
        static bool Exists(const Eden::Text::String& applicationName);

        static std::shared_ptr<IApplicationFinder> CreatePlatformFinder();
    };
}

#if defined(AUTO_USINGS)
using namespace Eden::System;
#endif

#endif // EDEN_SYSTEM_APPLICATION_FINDER_HPP
