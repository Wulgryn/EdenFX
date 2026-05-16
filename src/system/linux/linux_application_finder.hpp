#ifndef EDEN_SYSTEM_LINUX_APPLICATION_FINDER_HPP
#define EDEN_SYSTEM_LINUX_APPLICATION_FINDER_HPP

#include "system/i_application_finder.hpp"

namespace Eden::System
{
    class LinuxApplicationFinder : public IApplicationFinder
    {
    public:
        Collections::List<ApplicationInfo> find_all(
            const Eden::Text::String& applicationName,
            const Collections::List<Eden::IO::Path>& extraSearchPaths,
            const ApplicationSearchOptions& options) const override;

        Collections::List<Eden::IO::Path> get_search_roots(
            const Collections::List<Eden::IO::Path>& extraSearchPaths,
            const ApplicationSearchOptions& options) const override;
    };
}

#endif // EDEN_SYSTEM_LINUX_APPLICATION_FINDER_HPP
