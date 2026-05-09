//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

// ReSharper disable CppDFALocalValueEscapesFunction
#include "univ/animation/UNIV_AnimationEntries.h"
#include <algorithm>
#include <vector>
#include <string>
#include "internal/internalvectordef.h"

namespace CALUMI::UNIV
{
#pragma region TRANSLATION
    struct TranslationFrame::Impl
    {
        uint16_t m_frame = 0;
        Math::Vector3D m_translation;
    };
    uint16_t TranslationFrame::frame() const
    {
        return pImpl->m_frame;
    }
    void TranslationFrame::setFrame(const uint16_t frame) const
    {
        pImpl->m_frame = frame;
    }
    Math::Vector3D& TranslationFrame::translationVector() const
    {
        return pImpl->m_translation;
    }
    TranslationFrame::~TranslationFrame()
    {
        if (pImpl)
        {
            delete pImpl;
            pImpl = nullptr;
        }
    }
    TranslationFrame::TranslationFrame()
    {
        pImpl = new Impl;
    }
    TranslationFrame::TranslationFrame(const uint16_t& frame, const Math::Vector3D& translation) : TranslationFrame()
    {
        pImpl->m_frame = frame;
        pImpl->m_translation = translation;
    }

    TranslationFrame::TranslationFrame(const Utilities::JsonObject& data) : TranslationFrame()
    {
        if (data.contains("frame"))
            pImpl->m_frame = data["frame"].toUInt16();

        if (data.contains("translation"))
            pImpl->m_translation.fromJson(data["translation"].toObject());
    }

    TranslationFrame::TranslationFrame(const TranslationFrame& input) : TranslationFrame()
    {
        *this = input;
    }
    bool TranslationFrame::isIdentical(const TranslationFrame& input) const
    {
        return pImpl->m_frame == input.pImpl->m_frame &&
            pImpl->m_translation == input.pImpl->m_translation;
    }

    Utilities::JsonObject TranslationFrame::toJson() const
    {
        Utilities::JsonObject output;

        output["frame"] = pImpl->m_frame;
        output["translation"] = pImpl->m_translation.toJson();

        return output;
    }

    TranslationFrame& TranslationFrame::operator=(const TranslationFrame& other)
    {
        if (this != &other)
        {
            pImpl->m_frame = other.pImpl->m_frame;
            pImpl->m_translation = other.pImpl->m_translation;
        }
        return *this;
    }

    bool operator<(const TranslationFrame& A, const TranslationFrame& B) { return A.frame() < B.frame(); }
    bool operator<=(const TranslationFrame& A, const TranslationFrame& B) { return A.frame() <= B.frame(); }
    bool operator>(const TranslationFrame& A, const TranslationFrame& B) { return A.frame() > B.frame(); }
    bool operator>=(const TranslationFrame& A, const TranslationFrame& B) { return A.frame() >= B.frame(); }
    bool operator==(const TranslationFrame& A, const TranslationFrame& B) { return A.frame() == B.frame(); }
    bool operator!=(const TranslationFrame& A, const TranslationFrame& B) { return A.frame() != B.frame(); }

#pragma endregion

#pragma region ROTATION
    struct RotationFrame::Impl
    {
        uint16_t m_frame = 0;
        Math::Quaternion m_rotation;
        Impl() = default;
    };

    uint16_t RotationFrame::frame() const
    {
        return pImpl->m_frame;
    }
    void RotationFrame::setFrame(const uint16_t frame) const
    {
        pImpl->m_frame = frame;
    }
    Math::Quaternion& RotationFrame::rotationQuaternion() const
    {
        return pImpl->m_rotation;
    }
    RotationFrame::RotationFrame()
    {
        pImpl = new Impl;
    }
    RotationFrame::RotationFrame(const uint16_t& frame, const Math::Quaternion& rotation) : RotationFrame()
    {
        pImpl->m_frame = frame;
        pImpl->m_rotation = rotation;
    }

    RotationFrame::RotationFrame(const Utilities::JsonObject& data) : RotationFrame()
    {
        if (data.contains("frame"))
            pImpl->m_frame = data["frame"].toUInt16();

        if (data.contains("rotation"))
            pImpl->m_rotation.fromJson(data["rotation"].toObject());
    }

    RotationFrame::RotationFrame(const RotationFrame& input) : RotationFrame()
    {
        *this = input;
    }
    RotationFrame::~RotationFrame()
    {
        if (pImpl)
        {
            delete pImpl;
            pImpl = nullptr;
        }
    }

    bool RotationFrame::isIdentical(const RotationFrame& input) const
    {
        return pImpl->m_frame == input.pImpl->m_frame &&
            pImpl->m_rotation == input.pImpl->m_rotation;
    }

    Utilities::JsonObject RotationFrame::toJson() const
    {
        Utilities::JsonObject output;

        output["frame"] = pImpl->m_frame;
        output["rotation"] = pImpl->m_rotation.toJson();

        return output;
    }

    RotationFrame& RotationFrame::operator=(const RotationFrame& other)
    {
        if (this != &other)
        {
            pImpl->m_frame = other.pImpl->m_frame;
            pImpl->m_rotation = other.pImpl->m_rotation;
        }
        return *this;
    }

    bool operator<(const RotationFrame& A, const RotationFrame& B)    { return A.frame() < B.frame(); }
    bool operator<=(const RotationFrame& A, const RotationFrame& B)   { return A.frame() <= B.frame();}
    bool operator>(const RotationFrame& A, const RotationFrame& B)    { return A.frame() > B.frame(); }
    bool operator>=(const RotationFrame& A, const RotationFrame& B)   { return A.frame() >= B.frame();}
    bool operator==(const RotationFrame& A, const RotationFrame& B)   { return A.frame() == B.frame();}
    bool operator!=(const RotationFrame& A, const RotationFrame& B)   { return A.frame() != B.frame();}

#pragma endregion

#pragma region SCALAR
    struct ScalarFrame::Impl
    {
        uint16_t m_frame = 0;
        float m_scalar = 1.0;
        Impl() = default;
    };
    uint16_t ScalarFrame::frame() const { return pImpl->m_frame; }
    void ScalarFrame::setFrame(const uint16_t frame) const { pImpl->m_frame = frame; }
    float ScalarFrame::scalarValue() const { return pImpl->m_scalar; }
    void ScalarFrame::setScalarValue(const float value) const { pImpl->m_scalar = value; }

    ScalarFrame::ScalarFrame() { pImpl = new Impl; }
    ScalarFrame::~ScalarFrame()
    {
        if (pImpl)
        {
            delete pImpl;
            pImpl = nullptr;
        }
    }

    ScalarFrame::ScalarFrame(const uint16_t& frame, const float scalar) : ScalarFrame()
    {
        pImpl->m_frame = frame;
        pImpl->m_scalar = scalar;
    }

    ScalarFrame::ScalarFrame(const Utilities::JsonObject& data) : ScalarFrame()
    {
        if (data.contains("frame"))
            pImpl->m_frame = data["frame"].toUInt16();

        if (data.contains("scalar"))
            pImpl->m_scalar = data["scalar"].toFloat();
    }

    ScalarFrame::ScalarFrame(const ScalarFrame& input) : ScalarFrame()
    {
        *this = input;
    }

    bool ScalarFrame::isIdentical(const ScalarFrame& input) const
    {
        return pImpl->m_frame == input.pImpl->m_frame &&
            pImpl->m_scalar == input.pImpl->m_scalar;
    }

    Utilities::JsonObject ScalarFrame::toJson() const
    {
        const Utilities::JsonObject output;

        output["frame"] = pImpl->m_frame;
        output["scalar"] = pImpl->m_scalar;

        return output;
    }

    ScalarFrame& ScalarFrame::operator=(const ScalarFrame& other)
    {
        if (this != &other)
        {
            pImpl->m_frame = other.pImpl->m_frame;
            pImpl->m_scalar = other.pImpl->m_scalar;
        }
        return *this;
    }

    bool operator<(const ScalarFrame& A, const ScalarFrame& B)    { return A.frame()<  B.frame(); }
    bool operator<=(const ScalarFrame& A, const ScalarFrame& B)   { return A.frame()<= B.frame(); }
    bool operator>(const ScalarFrame& A, const ScalarFrame& B)    { return A.frame()>  B.frame(); }
    bool operator>=(const ScalarFrame& A, const ScalarFrame& B)   { return A.frame()>= B.frame(); }
    bool operator==(const ScalarFrame& A, const ScalarFrame& B)   { return A.frame()== B.frame(); }
    bool operator!=(const ScalarFrame& A, const ScalarFrame& B)   { return A.frame()!= B.frame(); }

#pragma endregion

#pragma region PRIORITY
    struct PriorityFrame::Impl
    {
        uint16_t m_frame = 0;
        uint8_t m_priority = 90;
        Impl() = default;
    };
    uint16_t PriorityFrame::frame() const { return pImpl->m_frame; }
    void PriorityFrame::setFrame(const uint16_t frame) const { pImpl->m_frame = frame; }
    uint8_t PriorityFrame::priorityValue() const { return pImpl->m_priority; }
    void PriorityFrame::setPriorityValue(const uint8_t value) const { pImpl->m_priority = value; }

    PriorityFrame::PriorityFrame() { pImpl = new Impl; }
    PriorityFrame::~PriorityFrame()
    {
        if (pImpl)
        {
            delete pImpl;
            pImpl = nullptr;
        }
    }
    PriorityFrame::PriorityFrame(const uint16_t& frame, const uint8_t& priority) : PriorityFrame() { pImpl->m_frame = frame; pImpl->m_priority = priority; }

    PriorityFrame::PriorityFrame(const Utilities::JsonObject& data) : PriorityFrame()
    {
        if (data.contains("frame"))
            pImpl->m_frame = data["frame"].toUInt16();

        if (data.contains("priority"))
            pImpl->m_priority = data["priority"].toUInt8();
    }

    PriorityFrame::PriorityFrame(const PriorityFrame& input) : PriorityFrame()
    {
        *this = input;
    }

    PriorityFrame& PriorityFrame::operator=(const PriorityFrame& other)
    {
        if (this != &other)
        {
            pImpl->m_frame = other.pImpl->m_frame;
            pImpl->m_priority = other.pImpl->m_priority;
        }
        return *this;
    }

    Utilities::JsonObject PriorityFrame::toJson() const
    {
        const Utilities::JsonObject output;

        output["frame"] = pImpl->m_frame;
        output["priority"] = pImpl->m_priority;

        return output;
    }

    bool PriorityFrame::isIdentical(const PriorityFrame& input) const
    {
        return pImpl->m_frame == input.pImpl->m_frame && pImpl->m_priority == input.pImpl->m_priority;
    }

    bool operator<(const PriorityFrame& A, const PriorityFrame& B)    { return A.frame() <  B.frame(); }
    bool operator<=(const PriorityFrame& A, const PriorityFrame& B)   { return A.frame() <= B.frame(); }
    bool operator>(const PriorityFrame& A, const PriorityFrame& B)    { return A.frame() >  B.frame(); }
    bool operator>=(const PriorityFrame& A, const PriorityFrame& B)   { return A.frame() >= B.frame(); }
    bool operator==(const PriorityFrame& A, const PriorityFrame& B)   { return A.frame() == B.frame(); }
    bool operator!=(const PriorityFrame& A, const PriorityFrame& B)   { return A.frame() != B.frame(); }

#pragma endregion

    VECTORDEF(TranslationSequence, CALUMI::UNIV::TranslationFrame)
    VECTORDEF(RotationSequence, CALUMI::UNIV::RotationFrame)
    VECTORDEF(ScalarSequence, CALUMI::UNIV::ScalarFrame)
    VECTORDEF(PrioritySequence, CALUMI::UNIV::PriorityFrame)

    void SortTranslationSequence(const TranslationSequence &sq, const bool highToLow)
    {
        if(sq.pImpl->vector.size() < 2)
            return;

        if (highToLow)
        {
            std::ranges::sort(sq.pImpl->vector, std::greater());
        }
        else
        {
            std::ranges::sort(sq.pImpl->vector);
        }
    }

    bool CompareTranslationSequence(const TranslationSequence& sq1, const TranslationSequence& sq2)
    {
        if (sq1.pImpl->vector.size() != sq2.pImpl->vector.size())
            return false;

        for (size_t i = 0; i < sq1.pImpl->vector.size(); i++)
        {
            if (!sq1.pImpl->vector.at(i).isIdentical(sq2.pImpl->vector.at(i)))
                return false;
        }

        return true;
    }

    void SortRotationSequence(const RotationSequence& sq, const bool highToLow)
    {
        if(sq.pImpl->vector.size() < 2)
            return;

        if (highToLow)
        {
            std::ranges::sort(sq.pImpl->vector, std::greater());
        }
        else
        {
            std::ranges::sort(sq.pImpl->vector);
        }
    }

    bool CompareRotationSequence(const RotationSequence& sq1, const RotationSequence& sq2)
    {
        if (sq1.pImpl->vector.size() != sq2.pImpl->vector.size())
            return false;

        for (size_t i = 0; i < sq1.pImpl->vector.size(); i++)
        {
            if (!sq1.pImpl->vector.at(i).isIdentical(sq2.pImpl->vector.at(i)))
                return false;
        }

        return true;
    }

    void SortScalarSequence(const ScalarSequence& sq, const bool highToLow)
    {
        if(sq.pImpl->vector.size() < 2)
            return;

        if (highToLow)
        {
            std::ranges::sort(sq.pImpl->vector, std::greater());
        }
        else
        {
            std::ranges::sort(sq.pImpl->vector);
        }
    }

    bool CompareScalarSequence(const ScalarSequence& sq1, const ScalarSequence& sq2)
    {
        if (sq1.pImpl->vector.size() != sq2.pImpl->vector.size())
            return false;

        for (size_t i = 0; i < sq1.pImpl->vector.size(); i++)
        {
            if (!sq1.pImpl->vector.at(i).isIdentical(sq2.pImpl->vector.at(i)))
                return false;
        }

        return true;
    }

    void SortPrioritySequence(const PrioritySequence& sq, const bool highToLow)
    {
        if(sq.pImpl->vector.size() < 2)
            return;

        if (highToLow)
        {
            std::ranges::sort(sq.pImpl->vector, std::greater());
        }
        else
        {
            std::ranges::sort(sq.pImpl->vector);
        }
    }

    bool ComparePrioritySequence(const PrioritySequence& sq1, const PrioritySequence& sq2)
    {
        if (sq1.pImpl->vector.size() != sq2.pImpl->vector.size())
            return false;

        for (size_t i = 0; i < sq1.pImpl->vector.size(); i++)
        {
            if (!sq1.pImpl->vector.at(i).isIdentical(sq2.pImpl->vector.at(i)))
                return false;
        }

        return true;
    }
}

#pragma region EXTERN"C"

CALUMI::UNIV::RotationFrame* CreateRotationFrameC(const uint16_t frame, float x, float y, float z, float w)
{
    const auto outputRotation = new CALUMI::UNIV::RotationFrame(frame, {x, y, z, w});
    outputRotation->rotationQuaternion().normalize();

    return outputRotation;
}

CALUMI::UNIV::RotationFrame* CreateRotationFrameFromEulerC(const uint16_t frame,
                                                           float x,
                                                           float y,
                                                           float z,
                                                           const
                                                           uint8_t
                                                           order)
{
    auto eOrder = CALUMI::Math::EulerDefinition::GetEulerOrder(order);

    const auto outputRotation = new CALUMI::UNIV::RotationFrame(frame, {x, y, z, eOrder});
    outputRotation->rotationQuaternion().normalize();

    return outputRotation;
}

int DeleteRotationFrameC(const CALUMI::UNIV::RotationFrame** ptr)
{
    if (ptr && *ptr)
    {
        delete *ptr;
        *ptr = nullptr;
        return 0;
    }
    return -1;
}

CALUMI::UNIV::TranslationFrame* CreateTranslationFrameC(const uint16_t frame, double x, double y, double z)
{
    const auto outputTranslation = new CALUMI::UNIV::TranslationFrame(frame, {x, y, z});

    return outputTranslation;
}

int DeleteTranslationFrameC(const CALUMI::UNIV::TranslationFrame** ptr)
{
    if (ptr && *ptr)
    {
        delete *ptr;
        *ptr = nullptr;
        return 0;
    }
    return -1;
}

CALUMI::UNIV::ScalarFrame* CreateScalarFrameC(const uint16_t frame, const float scalar)
{
    const auto outputScalar = new CALUMI::UNIV::ScalarFrame(frame, scalar);

    return outputScalar;
}

int DeleteScalarFrameC(const CALUMI::UNIV::ScalarFrame** ptr)
{
    if (ptr && *ptr)
    {
        delete *ptr;
        *ptr = nullptr;
        return 0;
    }
    return -1;
}

CALUMI::UNIV::PriorityFrame* CreatePriorityFrameC(const uint16_t frame, const uint8_t priority)
{
    const auto outputPriority = new CALUMI::UNIV::PriorityFrame(frame, priority);

    return outputPriority;
}

int DeletePriorityFrameC(const CALUMI::UNIV::PriorityFrame** ptr)
{
    if (ptr && *ptr)
    {
        delete *ptr;
        *ptr = nullptr;
        return 0;
    }
    return -1;
}

uint16_t GetFrameFromRotationFrameC(const CALUMI::UNIV::RotationFrame* source)
{
    return source->frame();
}

uint16_t GetFrameFromTranslationFrameC(const CALUMI::UNIV::TranslationFrame* source)
{
    if (source)
        try
        {
            return source->frame();
        }
    catch(const std::exception& )
    {
    }
    return 0;
}

uint16_t GetFrameFromScalarFrameC(const CALUMI::UNIV::ScalarFrame* source)
{
    if (source)
        try
        {
            return source->frame();
        }
    catch(const std::exception& )
    {
    }
    return 0;
}

uint16_t GetFrameFromPriorityFrameC(const CALUMI::UNIV::PriorityFrame* source)
{
    if (source)
        try
        {
            return source->frame();
        }
    catch(const std::exception& )
    {
    }
    return 0;
}

CALUMI::Math::Quaternion* GetValueFromRotationFrameC(const CALUMI::UNIV::RotationFrame* source)
{
    if (source)
        try
        {
            return &source->rotationQuaternion();
        }
    catch(const std::exception& )
    {
    }
    return nullptr;
}

CALUMI::Math::Vector3D* GetValueFromTranslationFrameC(const CALUMI::UNIV::TranslationFrame* source)
{
    if (source)
        try
        {
            return &source->translationVector();
        }
    catch(const std::exception& )
    {
    }
    return nullptr;
}

float GetValueFromScalarFrameC(const CALUMI::UNIV::ScalarFrame* source)
{
    if (source)
        try
        {
            return source->scalarValue();
        }
    catch(const std::exception& )
    {
    }
    return std::numeric_limits<float>::quiet_NaN();
}

uint8_t GetValueFromPriorityFrameC(const CALUMI::UNIV::PriorityFrame* source)
{
    if (source)
        try
        {
            return source->priorityValue();
        }
    catch(const std::exception& )
    {
    }
    return 0;
}
#pragma endregion




