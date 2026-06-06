//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com


#include <AnimStarfield>
#include <vector>
#include <AnimUniv>
#include <AnimUtil>
#include <limits>
#include <stdexcept>

// ReSharper disable once CppUnusedIncludeDirective
#include "internalplatform.h"

namespace CALUMI::SFBGS
{
    struct SFBGS_AnimationPackage::Impl {
        // leaving as vector until we know without doubt that order is not important...
        std::vector<std::pair<uint32_t, UNIV::AnimationBlock>> m_amendedBlocks;

        PrecisionSet m_precisionOverride = PrecisionSet::DefaultPrecision();
        bool m_useRigPrecision = true;
    };

    SFBGS_AnimationPackage::SFBGS_AnimationPackage() : pImpl(new Impl()) {}

    SFBGS_AnimationPackage::SFBGS_AnimationPackage(const SFBGS_AnimationPackage& other) : SFBGS_AnimationPackage()
    {
        *this = other;
    }

    SFBGS_AnimationPackage& SFBGS_AnimationPackage::operator=(const SFBGS_AnimationPackage& other)
    {
        if (this != &other)
        {
            pImpl->m_amendedBlocks = other.pImpl->m_amendedBlocks;
            pImpl->m_precisionOverride = other.pImpl->m_precisionOverride;
            pImpl->m_useRigPrecision = other.pImpl->m_useRigPrecision;
        }
        return *this;
    }

    bool SFBGS_AnimationPackage::operator==(const IPackage& other) const
    {
        if (const auto pOther = dynamic_cast<const SFBGS_AnimationPackage*>(&other))
        {
            return  pImpl->m_amendedBlocks == pOther->pImpl->m_amendedBlocks &&
                    pImpl->m_precisionOverride == pOther->pImpl->m_precisionOverride &&
                    pImpl->m_useRigPrecision == pOther->pImpl->m_useRigPrecision;
        }
        return false;
    }

    Utilities::JsonObject SFBGS_AnimationPackage::toJson() const
    {
        Utilities::JsonObject output;

        output["rig_precision"] = pImpl->m_useRigPrecision;
        output["precision"] = pImpl->m_precisionOverride.toJson();

        Utilities::JsonArray aBlocks;

        for (const auto& [key, blockEntry] : pImpl->m_amendedBlocks)
        {
            Utilities::JsonObject hashBlock;
            hashBlock["hash"] = key;
            hashBlock["block"] = blockEntry.toJson();
            aBlocks.push_back(hashBlock);
        }

        output["amended_blocks"] = aBlocks;

        return output;
    }

    void SFBGS_AnimationPackage::fromJson(const Utilities::JsonObject& data)
    {
        if (data.contains("rig_precision"))
            pImpl->m_useRigPrecision = data["rig_precision"].toBool();
        else
            pImpl->m_useRigPrecision = false;

        if (data.contains("precision"))
            pImpl->m_precisionOverride.fromJson(data["precision"].toObject());
        else
            pImpl->m_precisionOverride = PrecisionSet::DefaultPrecision();

        pImpl->m_amendedBlocks.clear();
        
        Utilities::JsonArray aBlocks = data["amended_blocks"].toArray();

        for (uint64_t i = 0; i < aBlocks.size(); i++)
        {
            if (Utilities::JsonObject hashBlock = aBlocks.at(i).toObject(); hashBlock.contains("hash") && hashBlock.contains("block"))
            {
                auto entry = UNIV::AnimationBlock(hashBlock["block"].toObject());
                addAmendedBlock(hashBlock["hash"].toUInt(), entry);
            }
        }
    }

    SFBGS_AnimationPackage::~SFBGS_AnimationPackage()
    {
        if (pImpl)
        {
            delete pImpl;
            pImpl = nullptr;
        }
    }

    uint16_t SFBGS_AnimationPackage::amendedBlockCount() const
    {
        return static_cast<uint16_t>(pImpl->m_amendedBlocks.size());
    }

    void SFBGS_AnimationPackage::overrideRigPrecision(const PrecisionSet& precision) const
    {
        pImpl->m_useRigPrecision = false;
        pImpl->m_precisionOverride = precision;
    }

    PrecisionSet SFBGS_AnimationPackage::overridePrecisionSet() const
    {
        return pImpl->m_precisionOverride;
    }

    bool SFBGS_AnimationPackage::usesRigForPrecision() const
    {
        return pImpl->m_useRigPrecision;
    }

    void SFBGS_AnimationPackage::resetPrecision() const
    {
        pImpl->m_useRigPrecision = true;
        pImpl->m_precisionOverride = PrecisionSet::DefaultPrecision();
    }

    bool SFBGS_AnimationPackage::addAmendedBlock(const UNIV::AnimationBlock& block, const bool overwrite) const
    {
        const uint32_t hash = Utilities::HashRegistry::Get().registerHash(block.boneName());

        return addAmendedBlock(hash, block, overwrite);
    }

    bool SFBGS_AnimationPackage::addAmendedBlock(uint32_t hash, const UNIV::AnimationBlock& block, const bool overwrite) const
    {
        if (pImpl->m_amendedBlocks.size() >= MaxAmendedBlockCount)
            return false;

        if (const int index = findAmendedBlock(hash); index >= 0)
        {
            if (!overwrite)
            {
                return false;
            }

            pImpl->m_amendedBlocks.at(index).second = block;
            return true;
        }

        pImpl->m_amendedBlocks.emplace_back( hash, block );
        return true;
    }

    bool SFBGS_AnimationPackage::removeAmendedBlock(const char* name) const
    {
        const uint32_t hash = Utilities::HashRegistry::Get().registerHash(name);

        return removeAmendedBlock(hash);
    }

    bool SFBGS_AnimationPackage::removeAmendedBlock(const uint32_t hash) const
    {
        return removeAmendedBlockUsingIndex(findAmendedBlock(hash));
    }

    bool SFBGS_AnimationPackage::removeAmendedBlockUsingIndex(const int index) const
    {
        if (index < 0 || index >= pImpl->m_amendedBlocks.size())
            return false;

        pImpl->m_amendedBlocks.erase(pImpl->m_amendedBlocks.begin() + index);

        return true;
    }

    bool SFBGS_AnimationPackage::hasAmendedBlock(const uint32_t hash) const
    {
        return findAmendedBlock(hash) >= 0;
    }

    bool SFBGS_AnimationPackage::hasAmendedBlock(const char* name) const
    {
        return findAmendedBlock(name) >= 0;
    }

    int SFBGS_AnimationPackage::findAmendedBlock(const uint32_t hash) const
    {
        for (uint64_t i = 0; i < pImpl->m_amendedBlocks.size(); i++)
        {
            if (pImpl->m_amendedBlocks.at(i).first == hash)
                return static_cast<int>(i);
        }

        return -1;
    }

    int SFBGS_AnimationPackage::findAmendedBlock(const char* name) const
    {
        const uint32_t hash = Utilities::HashRegistry::Get().registerHash(name);

        return findAmendedBlock(hash);
    }

    UNIV::AnimationBlock* SFBGS_AnimationPackage::amendedBlock(const int index) const
    {
        if (index < 0 || index >= pImpl->m_amendedBlocks.size())
            return nullptr;

        return &pImpl->m_amendedBlocks.at(index).second;
    }

    uint32_t SFBGS_AnimationPackage::amendedBlockHash(const int index) const
    {
        if (index < 0 || index >= pImpl->m_amendedBlocks.size())
            return 0xFFFFFFFF;

        return pImpl->m_amendedBlocks.at(index).first;
    }

    const char* SFBGS_AnimationPackage::packageType() const
    {
        return SFBGS_ANIM_PACKAGE;
    }

    SFBGS_AnimationPackage* SFBGS_AnimationPackage::clone() const
    {
        return new SFBGS_AnimationPackage(*this);
    }

    bool SFBGS_AnimationPackage::AddPackage(const UNIV::Animation& animation, const bool overwrite)
    {
        const auto& mgr = animation.packageManager();

        auto pkg = new SFBGS_AnimationPackage();

        if (mgr.addPackage(pkg, overwrite))
        {
            return true;
        }

        // ReSharper disable once CppDFAConstantConditions
        if (pkg)
        {
            delete pkg;
            // ReSharper disable once CppDFAUnusedValue
            pkg = nullptr;
        }

        return false;
    }

    bool SFBGS_AnimationPackage::RemovePackage(const UNIV::Animation& animation)
    {
        return animation.packageManager().removePackage(SFBGS_ANIM_PACKAGE);
    }

    SFBGS_AnimationPackage& SFBGS_AnimationPackage::GetPackage(const UNIV::Animation& animation)
    {
        const auto& mgr = animation.packageManager();
        if (const auto pkg = dynamic_cast<SFBGS_AnimationPackage*>(mgr.package(SFBGS_ANIM_PACKAGE)))
        {
            return *pkg;
        }

        AddPackage(animation, false);

        if (const auto pkg = dynamic_cast<SFBGS_AnimationPackage*>(mgr.package(SFBGS_ANIM_PACKAGE)))
        {
            return *pkg;
        }

        throw std::runtime_error("SFBGS_AnimationPackage::GetPackage() could not find nor add animation package.");
    }
}

#pragma region Extern "C"

int SFBGSAnimationPackage_AddPackageToAnimationC(const CALUMI::UNIV::Animation* animation, const bool overwrite)
{
    if (animation)
        try
        {
            return CALUMI::SFBGS::SFBGS_AnimationPackage::AddPackage(*animation, overwrite) ? 0 : 1;
        }
        catch (std::exception&){}

    return -1;
}

int SFBGSAnimationPackage_RemovePackageFromAnimationC(const CALUMI::UNIV::Animation* animation)
{
    if (animation)
        try
        {
            return CALUMI::SFBGS::SFBGS_AnimationPackage::RemovePackage(*animation) ? 0 : 1;
        }
        catch (std::exception&){}

    return -1;
}

int SFBGSAnimationPackage_HasAmendedBlockC(const CALUMI::UNIV::Animation* animation, const char* name)
{
    if (animation)
        try
        {
            return CALUMI::SFBGS::SFBGS_AnimationPackage::GetPackage(*animation).hasAmendedBlock(name) ? 1 : 0;
        }
    catch (std::exception&){}

    return -1;
}

int SFBGSAnimationPackage_HasAmendedBlockHashC(const CALUMI::UNIV::Animation* animation, const uint32_t hash)
{
    if (animation)
        try
        {
            return CALUMI::SFBGS::SFBGS_AnimationPackage::GetPackage(*animation).hasAmendedBlock(hash) ? 1 : 0;
        }
    catch (std::exception&){}

    return -1;
}

int SFBGSAnimationPackage_GetAmendedBlockCountC(const CALUMI::UNIV::Animation* animation)
{
    if (animation)
        try
        {
            return CALUMI::SFBGS::SFBGS_AnimationPackage::GetPackage(*animation).amendedBlockCount();
        }
    catch (std::exception&){}

    return -1;
}

int SFBGSAnimationPackage_OverrideRigWithDefaultPrecisionC(const CALUMI::UNIV::Animation* animation)
{
    if (animation)
        try
        {
            CALUMI::SFBGS::SFBGS_AnimationPackage::GetPackage(*animation).overrideRigPrecision(
                                                            CALUMI::SFBGS::PrecisionSet::DefaultPrecision());
            return 0;
        }
    catch (std::exception&){}

    return -1;
}

int SFBGSAnimationPackage_OverrideRigWith1stPersonPrecisionC(const CALUMI::UNIV::Animation* animation)
{
    if (animation)
        try
        {
            CALUMI::SFBGS::SFBGS_AnimationPackage::GetPackage(*animation).overrideRigPrecision(
                                                            CALUMI::SFBGS::PrecisionSet::FirstPersonPrecision());
            return 0;
        }
    catch (std::exception&){}

    return -1;
}

int SFBGSAnimationPackage_OverrideRigWithShipPrecisionC(const CALUMI::UNIV::Animation * animation)
{
    if (animation)
        try
        {
            CALUMI::SFBGS::SFBGS_AnimationPackage::GetPackage(*animation).overrideRigPrecision(
                                                            CALUMI::SFBGS::PrecisionSet::ShipPrecision());
            return 0;
        }
    catch (std::exception&){}

    return -1;
}

int SFBGSAnimationPackage_OverrideRigWithCustomPrecisionC(const CALUMI::UNIV::Animation* animation, const float custom1, const float custom2)
{
    if (animation)
        try
        {
            CALUMI::SFBGS::SFBGS_AnimationPackage::GetPackage(*animation).overrideRigPrecision(
                                                            CALUMI::SFBGS::PrecisionSet(custom1, custom2));
            return 0;
        }
    catch (std::exception&){}

    return -1;
}

float SFBGSAnimationPackage_GetOverridePrecisionHighC(const CALUMI::UNIV::Animation* animation)
{
    if (animation)
        try
        {
            return CALUMI::SFBGS::SFBGS_AnimationPackage::GetPackage(*animation).overridePrecisionSet().high();
        }
    catch (std::exception&){}

    return std::numeric_limits<float>::quiet_NaN();
}

float SFBGSAnimationPackage_GetOverridePrecisionLowC(const CALUMI::UNIV::Animation* animation)
{
    if (animation)
        try
        {
            return CALUMI::SFBGS::SFBGS_AnimationPackage::GetPackage(*animation).overridePrecisionSet().low();
        }
    catch (std::exception&){}

    return std::numeric_limits<float>::quiet_NaN();
}

const char* SFBGSAnimationPackage_GetPrecisionSet(const CALUMI::UNIV::Animation* animation)
{
    if (animation)
        try
        {
            const auto& pkg = CALUMI::SFBGS::SFBGS_AnimationPackage::GetPackage(*animation);
            return pkg.usesRigForPrecision() ? "Follows Rig" : pkg.overridePrecisionSet().precisionType();
        }
    catch (std::exception&){}
    return nullptr;
}

int SFBGSAnimationPackage_UsesRigPrecision(const CALUMI::UNIV::Animation* animation)
{
    if (animation)
        try
        {
            return CALUMI::SFBGS::SFBGS_AnimationPackage::GetPackage(*animation).usesRigForPrecision() ? 1 : 0;
        }
    catch (std::exception&){}
    return -1;
}

int SFBGSAnimationPackage_ResetPrecision(const CALUMI::UNIV::Animation* animation)
{
    if (animation)
        try
        {
            CALUMI::SFBGS::SFBGS_AnimationPackage::GetPackage(*animation).resetPrecision();
            return 0;
        }
    catch (std::exception&){}
    return -1;
}

int SFBGSAnimationPackage_AddAmendedBlockWithNameC(const CALUMI::UNIV::Animation* animation, const
    CALUMI::UNIV::AnimationBlock* block, const bool overwrite)
{
    if (animation)
        try
        {
            return CALUMI::SFBGS::SFBGS_AnimationPackage::GetPackage(*animation).addAmendedBlock(*block, overwrite) ? 0 : 1;
        }
        catch (std::exception&){}
    return -1;
}

int SFBGSAnimationPackage_AddAmendedBlockWithHashC(const CALUMI::UNIV::Animation* animation, const uint32_t hash,
    const CALUMI::UNIV::AnimationBlock* block, const bool overwrite)
{
    if (animation)
        try
        {
            return CALUMI::SFBGS::SFBGS_AnimationPackage::GetPackage(*animation).addAmendedBlock(hash, *block, overwrite) ? 0 : 1;
        }
        catch (std::exception&){}
    return -1;
}

int SFBGSAnimationPackage_RemoveAmendedBlockWithNameC(const CALUMI::UNIV::Animation* animation, const char* name)
{
    if (animation)
        try
        {
            return CALUMI::SFBGS::SFBGS_AnimationPackage::GetPackage(*animation).removeAmendedBlock(name) ? 0 : 1;
        }
    catch (std::exception&){}

    return -1;
}

int SFBGSAnimationPackage_RemoveAmendedBlockWithHashC(const CALUMI::UNIV::Animation* animation, const uint32_t hash)
{
    if (animation)
        try
        {
            return CALUMI::SFBGS::SFBGS_AnimationPackage::GetPackage(*animation).removeAmendedBlock(hash) ? 0 : 1;
        }
    catch (std::exception&){}

    return -1;
}

int SFBGSAnimationPackage_RemoveAmendedBlockWithIndexC(const CALUMI::UNIV::Animation* animation, const uint16_t index)
{
    if (animation)
        try
        {
            return CALUMI::SFBGS::SFBGS_AnimationPackage::GetPackage(*animation).removeAmendedBlockUsingIndex(index)
            ? 0 : 1;
        }
    catch (std::exception&){}

    return -1;
}

int SFBGSAnimationPackage_HasAmendedBlockWithNameC(const CALUMI::UNIV::Animation* animation, const char* name)
{
    if (animation)
        try
        {
            return CALUMI::SFBGS::SFBGS_AnimationPackage::GetPackage(*animation).hasAmendedBlock(name) ? 1 : 0;
        }
    catch (std::exception&){}

    return -1;
}

int SFBGSAnimationPackage_HasAmendedBlockWithHashC(const CALUMI::UNIV::Animation* animation, const uint32_t hash)
{
    if (animation)
        try
        {
            return CALUMI::SFBGS::SFBGS_AnimationPackage::GetPackage(*animation).hasAmendedBlock(hash) ? 1 : 0;
        }
    catch (std::exception&){}

    return -1;
}

int SFBGSAnimationPackage_FindAmendedBlockWithNameC(const CALUMI::UNIV::Animation* animation, const char* name)
{
    if (animation)
        try
        {
            return CALUMI::SFBGS::SFBGS_AnimationPackage::GetPackage(*animation).findAmendedBlock(name);
        }
    catch (std::exception&){}

    return -1;
}

int SFBGSAnimationPackage_FindAmendedBlockWithHashC(const CALUMI::UNIV::Animation* animation, const uint32_t hash)
{
    if (animation)
        try
        {
            return CALUMI::SFBGS::SFBGS_AnimationPackage::GetPackage(*animation).findAmendedBlock(hash);
        }
    catch (std::exception&){}

    return -1;
}


#pragma endregion


