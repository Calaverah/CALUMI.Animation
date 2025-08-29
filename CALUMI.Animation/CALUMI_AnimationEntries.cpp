#include "pch.h"
#include "CALUMI_AnimationEntries.h"

namespace CALUMI {
    namespace UNIV {

        //Extern C functions
        Rotation* CreateRotationEntryC(uint16_t frame, float x, float y, float z, float w)
        {
            Rotation* outputRotation = new Rotation(frame, {x,y,z,w});
            outputRotation->rotation.Normalize();
            return outputRotation;
        }
        bool DeleteRotationEntryC(Rotation* ptr)
        {
            if (ptr)
            {
                delete ptr;
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
                return true;
            }
            return false;
        }

        uint16_t GetFrameFromEntryC(void* source)
        {
            uint16_t output;
            std::memcpy(&output, source, sizeof(uint16_t));
            return output;
        }

        Math::Quaternion* GetValueFromRotationEntryC(Rotation* source)
        {
            return &source->rotation;
        }

        Math::Vector3D* GetValueFromTranslationEntryC(Translation* source)
        {
            return &source->translation;
        }

        float GetValueFromScalarEntryC(Scalar* source)
        {
            return source->scalar;
        }

        uint8_t GetValueFromPriorityEntryC(Priority* source)
        {
            return source->priority;
        }

        

} }



