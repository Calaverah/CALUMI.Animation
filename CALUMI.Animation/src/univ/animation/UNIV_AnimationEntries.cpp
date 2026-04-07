//Copyright � 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "univ/animation/UNIV_AnimationEntries.h"
#include <algorithm>
#include <vector>
#include <string>
#include "internal/internalvectordef.h"

namespace CALUMI {
    namespace UNIV {

#pragma region TRANSLATION
        struct Translation::Impl
        {
            uint16_t frame = 0;
            CALUMI::Math::Vector3D translation;
        };
        uint16_t UNIV::Translation::frame() const
        {
            return pImpl->frame;
        }
        void UNIV::Translation::setFrame(uint16_t frame)
        {
            pImpl->frame = frame;
        }
        Math::Vector3D& UNIV::Translation::translationVector() const
        {
            return pImpl->translation;
        }
        UNIV::Translation::~Translation()
        {
            if (pImpl)
            {
                delete pImpl;
                pImpl = nullptr;
            }
        }
        UNIV::Translation::Translation()
        {
            pImpl = new Impl;
        }
        UNIV::Translation::Translation(const uint16_t& frame, const CALUMI::Math::Vector3D& translation) : Translation()
        {
            pImpl->frame = frame;
            pImpl->translation = translation;
        }
        UNIV::Translation::Translation(const Translation& input) : Translation()
        {
            *this = input;
        }
        bool Translation::isIdentical(const Translation& input) const
        {
            return pImpl->frame == input.pImpl->frame &&
                pImpl->translation == input.pImpl->translation;
        }
        UNIV::Translation& UNIV::Translation::operator=(const UNIV::Translation& other)
        {
            pImpl->frame = other.pImpl->frame;
            pImpl->translation = other.pImpl->translation;
            return *this;
        }

        bool operator<(const UNIV::Translation& A, const UNIV::Translation& B) { return A.frame() < B.frame(); }
        bool operator<=(const UNIV::Translation& A, const UNIV::Translation& B) { return A.frame() <= B.frame(); }
        bool operator>(const UNIV::Translation& A, const UNIV::Translation& B) { return A.frame() > B.frame(); }
        bool operator>=(const UNIV::Translation& A, const UNIV::Translation& B) { return A.frame() >= B.frame(); }
        bool operator==(const UNIV::Translation& A, const UNIV::Translation& B) { return A.frame() == B.frame(); }
        bool operator!=(const UNIV::Translation& A, const UNIV::Translation& B) { return A.frame() != B.frame(); }

        Utilities::StringContainer Translation::toJSON(const uint64_t indents) const {
            std::string output(Utilities::Indent(indents + 1).c_str());
            output += ("{\"frame\": " + std::to_string(frame()) + ", \"value\": [" + std::to_string(translationVector().getX()) + ", " + std::to_string(translationVector().getY()) + ", " + std::to_string(translationVector().getZ()) + "]}");
            return output.c_str();
        }
#pragma endregion

#pragma region ROTATION
        struct Rotation::Impl
        {
            uint16_t frame = 0;
            CALUMI::Math::Quaternion rotation;
            Impl() = default;
        };

        uint16_t Rotation::frame() const
        {
            return pImpl->frame;
        }
        void Rotation::setFrame(uint16_t frame)
        {
            pImpl->frame = frame;
        }
        Math::Quaternion& Rotation::rotationQuaternion() const
        {
            return pImpl->rotation;
        }
        Rotation::Rotation()
        {
            pImpl = new Impl;
        }
        Rotation::Rotation(const uint16_t& frame, const CALUMI::Math::Quaternion& rotation) : Rotation()
        {
            pImpl->frame = frame; 
            pImpl->rotation = rotation;
        }
        Rotation::Rotation(const Rotation& input) : Rotation()
        {
            *this = input;
        }
        Rotation::~Rotation()
        {
            if (pImpl)
            {
                delete pImpl;
                pImpl = nullptr;
            }
        }

        bool Rotation::isIdentical(const Rotation& input) const
        {
            return pImpl->frame == input.pImpl->frame &&
                   pImpl->rotation == input.pImpl->rotation;
        }

        UNIV::Rotation& UNIV::Rotation::operator=(const UNIV::Rotation& other)
        {
            pImpl->frame = other.pImpl->frame;
            pImpl->rotation = other.pImpl->rotation;
            return *this;
        }

        bool operator<(const UNIV::Rotation& A, const UNIV::Rotation& B)    { return A.frame() < B.frame(); }
        bool operator<=(const UNIV::Rotation& A, const UNIV::Rotation& B)   { return A.frame() <= B.frame();}
        bool operator>(const UNIV::Rotation& A, const UNIV::Rotation& B)    { return A.frame() > B.frame(); }
        bool operator>=(const UNIV::Rotation& A, const UNIV::Rotation& B)   { return A.frame() >= B.frame();}
        bool operator==(const UNIV::Rotation& A, const UNIV::Rotation& B)   { return A.frame() == B.frame();}
        bool operator!=(const UNIV::Rotation& A, const UNIV::Rotation& B)   { return A.frame() != B.frame();}

        Utilities::StringContainer Rotation::toJSON(const uint64_t indents) const {
            std::string output(Utilities::Indent(indents).c_str());
            output += ("{\"frame\": " + std::to_string(pImpl->frame) + ", \"value\": [" + std::to_string(pImpl->rotation.getW()) + ", " + std::to_string(pImpl->rotation.getX()) + ", " + std::to_string(pImpl->rotation.getY()) + ", " + std::to_string(pImpl->rotation.getZ()) + "]}");
            return output.c_str();
        }
#pragma endregion

#pragma region SCALAR
        struct Scalar::Impl 
        {
            uint16_t frame = 0;
            float scalar = 1.0;
            Impl() = default;
        };
        uint16_t Scalar::frame() const { return pImpl->frame; }
        void Scalar::setFrame(uint16_t frame) { pImpl->frame = frame; }
        float Scalar::scalarValue() const { return pImpl->scalar; }
        void Scalar::setScalarValue(float value) { pImpl->scalar = value; }

        Scalar::Scalar() { pImpl = new Impl; }
        Scalar::~Scalar()
        {
            if (pImpl)
            {
                delete pImpl;
                pImpl = nullptr;
            }
        }

        Scalar::Scalar(const uint16_t& frame, float scalar) : Scalar()
        {
            pImpl->frame = frame;
            pImpl->scalar = scalar;
        }
        Scalar::Scalar(const Scalar& input) : Scalar()
        {
            *this = input;
        }

        bool Scalar::isIdentical(const Scalar& input) const
        {
            return pImpl->frame == input.pImpl->frame &&
                   pImpl->scalar == input.pImpl->scalar;
        }

        UNIV::Scalar& UNIV::Scalar::operator=(const UNIV::Scalar& other)
        {
            pImpl->frame = other.pImpl->frame;
            pImpl->scalar = other.pImpl->scalar;
            return *this;
        }

        bool operator<(const UNIV::Scalar& A, const UNIV::Scalar& B)    { return A.frame()<  B.frame(); }
        bool operator<=(const UNIV::Scalar& A, const UNIV::Scalar& B)   { return A.frame()<= B.frame(); }
        bool operator>(const UNIV::Scalar& A, const UNIV::Scalar& B)    { return A.frame()>  B.frame(); }
        bool operator>=(const UNIV::Scalar& A, const UNIV::Scalar& B)   { return A.frame()>= B.frame(); }
        bool operator==(const UNIV::Scalar& A, const UNIV::Scalar& B)   { return A.frame()== B.frame(); }
        bool operator!=(const UNIV::Scalar& A, const UNIV::Scalar& B)   { return A.frame()!= B.frame(); }

        Utilities::StringContainer Scalar::toJSON(const uint64_t indents) const {
            std::string output(Utilities::Indent(indents + 1).c_str());
            output += ("{\"frame\": " + std::to_string(pImpl->frame) + ", \"value\": " + std::to_string(pImpl->scalar) + "}").c_str();
            return output.c_str();
        }
#pragma endregion

#pragma region PRIORITY
        struct Priority::Impl
        {
            uint16_t frame = 0;
            uint8_t priority = 90;
            Impl() = default;
        };
        uint16_t Priority::frame() const { return pImpl->frame; }
        void Priority::setFrame(uint16_t frame) { pImpl->frame = frame; }
        uint8_t Priority::priorityValue() const { return pImpl->priority; }
        void Priority::setPriorityValue(uint8_t value) { pImpl->priority = value; }

        Priority::Priority() { pImpl = new Impl; }
        Priority::~Priority()
        {
            if (pImpl)
            {
                delete pImpl;
                pImpl = nullptr;
            }
        }
        Priority::Priority(const uint16_t& frame, const uint8_t& priority) : Priority() { pImpl->frame = frame; pImpl->priority = priority; }
        Priority::Priority(const Priority& input) : Priority()
        {
            *this = input;
        }

        UNIV::Priority& UNIV::Priority::operator=(const UNIV::Priority& other)
        {
            pImpl->frame = other.pImpl->frame;
            pImpl->priority = other.pImpl->priority;
            return *this;
        }
        bool operator<(const UNIV::Priority& A, const UNIV::Priority& B)    { return A.frame() <  B.frame(); }
        bool operator<=(const UNIV::Priority& A, const UNIV::Priority& B)   { return A.frame() <= B.frame(); }
        bool operator>(const UNIV::Priority& A, const UNIV::Priority& B)    { return A.frame() >  B.frame(); }
        bool operator>=(const UNIV::Priority& A, const UNIV::Priority& B)   { return A.frame() >= B.frame(); }
        bool operator==(const UNIV::Priority& A, const UNIV::Priority& B)   { return A.frame() == B.frame(); }
        bool operator!=(const UNIV::Priority& A, const UNIV::Priority& B)   { return A.frame() != B.frame(); }

        Utilities::StringContainer Priority::toJSON(const uint64_t indents) const {
            std::string output(Utilities::Indent(indents + 1).c_str());
            output += ("{\"frame\": " + std::to_string(pImpl->frame) + ", \"value\": " + std::to_string(pImpl->priority) + "}");
            return output.c_str();
        }
#pragma endregion

#pragma region EXTERN"C"

        Rotation* CreateRotationEntryC(uint16_t frame, float x, float y, float z, float w)
        {
            Rotation* outputRotation = new Rotation(frame, {x,y,z,w});
            outputRotation->rotationQuaternion().normalize();
            return outputRotation;
        }
        Rotation* CreateRotationEntryFromEulerC(uint16_t frame, float x, float y, float z, uint8_t order)
        {
            Math::Quaternion::EulerOrder eOrder = order > static_cast<uint8_t>(Math::Quaternion::EulerOrder::Max) ? Math::Quaternion::EulerOrder::XYZ : static_cast<Math::Quaternion::EulerOrder>(order);

            Rotation* outputRotation = new Rotation(frame, { x,y,z,eOrder });
            outputRotation->rotationQuaternion().normalize();
            return outputRotation;
        }
        bool DeleteRotationEntryC(Rotation* ptr)
        {
            if (ptr)
            {
                delete ptr;
                ptr = nullptr;
                return true;
            }
            return false;
        }
        Translation* CreateTranslationEntryC(uint16_t frame, double x, double y, double z)
        {
            Translation* outputTranslation = new Translation(frame, {x,y,z});

            return outputTranslation;
        }
        bool DeleteTranslationEntryC(Translation* ptr)
        {
            if (ptr)
            {
                delete ptr;
                ptr = nullptr;
                return true;
            }
            return false;
        }
        Scalar* CreateScalarEntryC(uint16_t frame, float scalar)
        {
            Scalar* outputScalar = new Scalar(frame, scalar);
            return outputScalar;
        }
        bool DeleteScalarEntryC(Scalar* ptr)
        {
            if (ptr)
            {
                delete ptr;
                ptr = nullptr;
                return true;
            }
            return false;
        }
        Priority* CreatePriorityEntryC(uint16_t frame, uint8_t priority)
        {
            Priority* outputPriority = new Priority(frame, priority);
            return outputPriority;
        }
        bool DeletePriorityEntryC(Priority* ptr)
        {
            if (ptr)
            {
                delete ptr;
                ptr = nullptr;
                return true;
            }
            return false;
        }

        uint16_t GetFrameFromRotationEntryC(Rotation* source)
        {
            return source->frame();
        }

        uint16_t GetFrameFromTranslationEntryC(Translation* source)
        {
            return source->frame();
        }

        uint16_t GetFrameFromScalarEntryC(Scalar* source)
        {
            return source->frame();
        }

        uint16_t GetFrameFromPriorityEntryC(Priority* source)
        {
            return source->frame();
        }

        Math::Quaternion* GetValueFromRotationEntryC(Rotation* source)
        {
            return &source->rotationQuaternion();
        }

        Math::Vector3D* GetValueFromTranslationEntryC(Translation* source)
        {
            return &source->translationVector();
        }

        float GetValueFromScalarEntryC(Scalar* source)
        {
            return source->scalarValue();
        }

        uint8_t GetValueFromPriorityEntryC(Priority* source)
        {
            return source->priorityValue();
        }
#pragma endregion        

        VECTORDEF(TranslationSequence, Translation)
        VECTORDEF(RotationSequence, Rotation)
        VECTORDEF(ScalarSequence, Scalar)
        VECTORDEF(PrioritySequence, Priority)

        void SortTranslationSequence(TranslationSequence &sq, bool highToLow)
        {
            if(sq.pImpl->vector.size() < 2)
                return;

            if (highToLow)
                {
                    std::sort(sq.pImpl->vector.begin(), sq.pImpl->vector.end(), std::greater<Translation>());
                }
                else
                {
                    std::sort(sq.pImpl->vector.begin(), sq.pImpl->vector.end());
                }
        }

        void SortRotationSequence(RotationSequence& sq, bool highToLow)
        {
            if(sq.pImpl->vector.size() < 2)
                return;

            if (highToLow)
            {
                std::sort(sq.pImpl->vector.begin(), sq.pImpl->vector.end(), std::greater<Rotation>());
            }
            else
            {
                std::sort(sq.pImpl->vector.begin(), sq.pImpl->vector.end());
            }
        }

        void SortScalarSequence(ScalarSequence& sq, bool highToLow)
        {
            if(sq.pImpl->vector.size() < 2)
                return;

            if (highToLow)
            {
                std::sort(sq.pImpl->vector.begin(), sq.pImpl->vector.end(), std::greater<Scalar>());
            }
            else
            {
                std::sort(sq.pImpl->vector.begin(), sq.pImpl->vector.end());
            }
        }

        void SortPrioritySequence(PrioritySequence& sq, bool highToLow)
        {
            if(sq.pImpl->vector.size() < 2)
                return;

            if (highToLow)
            {
                std::sort(sq.pImpl->vector.begin(), sq.pImpl->vector.end(), std::greater<Priority>());
            }
            else
            {
                std::sort(sq.pImpl->vector.begin(), sq.pImpl->vector.end());
            }
        }

}
}



