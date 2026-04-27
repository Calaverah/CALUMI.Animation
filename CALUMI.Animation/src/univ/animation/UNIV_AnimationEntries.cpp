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
        uint16_t frame = 0;
        Math::Vector3D translation;
    };
    uint16_t TranslationFrame::frame() const
    {
        return pImpl->frame;
    }
    void TranslationFrame::setFrame(const uint16_t frame) const
    {
        pImpl->frame = frame;
    }
    Math::Vector3D& TranslationFrame::translationVector() const
    {
        return pImpl->translation;
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
        pImpl->frame = frame;
        pImpl->translation = translation;
    }
    TranslationFrame::TranslationFrame(const TranslationFrame& input) : TranslationFrame()
    {
        *this = input;
    }
    bool TranslationFrame::isIdentical(const TranslationFrame& input) const
    {
        return pImpl->frame == input.pImpl->frame &&
            pImpl->translation == input.pImpl->translation;
    }
    TranslationFrame& TranslationFrame::operator=(const TranslationFrame& other)
    {
        if (this != &other)
        {
            pImpl->frame = other.pImpl->frame;
            pImpl->translation = other.pImpl->translation;
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
        uint16_t frame = 0;
        Math::Quaternion rotation;
        Impl() = default;
    };

    uint16_t RotationFrame::frame() const
    {
        return pImpl->frame;
    }
    void RotationFrame::setFrame(const uint16_t frame) const
    {
        pImpl->frame = frame;
    }
    Math::Quaternion& RotationFrame::rotationQuaternion() const
    {
        return pImpl->rotation;
    }
    RotationFrame::RotationFrame()
    {
        pImpl = new Impl;
    }
    RotationFrame::RotationFrame(const uint16_t& frame, const Math::Quaternion& rotation) : RotationFrame()
    {
        pImpl->frame = frame;
        pImpl->rotation = rotation;
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
        return pImpl->frame == input.pImpl->frame &&
            pImpl->rotation == input.pImpl->rotation;
    }

    RotationFrame& RotationFrame::operator=(const RotationFrame& other)
    {
        if (this != &other)
        {
            pImpl->frame = other.pImpl->frame;
            pImpl->rotation = other.pImpl->rotation;
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
        uint16_t frame = 0;
        float scalar = 1.0;
        Impl() = default;
    };
    uint16_t ScalarFrame::frame() const { return pImpl->frame; }
    void ScalarFrame::setFrame(const uint16_t frame) const { pImpl->frame = frame; }
    float ScalarFrame::scalarValue() const { return pImpl->scalar; }
    void ScalarFrame::setScalarValue(const float value) const { pImpl->scalar = value; }

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
        pImpl->frame = frame;
        pImpl->scalar = scalar;
    }
    ScalarFrame::ScalarFrame(const ScalarFrame& input) : ScalarFrame()
    {
        *this = input;
    }

    bool ScalarFrame::isIdentical(const ScalarFrame& input) const
    {
        return pImpl->frame == input.pImpl->frame &&
            pImpl->scalar == input.pImpl->scalar;
    }

    ScalarFrame& ScalarFrame::operator=(const ScalarFrame& other)
    {
        if (this != &other)
        {
            pImpl->frame = other.pImpl->frame;
            pImpl->scalar = other.pImpl->scalar;
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
        uint16_t frame = 0;
        uint8_t priority = 90;
        Impl() = default;
    };
    uint16_t PriorityFrame::frame() const { return pImpl->frame; }
    void PriorityFrame::setFrame(const uint16_t frame) const { pImpl->frame = frame; }
    uint8_t PriorityFrame::priorityValue() const { return pImpl->priority; }
    void PriorityFrame::setPriorityValue(const uint8_t value) const { pImpl->priority = value; }

    PriorityFrame::PriorityFrame() { pImpl = new Impl; }
    PriorityFrame::~PriorityFrame()
    {
        if (pImpl)
        {
            delete pImpl;
            pImpl = nullptr;
        }
    }
    PriorityFrame::PriorityFrame(const uint16_t& frame, const uint8_t& priority) : PriorityFrame() { pImpl->frame = frame; pImpl->priority = priority; }
    PriorityFrame::PriorityFrame(const PriorityFrame& input) : PriorityFrame()
    {
        *this = input;
    }

    PriorityFrame& PriorityFrame::operator=(const PriorityFrame& other)
    {
        if (this != &other)
        {
            pImpl->frame = other.pImpl->frame;
            pImpl->priority = other.pImpl->priority;
        }
        return *this;
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




