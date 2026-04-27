//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

// ReSharper disable CppNonExplicitConvertingConstructor
#pragma once

#include "univ/animation/UNIV_Animation.h"
#include "univ/skeletonrig/UNIV_SkeletonRig.h"
#include "utilities/CALUMI_Utilities.h"

namespace CALUMI::UNIV
{
	/**
	 * @brief Overarching container that holds multiple animations and a skeleton rig that is to be modified by the
	 * contained animations
	 */
	class CALUMIANIMATION_API AnimationScene
	{
	public:
		/// @name Rig
		/// @{
		/**
		 * @return
		 */
		const SkeletonRig& rig() const;
		/**
		 * @param skeletonRig
		 */
		void setRig(const SkeletonRig& skeletonRig) const;

		/// @}
		/// @name Animations
		/// @{

		/**
		 * @param idx
		 * @return
		 */
		Animation* animation(uint64_t idx) const;
		/**
		 * @param name
		 * @return
		 */
		Animation* animation(const char* name) const;
		/**
		 * @return
		 */
		uint64_t animationCount() const;

		/**
		 * @param animation
		 * @param overwrite
		 * @return
		 */
		bool addAnimationToScene(const Animation& animation, bool overwrite = true) const;
		/**
		 * @param sceneToRemove
		 * @return
		 */
		bool removeAnimationFromScene(const Utilities::StringContainer& sceneToRemove) const;
		/**
		 * @param idx
		 * @return
		 */
		bool removeAnimationFromScene(unsigned int idx) const;

		/// @}
		/// @name Scene Data
		/// @{

		/**
		 * @return
		 */
		const char* sceneName() const;
		/**
		 * @param name
		 */
		void setSceneName(const char* name) const;
		/**
		 * @param input
		 */
		void setSceneName(const Utilities::StringContainer& input) const;
		/// @}
		/// @name Intializer
		/// @{

		AnimationScene();
		/**
		 * @param sceneName
		 */
		AnimationScene(const Utilities::StringContainer& sceneName);
		/**
		 * @param sceneName
		 */
		AnimationScene(const char* sceneName);
		/**
		 * @param input
		 */
		AnimationScene(const AnimationScene& input);
		~AnimationScene();
		/// @}
		/// @name Operators
		/// @{

		/**
		 * @param input
		 * @return
		 */
		AnimationScene& operator=(const AnimationScene& input);
		/// @}

	private:
		struct Impl;
		Impl* pImpl;
	};






}

/// @addtogroup extern_c
/// @{
/// @addtogroup c_animation_scene
/// @{
/// @defgroup extern_c_univ_scene Universal
/// @{

extern "C" {
/**
 * @warning Heap allocated return value, if not nullptr, must be deleted using DeleteAnimationSceneC
 * @param sceneName
 * @return
 */
CALUMIANIMATION_API CALUMI::UNIV::AnimationScene* CreateAnimationSceneC(const char* sceneName);
/**
 * @param scene
 * @param rig
 * @return Error Code:\n -1 = Invalid Ptr\n 0 = Successfully Added
 */
CALUMIANIMATION_API int AddRigToAnimationSceneC(const CALUMI::UNIV::AnimationScene* scene,
												const CALUMI::UNIV::SkeletonRig* rig);
/**
 * @param scene
 * @param animation
 * @param overwrite
 * @return Error Code:\n -1 = Invalid Ptr\n 0 = Successfully Added\n 1 = Animation Not Added
 */
CALUMIANIMATION_API int AddAnimationToAnimationSceneC(const CALUMI::UNIV::AnimationScene* scene,
                                                      const CALUMI::UNIV::Animation* animation, bool overwrite);
/**
 * @param ptr
 * @return Error Code:\n -1 = Invalid Ptr\n 0 = Successfully Added
 */
CALUMIANIMATION_API int DeleteAnimationSceneC(const CALUMI::UNIV::AnimationScene** ptr);
/**
 * @param source
 * @param index
 * @return Reference to animation or nullptr if there is an error
 */
CALUMIANIMATION_API CALUMI::UNIV::Animation* GetAnimationWithIndexC(const CALUMI::UNIV::AnimationScene* source, int index);
/**
 * @param source
 * @param name
 * @return Reference to animation or nullptr if there is an error
 */
CALUMIANIMATION_API CALUMI::UNIV::Animation* GetAnimationWithNameC(const CALUMI::UNIV::AnimationScene* source, const char* name);
/**
 * @param source
 * @return Number of animations or simply 0 if there are errors
 */
CALUMIANIMATION_API uint64_t GetAnimationCountC(CALUMI::UNIV::AnimationScene* source);
/**
 * @param source
 * @return
 */
CALUMIANIMATION_API const char* GetAnimationSceneNameC(CALUMI::UNIV::AnimationScene* source);
/**
 * @param source
 * @return
 */
CALUMIANIMATION_API const CALUMI::UNIV::SkeletonRig* GetSkeletonRigC(CALUMI::UNIV::AnimationScene* source);
}

/// @}
/// @}
/// @}
