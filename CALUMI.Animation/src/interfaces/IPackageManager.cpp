//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "internalplatform.h"
#include "interfaces/IPackageManager.h"

namespace CALUMI {
	namespace UNIV{

        struct IPackageManager::Impl
        {
            Utilities::VectorContainer<IPackage*> _packages;

            void clear()
            {
                for (std::size_t i = 0; i < _packages.size(); i++)
                {
                    if (_packages.at(i))
                    {
                        delete _packages.at(i);
                        _packages.at(i) = nullptr;
                    }
                }
                _packages.clear();
            }

            Impl() = default;
            ~Impl() {
                clear();
            }
        };
        IPackageManager::IPackageManager() : pImpl(new Impl()) {}

        IPackageManager::IPackageManager(const IPackageManager& input) : IPackageManager()
        {
            *this = input;
        }
        IPackageManager& IPackageManager::operator=(const IPackageManager& other)
        {
            pImpl->clear();
            for (std::size_t i = 0; i < other.pImpl->_packages.size(); i++)
            {
                if (auto ptr = other.pImpl->_packages.at(i))
                {
                    pImpl->_packages.push_back(ptr->Clone());
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

        IPackage* IPackageManager::getPackage(const char* packageName)
        {
            int idx = findPackage(packageName);

            if (idx < 0)
                return nullptr;

            return pImpl->_packages.at(idx);
        }

        IPackage* IPackageManager::getPackage(int index)
        {
            if (pImpl->_packages.size() <= index || index < 0)
                return nullptr;

            return pImpl->_packages.at(index);
        }

        int IPackageManager::findPackage(const char* packageName) const
        {
            for (int i = 0; i < pImpl->_packages.size(); i++)
            {
                if (SCOMPARE(pImpl->_packages.at(i)->getPackageType(), packageName) == 0)
                {
                    return i;
                }
            }
            return -1;
        }

        bool IPackageManager::RemovePackage(const char* packageName)
        {
            return RemovePackage(findPackage(packageName));
        }

        bool IPackageManager::RemovePackage(int index)
        {
            if (index < 0 && index >= pImpl->_packages.size())
                return false;

            if (pImpl->_packages.at(index))
            {
                delete pImpl->_packages.at(index);
                pImpl->_packages.at(index) = nullptr;
                pImpl->_packages.erase(index);
                return true;
            }

            return false;
        }

        bool IPackageManager::AddPackage(IPackage* package, bool overwrite)
        {
            int idx = findPackage(package->getPackageType());

            if (idx >= 0)
            {
                if (!overwrite)
                    return false;

                if (!RemovePackage(idx))
                    return false;
            }

            pImpl->_packages.push_back(package);
            return true;
        }

        std::size_t IPackageManager::packageCount() const
        {
            return pImpl->_packages.size();
        }
}
}