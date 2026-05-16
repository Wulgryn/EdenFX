#ifndef EDEN_SYSTEM_I_APPLICATION_FINDER_HPP
#define EDEN_SYSTEM_I_APPLICATION_FINDER_HPP

#include "collections/list.hpp"
#include "core/object/object.hpp"
#include "io/path.hpp"
#include "system/application_info.hpp"
#include "text/string.hpp"

namespace Eden::System
{
    struct ApplicationSearchOptions
    {
        bool scanPath = true;
        bool scanSystemLocations = true;
        bool scanAllDrives = true;
        bool deepDriveScan = true;

        // -1 = nincs mélység limit. Ez tényleg végignézheti az egész meghajtót.
        int maxDriveScanDepth = -1;

        bool includeFiles = true;
        bool includeDirectories = true;
        bool caseInsensitive = true;
        bool allowContainsMatch = true;
    };

    class IApplicationFinder : public virtual OBJECT
    {
    public:
        virtual ~IApplicationFinder() = default;

        virtual Collections::List<ApplicationInfo> find_all(
            const Eden::Text::String& applicationName,
            const Collections::List<Eden::IO::Path>& extraSearchPaths,
            const ApplicationSearchOptions& options) const = 0;

        virtual Collections::List<Eden::IO::Path> get_search_roots(
            const Collections::List<Eden::IO::Path>& extraSearchPaths,
            const ApplicationSearchOptions& options) const = 0;
    };
}

#if defined(AUTO_USINGS)
using namespace Eden::System;
#endif

#endif // EDEN_SYSTEM_I_APPLICATION_FINDER_HPP
