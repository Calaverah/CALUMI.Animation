//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com


#include <AnimStarfield>
#include <vector>
#include <AnimUniv>
#include <AnimUtil>
#include <string>
#include <limits>

namespace CALUMI {
    namespace SFBGS {

        struct SFBGS_AnimationPackage::Impl {
            // leaving as vector until we know without doubt that order is not important...
            std::vector<std::pair<uint32_t, UNIV::AnimationBlock>> _amendedBlocks;
            
            SFBGS::PrecisionSet _precisionOverride = PrecisionSet::DefaultPrecision();
            bool _useRigPrecision = true;
        };

        SFBGS_AnimationPackage::SFBGS_AnimationPackage() : pImpl(new Impl()) {}

        SFBGS_AnimationPackage::SFBGS_AnimationPackage(const SFBGS_AnimationPackage& other) : SFBGS_AnimationPackage()
        {
            *this = other;
        }

        SFBGS_AnimationPackage& SFBGS_AnimationPackage::operator=(const SFBGS_AnimationPackage& other)
        {
            pImpl->_amendedBlocks = other.pImpl->_amendedBlocks;
            pImpl->_precisionOverride = other.pImpl->_precisionOverride;

            pImpl->_useRigPrecision = other.pImpl->_useRigPrecision;
            return *this;
        }

        SFBGS_AnimationPackage::~SFBGS_AnimationPackage()
        {
            if (pImpl)
            {
                delete pImpl;
                pImpl = nullptr;
            }
        }

        uint16_t SFBGS_AnimationPackage::getAmendedBlockCount() const
        {
            return static_cast<uint16_t>(pImpl->_amendedBlocks.size());
        }

        void SFBGS_AnimationPackage::overrideRigPrecision(PrecisionSet precision)
        {
            pImpl->_useRigPrecision = false;
            pImpl->_precisionOverride = precision;
        }

        SFBGS::PrecisionSet SFBGS_AnimationPackage::getOverridePrecisionSet() const
        {
            return pImpl->_precisionOverride;
        }

        bool SFBGS_AnimationPackage::usesRigForPrecision() const
        {
            return pImpl->_useRigPrecision;
        }

        void SFBGS_AnimationPackage::resetPrecision()
        {
            pImpl->_useRigPrecision = true;
            pImpl->_precisionOverride = PrecisionSet::DefaultPrecision();
        }

        bool SFBGS_AnimationPackage::addAmendedBlock(UNIV::AnimationBlock block, bool overwrite)
        {
            uint32_t hash = Utilities::HashRegistry::getInstance().registerHash(block.boneName());

            return addAmendedBlock(hash, block, overwrite);
        }

        bool SFBGS_AnimationPackage::addAmendedBlock(uint32_t hash, UNIV::AnimationBlock block, bool overwrite)
        {
            if (pImpl->_amendedBlocks.size() >= MaxAmendedBlockCount)
                return false;

            int index = findAmendedBlock(hash);

            if (index >= 0)
            {
                if (!overwrite)
                {
                    return false;
                }
                else
                {
                    pImpl->_amendedBlocks.at(index).second = block;
                    return true;
                }
            }
            
            pImpl->_amendedBlocks.push_back({ hash, block });
            return true;
        }

        bool SFBGS_AnimationPackage::removeAmendedBlock(const char* name)
        {
            uint32_t hash = Utilities::HashRegistry::getInstance().registerHash(name);

            return removeAmendedBlock(hash);
        }

        bool SFBGS_AnimationPackage::removeAmendedBlock(uint32_t hash)
        {
            return removeAmendedBlock(findAmendedBlock(hash));
        }

        bool SFBGS_AnimationPackage::removeAmendedBlock(int index)
        {
            if (index < 0 || index >= pImpl->_amendedBlocks.size())
                return false;

            pImpl->_amendedBlocks.erase(pImpl->_amendedBlocks.begin() + index);
            
            return true;
        }

        bool SFBGS_AnimationPackage::hasAmendedBlock(uint32_t hash) const
        {
            return findAmendedBlock(hash) >= 0;
        }

        bool SFBGS_AnimationPackage::hasAmendedBlock(const char* name) const
        {
            return findAmendedBlock(name) >= 0;
        }

        int SFBGS_AnimationPackage::findAmendedBlock(uint32_t hash) const
        {
            for (uint64_t i = 0; i < pImpl->_amendedBlocks.size(); i++)
            {
                if (pImpl->_amendedBlocks.at(i).first == hash)
                    return static_cast<int>(i);
            }

            return -1;
        }

        int SFBGS_AnimationPackage::findAmendedBlock(const char* name) const
        {
            uint32_t hash = Utilities::HashRegistry::getInstance().registerHash(name);

            return findAmendedBlock(hash);
        }

        UNIV::AnimationBlock* SFBGS_AnimationPackage::getAmendedBlock(int index) const
        {
            if (index < 0 || index >= pImpl->_amendedBlocks.size())
                return nullptr;

            return &pImpl->_amendedBlocks.at(index).second;
        }

        uint32_t SFBGS_AnimationPackage::getAmendedBlockHash(int index) const
        {
            if (index < 0 || index >= pImpl->_amendedBlocks.size())
                return 0xFFFFFFFF;

            return pImpl->_amendedBlocks.at(index).first;
        }

        const char* SFBGS_AnimationPackage::getPackageType() const
        {
            return SFBGS_ANIM_PACKAGE;
        }

        Utilities::StringContainer SFBGS_AnimationPackage::toJSON(uint64_t indents) const
        {
            return Utilities::StringContainer();
        }

        SFBGS_AnimationPackage* SFBGS_AnimationPackage::clone() const
        {
            SFBGS_AnimationPackage* output = new SFBGS_AnimationPackage(*this);
            return output;
        }
                
        bool CreateNewSFBGSAnimationPackage(UNIV::Animation& animation, bool overwrite)
        {
            auto& mgr = animation.getPackageManager();

            SFBGS_AnimationPackage* pkg = new SFBGS_AnimationPackage();

            if (mgr.AddPackage(pkg, overwrite))
            {
                return true;
            }

            if (pkg)
            {
                delete pkg;
                pkg = nullptr;
            }

            return false;
        }

        bool RemoveSFBGSAnimationPackage(UNIV::Animation& animation)
        {
            return animation.getPackageManager().RemovePackage(SFBGS_ANIM_PACKAGE);
        }

#pragma region Extern C

        bool SFBGSAnimationPackage_AddPackageToAnimationC(UNIV::Animation* animation, Utilities::StringContainer* errorMessage, bool overwrite)
        {
            Utilities::StringContainer tempErrorMessage;
            Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
            errorMessageHolder->clear();

            *errorMessageHolder += "[CALUMI.Animation API] ";
            *errorMessageHolder += SFBGS_ANIM_PACKAGE;

            if (CreateNewSFBGSAnimationPackage(*animation, overwrite))
            {
                *errorMessageHolder += " Successfully Added To";
                *errorMessageHolder += animation->animationTitle();
                return true;
            }
            else
            {
                *errorMessageHolder += " Was Not Added To ";
                *errorMessageHolder += animation->animationTitle();
                *errorMessageHolder += ", It May Already Exist And Was Not Set To Overwrite";
            }

            return false;
        }

        bool SFBGSAnimationPackage_RemovePackageFromAnimationC(UNIV::Animation* animation, Utilities::StringContainer* errorMessage)
        {
            Utilities::StringContainer tempErrorMessage;
            Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
            errorMessageHolder->clear();

            *errorMessageHolder += "[CALUMI.Animation API] ";
            *errorMessageHolder += SFBGS_ANIM_PACKAGE;

            if (RemoveSFBGSAnimationPackage(*animation))
            {
                *errorMessageHolder += " Successfully Removed From ";
                *errorMessageHolder += animation->animationTitle();
                return true;
            }
            else
            {
                *errorMessageHolder += " Was Not Removed From ";
                *errorMessageHolder += animation->animationTitle();
                *errorMessageHolder += ", It Either Does Not Exist Or Is Mislabeled";
            }
            return false;
        }

        bool SFBGSAnimationPackage_HasAmendedBlockC(UNIV::Animation* animation, const char* name)
        {
            if (auto pkg = dynamic_cast<SFBGS_AnimationPackage*>(animation->getPackageManager().getPackage(SFBGS_ANIM_PACKAGE)))
                return pkg->hasAmendedBlock(name);

            return false;
        }

        bool SFBGSAnimationPackage_HasAmendedBlockHashC(UNIV::Animation* animation, uint32_t hash)
        {
            if (auto pkg = dynamic_cast<SFBGS_AnimationPackage*>(animation->getPackageManager().getPackage(SFBGS_ANIM_PACKAGE)))
                return pkg->hasAmendedBlock(hash);

            return false;
        }

        uint16_t SFBGSAnimationPackage_GetAmendedBlockCountC(UNIV::Animation* animation)
        {
            if (!animation)
                return 0;

            if (auto pkg = dynamic_cast<SFBGS_AnimationPackage*>(animation->getPackageManager().getPackage(SFBGS_ANIM_PACKAGE)))
                return pkg->getAmendedBlockCount();

            return 0;
        }

        void SFBGSAnimationPackage_OverrideRigWithDefaultPrecisionC(UNIV::Animation* animation)
        {
            if (auto pkg = dynamic_cast<SFBGS_AnimationPackage*>(animation->getPackageManager().getPackage(SFBGS_ANIM_PACKAGE)))
                pkg->overrideRigPrecision(PrecisionSet::DefaultPrecision());
        }

        void SFBGSAnimationPackage_OverrideRigWith1stPersonPrecisionC(UNIV::Animation* animation)
        {
            if (auto pkg = dynamic_cast<SFBGS_AnimationPackage*>(animation->getPackageManager().getPackage(SFBGS_ANIM_PACKAGE)))
                pkg->overrideRigPrecision(PrecisionSet::FirstPersonPrecision());
        }

        void SFBGSAnimationPackage_OverrideRigWithShipPrecisionC(UNIV::Animation * animation)
        {
            if (auto pkg = dynamic_cast<SFBGS_AnimationPackage*>(animation->getPackageManager().getPackage(SFBGS_ANIM_PACKAGE)))
                pkg->overrideRigPrecision(PrecisionSet::ShipPrecision());
        }

        void SFBGSAnimationPackage_OverrideRigWithCustomPrecisionC(UNIV::Animation* animation, float custom1, float custom2)
        {
            if (auto pkg = dynamic_cast<SFBGS_AnimationPackage*>(animation->getPackageManager().getPackage(SFBGS_ANIM_PACKAGE)))
                pkg->overrideRigPrecision(PrecisionSet(custom1, custom2));
        }

        float SFBGSAnimationPackage_GetOverridePrecisionHighC(UNIV::Animation* animation)
        {
            if (auto pkg = dynamic_cast<SFBGS_AnimationPackage*>(animation->getPackageManager().getPackage(SFBGS_ANIM_PACKAGE)))
                return pkg->getOverridePrecisionSet().high();

            return std::numeric_limits<float>().quiet_NaN();
        }

        float SFBGSAnimationPackage_GetOverridePrecisionLowC(UNIV::Animation* animation)
        {
            if (auto pkg = dynamic_cast<SFBGS_AnimationPackage*>(animation->getPackageManager().getPackage(SFBGS_ANIM_PACKAGE)))
                return pkg->getOverridePrecisionSet().low();

            return std::numeric_limits<float>().quiet_NaN();
        }

        const char* SFBGSAnimationPackage_GetPrecisionSet(UNIV::Animation* animation)
        {
            if (auto pkg = dynamic_cast<SFBGS_AnimationPackage*>(animation->getPackageManager().getPackage(SFBGS_ANIM_PACKAGE)))
            {
                if (pkg->usesRigForPrecision())
                    return "Follows Rig";

                return pkg->getOverridePrecisionSet().precisionType();
            }

            return nullptr;
        }

        bool SFBGSAnimationPackage_UsesRigPrecision(UNIV::Animation* animation)
        {
            if (auto pkg = dynamic_cast<SFBGS_AnimationPackage*>(animation->getPackageManager().getPackage(SFBGS_ANIM_PACKAGE)))
            {
                return pkg->usesRigForPrecision();
            }

            return false;
        }

        void SFBGSAnimationPackage_ResetPrecision(UNIV::Animation* animation)
        {
            if (auto pkg = dynamic_cast<SFBGS_AnimationPackage*>(animation->getPackageManager().getPackage(SFBGS_ANIM_PACKAGE)))
            {
                pkg->resetPrecision();
            }
        }

        bool SFBGSAnimationPackage_AddAmendedBlockWithNameC(UNIV::Animation* animation, UNIV::AnimationBlock* block, Utilities::StringContainer* errorMessage, bool overwrite)
        {
            Utilities::StringContainer tempErrorMessage;
            Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
            errorMessageHolder->clear();

            *errorMessageHolder += "[CALUMI.Animation API] ";

            if (auto pkg = dynamic_cast<SFBGS_AnimationPackage*>(animation->getPackageManager().getPackage(SFBGS_ANIM_PACKAGE)))
            {
                *errorMessageHolder += block->boneName();
                
                if (pkg->addAmendedBlock(*block, overwrite))
                {
                    uint32_t hash = Utilities::BGS_Str_CRC32(block->boneName());
                    *errorMessageHolder += " added to SFBGS Animation Package with hash: ";
                    *errorMessageHolder += std::to_string(hash).c_str();

                    if (block)
                    {
                        delete block;
                        block = pkg->getAmendedBlock(pkg->findAmendedBlock(hash));
                    }

                    return true;
                }

                *errorMessageHolder += " could not be added to SFBGS Animation Package";
            }
            else
            {
                *errorMessageHolder += "Package Manager Not Found";
            }

            return false;
        }

        bool SFBGSAnimationPackage_AddAmendedBlockWithHashC(UNIV::Animation* animation, uint32_t hash, UNIV::AnimationBlock* block, Utilities::StringContainer* errorMessage, bool overwrite)
        {
            Utilities::StringContainer tempErrorMessage;
            Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
            errorMessageHolder->clear();

            *errorMessageHolder += "[CALUMI.Animation API] ";

            if (auto pkg = dynamic_cast<SFBGS_AnimationPackage*>(animation->getPackageManager().getPackage(SFBGS_ANIM_PACKAGE)))
            {
                *errorMessageHolder += "block";

                if (pkg->addAmendedBlock(hash, *block,overwrite))
                {
                    *errorMessageHolder += " added to SFBGS Animation Package with hash: ";
                    *errorMessageHolder += std::to_string(hash).c_str();

                    if (block)
                    {
                        delete block;
                        block = pkg->getAmendedBlock(pkg->findAmendedBlock(hash));
                    }

                    return true;
                }

                *errorMessageHolder += " could not be added to SFBGS Animation Package";
            }
            else
            {
                *errorMessageHolder += "Package Manager Not Found";
            }

            return false;
        }

        bool SFBGSAnimationPackage_RemoveAmendedBlockWithNameC(UNIV::Animation* animation, const char* name, Utilities::StringContainer* errorMessage)
        {
            Utilities::StringContainer tempErrorMessage;
            Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
            errorMessageHolder->clear();

            *errorMessageHolder += "[CALUMI.Animation API] ";

            if (auto pkg = dynamic_cast<SFBGS_AnimationPackage*>(animation->getPackageManager().getPackage(SFBGS_ANIM_PACKAGE)))
            {
                *errorMessageHolder += name;

                if (pkg->removeAmendedBlock(name))
                {
                    uint32_t hash = Utilities::BGS_Str_CRC32(name);
                    *errorMessageHolder += " with hash: ";
                    *errorMessageHolder += std::to_string(hash).c_str();
                    *errorMessageHolder += " removed from SFBGS Animation Package";

                    return true;
                }

                *errorMessageHolder += " could not be removed from SFBGS Animation Package. Are you sure it exists?";
            }
            else
            {
                *errorMessageHolder += "Package Manager Not Found";
            }

            return false;
        }

        bool SFBGSAnimationPackage_RemoveAmendedBlockWithHashC(UNIV::Animation* animation, uint32_t hash, Utilities::StringContainer* errorMessage)
        {
            Utilities::StringContainer tempErrorMessage;
            Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
            errorMessageHolder->clear();

            *errorMessageHolder += "[CALUMI.Animation API] ";

            if (auto pkg = dynamic_cast<SFBGS_AnimationPackage*>(animation->getPackageManager().getPackage(SFBGS_ANIM_PACKAGE)))
            {
                *errorMessageHolder += "block";

                if (pkg->removeAmendedBlock(hash))
                {
                    *errorMessageHolder += " with hash: ";
                    *errorMessageHolder += std::to_string(hash).c_str();
                    *errorMessageHolder += " removed from SFBGS Animation Package";

                    return true;
                }

                *errorMessageHolder += " could not be removed from SFBGS Animation Package. Are you sure it exists?";
            }
            else
            {
                *errorMessageHolder += "Package Manager Not Found";
            }

            return false;
        }

        bool SFBGSAnimationPackage_RemoveAmendedBlockWithIndexC(UNIV::Animation* animation, uint16_t index, Utilities::StringContainer* errorMessage)
        {
            Utilities::StringContainer tempErrorMessage;
            Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
            errorMessageHolder->clear();

            *errorMessageHolder += "[CALUMI.Animation API] ";

            if (auto pkg = dynamic_cast<SFBGS_AnimationPackage*>(animation->getPackageManager().getPackage(SFBGS_ANIM_PACKAGE)))
            {
                *errorMessageHolder += "block";

                if (pkg->removeAmendedBlock(index))
                {
                    *errorMessageHolder += " at index: ";
                    *errorMessageHolder += std::to_string(index).c_str();
                    *errorMessageHolder += " removed from SFBGS Animation Package";

                    return true;
                }

                *errorMessageHolder += " could not be removed from SFBGS Animation Package. Are you sure it exists?";
            }
            else
            {
                *errorMessageHolder += "Package Manager Not Found";
            }

            return false;
        }

        bool SFBGSAnimationPackage_HasAmendedBlockWithNameC(UNIV::Animation* animation, const char* name)
        {
            if (auto pkg = dynamic_cast<SFBGS_AnimationPackage*>(animation->getPackageManager().getPackage(SFBGS_ANIM_PACKAGE)))
            {
                return pkg->hasAmendedBlock(name);
            }

            return false;
        }

        bool SFBGSAnimationPackage_HasAmendedBlockWithHashC(UNIV::Animation* animation, uint32_t hash)
        {
            if (auto pkg = dynamic_cast<SFBGS_AnimationPackage*>(animation->getPackageManager().getPackage(SFBGS_ANIM_PACKAGE)))
            {
                return pkg->hasAmendedBlock(hash);
            }

            return false;
        }

        int SFBGSAnimationPackage_FindAmendedBlockWithNameC(UNIV::Animation* animation, const char* name)
        {
            if (auto pkg = dynamic_cast<SFBGS_AnimationPackage*>(animation->getPackageManager().getPackage(SFBGS_ANIM_PACKAGE)))
            {
                return pkg->findAmendedBlock(name);
            }

            return -1;
        }

        int SFBGSAnimationPackage_FindAmendedBlockWithHashC(UNIV::Animation* animation, uint32_t hash)
        {
            if (auto pkg = dynamic_cast<SFBGS_AnimationPackage*>(animation->getPackageManager().getPackage(SFBGS_ANIM_PACKAGE)))
            {
                return pkg->findAmendedBlock(hash);
            }

            return -1;
        }


#pragma endregion

}
}