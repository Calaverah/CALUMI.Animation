//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "internalplatform.h"
#include "interfaces/IPackageManager.h"
#include <unordered_map>
#include <memory>
#include <ranges>
#include <string>

namespace CALUMI::UNIV
{

    struct IPackageManager::Impl
    {
        std::unordered_map<std::string, std::shared_ptr<IPackage>> m_packages;
    };
    IPackageManager::IPackageManager() : pImpl(new Impl()) {}

    IPackageManager::IPackageManager(const IPackageManager& input) : IPackageManager()
    {
        *this = input;
    }
    IPackageManager& IPackageManager::operator=(const IPackageManager& other)
    {
        if (this != &other)
        {
            pImpl->m_packages.clear();

            for (const auto& pkg : other.pImpl->m_packages | std::views::values)
            {
                addPackage(pkg->clone(),true);
            }
        }
        return *this;
    }
    IPackageManager::~IPackageManager()
    {
        if (pImpl)
        {
            delete pImpl;
            pImpl = nullptr;
        }
    }

    IPackage* IPackageManager::package(const char* packageName)
    {
        if (!pImpl->m_packages.contains(packageName))
            return nullptr;

        return pImpl->m_packages.at(packageName).get();
    }

    Utilities::StringList IPackageManager::packageList() const
    {
        Utilities::StringList output;
        for (auto& pkg : pImpl->m_packages | std::views::keys)
        {
            output.push_back(pkg.c_str());
        }
        return output;
    }

    bool IPackageManager::removePackage(const char* packageName)
    {
        return static_cast<bool>(pImpl->m_packages.erase(packageName));
    }

    bool IPackageManager::addPackage(IPackage* package, const bool overwrite) const
    {
        if (pImpl->m_packages.contains(package->packageType()) && !overwrite)
            return false;


        pImpl->m_packages[package->packageType()] = std::shared_ptr<IPackage>(package);
        return true;
    }

    uint64_t IPackageManager::packageCount() const
    {
        return pImpl->m_packages.size();
    }
}
