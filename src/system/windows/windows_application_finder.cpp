#include "system/windows/windows_application_finder.hpp"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <filesystem>
#include <set>
#include <string>
#include <system_error>
#include <vector>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

namespace Eden::System
{
    namespace fs = std::filesystem;

    namespace
    {
        static std::string ToLower(std::string value)
        {
            std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c)
            {
                return static_cast<char>(std::tolower(c));
            });

            return value;
        }

        static std::string Trim(const std::string& value)
        {
            std::size_t start = 0;
            while (start < value.size() && std::isspace(static_cast<unsigned char>(value[start])))
            {
                start++;
            }

            std::size_t end = value.size();
            while (end > start && std::isspace(static_cast<unsigned char>(value[end - 1])))
            {
                end--;
            }

            return value.substr(start, end - start);
        }

        static std::string NormalizeForCompare(const std::string& value)
        {
            std::string result;
            result.reserve(value.size());

            for (char c : ToLower(Trim(value)))
            {
                if (std::isalnum(static_cast<unsigned char>(c)))
                {
                    result.push_back(c);
                }
            }

            return result;
        }

        static bool Contains(const std::string& value, const std::string& part)
        {
            return value.find(part) != std::string::npos;
        }

        static bool SafeExists(const fs::path& path)
        {
            std::error_code error;
            return fs::exists(path, error);
        }

        static bool SafeIsDirectory(const fs::path& path)
        {
            std::error_code error;
            return fs::is_directory(path, error);
        }

        static bool SafeIsRegularFile(const fs::path& path)
        {
            std::error_code error;
            return fs::is_regular_file(path, error);
        }

        static std::string PathKey(const fs::path& path)
        {
            std::error_code error;
            fs::path absolute = fs::absolute(path, error);
            if (error)
            {
                absolute = path;
            }

            return ToLower(absolute.lexically_normal().generic_string());
        }

        static void AddPathOnce(Collections::List<Eden::IO::Path>& result, std::set<std::string>& added, const fs::path& path)
        {
            if (path.empty() || !SafeExists(path))
            {
                return;
            }

            std::string key = PathKey(path);
            if (added.insert(key).second)
            {
                result.add(Eden::IO::Path(path));
            }
        }

        static std::vector<std::string> SplitPathLikeList(const std::string& value, char separator)
        {
            std::vector<std::string> result;
            std::string current;

            for (char c : value)
            {
                if (c == separator)
                {
                    if (!current.empty())
                    {
                        result.push_back(current);
                    }

                    current.clear();
                }
                else
                {
                    current.push_back(c);
                }
            }

            if (!current.empty())
            {
                result.push_back(current);
            }

            return result;
        }

        static void AddEnvPath(Collections::List<Eden::IO::Path>& result, std::set<std::string>& added, const char* name)
        {
            const char* value = std::getenv(name);
            if (!value)
            {
                return;
            }

            AddPathOnce(result, added, fs::path(value));
        }

        static bool IsExecutableExtension(const fs::path& path)
        {
            std::string extension = ToLower(path.extension().string());
            return extension == ".exe" || extension == ".com" || extension == ".bat" ||
                   extension == ".cmd" || extension == ".ps1" || extension == ".msi";
        }

        static bool IsLikelyExecutableFile(const fs::path& path)
        {
            return SafeIsRegularFile(path) && IsExecutableExtension(path);
        }

        static std::vector<fs::path> DirectExecutableNames(const Eden::Text::String& applicationName)
        {
            std::vector<fs::path> names;
            std::string query = Trim(applicationName.to_std_string());

            if (query.empty())
            {
                return names;
            }

            fs::path queryPath(query);
            names.push_back(queryPath);

            if (!queryPath.has_extension())
            {
                names.push_back(fs::path(query + ".exe"));
                names.push_back(fs::path(query + ".com"));
                names.push_back(fs::path(query + ".bat"));
                names.push_back(fs::path(query + ".cmd"));
                names.push_back(fs::path(query + ".ps1"));
            }

            return names;
        }

        static bool NameMatches(const Eden::Text::String& applicationName, const fs::path& path, const std::string& displayName, const ApplicationSearchOptions& options)
        {
            std::string rawQuery = Trim(applicationName.to_std_string());
            std::string query = options.caseInsensitive ? ToLower(rawQuery) : rawQuery;
            std::string normalizedQuery = NormalizeForCompare(rawQuery);

            if (query.empty() || normalizedQuery.empty())
            {
                return false;
            }

            std::vector<std::string> candidates;
            candidates.push_back(path.filename().string());
            candidates.push_back(path.stem().string());
            candidates.push_back(path.parent_path().filename().string());

            if (!displayName.empty())
            {
                candidates.push_back(displayName);
            }

            for (std::string candidate : candidates)
            {
                std::string compared = options.caseInsensitive ? ToLower(candidate) : candidate;
                std::string normalizedCandidate = NormalizeForCompare(candidate);

                if (compared == query || normalizedCandidate == normalizedQuery)
                {
                    return true;
                }

                if (options.allowContainsMatch && normalizedCandidate.size() >= normalizedQuery.size())
                {
                    if (Contains(normalizedCandidate, normalizedQuery))
                    {
                        return true;
                    }
                }
            }

            return false;
        }

        static void AddApplicationInfoOnce(
            Collections::List<ApplicationInfo>& result,
            std::set<std::string>& added,
            const Eden::Text::String& applicationName,
            const fs::path& path,
            const Eden::Text::String& source,
            bool isDirectory,
            const Eden::Text::String& displayName = "")
        {
            if (path.empty() || !SafeExists(path))
            {
                return;
            }

            std::string key = PathKey(path);
            if (!added.insert(key).second)
            {
                return;
            }

            Eden::Text::String name = displayName.empty()
                ? Eden::Text::String(path.filename().string())
                : displayName;

            result.add(ApplicationInfo(true, applicationName, Eden::IO::Path(path), source, isDirectory, name));
        }

        static void TryDirectNamesInDirectory(
            const fs::path& directory,
            const Eden::Text::String& applicationName,
            const ApplicationSearchOptions& options,
            Collections::List<ApplicationInfo>& result,
            std::set<std::string>& added,
            const Eden::Text::String& source)
        {
            if (!SafeIsDirectory(directory))
            {
                return;
            }

            for (const fs::path& name : DirectExecutableNames(applicationName))
            {
                fs::path file = directory / name;
                if (options.includeFiles && IsLikelyExecutableFile(file))
                {
                    AddApplicationInfoOnce(result, added, applicationName, file, source, false);
                }
            }
        }

        static void SearchDirectoryOneLevel(
            const fs::path& directory,
            const Eden::Text::String& applicationName,
            const ApplicationSearchOptions& options,
            Collections::List<ApplicationInfo>& result,
            std::set<std::string>& added,
            const Eden::Text::String& source)
        {
            if (!SafeIsDirectory(directory))
            {
                return;
            }

            TryDirectNamesInDirectory(directory, applicationName, options, result, added, source);

            std::error_code error;
            fs::directory_iterator iterator(directory, fs::directory_options::skip_permission_denied, error);
            fs::directory_iterator end;

            while (!error && iterator != end)
            {
                const fs::directory_entry& entry = *iterator;
                const fs::path path = entry.path();

                std::error_code typeError;
                bool isDir = entry.is_directory(typeError);
                bool isFile = entry.is_regular_file(typeError);

                if (isFile && options.includeFiles && IsExecutableExtension(path) && NameMatches(applicationName, path, "", options))
                {
                    AddApplicationInfoOnce(result, added, applicationName, path, source, false);
                }
                else if (isDir && options.includeDirectories && NameMatches(applicationName, path, "", options))
                {
                    AddApplicationInfoOnce(result, added, applicationName, path, source, true);
                }

                iterator.increment(error);
                if (error)
                {
                    error.clear();
                }
            }
        }

        static bool ShouldSkipDirectory(const fs::path& path)
        {
            std::string name = ToLower(path.filename().string());
            return name == "$recycle.bin" || name == "system volume information" || name == "recovery";
        }

        static void SearchDirectoryRecursive(
            const fs::path& root,
            const Eden::Text::String& applicationName,
            const ApplicationSearchOptions& options,
            Collections::List<ApplicationInfo>& result,
            std::set<std::string>& added,
            const Eden::Text::String& source)
        {
            if (!SafeIsDirectory(root))
            {
                return;
            }

            SearchDirectoryOneLevel(root, applicationName, options, result, added, source);

            if (!options.deepDriveScan)
            {
                return;
            }

            std::error_code error;
            fs::recursive_directory_iterator iterator(root, fs::directory_options::skip_permission_denied, error);
            fs::recursive_directory_iterator end;

            while (!error && iterator != end)
            {
                const fs::directory_entry& entry = *iterator;
                const fs::path path = entry.path();

                std::error_code typeError;
                bool isDir = entry.is_directory(typeError);
                bool isFile = entry.is_regular_file(typeError);

                if (isDir)
                {
                    if (ShouldSkipDirectory(path) || (options.maxDriveScanDepth >= 0 && iterator.depth() >= options.maxDriveScanDepth))
                    {
                        iterator.disable_recursion_pending();
                    }

                    if (options.includeDirectories && NameMatches(applicationName, path, "", options))
                    {
                        AddApplicationInfoOnce(result, added, applicationName, path, source, true);
                    }
                }
                else if (isFile && options.includeFiles && IsExecutableExtension(path) && NameMatches(applicationName, path, "", options))
                {
                    AddApplicationInfoOnce(result, added, applicationName, path, source, false);
                }

                iterator.increment(error);
                if (error)
                {
                    error.clear();
                }
            }
        }

#ifdef _WIN32
        static std::string WideToUtf8(const std::wstring& value)
        {
            if (value.empty())
            {
                return std::string();
            }

            int size = WideCharToMultiByte(CP_UTF8, 0, value.c_str(), static_cast<int>(value.size()), nullptr, 0, nullptr, nullptr);
            if (size <= 0)
            {
                return std::string(value.begin(), value.end());
            }

            std::string result(static_cast<std::size_t>(size), '\0');
            WideCharToMultiByte(CP_UTF8, 0, value.c_str(), static_cast<int>(value.size()), result.data(), size, nullptr, nullptr);
            return result;
        }

        static bool ReadRegistryString(HKEY key, const wchar_t* name, std::wstring& value)
        {
            DWORD type = 0;
            DWORD size = 0;

            LONG status = RegQueryValueExW(key, name, nullptr, &type, nullptr, &size);
            if (status != ERROR_SUCCESS || (type != REG_SZ && type != REG_EXPAND_SZ) || size == 0)
            {
                return false;
            }

            std::wstring buffer(size / sizeof(wchar_t), L'\0');
            status = RegQueryValueExW(key, name, nullptr, &type, reinterpret_cast<LPBYTE>(buffer.data()), &size);
            if (status != ERROR_SUCCESS)
            {
                return false;
            }

            while (!buffer.empty() && buffer.back() == L'\0')
            {
                buffer.pop_back();
            }

            value = buffer;
            return true;
        }

        static fs::path ExtractPathFromRegistryValue(const std::wstring& value)
        {
            if (value.empty())
            {
                return fs::path();
            }

            std::wstring text = value;

            if (!text.empty() && text[0] == L'\"')
            {
                std::size_t endQuote = text.find(L'\"', 1);
                if (endQuote != std::wstring::npos)
                {
                    text = text.substr(1, endQuote - 1);
                }
            }
            else
            {
                std::size_t exe = ToLower(WideToUtf8(text)).find(".exe");
                if (exe != std::string::npos)
                {
                    text = text.substr(0, exe + 4);
                }
                else
                {
                    std::size_t comma = text.find(L',');
                    if (comma != std::wstring::npos)
                    {
                        text = text.substr(0, comma);
                    }
                }
            }

            wchar_t expanded[32767];
            DWORD expandedSize = ExpandEnvironmentStringsW(text.c_str(), expanded, 32767);
            if (expandedSize > 0 && expandedSize < 32767)
            {
                text = expanded;
            }

            return fs::path(text);
        }

        static void SearchRegistryUninstallRoot(
            HKEY root,
            REGSAM view,
            const Eden::Text::String& applicationName,
            const ApplicationSearchOptions& options,
            Collections::List<ApplicationInfo>& result,
            std::set<std::string>& added)
        {
            static constexpr const wchar_t* uninstallPath = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall";

            HKEY uninstallKey = nullptr;
            if (RegOpenKeyExW(root, uninstallPath, 0, KEY_READ | view, &uninstallKey) != ERROR_SUCCESS)
            {
                return;
            }

            DWORD index = 0;
            wchar_t subKeyName[512];
            DWORD subKeyNameSize = 512;

            while (RegEnumKeyExW(uninstallKey, index, subKeyName, &subKeyNameSize, nullptr, nullptr, nullptr, nullptr) == ERROR_SUCCESS)
            {
                HKEY appKey = nullptr;
                if (RegOpenKeyExW(uninstallKey, subKeyName, 0, KEY_READ | view, &appKey) == ERROR_SUCCESS)
                {
                    std::wstring displayName;
                    if (ReadRegistryString(appKey, L"DisplayName", displayName))
                    {
                        std::string displayNameUtf8 = WideToUtf8(displayName);

                        if (NameMatches(applicationName, fs::path(displayNameUtf8), displayNameUtf8, options))
                        {
                            std::wstring installLocation;
                            std::wstring displayIcon;
                            std::wstring uninstallString;
                            fs::path candidate;
                            bool isDirectory = false;

                            if (ReadRegistryString(appKey, L"DisplayIcon", displayIcon))
                            {
                                candidate = ExtractPathFromRegistryValue(displayIcon);
                            }

                            if ((candidate.empty() || !SafeExists(candidate)) && ReadRegistryString(appKey, L"InstallLocation", installLocation))
                            {
                                candidate = ExtractPathFromRegistryValue(installLocation);
                                isDirectory = true;
                            }

                            if ((candidate.empty() || !SafeExists(candidate)) && ReadRegistryString(appKey, L"UninstallString", uninstallString))
                            {
                                candidate = ExtractPathFromRegistryValue(uninstallString);
                            }

                            if (!candidate.empty() && SafeExists(candidate))
                            {
                                isDirectory = isDirectory || SafeIsDirectory(candidate);
                                AddApplicationInfoOnce(
                                    result,
                                    added,
                                    applicationName,
                                    candidate,
                                    "registry",
                                    isDirectory,
                                    Eden::Text::String(displayNameUtf8));
                            }
                        }
                    }

                    RegCloseKey(appKey);
                }

                index++;
                subKeyNameSize = 512;
            }

            RegCloseKey(uninstallKey);
        }

        static void SearchRegistry(
            const Eden::Text::String& applicationName,
            const ApplicationSearchOptions& options,
            Collections::List<ApplicationInfo>& result,
            std::set<std::string>& added)
        {
            SearchRegistryUninstallRoot(HKEY_CURRENT_USER, KEY_WOW64_64KEY, applicationName, options, result, added);
            SearchRegistryUninstallRoot(HKEY_CURRENT_USER, KEY_WOW64_32KEY, applicationName, options, result, added);
            SearchRegistryUninstallRoot(HKEY_LOCAL_MACHINE, KEY_WOW64_64KEY, applicationName, options, result, added);
            SearchRegistryUninstallRoot(HKEY_LOCAL_MACHINE, KEY_WOW64_32KEY, applicationName, options, result, added);
        }

        static Collections::List<Eden::IO::Path> GetWindowsDriveRoots()
        {
            Collections::List<Eden::IO::Path> result;
            std::set<std::string> added;

            DWORD mask = GetLogicalDrives();
            for (char letter = 'A'; letter <= 'Z'; letter++)
            {
                DWORD bit = 1u << (letter - 'A');
                if ((mask & bit) == 0)
                {
                    continue;
                }

                std::string root;
                root.push_back(letter);
                root += ":\\";

                AddPathOnce(result, added, fs::path(root));
            }

            return result;
        }
#endif
    }

    Collections::List<ApplicationInfo> WindowsApplicationFinder::find_all(
        const Eden::Text::String& applicationName,
        const Collections::List<Eden::IO::Path>& extraSearchPaths,
        const ApplicationSearchOptions& options) const
    {
        Collections::List<ApplicationInfo> result;
        std::set<std::string> added;

        if (applicationName.trim().empty())
        {
            return result;
        }

        // Ha teljes útvonalat kap, először azt próbálja.
        fs::path queryPath(applicationName.to_std_string());
        if ((queryPath.has_parent_path() || queryPath.is_absolute()) && SafeExists(queryPath))
        {
            AddApplicationInfoOnce(result, added, applicationName, queryPath, "direct_path", SafeIsDirectory(queryPath));
        }

        Collections::List<Eden::IO::Path> roots = get_search_roots(extraSearchPaths, options);

        for (const Eden::IO::Path& root : roots)
        {
            fs::path nativeRoot = static_cast<fs::path>(root);

            // PATH és ismert mappák: először gyors, egy szintű keresés.
            SearchDirectoryOneLevel(nativeRoot, applicationName, options, result, added, "search_root");
        }

#ifdef _WIN32
        if (options.scanSystemLocations)
        {
            SearchRegistry(applicationName, options, result, added);
        }
#endif

        if (options.scanAllDrives && options.deepDriveScan)
        {
            Collections::List<Eden::IO::Path> driveRoots;

#ifdef _WIN32
            driveRoots = GetWindowsDriveRoots();
#endif

            for (const Eden::IO::Path& drive : driveRoots)
            {
                SearchDirectoryRecursive(static_cast<fs::path>(drive), applicationName, options, result, added, "drive_scan");
            }
        }

        return result;
    }

    Collections::List<Eden::IO::Path> WindowsApplicationFinder::get_search_roots(
        const Collections::List<Eden::IO::Path>& extraSearchPaths,
        const ApplicationSearchOptions& options) const
    {
        Collections::List<Eden::IO::Path> result;
        std::set<std::string> added;

        for (const Eden::IO::Path& path : extraSearchPaths)
        {
            AddPathOnce(result, added, static_cast<fs::path>(path));
        }

        if (options.scanPath)
        {
            const char* pathValue = std::getenv("PATH");
            if (pathValue)
            {
                for (const std::string& item : SplitPathLikeList(pathValue, ';'))
                {
                    AddPathOnce(result, added, fs::path(item));
                }
            }
        }

        if (options.scanSystemLocations)
        {
            AddEnvPath(result, added, "ProgramFiles");
            AddEnvPath(result, added, "ProgramFiles(x86)");
            AddEnvPath(result, added, "ProgramW6432");

            const char* localAppData = std::getenv("LOCALAPPDATA");
            if (localAppData)
            {
                AddPathOnce(result, added, fs::path(localAppData) / "Programs");
            }

            const char* appData = std::getenv("APPDATA");
            if (appData)
            {
                AddPathOnce(result, added, fs::path(appData));
            }
        }

        if (options.scanAllDrives)
        {
#ifdef _WIN32
            Collections::List<Eden::IO::Path> drives = GetWindowsDriveRoots();
            for (const Eden::IO::Path& drive : drives)
            {
                fs::path root = static_cast<fs::path>(drive);
                AddPathOnce(result, added, root);
                AddPathOnce(result, added, root / "Program Files");
                AddPathOnce(result, added, root / "Program Files (x86)");
                AddPathOnce(result, added, root / "msys64");
                AddPathOnce(result, added, root / "msys32");
                AddPathOnce(result, added, root / "Tools");
                AddPathOnce(result, added, root / "Apps");
            }
#endif
        }

        return result;
    }
}
