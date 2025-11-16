#include "pch.h"
#include "SFBGS_AnimationGraphComponents.h"
//#include <cstring>
//#include <string>
//#include <map>
//#include <unordered_map>
//#include <memory>
//#include <stdexcept>
//#include <print>
//#include <sstream>
//
//
//namespace CALUMI {
//	namespace SFBGS {
//
//#pragma region AgxVariables
//		static std::unordered_map<AnimationVariable, std::pair<const char*, AnimationVariableType>> AnimationVarTable =
//		{
//			{ AnimationVariable::NONE, { "", AnimationVariableType::Any}},
//			{ AnimationVariable::Acceleration, { "Acceleration", AnimationVariableType::Float}},
//			{ AnimationVariable::ActiveTurnInPlace, { "ActiveTurnInPlace", AnimationVariableType::Integer}},
//			{ AnimationVariable::AimAndAttack, { "AimAndAttack", AnimationVariableType::Integer}},
//			{ AnimationVariable::AimAtLocation, { "AimAtLocation", AnimationVariableType::Vector}},
//			{ AnimationVariable::AimWobble, { "AimWobble", AnimationVariableType::Float}},
//			{ AnimationVariable::AimWobbleSpeedMult, { "AimWobbleSpeedMult", AnimationVariableType::Float}},
//			{ AnimationVariable::Amount, { "Amount", AnimationVariableType::Float}},
//			{ AnimationVariable::AnimObjActive, { "AnimObjActive", AnimationVariableType::Integer}},
//			{ AnimationVariable::AnimSpeed, { "AnimSpeed", AnimationVariableType::Float}},
//			{ AnimationVariable::ArmillaryAssemblePhase1, { "ArmillaryAssemblePhase1", AnimationVariableType::Integer}},
//			{ AnimationVariable::ArmillaryAssemblePhase2, { "ArmillaryAssemblePhase2", AnimationVariableType::Integer}},
//			{ AnimationVariable::ArmillaryAssemblePhase3, { "ArmillaryAssemblePhase3", AnimationVariableType::Integer}},
//			{ AnimationVariable::ArmillaryAssemblePhase4, { "ArmillaryAssemblePhase4", AnimationVariableType::Integer}},
//			{ AnimationVariable::ArmillaryAssemblePhase5, { "ArmillaryAssemblePhase5", AnimationVariableType::Integer}},
//			{ AnimationVariable::ArmillaryAssemblePhase6, { "ArmillaryAssemblePhase6", AnimationVariableType::Integer}},
//			{ AnimationVariable::ArmillaryPhase1, { "ArmillaryPhase1", AnimationVariableType::Integer}},
//			{ AnimationVariable::ArmillaryPhase2, { "ArmillaryPhase2", AnimationVariableType::Integer}},
//			{ AnimationVariable::ArmillaryPhase3, { "ArmillaryPhase3", AnimationVariableType::Integer}},
//			{ AnimationVariable::ArmillaryPhase4, { "ArmillaryPhase4", AnimationVariableType::Integer}},
//			{ AnimationVariable::ArmillaryPhase5, { "ArmillaryPhase5", AnimationVariableType::Integer}},
//			{ AnimationVariable::ArmillaryPhase6, { "ArmillaryPhase6", AnimationVariableType::Integer}},
//			{ AnimationVariable::bAimActive, { "bAimActive", AnimationVariableType::Boolean}},
//			{ AnimationVariable::bAlignWithAimAssistTarget, { "bAlignWithAimAssistTarget", AnimationVariableType::Integer}},
//			{ AnimationVariable::bAllowFinisher, { "bAllowFinisher", AnimationVariableType::Integer}},
//			{ AnimationVariable::bAllowReversal, { "bAllowReversal", AnimationVariableType::Integer}},
//			{ AnimationVariable::bBlockMeleeBash, { "bBlockMeleeBash", AnimationVariableType::Integer}},
//			{ AnimationVariable::bCameraHits, { "bCameraHits", AnimationVariableType::Integer}},
//			{ AnimationVariable::bCanComboAttack, { "bCanComboAttack", AnimationVariableType::Integer}},
//			{ AnimationVariable::bCanQueueAttackWhileReloading, { "bCanQueueAttackWhileReloading", AnimationVariableType::Integer}},
//			{ AnimationVariable::bChargeRecharging, { "bChargeRecharging", AnimationVariableType::Boolean}},
//			{ AnimationVariable::bCombatWalk, { "bCombatWalk", AnimationVariableType::Integer}},
//			{ AnimationVariable::bDelayReloading, { "bDelayReloading", AnimationVariableType::Integer}},
//			{ AnimationVariable::bDelayStandStart, { "bDelayStandStart", AnimationVariableType::Integer}},
//			{ AnimationVariable::bDisableAlert, { "bDisableAlert", AnimationVariableType::Integer}},
//			{ AnimationVariable::bDisableFurnitureHeadtrack, { "bDisableFurnitureHeadtrack", AnimationVariableType::Integer}},
//			{ AnimationVariable::bDisableWeaponConstraint, { "bDisableWeaponConstraint", AnimationVariableType::Integer}},
//			{ AnimationVariable::bDoBoltCharge, { "bDoBoltCharge", AnimationVariableType::Boolean}},
//			{ AnimationVariable::bDontAllowAttack, { "bDontAllowAttack", AnimationVariableType::Integer}},
//			{ AnimationVariable::bDontAllowBlock, { "bDontAllowBlock", AnimationVariableType::Integer}},
//			{ AnimationVariable::bDontAllowFlinch, { "bDontAllowFlinch", AnimationVariableType::Integer}},
//			{ AnimationVariable::bDontAllowPowerAttack, { "bDontAllowPowerAttack", AnimationVariableType::Integer}},
//			{ AnimationVariable::bDontAllowReloading, { "bDontAllowReloading", AnimationVariableType::Integer}},
//			{ AnimationVariable::bDoReloadLoop, { "bDoReloadLoop", AnimationVariableType::Integer}},
//			{ AnimationVariable::bEnableHandIK, { "bEnableHandIK", AnimationVariableType::Integer}},
//			{ AnimationVariable::bEnableLeftHandIK, { "bEnableLeftHandIK", AnimationVariableType::Integer}},
//			{ AnimationVariable::bEnableRightHandIK, { "bEnableRightHandIK", AnimationVariableType::Integer}},
//			{ AnimationVariable::bFootLeftLock, { "bFootLeftLock", AnimationVariableType::Integer}},
//			{ AnimationVariable::bFootLeftUnlock, { "bFootLeftUnlock", AnimationVariableType::Integer}},
//			{ AnimationVariable::bFootRightLock, { "bFootRightLock", AnimationVariableType::Integer}},
//			{ AnimationVariable::bFootRightUnlock, { "bFootRightUnlock", AnimationVariableType::Integer}},
//			{ AnimationVariable::bFreeMovement, { "bFreeMovement", AnimationVariableType::Integer}},
//			{ AnimationVariable::bHasAnimObjectVisible, { "bHasAnimObjectVisible", AnimationVariableType::Integer}},
//			{ AnimationVariable::bHeadLightIsVisible, { "bHeadLightIsVisible", AnimationVariableType::Integer}},
//			{ AnimationVariable::bHeadTrackingActive, { "bHeadTrackingActive", AnimationVariableType::Boolean}},
//			{ AnimationVariable::bHelmetIsVisible, { "bHelmetIsVisible", AnimationVariableType::Integer}},
//			{ AnimationVariable::bHelmetLightIsOn, { "bHelmetLightIsOn", AnimationVariableType::Boolean}},
//			{ AnimationVariable::bInitializeInstant, { "bInitializeInstant", AnimationVariableType::Integer}},
//			{ AnimationVariable::bInRelaxedPose, { "bInRelaxedPose", AnimationVariableType::Integer}},
//			{ AnimationVariable::bInReloadState, { "bInReloadState", AnimationVariableType::Integer}},
//			{ AnimationVariable::bInReloadStateWithoutSightMode, { "bInReloadStateWithoutSightMode", AnimationVariableType::Integer}},
//			{ AnimationVariable::bIsAttacking, { "bIsAttacking", AnimationVariableType::Integer}},
//			{ AnimationVariable::bIsBleedingOut, { "bIsBleedingOut", AnimationVariableType::Integer}},
//			{ AnimationVariable::bIsBlocking, { "bIsBlocking", AnimationVariableType::Integer}},
//			{ AnimationVariable::bIsEnteringCover, { "bIsEnteringCover", AnimationVariableType::Integer}},
//			{ AnimationVariable::bIsEquipping, { "bIsEquipping", AnimationVariableType::Integer}},
//			{ AnimationVariable::bIsExitingCover, { "bIsExitingCover", AnimationVariableType::Integer}},
//			{ AnimationVariable::bIsFirstPerson, { "bIsFirstPerson", AnimationVariableType::Integer}},
//			{ AnimationVariable::bIsFurnitureExit, { "bIsFurnitureExit", AnimationVariableType::Integer}},
//			{ AnimationVariable::bIsGrazing, { "bIsGrazing", AnimationVariableType::Integer}},
//			{ AnimationVariable::bIsHiding, { "bIsHiding", AnimationVariableType::Integer}},
//			{ AnimationVariable::bIsInAir, { "bIsInAir", AnimationVariableType::Integer}},
//			{ AnimationVariable::bIsInCamera, { "bIsInCamera", AnimationVariableType::Boolean}},
//			{ AnimationVariable::bIsInCover, { "bIsInCover", AnimationVariableType::Integer}},
//			{ AnimationVariable::bIsInLargeFlavor, { "bIsInLargeFlavor", AnimationVariableType::Integer}},
//			{ AnimationVariable::bIsInScene, { "bIsInScene", AnimationVariableType::Integer}},
//			{ AnimationVariable::bIsLanding, { "bIsLanding", AnimationVariableType::Integer}},
//			{ AnimationVariable::bIsMantling, { "bIsMantling", AnimationVariableType::Integer}},
//			{ AnimationVariable::bIsMelee, { "bIsMelee", AnimationVariableType::Integer}},
//			{ AnimationVariable::bIsMirrored, { "bIsMirrored", AnimationVariableType::Integer}},
//			{ AnimationVariable::bIsNotTalking, { "bIsNotTalking", AnimationVariableType::Integer}},
//			{ AnimationVariable::bIsPathing, { "bIsPathing", AnimationVariableType::Boolean}},
//			{ AnimationVariable::bIsPlayerShip, { "bIsPlayerShip", AnimationVariableType::Boolean}},
//			{ AnimationVariable::bIsReloading, { "bIsReloading", AnimationVariableType::Integer}},
//			{ AnimationVariable::bIsShuffling, { "bIsShuffling", AnimationVariableType::Integer}},
//			{ AnimationVariable::bIsSliding, { "bIsSliding", AnimationVariableType::Integer}},
//			{ AnimationVariable::bIsStaggered, { "bIsStaggered", AnimationVariableType::Integer}},
//			{ AnimationVariable::bIsTouchingGround, { "bIsTouchingGround", AnimationVariableType::Integer}},
//			{ AnimationVariable::bIsTunneling, { "bIsTunneling", AnimationVariableType::Integer}},
//			{ AnimationVariable::bIsUnequipping, { "bIsUnequipping", AnimationVariableType::Integer}},
//			{ AnimationVariable::bLadderLeftHandUp, { "bLadderLeftHandUp", AnimationVariableType::Integer}},
//			{ AnimationVariable::bLandInPlace, { "bLandInPlace", AnimationVariableType::Integer}},
//			{ AnimationVariable::bLeftHandCasting, { "bLeftHandCasting", AnimationVariableType::Integer}},
//			{ AnimationVariable::bManualAnimObjUnequip, { "bManualAnimObjUnequip", AnimationVariableType::Integer}},
//			{ AnimationVariable::bNoAdditivePose, { "bNoAdditivePose", AnimationVariableType::Integer}},
//			{ AnimationVariable::bNoAim, { "bNoAim", AnimationVariableType::Integer}},
//			{ AnimationVariable::bNoAlertAdditive, { "bNoAlertAdditive", AnimationVariableType::Integer}},
//			{ AnimationVariable::bNoArmorOffset, { "bNoArmorOffset", AnimationVariableType::Integer}},
//			{ AnimationVariable::bNoFeatherSpine, { "bNoFeatherSpine", AnimationVariableType::Integer}},
//			{ AnimationVariable::bNoFlavor, { "bNoFlavor", AnimationVariableType::Integer}},
//			{ AnimationVariable::bNoFootIK, { "bNoFootIK", AnimationVariableType::Integer}},
//			{ AnimationVariable::bNoHeadTrack, { "bNoHeadTrack", AnimationVariableType::Integer}},
//			{ AnimationVariable::bNoIdleBack, { "bNoIdleBack", AnimationVariableType::Integer}},
//			{ AnimationVariable::bNoIdleFace, { "bNoIdleFace", AnimationVariableType::Integer}},
//			{ AnimationVariable::bNoLeftHandIK, { "bNoLeftHandIK", AnimationVariableType::Integer}},
//			{ AnimationVariable::bNoRightHandIK, { "bNoRightHandIK", AnimationVariableType::Integer}},
//			{ AnimationVariable::Body_Length, 	{ "Body Length", AnimationVariableType::Float}},
//			{ AnimationVariable::BoneWeightsAction, { "BoneWeightsAction", AnimationVariableType::Integer}},
//			{ AnimationVariable::bPairedLead, { "bPairedLead", AnimationVariableType::Boolean}},
//			{ AnimationVariable::bPartialBodyInstant, { "bPartialBodyInstant", AnimationVariableType::Integer}},
//			{ AnimationVariable::bPlayerMoveStartActive, { "bPlayerMoveStartActive", AnimationVariableType::Integer}},
//			{ AnimationVariable::bPlayReloadEnd, { "bPlayReloadEnd", AnimationVariableType::Integer}},
//			{ AnimationVariable::bReloadAborted, { "bReloadAborted", AnimationVariableType::Integer}},
//			{ AnimationVariable::bRetainEquipment, { "bRetainEquipment", AnimationVariableType::Integer}},
//			{ AnimationVariable::bShipPitchDown, { "bShipPitchDown", AnimationVariableType::Integer}},
//			{ AnimationVariable::bShipPitchUp, { "bShipPitchUp", AnimationVariableType::Integer}},
//			{ AnimationVariable::bShipRoll, { "bShipRoll", AnimationVariableType::Integer}},
//			{ AnimationVariable::bShipStrafeDown, { "bShipStrafeDown", AnimationVariableType::Integer}},
//			{ AnimationVariable::bShipStrafeLeft, { "bShipStrafeLeft", AnimationVariableType::Integer}},
//			{ AnimationVariable::bShipStrafeLR, { "bShipStrafeLR", AnimationVariableType::Integer}},
//			{ AnimationVariable::bShipStrafeRight, { "bShipStrafeRight", AnimationVariableType::Integer}},
//			{ AnimationVariable::bShipStrafeUD, { "bShipStrafeUD", AnimationVariableType::Integer}},
//			{ AnimationVariable::bShipStrafeUp, { "bShipStrafeUp", AnimationVariableType::Integer}},
//			{ AnimationVariable::bShipYawLeft, { "bShipYawLeft", AnimationVariableType::Integer}},
//			{ AnimationVariable::bShipYawRight, { "bShipYawRight", AnimationVariableType::Integer}},
//			{ AnimationVariable::bTailAttack, { "bTailAttack", AnimationVariableType::Integer}},
//			{ AnimationVariable::bUseSequentialReloading, { "bUseSequentialReloading", AnimationVariableType::Integer}},
//			{ AnimationVariable::bUseSoaring, { "bUseSoaring", AnimationVariableType::Integer}},
//			{ AnimationVariable::bUseStandToExit, { "bUseStandToExit", AnimationVariableType::Integer}},
//			{ AnimationVariable::bWeaponLeftToLAnimObject, { "bWeaponLeftToLAnimObject", AnimationVariableType::Integer}},
//			{ AnimationVariable::bWeaponLeftToRAnimObject, { "bWeaponLeftToRAnimObject", AnimationVariableType::Integer}},
//			{ AnimationVariable::bWeaponLeftToRAnimObject3, { "bWeaponLeftToRAnimObject3", AnimationVariableType::Integer}},
//			{ AnimationVariable::bWeaponToRAnimObject, { "bWeaponToRAnimObject", AnimationVariableType::Integer}},
//			{ AnimationVariable::bZeroGSpine, { "bZeroGSpine", AnimationVariableType::Integer}},
//			{ AnimationVariable::CameraFOV, { "CameraFOV", AnimationVariableType::Float}},
//			{ AnimationVariable::CameraNearFar, { "CameraNearFar", AnimationVariableType::Integer}},
//			{ AnimationVariable::CameraYawSpeed, { "CameraYawSpeed", AnimationVariableType::Float}},
//			{ AnimationVariable::CanEnableGunFailsafe, { "CanEnableGunFailsafe", AnimationVariableType::Integer}},
//			{ AnimationVariable::CanStaySightedWhileReloading, { "CanStaySightedWhileReloading", AnimationVariableType::Integer}},
//			{ AnimationVariable::Capacity, { "Capacity", AnimationVariableType::Float}},
//			{ AnimationVariable::CapacityPercent, { "CapacityPercent", AnimationVariableType::Float}},
//			{ AnimationVariable::ChestHeadingMax, { "ChestHeadingMax", AnimationVariableType::Float}},
//			{ AnimationVariable::ChestHeadingMin, { "ChestHeadingMin", AnimationVariableType::Float}},
//			{ AnimationVariable::cHitReactionBodyPart, { "cHitReactionBodyPart", AnimationVariableType::Integer}},
//			{ AnimationVariable::cHitReactionDir, { "cHitReactionDir", AnimationVariableType::Integer}},
//			{ AnimationVariable::CumulativeAnimationsCount, { "CumulativeAnimationsCount", AnimationVariableType::Integer}},
//			{ AnimationVariable::CurrentAmount, { "CurrentAmount", AnimationVariableType::Float}},
//			{ AnimationVariable::CurrentAperture, { "CurrentAperture", AnimationVariableType::Float}},
//			{ AnimationVariable::CurrentGraphState, { "CurrentGraphState", AnimationVariableType::Float}},
//			{ AnimationVariable::CurrentPosition, { "CurrentPosition", AnimationVariableType::Float}},
//			{ AnimationVariable::CurrentRotation, { "CurrentRotation", AnimationVariableType::Float}},
//			{ AnimationVariable::CurrentSpeed, { "CurrentSpeed", AnimationVariableType::Float}},
//			{ AnimationVariable::DesiredLocation, { "DesiredLocation", AnimationVariableType::Vector}},
//			{ AnimationVariable::DirectAt_Heading_Delta, { "DirectAt Heading Delta", AnimationVariableType::Float}},
//			{ AnimationVariable::DirectAtHeadingMax, { "DirectAtHeadingMax", AnimationVariableType::Float}},
//			{ AnimationVariable::DirectAtHeadingMin, { "DirectAtHeadingMin", AnimationVariableType::Float}},
//			{ AnimationVariable::DirectAtHeadingVar, { "DirectAtHeadingVar", AnimationVariableType::Float}},
//			{ AnimationVariable::DirectAtPitchMax, { "DirectAtPitchMax", AnimationVariableType::Float}},
//			{ AnimationVariable::DirectAtPitchMin, { "DirectAtPitchMin", AnimationVariableType::Float}},
//			{ AnimationVariable::DirectAtPitchVar, { "DirectAtPitchVar", AnimationVariableType::Float}},
//			{ AnimationVariable::Direction, { "Direction", AnimationVariableType::Float}},
//			{ AnimationVariable::DirectionSmoothed, { "DirectionSmoothed", AnimationVariableType::Float}},
//			{ AnimationVariable::DisableAllowRotation, { "DisableAllowRotation", AnimationVariableType::Integer}},
//			{ AnimationVariable::DisableAnimationDriven, { "DisableAnimationDriven", AnimationVariableType::Integer}},
//			{ AnimationVariable::DisableHeadingMax, { "DisableHeadingMax", AnimationVariableType::Float}},
//			{ AnimationVariable::DisableHeadingMin, { "DisableHeadingMin", AnimationVariableType::Float}},
//			{ AnimationVariable::DistanceToStop, { "DistanceToStop", AnimationVariableType::Float}},
//			{ AnimationVariable::DrawSpeedMult, { "DrawSpeedMult", AnimationVariableType::Float}},
//			{ AnimationVariable::fActorScale, { "fActorScale", AnimationVariableType::Float}},
//			{ AnimationVariable::fActorScaleInverse, { "fActorScaleInverse", AnimationVariableType::Float}},
//			{ AnimationVariable::fAmmoCountAsTimePercent, { "fAmmoCountAsTimePercent", AnimationVariableType::Float}},
//			{ AnimationVariable::fAmmoCountPercentage, { "fAmmoCountPercentage", AnimationVariableType::Float}},
//			{ AnimationVariable::fAmmoRemaining, { "fAmmoRemaining", AnimationVariableType::Float}},
//			{ AnimationVariable::fBlendTime_Combat, { "fBlendTime_Combat", AnimationVariableType::Float}},
//			{ AnimationVariable::fBlendTime_Default, { "fBlendTime_Default", AnimationVariableType::Float}},
//			{ AnimationVariable::fBlendTime_IdleLocomotionSwitch, { "fBlendTime_IdleLocomotionSwitch", AnimationVariableType::Float}},
//			{ AnimationVariable::fCameraPitch, { "fCameraPitch", AnimationVariableType::Float}},
//			{ AnimationVariable::fCameraYaw, { "fCameraYaw", AnimationVariableType::Float}},
//			{ AnimationVariable::fChargePercentage, { "fChargePercentage", AnimationVariableType::Float}},
//			{ AnimationVariable::fCockpitBoost, { "fCockpitBoost", AnimationVariableType::Float}},
//			{ AnimationVariable::fCockpitBrakeThrusters, { "fCockpitBrakeThrusters", AnimationVariableType::Float}},
//			{ AnimationVariable::fCockpitEngineState, { "fCockpitEngineState", AnimationVariableType::Float}},
//			{ AnimationVariable::fCockpitGForce, { "fCockpitGForce", AnimationVariableType::Float}},
//			{ AnimationVariable::fCockpitGravDriveState, { "fCockpitGravDriveState", AnimationVariableType::Float}},
//			{ AnimationVariable::fCockpitHorizontalInput, { "fCockpitHorizontalInput", AnimationVariableType::Float}},
//			{ AnimationVariable::fCockpitHullIntegrityState, { "fCockpitHullIntegrityState", AnimationVariableType::Float}},
//			{ AnimationVariable::fCockpitPitch, { "fCockpitPitch", AnimationVariableType::Float}},
//			{ AnimationVariable::fCockpitReactorState, { "fCockpitReactorState", AnimationVariableType::Float}},
//			{ AnimationVariable::fCockpitRoll, { "fCockpitRoll", AnimationVariableType::Float}},
//			{ AnimationVariable::fCockpitShieldState, { "fCockpitShieldState", AnimationVariableType::Float}},
//			{ AnimationVariable::fCockpitSpeed, { "fCockpitSpeed", AnimationVariableType::Float}},
//			{ AnimationVariable::fCockpitTargetLock, { "fCockpitTargetLock", AnimationVariableType::Float}},
//			{ AnimationVariable::fCockpitThrottle, { "fCockpitThrottle", AnimationVariableType::Float}},
//			{ AnimationVariable::fCockpitThrottleInput, { "fCockpitThrottleInput", AnimationVariableType::Float}},
//			{ AnimationVariable::fCockpitThrottleReverse, { "fCockpitThrottleReverse", AnimationVariableType::Float}},
//			{ AnimationVariable::fCockpitThrusters, { "fCockpitThrusters", AnimationVariableType::Float}},
//			{ AnimationVariable::fCockpitVerticalInput, { "fCockpitVerticalInput", AnimationVariableType::Float}},
//			{ AnimationVariable::fCockpitYaw, { "fCockpitYaw", AnimationVariableType::Float}},
//			{ AnimationVariable::fControllerMoveXDampen, { "fControllerMoveXDampen", AnimationVariableType::Float}},
//			{ AnimationVariable::fControllerMoveXRaw, { "fControllerMoveXRaw", AnimationVariableType::Float}},
//			{ AnimationVariable::fControllerMoveYRaw, { "fControllerMoveYRaw", AnimationVariableType::Float}},
//			{ AnimationVariable::fControllerMoveZRaw, { "fControllerMoveZRaw", AnimationVariableType::Float}},
//			{ AnimationVariable::fControllerXRaw, { "fControllerXRaw", AnimationVariableType::Float}},
//			{ AnimationVariable::fControllerXRawDampen, { "fControllerXRawDampen", AnimationVariableType::Float}},
//			{ AnimationVariable::fControllerYRaw, { "fControllerYRaw", AnimationVariableType::Float}},
//			{ AnimationVariable::fControllerYRawDampen, { "fControllerYRawDampen", AnimationVariableType::Float}},
//			{ AnimationVariable::fDesiredHeading, { "fDesiredHeading", AnimationVariableType::Float}},
//			{ AnimationVariable::fFastWalkSpeed, { "fFastWalkSpeed", AnimationVariableType::Float}},
//			{ AnimationVariable::fFlightHorizontalInput, { "fFlightHorizontalInput", AnimationVariableType::Float}},
//			{ AnimationVariable::fFlightMaxSpeed, { "fFlightMaxSpeed", AnimationVariableType::Float}},
//			{ AnimationVariable::fFlightPitchInput, { "fFlightPitchInput", AnimationVariableType::Float}},
//			{ AnimationVariable::fFlightRollInput, { "fFlightRollInput", AnimationVariableType::Float}},
//			{ AnimationVariable::fFlightThrottleInput, { "fFlightThrottleInput", AnimationVariableType::Float}},
//			{ AnimationVariable::fFlightVerticalInput, { "fFlightVerticalInput", AnimationVariableType::Float}},
//			{ AnimationVariable::fFlightYawInput, { "fFlightYawInput", AnimationVariableType::Float}},
//			{ AnimationVariable::fFXgeneric01, { "fFXgeneric01", AnimationVariableType::Float}},
//			{ AnimationVariable::fFXgeneric02, { "fFXgeneric02", AnimationVariableType::Float}},
//			{ AnimationVariable::fFXgeneric03, { "fFXgeneric03", AnimationVariableType::Float}},
//			{ AnimationVariable::fGravityScale, { "fGravityScale", AnimationVariableType::Float}},
//			{ AnimationVariable::fGunDownToGunUpDuration, { "fGunDownToGunUpDuration", AnimationVariableType::Float}},
//			{ AnimationVariable::fGunUpToGunDownDuration, { "fGunUpToGunDownDuration", AnimationVariableType::Float}},
//			{ AnimationVariable::fHeadingSpeed, { "fHeadingSpeed", AnimationVariableType::Float}},
//			{ AnimationVariable::fHintVisibility, { "fHintVisibility", AnimationVariableType::Float}},
//			{ AnimationVariable::fHipRelativeMotion, { "fHipRelativeMotion", AnimationVariableType::Float}},
//			{ AnimationVariable::fImpactDist, { "fImpactDist", AnimationVariableType::Float}},
//			{ AnimationVariable::fInventoryHeading, { "fInventoryHeading", AnimationVariableType::Float}},
//			{ AnimationVariable::fMeterCount, { "fMeterCount", AnimationVariableType::Float}},
//			{ AnimationVariable::fMeterCountPercentage, { "fMeterCountPercentage", AnimationVariableType::Float}},
//			{ AnimationVariable::fPitchSpeed, { "fPitchSpeed", AnimationVariableType::Float}},
//			{ AnimationVariable::fPowerIntensity, { "fPowerIntensity", AnimationVariableType::Float}},
//			{ AnimationVariable::fProjectilePower, { "fProjectilePower", AnimationVariableType::Float}},
//			{ AnimationVariable::fRandomClipStartTimePercentage, { "fRandomClipStartTimePercentage", AnimationVariableType::Float}},
//			{ AnimationVariable::fRingControl, { "fRingControl", AnimationVariableType::Float}},
//			{ AnimationVariable::fRingSpeed, { "fRingSpeed", AnimationVariableType::Float}},
//			{ AnimationVariable::fRingSpeedFast, { "fRingSpeedFast", AnimationVariableType::Float}},
//			{ AnimationVariable::fRingSpeedMedium, { "fRingSpeedMedium", AnimationVariableType::Float}},
//			{ AnimationVariable::fRingSpeedSlow, { "fRingSpeedSlow", AnimationVariableType::Float}},
//			{ AnimationVariable::fRingSpin, { "fRingSpin", AnimationVariableType::Float}},
//			{ AnimationVariable::fRingSuccessSpeed, { "fRingSuccessSpeed", AnimationVariableType::Float}},
//			{ AnimationVariable::fRingTarget, { "fRingTarget", AnimationVariableType::Float}},
//			{ AnimationVariable::fShipQuadrant, { "fShipQuadrant", AnimationVariableType::Float}},
//			{ AnimationVariable::fThrottleDamp, { "fThrottleDamp", AnimationVariableType::Float}},
//			{ AnimationVariable::fTurretHeadingMax, { "fTurretHeadingMax", AnimationVariableType::Float}},
//			{ AnimationVariable::fTurretHeadingMin, { "fTurretHeadingMin", AnimationVariableType::Float}},
//			{ AnimationVariable::fTurretPitchMax, { "fTurretPitchMax", AnimationVariableType::Float}},
//			{ AnimationVariable::fTurretPitchMin, { "fTurretPitchMin", AnimationVariableType::Float}},
//			{ AnimationVariable::fVariableRangeAperture, { "fVariableRangeAperture", AnimationVariableType::Float}},
//			{ AnimationVariable::fVariableRangeDistance, { "fVariableRangeDistance", AnimationVariableType::Float}},
//			{ AnimationVariable::GripOffset, { "GripOffset", AnimationVariableType::Float}},
//			{ AnimationVariable::HasPower, { "HasPower", AnimationVariableType::Integer}},
//			{ AnimationVariable::HeadHeadingMax, { "HeadHeadingMax", AnimationVariableType::Float}},
//			{ AnimationVariable::HeadHeadingMin, { "HeadHeadingMin", AnimationVariableType::Float}},
//			{ AnimationVariable::HitReactionTransferVariable, { "HitReactionTransferVariable", AnimationVariableType::Integer}},
//			{ AnimationVariable::iAmmoCount, { "iAmmoCount", AnimationVariableType::Integer}},
//			{ AnimationVariable::iAttackState, { "iAttackState", AnimationVariableType::Integer}},
//			{ AnimationVariable::iAttackState0, { "iAttackState0", AnimationVariableType::Integer}},
//			{ AnimationVariable::iAttackState1, { "iAttackState1", AnimationVariableType::Integer}},
//			{ AnimationVariable::iAttackState2, { "iAttackState2", AnimationVariableType::Integer}},
//			{ AnimationVariable::iAttackState3, { "iAttackState3", AnimationVariableType::Integer}},
//			{ AnimationVariable::iAttackState4, { "iAttackState4", AnimationVariableType::Integer}},
//			{ AnimationVariable::iAttackState5, { "iAttackState5", AnimationVariableType::Integer}},
//			{ AnimationVariable::iAttackState6, { "iAttackState6", AnimationVariableType::Integer}},
//			{ AnimationVariable::iAttackState7, { "iAttackState7", AnimationVariableType::Integer}},
//			{ AnimationVariable::iCriticalHitType, { "iCriticalHitType", AnimationVariableType::Integer}},
//			{ AnimationVariable::iIsBlindFiring, { "iIsBlindFiring", AnimationVariableType::Integer}},
//			{ AnimationVariable::iIsGunDown, { "iIsGunDown", AnimationVariableType::Integer}},
//			{ AnimationVariable::iIsInSneak, { "iIsInSneak", AnimationVariableType::Integer}},
//			{ AnimationVariable::iIsPlayer, { "iIsPlayer", AnimationVariableType::Integer}},
//			{ AnimationVariable::iIsPoseB, { "iIsPoseB", AnimationVariableType::Integer}},
//			{ AnimationVariable::iIsSighted, { "iIsSighted", AnimationVariableType::Integer}},
//			{ AnimationVariable::iLadderClimbState, { "iLadderClimbState", AnimationVariableType::Integer}},
//			{ AnimationVariable::iMagazineCapacity, { "iMagazineCapacity", AnimationVariableType::Integer}},
//			{ AnimationVariable::iPartialCover, { "iPartialCover", AnimationVariableType::Integer}},
//			{ AnimationVariable::iPlayerLadderClimbAnimation, { "iPlayerLadderClimbAnimation", AnimationVariableType::Integer}},
//			{ AnimationVariable::iPlayingSprintAnimation, { "iPlayingSprintAnimation", AnimationVariableType::Integer}},
//			{ AnimationVariable::iRigIndex, { "iRigIndex", AnimationVariableType::Integer}},
//			{ AnimationVariable::iRootSwitch, { "iRootSwitch", AnimationVariableType::Integer}},
//			{ AnimationVariable::IsCompletelyStopped, { "IsCompletelyStopped", AnimationVariableType::Integer}},
//			{ AnimationVariable::iSightedRequested, { "iSightedRequested", AnimationVariableType::Integer}},
//			{ AnimationVariable::IsInstantInit, { "IsInstantInit", AnimationVariableType::Boolean}},
//			{ AnimationVariable::IsNotAligned, { "IsNotAligned", AnimationVariableType::Integer}},
//			{ AnimationVariable::IsOn, { "IsOn", AnimationVariableType::Integer}},
//			{ AnimationVariable::iStaggerType, { "iStaggerType", AnimationVariableType::Integer}},
//			{ AnimationVariable::iState, { "iState", AnimationVariableType::Integer}},
//			{ AnimationVariable::IsUsingCodeDrivenRotation, { "IsUsingCodeDrivenRotation", AnimationVariableType::Boolean}},
//			{ AnimationVariable::iSwitchingPerspectives, { "iSwitchingPerspectives", AnimationVariableType::Integer}},
//			{ AnimationVariable::iSyncActions, { "iSyncActions", AnimationVariableType::Integer}},
//			{ AnimationVariable::iSyncActorStance, { "iSyncActorStance", AnimationVariableType::Integer}},
//			{ AnimationVariable::iSyncBleedout, { "iSyncBleedout", AnimationVariableType::Integer}},
//			{ AnimationVariable::iSyncCoverStates, { "iSyncCoverStates", AnimationVariableType::Integer}},
//			{ AnimationVariable::iSyncFullbodyActions, { "iSyncFullbodyActions", AnimationVariableType::Integer}},
//			{ AnimationVariable::iSyncGravDash, { "iSyncGravDash", AnimationVariableType::Integer}},
//			{ AnimationVariable::iSyncGravity, { "iSyncGravity", AnimationVariableType::Integer}},
//			{ AnimationVariable::iSyncGunActions, { "iSyncGunActions", AnimationVariableType::Integer}},
//			{ AnimationVariable::iSyncIdleLocomotion, { "iSyncIdleLocomotion", AnimationVariableType::Integer}},
//			{ AnimationVariable::iSyncIdleTurnState, { "iSyncIdleTurnState", AnimationVariableType::Integer}},
//			{ AnimationVariable::iSyncIdleWalkRun, { "iSyncIdleWalkRun", AnimationVariableType::Integer}},
//			{ AnimationVariable::iSyncInDialogueScene, { "iSyncInDialogueScene", AnimationVariableType::Integer}},
//			{ AnimationVariable::iSyncInjured, { "iSyncInjured", AnimationVariableType::Integer}},
//			{ AnimationVariable::iSyncInventoryCamera, { "iSyncInventoryCamera", AnimationVariableType::Integer}},
//			{ AnimationVariable::iSyncJumpState, { "iSyncJumpState", AnimationVariableType::Integer}},
//			{ AnimationVariable::iSyncMeleeActions, { "iSyncMeleeActions", AnimationVariableType::Integer}},
//			{ AnimationVariable::iSyncMoveDirection, { "iSyncMoveDirection", AnimationVariableType::Integer}},
//			{ AnimationVariable::iSyncOnGround, { "iSyncOnGround", AnimationVariableType::Integer}},
//			{ AnimationVariable::iSyncOpenClose, { "iSyncOpenClose", AnimationVariableType::Integer}},
//			{ AnimationVariable::iSyncPartialBody, { "iSyncPartialBody", AnimationVariableType::Integer}},
//			{ AnimationVariable::iSyncPartialBodyBlend, { "iSyncPartialBodyBlend", AnimationVariableType::Integer}},
//			{ AnimationVariable::iSyncPartialBodyState, { "iSyncPartialBodyState", AnimationVariableType::Integer}},
//			{ AnimationVariable::iSyncPose, { "iSyncPose", AnimationVariableType::Integer}},
//			{ AnimationVariable::iSyncPower, { "iSyncPower", AnimationVariableType::Integer}},
//			{ AnimationVariable::iSyncRelaxedReadyAlert, { "iSyncRelaxedReadyAlert", AnimationVariableType::Integer}},
//			{ AnimationVariable::iSyncRelaxReadySneak, { "iSyncRelaxReadySneak", AnimationVariableType::Integer}},
//			{ AnimationVariable::iSyncRightLeft, { "iSyncRightLeft", AnimationVariableType::Integer}},
//			{ AnimationVariable::iSyncSighted, { "iSyncSighted", AnimationVariableType::Integer}},
//			{ AnimationVariable::iSyncSprintState, { "iSyncSprintState", AnimationVariableType::Integer}},
//			{ AnimationVariable::iSyncStandingCrouching, { "iSyncStandingCrouching", AnimationVariableType::Integer}},
//			{ AnimationVariable::iSyncStateAB, { "iSyncStateAB", AnimationVariableType::Integer}},
//			{ AnimationVariable::iSyncStunState, { "iSyncStunState", AnimationVariableType::Integer}},
//			{ AnimationVariable::iSyncSwimState, { "iSyncSwimState", AnimationVariableType::Integer}},
//			{ AnimationVariable::iSyncTransitions, { "iSyncTransitions", AnimationVariableType::Integer}},
//			{ AnimationVariable::iSyncTurnLeft, { "iSyncTurnLeft", AnimationVariableType::Integer}},
//			{ AnimationVariable::iSyncTurnRight, { "iSyncTurnRight", AnimationVariableType::Integer}},
//			{ AnimationVariable::iSyncTurnState, { "iSyncTurnState", AnimationVariableType::Integer}},
//			{ AnimationVariable::iTurnType, { "iTurnType", AnimationVariableType::Integer}},
//			{ AnimationVariable::iWantBlock, { "iWantBlock", AnimationVariableType::Integer}},
//			{ AnimationVariable::LadderStepDesiredLocation, { "LadderStepDesiredLocation", AnimationVariableType::Vector}},
//			{ AnimationVariable::LockAlignementToCameraDuringTurnAnimation, { "LockAlignementToCameraDuringTurnAnimation", AnimationVariableType::Float}},
//			{ AnimationVariable::LookAtDir, { "LookAtDir", AnimationVariableType::Vector}},
//			{ AnimationVariable::LookAtHeadingMax, { "LookAtHeadingMax", AnimationVariableType::Float}},
//			{ AnimationVariable::LookAtHeadingMin, { "LookAtHeadingMin", AnimationVariableType::Float}},
//			{ AnimationVariable::LookAtHeadingVar, { "LookAtHeadingVar", AnimationVariableType::Float}},
//			{ AnimationVariable::LookAtLocation, { "LookAtLocation", AnimationVariableType::Vector}},
//			{ AnimationVariable::LookAtPitchMax, { "LookAtPitchMax", AnimationVariableType::Float}},
//			{ AnimationVariable::LookAtPitchMin, { "LookAtPitchMin", AnimationVariableType::Float}},
//			{ AnimationVariable::LookAtPitchVar, { "LookAtPitchVar", AnimationVariableType::Float}},
//			{ AnimationVariable::LookHeading, { "LookHeading", AnimationVariableType::Float}},
//			{ AnimationVariable::LookHeadingOffset, { "LookHeadingOffset", AnimationVariableType::Float}},
//			{ AnimationVariable::MantleEndPos, { "MantleEndPos", AnimationVariableType::Vector}},
//			{ AnimationVariable::MantleHeadingOffset, { "MantleHeadingOffset", AnimationVariableType::Float}},
//			{ AnimationVariable::MantleTogether, { "MantleTogether", AnimationVariableType::Boolean}},
//			{ AnimationVariable::MeleeAttack_Bite02, { "MeleeAttack_Bite02", AnimationVariableType::Float}},
//			{ AnimationVariable::NeedPower, { "NeedPower", AnimationVariableType::Integer}},
//			{ AnimationVariable::NPCHeading, { "NPCHeading", AnimationVariableType::Float}},
//			{ AnimationVariable::NPCHeadingOffset, { "NPCHeadingOffset", AnimationVariableType::Float}},
//			{ AnimationVariable::OpenCloseFormState, { "OpenCloseFormState", AnimationVariableType::Float}},
//			{ AnimationVariable::PairedAnimHash, { "PairedAnimHash", AnimationVariableType::Integer}},
//			{ AnimationVariable::PairedHeadingOffset, { "PairedHeadingOffset", AnimationVariableType::Float}},
//			{ AnimationVariable::PairedTranslationOffset, { "PairedTranslationOffset", AnimationVariableType::Vector}},
//			{ AnimationVariable::Pitch, { "Pitch", AnimationVariableType::Float}},
//			{ AnimationVariable::PitchDelta, { "PitchDelta", AnimationVariableType::Float}},
//			{ AnimationVariable::PitchOffset, { "PitchOffset", AnimationVariableType::Float}},
//			{ AnimationVariable::PlaybackSpeedMult, { "PlaybackSpeedMult", AnimationVariableType::Float}},
//			{ AnimationVariable::PlayPartialBodyStart, { "PlayPartialBodyStart", AnimationVariableType::Boolean}},
//			{ AnimationVariable::Position, { "Position", AnimationVariableType::Float}},
//			{ AnimationVariable::ReloadSpeedMult, { "ReloadSpeedMult", AnimationVariableType::Float}},
//			{ AnimationVariable::Rotation, { "Rotation", AnimationVariableType::Float}},
//			{ AnimationVariable::RotationSpeed, { "RotationSpeed", AnimationVariableType::Float}},
//			{ AnimationVariable::SetColorA, { "SetColorA", AnimationVariableType::Integer}},
//			{ AnimationVariable::SetColorB, { "SetColorB", AnimationVariableType::Integer}},
//			{ AnimationVariable::SetStateA, { "SetStateA", AnimationVariableType::Integer}},
//			{ AnimationVariable::SetStateB, { "SetStateB", AnimationVariableType::Integer}},
//			{ AnimationVariable::ShipPitchDown, { "ShipPitchDown", AnimationVariableType::Float}},
//			{ AnimationVariable::ShipPitchUp, { "ShipPitchUp", AnimationVariableType::Float}},
//			{ AnimationVariable::ShipRoll, { "ShipRoll", AnimationVariableType::Float}},
//			{ AnimationVariable::ShipStrafeDown, { "ShipStrafeDown", AnimationVariableType::Float}},
//			{ AnimationVariable::ShipStrafeLeft, { "ShipStrafeLeft", AnimationVariableType::Float}},
//			{ AnimationVariable::ShipStrafeRight, { "ShipStrafeRight", AnimationVariableType::Float}},
//			{ AnimationVariable::ShipStrafeUp, { "ShipStrafeUp", AnimationVariableType::Float}},
//			{ AnimationVariable::ShipYawLeft, { "ShipYawLeft", AnimationVariableType::Float}},
//			{ AnimationVariable::ShipYawRight, { "ShipYawRight", AnimationVariableType::Float}},
//			{ AnimationVariable::ShouldUseTurnOnSpotGraph, { "ShouldUseTurnOnSpotGraph", AnimationVariableType::Boolean}},
//			{ AnimationVariable::SightedSpeedMult, { "SightedSpeedMult", AnimationVariableType::Float}},
//			{ AnimationVariable::SightedTransitionSec, { "SightedTransitionSec", AnimationVariableType::Float}},
//			{ AnimationVariable::Speed, { "Speed", AnimationVariableType::Float}},
//			{ AnimationVariable::StaggerDirection, { "StaggerDirection", AnimationVariableType::Float}},
//			{ AnimationVariable::StaggerMagnitude, { "StaggerMagnitude", AnimationVariableType::Integer}},
//			{ AnimationVariable::StartAltIdle, { "StartAltIdle", AnimationVariableType::Float}},
//			{ AnimationVariable::StartsOff, { "StartsOff", AnimationVariableType::Float}},
//			{ AnimationVariable::State01Index, { "State01Index", AnimationVariableType::Integer}},
//			{ AnimationVariable::State02Index, { "State02Index", AnimationVariableType::Integer}},
//			{ AnimationVariable::StateA01Index, { "StateA01Index", AnimationVariableType::Integer}},
//			{ AnimationVariable::StateA02Index, { "StateA02Index", AnimationVariableType::Integer}},
//			{ AnimationVariable::StateB01Index, { "StateB01Index", AnimationVariableType::Integer}},
//			{ AnimationVariable::StateB02Index, { "StateB02Index", AnimationVariableType::Integer}},
//			{ AnimationVariable::SyncJumpBlend, { "SyncJumpBlend", AnimationVariableType::Integer}},
//			{ AnimationVariable::TurnDelta, { "TurnDelta", AnimationVariableType::Float}},
//			{ AnimationVariable::TurnDeltaSmoothed, { "TurnDeltaSmoothed", AnimationVariableType::Float}},
//			{ AnimationVariable::TurnOnSpotEnforceDirectAtLimits, { "TurnOnSpotEnforceDirectAtLimits", AnimationVariableType::Boolean}},
//			{ AnimationVariable::UnequipInterruptable, { "UnequipInterruptable", AnimationVariableType::Integer}},
//			{ AnimationVariable::VehicleAirTime, { "VehicleAirTime", AnimationVariableType::Float}},
//			{ AnimationVariable::VehicleForwardVelocity, { "VehicleForwardVelocity", AnimationVariableType::Float}},
//			{ AnimationVariable::VehicleIsBoosting, { "VehicleIsBoosting", AnimationVariableType::Integer}},
//			{ AnimationVariable::VehicleIsVertBoosting, { "VehicleIsVertBoosting", AnimationVariableType::Integer}},
//			{ AnimationVariable::VehicleLateralVelocity, { "VehicleLateralVelocity", AnimationVariableType::Float}},
//			{ AnimationVariable::VehicleSpeed, { "VehicleSpeed", AnimationVariableType::Float}},
//			{ AnimationVariable::VehicleWheelForwardSlipVelocity, { "VehicleWheelForwardSlipVelocity", AnimationVariableType::Float}},
//			{ AnimationVariable::VehicleWheelOnGround, { "VehicleWheelOnGround", AnimationVariableType::Integer}},
//			{ AnimationVariable::VehicleZVelocity, { "VehicleZVelocity", AnimationVariableType::Float}},
//			{ AnimationVariable::VelocityZ, { "VelocityZ", AnimationVariableType::Float}},
//			{ AnimationVariable::VerticalDirection, { "VerticalDirection", AnimationVariableType::Float}},
//			{ AnimationVariable::WeaponAttackAnimationDuration, { "WeaponAttackAnimationDuration", AnimationVariableType::Float}},
//			{ AnimationVariable::WeaponSpeedMult, { "WeaponSpeedMult", AnimationVariableType::Float}},
//			{ AnimationVariable::WeaponSpeedMult1, { "WeaponSpeedMult1", AnimationVariableType::Float}},
//			{ AnimationVariable::WeaponSpeedMult2, { "WeaponSpeedMult2", AnimationVariableType::Float}}
//
//		};
//
//		const char* AnimationVariableToString(AnimationVariable vType)
//		{
//			if (AnimationVarTable.count(vType)) return AnimationVarTable[vType].first;
//
//			return "";
//		}
//
//		AnimationVariable AnimationVariableFromString(const char* vStr)
//		{
//			if (std::string(vStr).length() == 0) return AnimationVariable::NONE;
//
//			for (auto& entry : AnimationVarTable)
//			{
//				if (std::strcmp(vStr, entry.second.first) == 0) return entry.first;
//			}
//
//			return AnimationVariable::UNKNOWN;
//		}
//		
//		AnimationVariableType GetAnimationVariableType(AnimationVariable vType)
//		{
//			if (AnimationVarTable.count(vType)) return AnimationVarTable[vType].second;
//
//			return AnimationVariableType::Any;
//		}
//
//#pragma endregion
//
//
//#pragma region CoreTypes
//
//		intEntry& intEntry::operator=(AnimationVariable iVal)
//		{
//			assign(AnimationVariableToString(iVal));
//			return *this;
//		}
//		intEntry::intEntry(int iVal)
//		{
//			assign(std::to_string(iVal).c_str());
//		}
//		intEntry& intEntry::operator=(int iVal)
//		{
//			assign(std::to_string(iVal).c_str());
//			return *this;
//		}
//
//		floatEntry& floatEntry::operator=(AnimationVariable fVal)
//		{
//			assign(AnimationVariableToString(fVal));
//			return *this;
//		}
//		floatEntry::floatEntry(float fVal)
//		{
//			assign(std::to_string(fVal).c_str());
//		}
//		floatEntry& floatEntry::operator=(float fVal)
//		{
//			assign(std::to_string(fVal).c_str());
//			return *this;
//		}
//
//		boolEntry& boolEntry::operator=(AnimationVariable bVal)
//		{
//			assign(AnimationVariableToString(bVal));
//			return *this;
//		}
//		boolEntry::boolEntry(bool bVal)
//		{
//			if (bVal) assign("TRUE");
//			else assign("FALSE");
//		}
//		boolEntry& boolEntry::operator=(bool bVal)
//		{
//			if (bVal) assign("TRUE");
//			else assign("FALSE");
//
//			return *this;
//		}
//
//		vectorEntry& vectorEntry::operator=(AnimationVariable v3Val)
//		{
//			assign(AnimationVariableToString(v3Val));
//			return *this;
//		}
//		vectorEntry::vectorEntry(const Math::Vector3& v3Val)
//		{
//			Clear();
//			*this += std::to_string(v3Val.getX()).c_str();
//			*this += ";";
//			*this += std::to_string(v3Val.getY()).c_str();
//			*this += ";";
//			*this += std::to_string(v3Val.getZ()).c_str();
//		}
//		vectorEntry& vectorEntry::operator=(const Math::Vector3& v3Val)
//		{
//			Clear();
//			*this += std::to_string(v3Val.getX()).c_str();
//			*this += ";";
//			*this += std::to_string(v3Val.getY()).c_str();
//			*this += ";";
//			*this += std::to_string(v3Val.getZ()).c_str();
//			return *this;
//		}
//
//		int intEntry::GetValueAsType()
//		{
//			int iVal = -999;
//
//			if (!Utilities::IsNumeric(*this)) return iVal;
//
//			try
//			{
//				iVal = std::stoi(c_str());
//			}
//			catch (const std::exception& e)
//			{
//				std::println("[CALUMI.Animation API] Failed to convert {} to type INT: ()", c_str(),e.what());
//			}
//
//			return iVal;
//		}
//
//		bool intEntry::IsValueType()
//		{
//			if (Utilities::IsNumeric(*this)) return true;
//
//			return false;
//		}
//
//		float floatEntry::GetValueAsType()
//		{
//			float fVal = std::numeric_limits<float>::quiet_NaN();
//
//			if (!Utilities::IsNumeric(*this)) return fVal;
//
//			try
//			{
//				fVal = static_cast<float>(std::stod(c_str()));
//			}
//			catch (const std::exception& e)
//			{
//				std::println("[CALUMI.Animation API] Failed to convert {} to type FLOAT: ()", c_str(), e.what());
//			}
//
//			return fVal;
//		}
//
//		bool floatEntry::IsValueType()
//		{
//			if (Utilities::IsNumeric(*this)) return true;
//
//			return false;
//		}
//
//		bool boolEntry::GetValueAsType()
//		{
//			if (_stricmp(c_str(), "true")==0) return true;
//			if (_stricmp(c_str(), "false") != 0)
//				std::println("[CALUMI.Animation API] Failed to convert {} to type BOOL: ()", c_str());
//				
//			return false;
//		}
//
//		bool boolEntry::IsValueType()
//		{
//			if (_stricmp(c_str(), "true") == 0 || _stricmp(c_str(), "false") == 0) return true;
//
//			return false;
//		}
//
//		Math::Vector3 vectorEntry::GetValueAsType()
//		{
//			constexpr float nAn = std::numeric_limits<float>::quiet_NaN();
//			Math::Vector3 vec(nAn,nAn,nAn);
//			std::vector<float> components;
//			std::istringstream inStream(c_str());
//			std::string temp;
//			while(std::getline(inStream, temp, ';'))
//			{
//				if (!Utilities::IsNumeric(temp.c_str())) return vec;
//				try
//				{
//					components.push_back(static_cast<float>(std::stod(temp)));
//				}
//				catch (const std::exception& e)
//				{
//					std::println("[CALUMI.Animation API] Failed to convert {} to type VECTOR: ()", c_str(), e.what());
//					return vec;
//				}
//			}
//
//			if (components.size() < 3) return vec;
//
//			vec.setX(components.at(0));
//			vec.setY(components.at(1));
//			vec.setZ(components.at(2));
//			return vec;
//		}
//
//		bool vectorEntry::IsValueType()
//		{
//			std::istringstream inStream(c_str());
//			std::string temp;
//			uint8_t count = 0;
//			while (std::getline(inStream, temp, ';'))
//			{
//				count++;
//				if (!Utilities::IsNumeric(temp.c_str())) return false;
//			}
//			if (count != 3) return false;
//			
//			return true;
//		}
//
//#pragma endregion
//
//#pragma region Basic Enums
//
//		constexpr static const char* cWeight = "Weight";
//		constexpr static const char* cOneMinusWeight = "One Minus Weight";
//
//		const char* CALUMI::SFBGS::BlendModeFunctionToString(const BlendModeFunction& eVal)
//		{
//			switch (eVal)
//			{
//			case BlendModeFunction::Weight: return cWeight;
//			case BlendModeFunction::OneMinusWeight: return cOneMinusWeight;
//			default:
//				return "None";
//			}
//		}
//
//		BlendModeFunction CALUMI::SFBGS::BlendModeFunctionFromString(const char* str)
//		{
//			if (_stricmp(str, cWeight) == 0)
//				return BlendModeFunction::Weight;
//			else if (_stricmp(str, cOneMinusWeight) == 0)
//				return BlendModeFunction::OneMinusWeight;
//
//			return BlendModeFunction::None;
//		}
//
//		const char* AgxAxisToString(const AgxAxis& axis)
//		{
//			switch (axis)
//			{
//			case AgxAxis::X: return "X";
//			case AgxAxis::Y: return "Y";
//			default:
//				return "Z";
//			}
//		}
//
//		AgxAxis AgxAxisFromString(const char* str)
//		{
//			if (_stricmp(str, "X")) return AgxAxis::X;
//			if (_stricmp(str, "Y")) return AgxAxis::Y;
//			return AgxAxis::Z;
//		}
//
//#pragma endregion
//
//
//
//#pragma region Event Table Static Map
//
//		static std::unordered_map<AnimationEvent, const char*> EventTable = {
//		 { CALUMI::SFBGS::AnimationEvent::NONE,"" },
//		 { CALUMI::SFBGS::AnimationEvent::ActionEnd, "ActionEnd" },
//		 { CALUMI::SFBGS::AnimationEvent::ActionInterrupt, "ActionInterrupt" },
//		 { CALUMI::SFBGS::AnimationEvent::Activate, "Activate" },
//		 { CALUMI::SFBGS::AnimationEvent::ActivateInstant, "ActivateInstant" },
//		 { CALUMI::SFBGS::AnimationEvent::ActivationDone, "ActivationDone" },
//		 { CALUMI::SFBGS::AnimationEvent::AddRagdollToWorld, "AddRagdollToWorld" },
//		 { CALUMI::SFBGS::AnimationEvent::AlertStart, "AlertStart" },
//		 { CALUMI::SFBGS::AnimationEvent::AlphaEnd, "AlphaEnd" },
//		 { CALUMI::SFBGS::AnimationEvent::AlphaStart, "AlphaStart" },
//		 { CALUMI::SFBGS::AnimationEvent::AmbushEnter, "AmbushEnter" },
//		 { CALUMI::SFBGS::AnimationEvent::AmbushEnterInstant, "AmbushEnterInstant" },
//		 { CALUMI::SFBGS::AnimationEvent::AmbushExit, "AmbushExit" },
//		 { CALUMI::SFBGS::AnimationEvent::AnimationComplete, "AnimationComplete" },
//		 { CALUMI::SFBGS::AnimationEvent::AnimationOff, "AnimationOff" },
//		 { CALUMI::SFBGS::AnimationEvent::AnimationOn, "AnimationOn" },
//		 { CALUMI::SFBGS::AnimationEvent::AnimFinish, "AnimFinish" },
//		 { CALUMI::SFBGS::AnimationEvent::AnimObjDraw, "AnimObjDraw" },
//		 { CALUMI::SFBGS::AnimationEvent::AnimObjHide, "AnimObjHide" },
//		 { CALUMI::SFBGS::AnimationEvent::AnimObjLoad, "AnimObjLoad" },
//		 { CALUMI::SFBGS::AnimationEvent::AnimObjUnequip, "AnimObjUnequip" },
//		 { CALUMI::SFBGS::AnimationEvent::AnimObjUnequipAll, "AnimObjUnequipAll" },
//		 { CALUMI::SFBGS::AnimationEvent::Arrive, "Arrive" },
//		 { CALUMI::SFBGS::AnimationEvent::AssignVariable, "AssignVariable" },
//		 { CALUMI::SFBGS::AnimationEvent::AttackEnd, "AttackEnd" },
//		 { CALUMI::SFBGS::AnimationEvent::AttackRelease, "AttackRelease" },
//		 { CALUMI::SFBGS::AnimationEvent::AttackStart, "AttackStart" },
//		 { CALUMI::SFBGS::AnimationEvent::AttackState, "AttackState" },
//		 { CALUMI::SFBGS::AnimationEvent::AttackState2, "AttackState2" },
//		 { CALUMI::SFBGS::AnimationEvent::AttackStateNextAttack, "AttackStateNextAttack" },
//		 { CALUMI::SFBGS::AnimationEvent::AttackStateReset, "AttackStateReset" },
//		 { CALUMI::SFBGS::AnimationEvent::AttackStop, "AttackStop" },
//		 { CALUMI::SFBGS::AnimationEvent::BeginCastLeft, "BeginCastLeft" },
//		 { CALUMI::SFBGS::AnimationEvent::BeginWeaponDraw, "BeginWeaponDraw" },
//		 { CALUMI::SFBGS::AnimationEvent::BeginWeaponSheathe, "BeginWeaponSheathe" },
//		 { CALUMI::SFBGS::AnimationEvent::BetaStart, "BetaStart" },
//		 { CALUMI::SFBGS::AnimationEvent::BetaStop, "BetaStop" },
//		 { CALUMI::SFBGS::AnimationEvent::BleedOutBegin, "BleedOutBegin" },
//		 { CALUMI::SFBGS::AnimationEvent::BleedOutEnd, "BleedOutEnd" },
//		 { CALUMI::SFBGS::AnimationEvent::BleedOutStart, "BleedOutStart" },
//		 { CALUMI::SFBGS::AnimationEvent::BleedOutStop, "BleedOutStop" },
//		 { CALUMI::SFBGS::AnimationEvent::BlindFireEnd, "BlindFireEnd" },
//		 { CALUMI::SFBGS::AnimationEvent::BlindFireOverStart, "BlindFireOverStart" },
//		 { CALUMI::SFBGS::AnimationEvent::BlindFireStart, "BlindFireStart" },
//		 { CALUMI::SFBGS::AnimationEvent::BlockHitStart, "BlockHitStart" },
//		 { CALUMI::SFBGS::AnimationEvent::BlockStart, "BlockStart" },
//		 { CALUMI::SFBGS::AnimationEvent::BlockStop, "BlockStop" },
//		 { CALUMI::SFBGS::AnimationEvent::Blue, "Blue" },
//		 { CALUMI::SFBGS::AnimationEvent::BoltChargeStart, "BoltChargeStart" },
//		 { CALUMI::SFBGS::AnimationEvent::BoostAnimFinish, "BoostAnimFinish" },
//		 { CALUMI::SFBGS::AnimationEvent::BoostBegin, "BoostBegin" },
//		 { CALUMI::SFBGS::AnimationEvent::BoostpackExplode, "BoostpackExplode" },
//		 { CALUMI::SFBGS::AnimationEvent::BoostStop, "BoostStop" },
//		 { CALUMI::SFBGS::AnimationEvent::CameraBack, "CameraBack" },
//		 { CALUMI::SFBGS::AnimationEvent::CameraFront, "CameraFront" },
//		 { CALUMI::SFBGS::AnimationEvent::CameraLeft, "CameraLeft" },
//		 { CALUMI::SFBGS::AnimationEvent::CameraOverrideStart, "CameraOverrideStart" },
//		 { CALUMI::SFBGS::AnimationEvent::CameraOverrideStop, "CameraOverrideStop" },
//		 { CALUMI::SFBGS::AnimationEvent::CameraRight, "CameraRight" },
//		 { CALUMI::SFBGS::AnimationEvent::CameraShake, "CameraShake" },
//		 { CALUMI::SFBGS::AnimationEvent::CancelAbort, "CancelAbort" },
//		 { CALUMI::SFBGS::AnimationEvent::CardSwiped, "CardSwiped" },
//		 { CALUMI::SFBGS::AnimationEvent::CastEnd, "CastEnd" },
//		 { CALUMI::SFBGS::AnimationEvent::CastSelfStart, "CastSelfStart" },
//		 { CALUMI::SFBGS::AnimationEvent::CastTargetStart, "CastTargetStart" },
//		 { CALUMI::SFBGS::AnimationEvent::CharacterGenStart, "CharacterGenStart" },
//		 { CALUMI::SFBGS::AnimationEvent::ChargeDownStart, "ChargeDownStart" },
//		 { CALUMI::SFBGS::AnimationEvent::ChargeHoldStart, "ChargeHoldStart" },
//		 { CALUMI::SFBGS::AnimationEvent::CharGenBackground, "CharGenBackground" },
//		 { CALUMI::SFBGS::AnimationEvent::CharGenBody, "CharGenBody" },
//		 { CALUMI::SFBGS::AnimationEvent::CharGenFace, "CharGenFace" },
//		 { CALUMI::SFBGS::AnimationEvent::CharGenPresets, "CharGenPresets" },
//		 { CALUMI::SFBGS::AnimationEvent::CharGenTraits, "CharGenTraits" },
//		 { CALUMI::SFBGS::AnimationEvent::ChargeRelease, "ChargeRelease" },
//		 { CALUMI::SFBGS::AnimationEvent::ChargeRelease_LeftArm, "ChargeRelease_LeftArm" },
//		 { CALUMI::SFBGS::AnimationEvent::ChargeRelease_RightArm, "ChargeRelease_RightArm" },
//		 { CALUMI::SFBGS::AnimationEvent::ChargeUpStart, "ChargeUpStart" },
//		 { CALUMI::SFBGS::AnimationEvent::ChargeUpStart_LeftArm, "ChargeUpStart_LeftArm" },
//		 { CALUMI::SFBGS::AnimationEvent::ChargeUpStart_RightArm, "ChargeUpStart_RightArm" },
//		 { CALUMI::SFBGS::AnimationEvent::ChiStart, "ChiStart" },
//		 { CALUMI::SFBGS::AnimationEvent::ChiStop, "ChiStop" },
//		 { CALUMI::SFBGS::AnimationEvent::Close, "Close" },
//		 { CALUMI::SFBGS::AnimationEvent::Closed, "Closed" },
//		 { CALUMI::SFBGS::AnimationEvent::Collect, "Collect" },
//		 { CALUMI::SFBGS::AnimationEvent::CollectEmpty, "CollectEmpty" },
//		 { CALUMI::SFBGS::AnimationEvent::CollectFull, "CollectFull" },
//		 { CALUMI::SFBGS::AnimationEvent::CoverActionsEnd, "CoverActionsEnd" },
//		 { CALUMI::SFBGS::AnimationEvent::CoverEnd, "CoverEnd" },
//		 { CALUMI::SFBGS::AnimationEvent::CoverExit, "CoverExit" },
//		 { CALUMI::SFBGS::AnimationEvent::CriticalHitEnd, "CriticalHitEnd" },
//		 { CALUMI::SFBGS::AnimationEvent::CriticalHitStart, "CriticalHitStart" },
//		 { CALUMI::SFBGS::AnimationEvent::CullBone, "CullBone" },
//		 { CALUMI::SFBGS::AnimationEvent::CullFurnitureBone, "CullFurnitureBone" },
//		 { CALUMI::SFBGS::AnimationEvent::CullWeapons, "CullWeapons" },
//		 { CALUMI::SFBGS::AnimationEvent::DataSlateClose, "DataSlateClose" },
//		 { CALUMI::SFBGS::AnimationEvent::DataSlateDown, "DataSlateDown" },
//		 { CALUMI::SFBGS::AnimationEvent::DataSlateEnd, "DataSlateEnd" },
//		 { CALUMI::SFBGS::AnimationEvent::DataSlateUp, "DataSlateUp" },
//		 { CALUMI::SFBGS::AnimationEvent::Deactivation, "Deactivation" },
//		 { CALUMI::SFBGS::AnimationEvent::DeathBegin, "DeathBegin" },
//		 { CALUMI::SFBGS::AnimationEvent::DeathBleedout, "DeathBleedout" },
//		 { CALUMI::SFBGS::AnimationEvent::DeathBoostpackBleedout, "DeathBoostpackBleedout" },
//		 { CALUMI::SFBGS::AnimationEvent::DeathBoostpackStart, "DeathBoostpackStart" },
//		 { CALUMI::SFBGS::AnimationEvent::DeathFurniture, "DeathFurniture" },
//		 { CALUMI::SFBGS::AnimationEvent::DeathMeleeStart, "DeathMeleeStart" },
//		 { CALUMI::SFBGS::AnimationEvent::DeathMovingStart, "DeathMovingStart" },
//		 { CALUMI::SFBGS::AnimationEvent::DeathStart, "DeathStart" },
//		 { CALUMI::SFBGS::AnimationEvent::DeltaStart, "DeltaStart" },
//		 { CALUMI::SFBGS::AnimationEvent::DeltaStop, "DeltaStop" },
//		 { CALUMI::SFBGS::AnimationEvent::Depart, "Depart" },
//		 { CALUMI::SFBGS::AnimationEvent::Deploy, "Deploy" },
//		 { CALUMI::SFBGS::AnimationEvent::Destruction, "Destruction" },
//		 { CALUMI::SFBGS::AnimationEvent::DigiPicClose, "DigiPicClose" },
//		 { CALUMI::SFBGS::AnimationEvent::DisableBumper, "DisableBumper" },
//		 { CALUMI::SFBGS::AnimationEvent::DisableCharacterController, "DisableCharacterController" },
//		 { CALUMI::SFBGS::AnimationEvent::disableCollision, "disableCollision" },
//		 { CALUMI::SFBGS::AnimationEvent::Dock, "Dock" },
//		 { CALUMI::SFBGS::AnimationEvent::Dock_Bottom, "Dock_Bottom" },
//		 { CALUMI::SFBGS::AnimationEvent::Dock_Nose, "Dock_Nose" },
//		 { CALUMI::SFBGS::AnimationEvent::Dock_Port, "Dock_Port" },
//		 { CALUMI::SFBGS::AnimationEvent::Dock_Starboard, "Dock_Starboard" },
//		 { CALUMI::SFBGS::AnimationEvent::Dock_Top, "Dock_Top" },
//		 { CALUMI::SFBGS::AnimationEvent::Docked, "Docked" },
//		 { CALUMI::SFBGS::AnimationEvent::DockingBridgeClose, "DockingBridgeClose" },
//		 { CALUMI::SFBGS::AnimationEvent::DockingBridgeOpen, "DockingBridgeOpen" },
//		 { CALUMI::SFBGS::AnimationEvent::DockingFinished, "DockingFinished" },
//		 { CALUMI::SFBGS::AnimationEvent::DockStarborn, "DockStarborn" },
//		 { CALUMI::SFBGS::AnimationEvent::DodgeBackStart, "DodgeBackStart" },
//		 { CALUMI::SFBGS::AnimationEvent::DodgeLeftStart, "DodgeLeftStart" },
//		 { CALUMI::SFBGS::AnimationEvent::DodgeRightStart, "DodgeRightStart" },
//		 { CALUMI::SFBGS::AnimationEvent::Done, "Done" },
//		 { CALUMI::SFBGS::AnimationEvent::DoneQ01, "DoneQ01" },
//		 { CALUMI::SFBGS::AnimationEvent::dyn_Activation, "dyn_Activation" },
//		 { CALUMI::SFBGS::AnimationEvent::DynamicCameraShot, "DynamicCameraShot" },
//		 { CALUMI::SFBGS::AnimationEvent::DynamicFlavor, "DynamicFlavor" },
//		 { CALUMI::SFBGS::AnimationEvent::DynamicIdle, "DynamicIdle" },
//		 { CALUMI::SFBGS::AnimationEvent::DynamicIdleAnimObject, "DynamicIdleAnimObject" },
//		 { CALUMI::SFBGS::AnimationEvent::DynamicInstant, "DynamicInstant" },
//		 { CALUMI::SFBGS::AnimationEvent::DynamicPartialBody, "DynamicPartialBody" },
//		 { CALUMI::SFBGS::AnimationEvent::DynamicPartialBodyIdle, "DynamicPartialBodyIdle" },
//		 { CALUMI::SFBGS::AnimationEvent::DynamicPathing, "DynamicPathing" },
//		 { CALUMI::SFBGS::AnimationEvent::DynamicPhotomode, "DynamicPhotomode" },
//		 { CALUMI::SFBGS::AnimationEvent::DynamicPoseSwitch, "DynamicPoseSwitch" },
//		 { CALUMI::SFBGS::AnimationEvent::DynamicRootEnd, "DynamicRootEnd" },
//		 { CALUMI::SFBGS::AnimationEvent::DynamicRootStart, "DynamicRootStart" },
//		 { CALUMI::SFBGS::AnimationEvent::DynamicScene, "DynamicScene" },
//		 { CALUMI::SFBGS::AnimationEvent::DynamicSceneHold, "DynamicSceneHold" },
//		 { CALUMI::SFBGS::AnimationEvent::DynamicShuffle, "DynamicShuffle" },
//		 { CALUMI::SFBGS::AnimationEvent::DynamicTalk, "DynamicTalk" },
//		 { CALUMI::SFBGS::AnimationEvent::DynamicTalkLarge, "DynamicTalkLarge" },
//		 { CALUMI::SFBGS::AnimationEvent::DynamicTalkMedium, "DynamicTalkMedium" },
//		 { CALUMI::SFBGS::AnimationEvent::DynamicTalkSmall, "DynamicTalkSmall" },
//		 { CALUMI::SFBGS::AnimationEvent::DynamicThreat, "DynamicThreat" },
//		 { CALUMI::SFBGS::AnimationEvent::DynamicTransition, "DynamicTransition" },
//		 { CALUMI::SFBGS::AnimationEvent::DynamicTraversal, "DynamicTraversal" },
//		 { CALUMI::SFBGS::AnimationEvent::DynamicTurn, "DynamicTurn" },
//		 { CALUMI::SFBGS::AnimationEvent::EarlyExit, "EarlyExit" },
//		 { CALUMI::SFBGS::AnimationEvent::EffectSequenceFinish, "EffectSequenceFinish" },
//		 { CALUMI::SFBGS::AnimationEvent::EjectShellCasing, "EjectShellCasing" },
//		 { CALUMI::SFBGS::AnimationEvent::EnableBumper, "EnableBumper" },
//		 { CALUMI::SFBGS::AnimationEvent::EnableCharacterController, "EnableCharacterController" },
//		 { CALUMI::SFBGS::AnimationEvent::enableCollision, "enableCollision" },
//		 { CALUMI::SFBGS::AnimationEvent::End, "End" },
//		 { CALUMI::SFBGS::AnimationEvent::EndAnim, "EndAnim" },
//		 { CALUMI::SFBGS::AnimationEvent::EndAnimatedCamera, "EndAnimatedCamera" },
//		 { CALUMI::SFBGS::AnimationEvent::EnterCoverCrouchingLeft, "EnterCoverCrouchingLeft" },
//		 { CALUMI::SFBGS::AnimationEvent::EnterCoverCrouchingRight, "EnterCoverCrouchingRight" },
//		 { CALUMI::SFBGS::AnimationEvent::EnterCoverStandingLeft, "EnterCoverStandingLeft" },
//		 { CALUMI::SFBGS::AnimationEvent::EnterCoverStandingRight, "EnterCoverStandingRight" },
//		 { CALUMI::SFBGS::AnimationEvent::EnterFromStand, "EnterFromStand" },
//		 { CALUMI::SFBGS::AnimationEvent::EpsilonStart, "EpsilonStart" },
//		 { CALUMI::SFBGS::AnimationEvent::EpsilonStop, "EpsilonStop" },
//		 { CALUMI::SFBGS::AnimationEvent::Equip, "Equip" },
//		 { CALUMI::SFBGS::AnimationEvent::EtaStart, "EtaStart" },
//		 { CALUMI::SFBGS::AnimationEvent::EvadeLeftStart, "EvadeLeftStart" },
//		 { CALUMI::SFBGS::AnimationEvent::EvadeRightStart, "EvadeRightStart" },
//		 { CALUMI::SFBGS::AnimationEvent::EvadeStart, "EvadeStart" },
//		 { CALUMI::SFBGS::AnimationEvent::ExitCover, "ExitCover" },
//		 { CALUMI::SFBGS::AnimationEvent::ExitLoopEnd, "ExitLoopEnd" },
//		 { CALUMI::SFBGS::AnimationEvent::ExitToDeadStart, "ExitToDeadStart" },
//		 { CALUMI::SFBGS::AnimationEvent::ExitToStand, "ExitToStand" },
//		 { CALUMI::SFBGS::AnimationEvent::ExitToStandAmbush, "ExitToStandAmbush" },
//		 { CALUMI::SFBGS::AnimationEvent::EyeTrackingOff, "EyeTrackingOff" },
//		 { CALUMI::SFBGS::AnimationEvent::EyeTrackingOn, "EyeTrackingOn" },
//		 { CALUMI::SFBGS::AnimationEvent::FaceLightOff, "FaceLightOff" },
//		 { CALUMI::SFBGS::AnimationEvent::FaceLightOn, "FaceLightOn" },
//		 { CALUMI::SFBGS::AnimationEvent::FaceLightPowerOff, "FaceLightPowerOff" },
//		 { CALUMI::SFBGS::AnimationEvent::FaceLightPowerOn, "FaceLightPowerOn" },
//		 { CALUMI::SFBGS::AnimationEvent::FireAutoStart, "FireAutoStart" },
//		 { CALUMI::SFBGS::AnimationEvent::FireAutoStart_LeftArm, "FireAutoStart_LeftArm" },
//		 { CALUMI::SFBGS::AnimationEvent::FireAutoStart_RightArm, "FireAutoStart_RightArm" },
//		 { CALUMI::SFBGS::AnimationEvent::FireAutoStart_Tail, "FireAutoStart_Tail" },
//		 { CALUMI::SFBGS::AnimationEvent::FireRelease, "FireRelease" },
//		 { CALUMI::SFBGS::AnimationEvent::FireRelease_LeftArm, "FireRelease_LeftArm" },
//		 { CALUMI::SFBGS::AnimationEvent::FireRelease_RightArm, "FireRelease_RightArm" },
//		 { CALUMI::SFBGS::AnimationEvent::FireSingleBinaryStart, "FireSingleBinaryStart" },
//		 { CALUMI::SFBGS::AnimationEvent::FireSingleStart, "FireSingleStart" },
//		 { CALUMI::SFBGS::AnimationEvent::FireSingleStart_LeftArm, "FireSingleStart_LeftArm" },
//		 { CALUMI::SFBGS::AnimationEvent::FireSingleStart_Mouth1, "FireSingleStart_Mouth1" },
//		 { CALUMI::SFBGS::AnimationEvent::FireSingleStart_Mouth2, "FireSingleStart_Mouth2" },
//		 { CALUMI::SFBGS::AnimationEvent::FireSingleStart_RightArm, "FireSingleStart_RightArm" },
//		 { CALUMI::SFBGS::AnimationEvent::FireSingleStart_Tail, "FireSingleStart_Tail" },
//		 { CALUMI::SFBGS::AnimationEvent::FlavorEnd, "FlavorEnd" },
//		 { CALUMI::SFBGS::AnimationEvent::FlinchEnd, "FlinchEnd" },
//		 { CALUMI::SFBGS::AnimationEvent::FlinchStart, "FlinchStart" },
//		 { CALUMI::SFBGS::AnimationEvent::Flying, "Flying" },
//		 { CALUMI::SFBGS::AnimationEvent::FootLeft, "FootLeft" },
//		 { CALUMI::SFBGS::AnimationEvent::FootRight, "FootRight" },
//		 { CALUMI::SFBGS::AnimationEvent::FootSneakLeft, "FootSneakLeft" },
//		 { CALUMI::SFBGS::AnimationEvent::FootSneakRight, "FootSneakRight" },
//		 { CALUMI::SFBGS::AnimationEvent::FootSprintLeft, "FootSprintLeft" },
//		 { CALUMI::SFBGS::AnimationEvent::FootSprintRight, "FootSprintRight" },
//		 { CALUMI::SFBGS::AnimationEvent::ForceEquip, "ForceEquip" },
//		 { CALUMI::SFBGS::AnimationEvent::ForceFaceLightOff, "ForceFaceLightOff" },
//		 { CALUMI::SFBGS::AnimationEvent::ForceFaceLightOn, "ForceFaceLightOn" },
//		 { CALUMI::SFBGS::AnimationEvent::ForceFurnitureExit, "ForceFurnitureExit" },
//		 { CALUMI::SFBGS::AnimationEvent::ForceWeaponFire, "ForceWeaponFire" },
//		 { CALUMI::SFBGS::AnimationEvent::FrontExitToRun, "FrontExitToRun" },
//		 { CALUMI::SFBGS::AnimationEvent::FrontExitToStand, "FrontExitToStand" },
//		 { CALUMI::SFBGS::AnimationEvent::FrontExitToStandAmbush, "FrontExitToStandAmbush" },
//		 { CALUMI::SFBGS::AnimationEvent::FrontExitToWalk, "FrontExitToWalk" },
//		 { CALUMI::SFBGS::AnimationEvent::FurnitureCameraReset, "FurnitureCameraReset" },
//		 { CALUMI::SFBGS::AnimationEvent::FurnitureCameraStart, "FurnitureCameraStart" },
//		 { CALUMI::SFBGS::AnimationEvent::FXAnimFinish, "FXAnimFinish" },
//		 { CALUMI::SFBGS::AnimationEvent::FXCoolantActive, "FXCoolantActive" },
//		 { CALUMI::SFBGS::AnimationEvent::FXIdleOff, "FXIdleOff" },
//		 { CALUMI::SFBGS::AnimationEvent::FXLightsOff, "FXLightsOff" },
//		 { CALUMI::SFBGS::AnimationEvent::FXLightsOn, "FXLightsOn" },
//		 { CALUMI::SFBGS::AnimationEvent::FXMuzzleFlash, "FXMuzzleFlash" },
//		 { CALUMI::SFBGS::AnimationEvent::FXPlay01, "FXPlay01" },
//		 { CALUMI::SFBGS::AnimationEvent::FXPlay02, "FXPlay02" },
//		 { CALUMI::SFBGS::AnimationEvent::FXSprayImpactEffect, "FXSprayImpactEffect" },
//		 { CALUMI::SFBGS::AnimationEvent::GammaStart, "GammaStart" },
//		 { CALUMI::SFBGS::AnimationEvent::GammaStop, "GammaStop" },
//		 { CALUMI::SFBGS::AnimationEvent::GetUp, "GetUp" },
//		 { CALUMI::SFBGS::AnimationEvent::GetUpEnd, "GetUpEnd" },
//		 { CALUMI::SFBGS::AnimationEvent::GetUpStart, "GetUpStart" },
//		 { CALUMI::SFBGS::AnimationEvent::gra, "gra" },
//		 { CALUMI::SFBGS::AnimationEvent::GravDashStart_Long, "GravDashStart_Long" },
//		 { CALUMI::SFBGS::AnimationEvent::GravDashStart_Medium, "GravDashStart_Medium" },
//		 { CALUMI::SFBGS::AnimationEvent::GravDashStart_Short, "GravDashStart_Short" },
//		 { CALUMI::SFBGS::AnimationEvent::GravityOff, "GravityOff" },
//		 { CALUMI::SFBGS::AnimationEvent::GravityOn, "GravityOn" },
//		 { CALUMI::SFBGS::AnimationEvent::GravJumpCancel, "GravJumpCancel" },
//		 { CALUMI::SFBGS::AnimationEvent::GravJumpExecute, "GravJumpExecute" },
//		 { CALUMI::SFBGS::AnimationEvent::GravJumpFinish, "GravJumpFinish" },
//		 { CALUMI::SFBGS::AnimationEvent::GravJumpInitiate, "GravJumpInitiate" },
//		 { CALUMI::SFBGS::AnimationEvent::GrazeEnd, "GrazeEnd" },
//		 { CALUMI::SFBGS::AnimationEvent::GrazeStart, "GrazeStart" },
//		 { CALUMI::SFBGS::AnimationEvent::GrazeStop, "GrazeStop" },
//		 { CALUMI::SFBGS::AnimationEvent::Green, "Green" },
//		 { CALUMI::SFBGS::AnimationEvent::GunDownInterrupted, "GunDownInterrupted" },
//		 { CALUMI::SFBGS::AnimationEvent::GunDownStart, "GunDownStart" },
//		 { CALUMI::SFBGS::AnimationEvent::GunDownStop, "GunDownStop" },
//		 { CALUMI::SFBGS::AnimationEvent::HalfClose01, "HalfClose01" },
//		 { CALUMI::SFBGS::AnimationEvent::HalfClose02, "HalfClose02" },
//		 { CALUMI::SFBGS::AnimationEvent::HalfOpen01, "HalfOpen01" },
//		 { CALUMI::SFBGS::AnimationEvent::HalfOpen02, "HalfOpen02" },
//		 { CALUMI::SFBGS::AnimationEvent::HandIKOff, "HandIKOff" },
//		 { CALUMI::SFBGS::AnimationEvent::HandIKOn, "HandIKOn" },
//		 { CALUMI::SFBGS::AnimationEvent::HeadLightPowerOff, "HeadLightPowerOff" },
//		 { CALUMI::SFBGS::AnimationEvent::HeadLightPowerOn, "HeadLightPowerOn" },
//		 { CALUMI::SFBGS::AnimationEvent::HeadTrackingOff, "HeadTrackingOff" },
//		 { CALUMI::SFBGS::AnimationEvent::HeadTrackingOn, "HeadTrackingOn" },
//		 { CALUMI::SFBGS::AnimationEvent::HelmetLightOff, "HelmetLightOff" },
//		 { CALUMI::SFBGS::AnimationEvent::HelmetLightOn, "HelmetLightOn" },
//		 { CALUMI::SFBGS::AnimationEvent::HelmetLightPowerOff, "HelmetLightPowerOff" },
//		 { CALUMI::SFBGS::AnimationEvent::HelmetLightPowerOn, "HelmetLightPowerOn" },
//		 { CALUMI::SFBGS::AnimationEvent::HideEnd, "HideEnd" },
//		 { CALUMI::SFBGS::AnimationEvent::HideStart, "HideStart" },
//		 { CALUMI::SFBGS::AnimationEvent::HideStop, "HideStop" },
//		 { CALUMI::SFBGS::AnimationEvent::HideSubsegment, "HideSubsegment" },
//		 { CALUMI::SFBGS::AnimationEvent::HitFrame, "HitFrame" },
//		 { CALUMI::SFBGS::AnimationEvent::IdleArmCrossedEquip, "IdleArmCrossedEquip" },
//		 { CALUMI::SFBGS::AnimationEvent::IdleArtifactEquip, "IdleArtifactEquip" },
//		 { CALUMI::SFBGS::AnimationEvent::IdleBackTimer, "IdleBackTimer" },
//		 { CALUMI::SFBGS::AnimationEvent::IdleBedSleeping, "IdleBedSleeping" },
//		 { CALUMI::SFBGS::AnimationEvent::IdleBriefcaseEquip, "IdleBriefcaseEquip" },
//		 { CALUMI::SFBGS::AnimationEvent::IdleChairGetUp, "IdleChairGetUp" },
//		 { CALUMI::SFBGS::AnimationEvent::IdleChairSitting, "IdleChairSitting" },
//		 { CALUMI::SFBGS::AnimationEvent::IdleCoffeeCupEquip, "IdleCoffeeCupEquip" },
//		 { CALUMI::SFBGS::AnimationEvent::IdleCount, "IdleCount" },
//		 { CALUMI::SFBGS::AnimationEvent::IdleCowerStart, "IdleCowerStart" },
//		 { CALUMI::SFBGS::AnimationEvent::IdleDataSlateEquip, "IdleDataSlateEquip" },
//		 { CALUMI::SFBGS::AnimationEvent::IdleDataSlateReceiveAndReadEquip, "IdleDataSlateReceiveAndReadEquip" },
//		 { CALUMI::SFBGS::AnimationEvent::IdleDialogueAdditive, "IdleDialogueAdditive" },
//		 { CALUMI::SFBGS::AnimationEvent::IdleDigiPicEquip, "IdleDigiPicEquip" },
//		 { CALUMI::SFBGS::AnimationEvent::IdleDuffleBagEquip, "IdleDuffleBagEquip" },
//		 { CALUMI::SFBGS::AnimationEvent::IdleFaceTimer, "IdleFaceTimer" },
//		 { CALUMI::SFBGS::AnimationEvent::IdleFlavor, "IdleFlavor" },
//		 { CALUMI::SFBGS::AnimationEvent::IdleFlavorAdditive, "IdleFlavorAdditive" },
//		 { CALUMI::SFBGS::AnimationEvent::IdleFlavorLarge, "IdleFlavorLarge" },
//		 { CALUMI::SFBGS::AnimationEvent::IdleFlavorLargeEnd, "IdleFlavorLargeEnd" },
//		 { CALUMI::SFBGS::AnimationEvent::IdleFlavorNPC, "IdleFlavorNPC" },
//		 { CALUMI::SFBGS::AnimationEvent::IdleFlavorPartialBody, "IdleFlavorPartialBody" },
//		 { CALUMI::SFBGS::AnimationEvent::IdleFlavorPlayer, "IdleFlavorPlayer" },
//		 { CALUMI::SFBGS::AnimationEvent::IdleGiveWatchStart, "IdleGiveWatchStart" },
//		 { CALUMI::SFBGS::AnimationEvent::IdleHandCuffedEquip, "IdleHandCuffedEquip" },
//		 { CALUMI::SFBGS::AnimationEvent::IdleHandsBehindEquip, "IdleHandsBehindEquip" },
//		 { CALUMI::SFBGS::AnimationEvent::IdleInterrupt, "IdleInterrupt" },
//		 { CALUMI::SFBGS::AnimationEvent::IdlePartialBodyEnd, "IdlePartialBodyEnd" },
//		 { CALUMI::SFBGS::AnimationEvent::IdlePartialBodyStop, "IdlePartialBodyStop" },
//		 { CALUMI::SFBGS::AnimationEvent::IdlePartialBodyStopInstant, "IdlePartialBodyStopInstant" },
//		 { CALUMI::SFBGS::AnimationEvent::IdlePartialBodySwitch, "IdlePartialBodySwitch" },
//		 { CALUMI::SFBGS::AnimationEvent::IdleQuantumCalibratorEquip, "IdleQuantumCalibratorEquip" },
//		 { CALUMI::SFBGS::AnimationEvent::IdleShoppingBagEquip, "IdleShoppingBagEquip" },
//		 { CALUMI::SFBGS::AnimationEvent::IdleSitting, "IdleSitting" },
//		 { CALUMI::SFBGS::AnimationEvent::IdleStart, "IdleStart" },
//		 { CALUMI::SFBGS::AnimationEvent::IdleStop, "IdleStop" },
//		 { CALUMI::SFBGS::AnimationEvent::IdleSurrenderEquip, "IdleSurrenderEquip" },
//		 { CALUMI::SFBGS::AnimationEvent::IdleSyncStart, "IdleSyncStart" },
//		 { CALUMI::SFBGS::AnimationEvent::IdleSyncStop, "IdleSyncStop" },
//		 { CALUMI::SFBGS::AnimationEvent::IdleTaunt, "IdleTaunt" },
//		 { CALUMI::SFBGS::AnimationEvent::IdleTechReader2handsBulkyEquip, "IdleTechReader2handsBulkyEquip" },
//		 { CALUMI::SFBGS::AnimationEvent::IdleTechReader2handsEquip, "IdleTechReader2handsEquip" },
//		 { CALUMI::SFBGS::AnimationEvent::IdleTechReaderEquip, "IdleTechReaderEquip" },
//		 { CALUMI::SFBGS::AnimationEvent::IdleWoundedEquip, "IdleWoundedEquip" },
//		 { CALUMI::SFBGS::AnimationEvent::InitializeCharGen, "InitializeCharGen" },
//		 { CALUMI::SFBGS::AnimationEvent::InitializeGunGraph, "InitializeGunGraph" },
//		 { CALUMI::SFBGS::AnimationEvent::InitializeInventory, "InitializeInventory" },
//		 { CALUMI::SFBGS::AnimationEvent::InitializeMeleeGraph, "InitializeMeleeGraph" },
//		 { CALUMI::SFBGS::AnimationEvent::InitializeMTGraph, "InitializeMTGraph" },
//		 { CALUMI::SFBGS::AnimationEvent::InjuredBegin, "InjuredBegin" },
//		 { CALUMI::SFBGS::AnimationEvent::InjuredEnd, "InjuredEnd" },
//		 { CALUMI::SFBGS::AnimationEvent::InjuredStart, "InjuredStart" },
//		 { CALUMI::SFBGS::AnimationEvent::InjuredStop, "InjuredStop" },
//		 { CALUMI::SFBGS::AnimationEvent::InstantInitializeCharGen, "InstantInitializeCharGen" },
//		 { CALUMI::SFBGS::AnimationEvent::InstantInitializeGunGraph, "InstantInitializeGunGraph" },
//		 { CALUMI::SFBGS::AnimationEvent::InstantInitializeInventory, "InstantInitializeInventory" },
//		 { CALUMI::SFBGS::AnimationEvent::InstantInitializeMeleeGraph, "InstantInitializeMeleeGraph" },
//		 { CALUMI::SFBGS::AnimationEvent::InstantInitializeMTGraph, "InstantInitializeMTGraph" },
//		 { CALUMI::SFBGS::AnimationEvent::InstantInitializeUnconscious, "InstantInitializeUnconscious" },
//		 { CALUMI::SFBGS::AnimationEvent::InterpolationMark, "InterpolationMark" },
//		 { CALUMI::SFBGS::AnimationEvent::InventoryCameraBack, "InventoryCameraBack" },
//		 { CALUMI::SFBGS::AnimationEvent::InventoryCameraBody, "InventoryCameraBody" },
//		 { CALUMI::SFBGS::AnimationEvent::InventoryCameraFaceGen, "InventoryCameraFaceGen" },
//		 { CALUMI::SFBGS::AnimationEvent::InventoryCameraHead, "InventoryCameraHead" },
//		 { CALUMI::SFBGS::AnimationEvent::InventoryCameraMain, "InventoryCameraMain" },
//		 { CALUMI::SFBGS::AnimationEvent::InventoryCameraOutfit, "InventoryCameraOutfit" },
//		 { CALUMI::SFBGS::AnimationEvent::InventoryCameraWeapon, "InventoryCameraWeapon" },
//		 { CALUMI::SFBGS::AnimationEvent::InventoryOpen, "InventoryOpen" },
//		 { CALUMI::SFBGS::AnimationEvent::InventoryOpenDraw, "InventoryOpenDraw" },
//		 { CALUMI::SFBGS::AnimationEvent::InvertMirror, "InvertMirror" },
//		 { CALUMI::SFBGS::AnimationEvent::InvertMirrorAndCrouch, "InvertMirrorAndCrouch" },
//		 { CALUMI::SFBGS::AnimationEvent::IotaStart, "IotaStart" },
//		 { CALUMI::SFBGS::AnimationEvent::IotaStop, "IotaStop" },
//		 { CALUMI::SFBGS::AnimationEvent::IsOff, "IsOff" },
//		 { CALUMI::SFBGS::AnimationEvent::IsOn, "IsOn" },
//		 { CALUMI::SFBGS::AnimationEvent::JiggleEvaluate, "JiggleEvaluate" },
//		 { CALUMI::SFBGS::AnimationEvent::JigglePitchDown, "JigglePitchDown" },
//		 { CALUMI::SFBGS::AnimationEvent::JigglePitchUp, "JigglePitchUp" },
//		 { CALUMI::SFBGS::AnimationEvent::JiggleRollLeft, "JiggleRollLeft" },
//		 { CALUMI::SFBGS::AnimationEvent::JiggleRollRight, "JiggleRollRight" },
//		 { CALUMI::SFBGS::AnimationEvent::JiggleYawLeft, "JiggleYawLeft" },
//		 { CALUMI::SFBGS::AnimationEvent::JiggleYawRight, "JiggleYawRight" },
//		 { CALUMI::SFBGS::AnimationEvent::JumpBoost, "JumpBoost" },
//		 { CALUMI::SFBGS::AnimationEvent::JumpBoostBegin, "JumpBoostBegin" },
//		 { CALUMI::SFBGS::AnimationEvent::JumpDown, "JumpDown" },
//		 { CALUMI::SFBGS::AnimationEvent::JumpFall, "JumpFall" },
//		 { CALUMI::SFBGS::AnimationEvent::JumpIdle01, "JumpIdle01" },
//		 { CALUMI::SFBGS::AnimationEvent::JumpIdle02, "JumpIdle02" },
//		 { CALUMI::SFBGS::AnimationEvent::JumpLand, "JumpLand" },
//		 { CALUMI::SFBGS::AnimationEvent::JumpLandAdditive, "JumpLandAdditive" },
//		 { CALUMI::SFBGS::AnimationEvent::JumpLandStop, "JumpLandStop" },
//		 { CALUMI::SFBGS::AnimationEvent::JumpStart, "JumpStart" },
//		 { CALUMI::SFBGS::AnimationEvent::JumpTraversal, "JumpTraversal" },
//		 { CALUMI::SFBGS::AnimationEvent::JumpTraversalLand, "JumpTraversalLand" },
//		 { CALUMI::SFBGS::AnimationEvent::JumpUp, "JumpUp" },
//		 { CALUMI::SFBGS::AnimationEvent::JunoReset, "JunoReset" },
//		 { CALUMI::SFBGS::AnimationEvent::KappaStart, "KappaStart" },
//		 { CALUMI::SFBGS::AnimationEvent::KappaStop, "KappaStop" },
//		 { CALUMI::SFBGS::AnimationEvent::KillActor, "KillActor" },
//		 { CALUMI::SFBGS::AnimationEvent::KnockDownStart, "KnockDownStart" },
//		 { CALUMI::SFBGS::AnimationEvent::LadderClimb, "LadderClimb" },
//		 { CALUMI::SFBGS::AnimationEvent::LadderClimbDown, "LadderClimbDown" },
//		 { CALUMI::SFBGS::AnimationEvent::LadderClimbIdle, "LadderClimbIdle" },
//		 { CALUMI::SFBGS::AnimationEvent::LadderClimbLeftHandUp, "LadderClimbLeftHandUp" },
//		 { CALUMI::SFBGS::AnimationEvent::LadderClimbRightHandUp, "LadderClimbRightHandUp" },
//		 { CALUMI::SFBGS::AnimationEvent::LadderClimbUp, "LadderClimbUp" },
//		 { CALUMI::SFBGS::AnimationEvent::LadderEnd, "LadderEnd" },
//		 { CALUMI::SFBGS::AnimationEvent::LadderEnterClimbDown, "LadderEnterClimbDown" },
//		 { CALUMI::SFBGS::AnimationEvent::LadderEnterClimbUp, "LadderEnterClimbUp" },
//		 { CALUMI::SFBGS::AnimationEvent::LadderExit, "LadderExit" },
//		 { CALUMI::SFBGS::AnimationEvent::LadderLeftHandUp, "LadderLeftHandUp" },
//		 { CALUMI::SFBGS::AnimationEvent::LadderRightHandUp, "LadderRightHandUp" },
//		 { CALUMI::SFBGS::AnimationEvent::LambdaStart, "LambdaStart" },
//		 { CALUMI::SFBGS::AnimationEvent::LambdaStop, "LambdaStop" },
//		 { CALUMI::SFBGS::AnimationEvent::Land, "Land" },
//		 { CALUMI::SFBGS::AnimationEvent::Landed, "Landed" },
//		 { CALUMI::SFBGS::AnimationEvent::LandedClosed, "LandedClosed" },
//		 { CALUMI::SFBGS::AnimationEvent::LandedSlave, "LandedSlave" },
//		 { CALUMI::SFBGS::AnimationEvent::LandingFinished, "LandingFinished" },
//		 { CALUMI::SFBGS::AnimationEvent::LandingFinishedFailsafe, "LandingFinishedFailsafe" },
//		 { CALUMI::SFBGS::AnimationEvent::LandingKreet, "LandingKreet" },
//		 { CALUMI::SFBGS::AnimationEvent::LandingKreetPirates, "LandingKreetPirates" },
//		 { CALUMI::SFBGS::AnimationEvent::LandKreetSlave, "LandKreetSlave" },
//		 { CALUMI::SFBGS::AnimationEvent::LandLarge, "LandLarge" },
//		 { CALUMI::SFBGS::AnimationEvent::LandNPC, "LandNPC" },
//		 { CALUMI::SFBGS::AnimationEvent::LandSlave, "LandSlave" },
//		 { CALUMI::SFBGS::AnimationEvent::LandSlaveNPC, "LandSlaveNPC" },
//		 { CALUMI::SFBGS::AnimationEvent::LandStarborn, "LandStarborn" },
//		 { CALUMI::SFBGS::AnimationEvent::LandStarbornNPC, "LandStarbornNPC" },
//		 { CALUMI::SFBGS::AnimationEvent::LandVertical, "LandVertical" },
//		 { CALUMI::SFBGS::AnimationEvent::LeftExitToRun, "LeftExitToRun" },
//		 { CALUMI::SFBGS::AnimationEvent::LeftExitToStand, "LeftExitToStand" },
//		 { CALUMI::SFBGS::AnimationEvent::LeftExitToWalk, "LeftExitToWalk" },
//		 { CALUMI::SFBGS::AnimationEvent::LeftQuickExitToStand, "LeftQuickExitToStand" },
//		 { CALUMI::SFBGS::AnimationEvent::LimbCrippleStart, "LimbCrippleStart" },
//		 { CALUMI::SFBGS::AnimationEvent::Link, "Link" },
//		 { CALUMI::SFBGS::AnimationEvent::LoadFurnitureGraph, "LoadFurnitureGraph" },
//		 { CALUMI::SFBGS::AnimationEvent::LoadGunAssemblyGraph, "LoadGunAssemblyGraph" },
//		 { CALUMI::SFBGS::AnimationEvent::Loop01, "Loop01" },
//		 { CALUMI::SFBGS::AnimationEvent::LoopCCW01, "LoopCCW01" },
//		 { CALUMI::SFBGS::AnimationEvent::LoopCW01, "LoopCW01" },
//		 { CALUMI::SFBGS::AnimationEvent::MainTrack, "MainTrack" },
//		 { CALUMI::SFBGS::AnimationEvent::MantleCancel, "MantleCancel" },
//		 { CALUMI::SFBGS::AnimationEvent::MantleEnd, "MantleEnd" },
//		 { CALUMI::SFBGS::AnimationEvent::MantleHands, "MantleHands" },
//		 { CALUMI::SFBGS::AnimationEvent::MantleStart, "MantleStart" },
//		 { CALUMI::SFBGS::AnimationEvent::MaxCumulativeAnimationsReached, "MaxCumulativeAnimationsReached" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttack_1, "MeleeAttack_1" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttack_2, "MeleeAttack_2" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttack_3, "MeleeAttack_3" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttack_4, "MeleeAttack_4" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttack_5, "MeleeAttack_5" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttack_A, "MeleeAttack_A" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttack_B, "MeleeAttack_B" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttack_Bite01, "MeleeAttack_Bite01" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttack_Bite02, "MeleeAttack_Bite02" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttack_Bite03, "MeleeAttack_Bite03" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttack_Claw01, "MeleeAttack_Claw01" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttack_Claw02, "MeleeAttack_Claw02" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttack_Head01, "MeleeAttack_Head01" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttack_Head02, "MeleeAttack_Head02" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttack_Head03, "MeleeAttack_Head03" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttack_Left180, "MeleeAttack_Left180" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttack_Left90, "MeleeAttack_Left90" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttack_LeftArm, "MeleeAttack_LeftArm" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttack_Right180, "MeleeAttack_Right180" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttack_Right90, "MeleeAttack_Right90" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttack_RightArm, "MeleeAttack_RightArm" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttack_RightClaw01, "MeleeAttack_RightClaw01" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttack_Tail01, "MeleeAttack_Tail01" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttack_Tail02, "MeleeAttack_Tail02" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttack01, "MeleeAttack01" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttack02, "MeleeAttack02" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttack03, "MeleeAttack03" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttack04, "MeleeAttack04" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttack05, "MeleeAttack05" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttackBashMiss, "MeleeAttackBashMiss" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttackBashStart, "MeleeAttackBashStart" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttackFinisherStart, "MeleeAttackFinisherStart" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttackPower_1, "MeleeAttackPower_1" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttackPower_2, "MeleeAttackPower_2" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttackPower_B, "MeleeAttackPower_B" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttackPower_Bite01, "MeleeAttackPower_Bite01" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttackPower_Bite02, "MeleeAttackPower_Bite02" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttackPower_Claw01, "MeleeAttackPower_Claw01" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttackPower_Head01, "MeleeAttackPower_Head01" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttackPower_Tail01, "MeleeAttackPower_Tail01" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttackPower01, "MeleeAttackPower01" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttackPower02, "MeleeAttackPower02" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttackPowerStart, "MeleeAttackPowerStart" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttackPowerStartZeroG, "MeleeAttackPowerStartZeroG" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttackSprintStart, "MeleeAttackSprintStart" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttackStart, "MeleeAttackStart" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttackStartInAir, "MeleeAttackStartInAir" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleeAttackStartZeroG, "MeleeAttackStartZeroG" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleePowerAttack_Forward, "MeleePowerAttack_Forward" },
//		 { CALUMI::SFBGS::AnimationEvent::MeleePowerAttack_ForwardSlam, "MeleePowerAttack_ForwardSlam" },
//		 { CALUMI::SFBGS::AnimationEvent::MinigameEnd, "MinigameEnd" },
//		 { CALUMI::SFBGS::AnimationEvent::MinigameStart, "MinigameStart" },
//		 { CALUMI::SFBGS::AnimationEvent::MuStart, "MuStart" },
//		 { CALUMI::SFBGS::AnimationEvent::MuStop, "MuStop" },
//		 { CALUMI::SFBGS::AnimationEvent::NuStart, "NuStart" },
//		 { CALUMI::SFBGS::AnimationEvent::NuStop, "NuStop" },
//		 { CALUMI::SFBGS::AnimationEvent::OffRamp, "OffRamp" },
//		 { CALUMI::SFBGS::AnimationEvent::OmegaStart, "OmegaStart" },
//		 { CALUMI::SFBGS::AnimationEvent::OmegaStop, "OmegaStop" },
//		 { CALUMI::SFBGS::AnimationEvent::OmicronStart, "OmicronStart" },
//		 { CALUMI::SFBGS::AnimationEvent::OmicronStop, "OmicronStop" },
//		 { CALUMI::SFBGS::AnimationEvent::OnReloadExit, "OnReloadExit" },
//		 { CALUMI::SFBGS::AnimationEvent::Open, "Open" },
//		 { CALUMI::SFBGS::AnimationEvent::Opened, "Opened" },
//		 { CALUMI::SFBGS::AnimationEvent::Orange, "Orange" },
//		 { CALUMI::SFBGS::AnimationEvent::pa_KnifeAttack, "pa_KnifeAttack" },
//		 { CALUMI::SFBGS::AnimationEvent::pa_TerrormorphBiteHead, "pa_TerrormorphBiteHead" },
//		 { CALUMI::SFBGS::AnimationEvent::pa_TerrormorphBiteHead01, "pa_TerrormorphBiteHead01" },
//		 { CALUMI::SFBGS::AnimationEvent::pa_TerrormorphBiteHead02, "pa_TerrormorphBiteHead02" },
//		 { CALUMI::SFBGS::AnimationEvent::pa_TerrormorphSlamHead01, "pa_TerrormorphSlamHead01" },
//		 { CALUMI::SFBGS::AnimationEvent::pa_TerrormorphSlamHead02, "pa_TerrormorphSlamHead02" },
//		 { CALUMI::SFBGS::AnimationEvent::PairedStop, "PairedStop" },
//		 { CALUMI::SFBGS::AnimationEvent::ParasiteFXStart, "ParasiteFXStart" },
//		 { CALUMI::SFBGS::AnimationEvent::PartialBodyAnimObjUnequip, "PartialBodyAnimObjUnequip" },
//		 { CALUMI::SFBGS::AnimationEvent::PartialCoverStart, "PartialCoverStart" },
//		 { CALUMI::SFBGS::AnimationEvent::PathingBegin, "PathingBegin" },
//		 { CALUMI::SFBGS::AnimationEvent::PathingEnd, "PathingEnd" },
//		 { CALUMI::SFBGS::AnimationEvent::PathTweenerEnd, "PathTweenerEnd" },
//		 { CALUMI::SFBGS::AnimationEvent::PathTweenerEndTween, "PathTweenerEndTween" },
//		 { CALUMI::SFBGS::AnimationEvent::PathTweenerStart, "PathTweenerStart" },
//		 { CALUMI::SFBGS::AnimationEvent::PhiStart, "PhiStart" },
//		 { CALUMI::SFBGS::AnimationEvent::PhiStop, "PhiStop" },
//		 { CALUMI::SFBGS::AnimationEvent::PhotoMode_Boxer, "PhotoMode_Boxer" },
//		 { CALUMI::SFBGS::AnimationEvent::PhotoMode_Cower, "PhotoMode_Cower" },
//		 { CALUMI::SFBGS::AnimationEvent::PhotoMode_Gift, "PhotoMode_Gift" },
//		 { CALUMI::SFBGS::AnimationEvent::PhotoMode_HandsOnHips, "PhotoMode_HandsOnHips" },
//		 { CALUMI::SFBGS::AnimationEvent::PhotoMode_Hero, "PhotoMode_Hero" },
//		 { CALUMI::SFBGS::AnimationEvent::PhotoMode_Jump, "PhotoMode_Jump" },
//		 { CALUMI::SFBGS::AnimationEvent::PhotoMode_Kneeling, "PhotoMode_Kneeling" },
//		 { CALUMI::SFBGS::AnimationEvent::PhotoMode_LookUp, "PhotoMode_LookUp" },
//		 { CALUMI::SFBGS::AnimationEvent::PhotoMode_Love, "PhotoMode_Love" },
//		 { CALUMI::SFBGS::AnimationEvent::PhotoMode_Propose, "PhotoMode_Propose" },
//		 { CALUMI::SFBGS::AnimationEvent::PhotoMode_SitOnGround, "PhotoMode_SitOnGround" },
//		 { CALUMI::SFBGS::AnimationEvent::PhotoMode_Sneak, "PhotoMode_Sneak" },
//		 { CALUMI::SFBGS::AnimationEvent::PhotoMode_ThumbsUp, "PhotoMode_ThumbsUp" },
//		 { CALUMI::SFBGS::AnimationEvent::PhotoMode_Wave, "PhotoMode_Wave" },
//		 { CALUMI::SFBGS::AnimationEvent::PhotoMode_WhatsUp, "PhotoMode_WhatsUp" },
//		 { CALUMI::SFBGS::AnimationEvent::PhotoModeExit, "PhotoModeExit" },
//		 { CALUMI::SFBGS::AnimationEvent::PickNewIdle, "PickNewIdle" },
//		 { CALUMI::SFBGS::AnimationEvent::PipboyClosed, "PipboyClosed" },
//		 { CALUMI::SFBGS::AnimationEvent::PipboyOpened, "PipboyOpened" },
//		 { CALUMI::SFBGS::AnimationEvent::PiStart, "PiStart" },
//		 { CALUMI::SFBGS::AnimationEvent::PiStop, "PiStop" },
//		 { CALUMI::SFBGS::AnimationEvent::PitchOverrideEnd, "PitchOverrideEnd" },
//		 { CALUMI::SFBGS::AnimationEvent::PitchOverrideStart, "PitchOverrideStart" },
//		 { CALUMI::SFBGS::AnimationEvent::Play01, "Play01" },
//		 { CALUMI::SFBGS::AnimationEvent::Play02, "Play02" },
//		 { CALUMI::SFBGS::AnimationEvent::Play03, "Play03" },
//		 { CALUMI::SFBGS::AnimationEvent::Play04, "Play04" },
//		 { CALUMI::SFBGS::AnimationEvent::Play05, "Play05" },
//		 { CALUMI::SFBGS::AnimationEvent::Play06, "Play06" },
//		 { CALUMI::SFBGS::AnimationEvent::PlayerTerminalEnter, "PlayerTerminalEnter" },
//		 { CALUMI::SFBGS::AnimationEvent::PlayerTerminalExit, "PlayerTerminalExit" },
//		 { CALUMI::SFBGS::AnimationEvent::PlayImod, "PlayImod" },
//		 { CALUMI::SFBGS::AnimationEvent::PlayLandedFX, "PlayLandedFX" },
//		 { CALUMI::SFBGS::AnimationEvent::PlayLandingCamera, "PlayLandingCamera" },
//		 { CALUMI::SFBGS::AnimationEvent::PlayLandingEngineFX, "PlayLandingEngineFX" },
//		 { CALUMI::SFBGS::AnimationEvent::PlayLandingEngineFXStatic, "PlayLandingEngineFXStatic" },
//		 { CALUMI::SFBGS::AnimationEvent::PlayLandingFX, "PlayLandingFX" },
//		 { CALUMI::SFBGS::AnimationEvent::PlayReaction01, "PlayReaction01" },
//		 { CALUMI::SFBGS::AnimationEvent::PlayReaction02, "PlayReaction02" },
//		 { CALUMI::SFBGS::AnimationEvent::PlayReaction03, "PlayReaction03" },
//		 { CALUMI::SFBGS::AnimationEvent::PlayReaction04, "PlayReaction04" },
//		 { CALUMI::SFBGS::AnimationEvent::PlayReaction05, "PlayReaction05" },
//		 { CALUMI::SFBGS::AnimationEvent::PlayTakeOffCamera, "PlayTakeOffCamera" },
//		 { CALUMI::SFBGS::AnimationEvent::PlayTakeOffFX, "PlayTakeOffFX" },
//		 { CALUMI::SFBGS::AnimationEvent::PopToClose, "PopToClose" },
//		 { CALUMI::SFBGS::AnimationEvent::PopToOpen, "PopToOpen" },
//		 { CALUMI::SFBGS::AnimationEvent::PositionChange, "PositionChange" },
//		 { CALUMI::SFBGS::AnimationEvent::Powered, "Powered" },
//		 { CALUMI::SFBGS::AnimationEvent::PreHitFrame, "PreHitFrame" },
//		 { CALUMI::SFBGS::AnimationEvent::PsiStart, "PsiStart" },
//		 { CALUMI::SFBGS::AnimationEvent::PsiStop, "PsiStop" },
//		 { CALUMI::SFBGS::AnimationEvent::QuickExitToStand, "QuickExitToStand" },
//		 { CALUMI::SFBGS::AnimationEvent::RampClose, "RampClose" },
//		 { CALUMI::SFBGS::AnimationEvent::RampOpen, "RampOpen" },
//		 { CALUMI::SFBGS::AnimationEvent::ReactionEnd, "ReactionEnd" },
//		 { CALUMI::SFBGS::AnimationEvent::ReactionStart, "ReactionStart" },
//		 { CALUMI::SFBGS::AnimationEvent::ReactionStop, "ReactionStop" },
//		 { CALUMI::SFBGS::AnimationEvent::ReadyStart, "ReadyStart" },
//		 { CALUMI::SFBGS::AnimationEvent::ReadyState, "ReadyState" },
//		 { CALUMI::SFBGS::AnimationEvent::RearExitToRun, "RearExitToRun" },
//		 { CALUMI::SFBGS::AnimationEvent::RearExitToStand, "RearExitToStand" },
//		 { CALUMI::SFBGS::AnimationEvent::RearExitToWalk, "RearExitToWalk" },
//		 { CALUMI::SFBGS::AnimationEvent::RearQuickExitToStand, "RearQuickExitToStand" },
//		 { CALUMI::SFBGS::AnimationEvent::Recoil_LeftArm, "Recoil_LeftArm" },
//		 { CALUMI::SFBGS::AnimationEvent::Recoil_RightArm, "Recoil_RightArm" },
//		 { CALUMI::SFBGS::AnimationEvent::RecoilStart, "RecoilStart" },
//		 { CALUMI::SFBGS::AnimationEvent::Red, "Red" },
//		 { CALUMI::SFBGS::AnimationEvent::ReevaluateGraphState, "ReevaluateGraphState" },
//		 { CALUMI::SFBGS::AnimationEvent::RelaxedStart, "RelaxedStart" },
//		 { CALUMI::SFBGS::AnimationEvent::ReloadAbort, "ReloadAbort" },
//		 { CALUMI::SFBGS::AnimationEvent::ReloadBegin, "ReloadBegin" },
//		 { CALUMI::SFBGS::AnimationEvent::ReloadComplete, "ReloadComplete" },
//		 { CALUMI::SFBGS::AnimationEvent::ReloadExit, "ReloadExit" },
//		 { CALUMI::SFBGS::AnimationEvent::ReloadLoopStart, "ReloadLoopStart" },
//		 { CALUMI::SFBGS::AnimationEvent::ReloadStart, "ReloadStart" },
//		 { CALUMI::SFBGS::AnimationEvent::ReloadState, "ReloadState" },
//		 { CALUMI::SFBGS::AnimationEvent::ReloadTacticalStart, "ReloadTacticalStart" },
//		 { CALUMI::SFBGS::AnimationEvent::RemoveCharacterControllerFromWorld, "RemoveCharacterControllerFromWorld" },
//		 { CALUMI::SFBGS::AnimationEvent::Reset, "Reset" },
//		 { CALUMI::SFBGS::AnimationEvent::ResetCumulative, "ResetCumulative" },
//		 { CALUMI::SFBGS::AnimationEvent::ResurrectStart, "ResurrectStart" },
//		 { CALUMI::SFBGS::AnimationEvent::RhoStart, "RhoStart" },
//		 { CALUMI::SFBGS::AnimationEvent::RhoStop, "RhoStop" },
//		 { CALUMI::SFBGS::AnimationEvent::RightExitToRun, "RightExitToRun" },
//		 { CALUMI::SFBGS::AnimationEvent::RightExitToStand, "RightExitToStand" },
//		 { CALUMI::SFBGS::AnimationEvent::RightExitToWalk, "RightExitToWalk" },
//		 { CALUMI::SFBGS::AnimationEvent::RightQuickExitToStand, "RightQuickExitToStand" },
//		 { CALUMI::SFBGS::AnimationEvent::RingInner_CCW, "RingInner_CCW" },
//		 { CALUMI::SFBGS::AnimationEvent::RingInner_CW, "RingInner_CW" },
//		 { CALUMI::SFBGS::AnimationEvent::RingInner_Success, "RingInner_Success" },
//		 { CALUMI::SFBGS::AnimationEvent::RingMiddle_CCW, "RingMiddle_CCW" },
//		 { CALUMI::SFBGS::AnimationEvent::RingMiddle_CW, "RingMiddle_CW" },
//		 { CALUMI::SFBGS::AnimationEvent::RingMiddle_Success, "RingMiddle_Success" },
//		 { CALUMI::SFBGS::AnimationEvent::RingOuter_CCW, "RingOuter_CCW" },
//		 { CALUMI::SFBGS::AnimationEvent::RingOuter_CW, "RingOuter_CW" },
//		 { CALUMI::SFBGS::AnimationEvent::RingOuter_Success, "RingOuter_Success" },
//		 { CALUMI::SFBGS::AnimationEvent::RootAnimOffStart, "RootAnimOffStart" },
//		 { CALUMI::SFBGS::AnimationEvent::RootAnimOffStop, "RootAnimOffStop" },
//		 { CALUMI::SFBGS::AnimationEvent::Rotate_Long, "Rotate_Long" },
//		 { CALUMI::SFBGS::AnimationEvent::Rotate_Long01, "Rotate_Long01" },
//		 { CALUMI::SFBGS::AnimationEvent::Rotate_Short, "Rotate_Short" },
//		 { CALUMI::SFBGS::AnimationEvent::Rotate_Short01, "Rotate_Short01" },
//		 { CALUMI::SFBGS::AnimationEvent::Rotate_Single, "Rotate_Single" },
//		 { CALUMI::SFBGS::AnimationEvent::Rotate_Single01, "Rotate_Single01" },
//		 { CALUMI::SFBGS::AnimationEvent::RotateCCW_Loop, "RotateCCW_Loop" },
//		 { CALUMI::SFBGS::AnimationEvent::RotateCCW_Loop01, "RotateCCW_Loop01" },
//		 { CALUMI::SFBGS::AnimationEvent::RotateCW_Loop, "RotateCW_Loop" },
//		 { CALUMI::SFBGS::AnimationEvent::RotateCW_Loop01, "RotateCW_Loop01" },
//		 { CALUMI::SFBGS::AnimationEvent::RotateCW_Single01, "RotateCW_Single01" },
//		 { CALUMI::SFBGS::AnimationEvent::RotateSimple01, "RotateSimple01" },
//		 { CALUMI::SFBGS::AnimationEvent::RunAction, "RunAction" },
//		 { CALUMI::SFBGS::AnimationEvent::RunStart, "RunStart" },
//		 { CALUMI::SFBGS::AnimationEvent::SceneAnimationDoneHandler, "SceneAnimationDoneHandler" },
//		 { CALUMI::SFBGS::AnimationEvent::SceneEnd, "SceneEnd" },
//		 { CALUMI::SFBGS::AnimationEvent::SceneNextEvent, "SceneNextEvent" },
//		 { CALUMI::SFBGS::AnimationEvent::SceneNextPhase, "SceneNextPhase" },
//		 { CALUMI::SFBGS::AnimationEvent::SceneStart, "SceneStart" },
//		 { CALUMI::SFBGS::AnimationEvent::SceneStop, "SceneStop" },
//		 { CALUMI::SFBGS::AnimationEvent::SequenceA01, "SequenceA01" },
//		 { CALUMI::SFBGS::AnimationEvent::SequenceA02, "SequenceA02" },
//		 { CALUMI::SFBGS::AnimationEvent::SequenceA03, "SequenceA03" },
//		 { CALUMI::SFBGS::AnimationEvent::SequenceA04, "SequenceA04" },
//		 { CALUMI::SFBGS::AnimationEvent::SequenceA05, "SequenceA05" },
//		 { CALUMI::SFBGS::AnimationEvent::SequenceA06, "SequenceA06" },
//		 { CALUMI::SFBGS::AnimationEvent::SequenceA07, "SequenceA07" },
//		 { CALUMI::SFBGS::AnimationEvent::SequenceB01, "SequenceB01" },
//		 { CALUMI::SFBGS::AnimationEvent::SequenceB02, "SequenceB02" },
//		 { CALUMI::SFBGS::AnimationEvent::SequenceB03, "SequenceB03" },
//		 { CALUMI::SFBGS::AnimationEvent::SequenceB04, "SequenceB04" },
//		 { CALUMI::SFBGS::AnimationEvent::SequenceB05, "SequenceB05" },
//		 { CALUMI::SFBGS::AnimationEvent::SequenceB06, "SequenceB06" },
//		 { CALUMI::SFBGS::AnimationEvent::SequenceB07, "SequenceB07" },
//		 { CALUMI::SFBGS::AnimationEvent::SetOrientBegin, "SetOrientBegin" },
//		 { CALUMI::SFBGS::AnimationEvent::SetOrientEnd, "SetOrientEnd" },
//		 { CALUMI::SFBGS::AnimationEvent::SetPosBegin, "SetPosBegin" },
//		 { CALUMI::SFBGS::AnimationEvent::SetPosEnd, "SetPosEnd" },
//		 { CALUMI::SFBGS::AnimationEvent::SetupGraphRagdoll, "SetupGraphRagdoll" },
//		 { CALUMI::SFBGS::AnimationEvent::ShipImpactStart, "ShipImpactStart" },
//		 { CALUMI::SFBGS::AnimationEvent::ShowSubsegment, "ShowSubsegment" },
//		 { CALUMI::SFBGS::AnimationEvent::ShuffleBackward, "ShuffleBackward" },
//		 { CALUMI::SFBGS::AnimationEvent::ShuffleEnd, "ShuffleEnd" },
//		 { CALUMI::SFBGS::AnimationEvent::ShuffleForward, "ShuffleForward" },
//		 { CALUMI::SFBGS::AnimationEvent::SightedEnd, "SightedEnd" },
//		 { CALUMI::SFBGS::AnimationEvent::SightedOverLowStart, "SightedOverLowStart" },
//		 { CALUMI::SFBGS::AnimationEvent::SightedOverStart, "SightedOverStart" },
//		 { CALUMI::SFBGS::AnimationEvent::SightedStart, "SightedStart" },
//		 { CALUMI::SFBGS::AnimationEvent::SightedState, "SightedState" },
//		 { CALUMI::SFBGS::AnimationEvent::SightedStop, "SightedStop" },
//		 { CALUMI::SFBGS::AnimationEvent::SightedStopRequest, "SightedStopRequest" },
//		 { CALUMI::SFBGS::AnimationEvent::SigmaStart, "SigmaStart" },
//		 { CALUMI::SFBGS::AnimationEvent::sitStartFromStand, "sitStartFromStand" },
//		 { CALUMI::SFBGS::AnimationEvent::sitStartFromStandInstant, "sitStartFromStandInstant" },
//		 { CALUMI::SFBGS::AnimationEvent::sitStartFrontFromStand, "sitStartFrontFromStand" },
//		 { CALUMI::SFBGS::AnimationEvent::sitStartFrontFromStandInstant, "sitStartFrontFromStandInstant" },
//		 { CALUMI::SFBGS::AnimationEvent::sitStartFrontFromWalk, "sitStartFrontFromWalk" },
//		 { CALUMI::SFBGS::AnimationEvent::sitStartLeftFromStand, "sitStartLeftFromStand" },
//		 { CALUMI::SFBGS::AnimationEvent::sitStartLeftFromStandInstant, "sitStartLeftFromStandInstant" },
//		 { CALUMI::SFBGS::AnimationEvent::sitStartLeftFromWalk, "sitStartLeftFromWalk" },
//		 { CALUMI::SFBGS::AnimationEvent::sitStartRearFromStand, "sitStartRearFromStand" },
//		 { CALUMI::SFBGS::AnimationEvent::sitStartRearFromStandInstant, "sitStartRearFromStandInstant" },
//		 { CALUMI::SFBGS::AnimationEvent::sitStartRearFromWalk, "sitStartRearFromWalk" },
//		 { CALUMI::SFBGS::AnimationEvent::sitStartRightFromStand, "sitStartRightFromStand" },
//		 { CALUMI::SFBGS::AnimationEvent::sitStartRightFromStandInstant, "sitStartRightFromStandInstant" },
//		 { CALUMI::SFBGS::AnimationEvent::sitStartRightFromWalk, "sitStartRightFromWalk" },
//		 { CALUMI::SFBGS::AnimationEvent::SlavedIdle, "SlavedIdle" },
//		 { CALUMI::SFBGS::AnimationEvent::SlavedIdleFlavor, "SlavedIdleFlavor" },
//		 { CALUMI::SFBGS::AnimationEvent::SlavedIdleFlavor01, "SlavedIdleFlavor01" },
//		 { CALUMI::SFBGS::AnimationEvent::SlavedIdleFlavor02, "SlavedIdleFlavor02" },
//		 { CALUMI::SFBGS::AnimationEvent::SlavedIdleFlavor03, "SlavedIdleFlavor03" },
//		 { CALUMI::SFBGS::AnimationEvent::SlavedIdleFlavor04, "SlavedIdleFlavor04" },
//		 { CALUMI::SFBGS::AnimationEvent::SlavedIdleFlavor05, "SlavedIdleFlavor05" },
//		 { CALUMI::SFBGS::AnimationEvent::SlavedIdleFlavor06, "SlavedIdleFlavor06" },
//		 { CALUMI::SFBGS::AnimationEvent::SlavedIdleFlavor07, "SlavedIdleFlavor07" },
//		 { CALUMI::SFBGS::AnimationEvent::SlavedIdleFlavor08, "SlavedIdleFlavor08" },
//		 { CALUMI::SFBGS::AnimationEvent::SlavedIdleFlavor09, "SlavedIdleFlavor09" },
//		 { CALUMI::SFBGS::AnimationEvent::Slide, "Slide" },
//		 { CALUMI::SFBGS::AnimationEvent::SlideStart, "SlideStart" },
//		 { CALUMI::SFBGS::AnimationEvent::SneakStart, "SneakStart" },
//		 { CALUMI::SFBGS::AnimationEvent::SneakState, "SneakState" },
//		 { CALUMI::SFBGS::AnimationEvent::SneakStop, "SneakStop" },
//		 { CALUMI::SFBGS::AnimationEvent::SoarStart, "SoarStart" },
//		 { CALUMI::SFBGS::AnimationEvent::SoarStop, "SoarStop" },
//		 { CALUMI::SFBGS::AnimationEvent::SoundPlay, "SoundPlay" },
//		 { CALUMI::SFBGS::AnimationEvent::SoundPlayAt, "SoundPlayAt" },
//		 { CALUMI::SFBGS::AnimationEvent::SoundStop, "SoundStop" },
//		 { CALUMI::SFBGS::AnimationEvent::SpellFireLeftHand, "SpellFireLeftHand" },
//		 { CALUMI::SFBGS::AnimationEvent::SpellFireRightHand, "SpellFireRightHand" },
//		 { CALUMI::SFBGS::AnimationEvent::SprintEnd, "SprintEnd" },
//		 { CALUMI::SFBGS::AnimationEvent::SprintStart, "SprintStart" },
//		 { CALUMI::SFBGS::AnimationEvent::SprintStartEnd, "SprintStartEnd" },
//		 { CALUMI::SFBGS::AnimationEvent::SprintStop, "SprintStop" },
//		 { CALUMI::SFBGS::AnimationEvent::Stage_2, "Stage 2" },
//		 { CALUMI::SFBGS::AnimationEvent::Stage01_Loop, "Stage01_Loop" },
//		 { CALUMI::SFBGS::AnimationEvent::Stage01_Off, "Stage01_Off" },
//		 { CALUMI::SFBGS::AnimationEvent::Stage01_On, "Stage01_On" },
//		 { CALUMI::SFBGS::AnimationEvent::Stage01_Start, "Stage01_Start" },
//		 { CALUMI::SFBGS::AnimationEvent::Stage01_Stop, "Stage01_Stop" },
//		 { CALUMI::SFBGS::AnimationEvent::Stage02_Loop, "Stage02_Loop" },
//		 { CALUMI::SFBGS::AnimationEvent::Stage02_Off, "Stage02_Off" },
//		 { CALUMI::SFBGS::AnimationEvent::Stage02_On, "Stage02_On" },
//		 { CALUMI::SFBGS::AnimationEvent::Stage02_Start, "Stage02_Start" },
//		 { CALUMI::SFBGS::AnimationEvent::Stage02_Stop, "Stage02_Stop" },
//		 { CALUMI::SFBGS::AnimationEvent::Stage03_Loop, "Stage03_Loop" },
//		 { CALUMI::SFBGS::AnimationEvent::Stage03_Off, "Stage03_Off" },
//		 { CALUMI::SFBGS::AnimationEvent::Stage03_On, "Stage03_On" },
//		 { CALUMI::SFBGS::AnimationEvent::Stage03_Start, "Stage03_Start" },
//		 { CALUMI::SFBGS::AnimationEvent::Stage03_Stop, "Stage03_Stop" },
//		 { CALUMI::SFBGS::AnimationEvent::Stage04_Loop, "Stage04_Loop" },
//		 { CALUMI::SFBGS::AnimationEvent::Stage04_Off, "Stage04_Off" },
//		 { CALUMI::SFBGS::AnimationEvent::Stage04_On, "Stage04_On" },
//		 { CALUMI::SFBGS::AnimationEvent::Stage04_Start, "Stage04_Start" },
//		 { CALUMI::SFBGS::AnimationEvent::Stage04_Stop, "Stage04_Stop" },
//		 { CALUMI::SFBGS::AnimationEvent::Stage05_Loop, "Stage05_Loop" },
//		 { CALUMI::SFBGS::AnimationEvent::Stage05_Off, "Stage05_Off" },
//		 { CALUMI::SFBGS::AnimationEvent::Stage05_On, "Stage05_On" },
//		 { CALUMI::SFBGS::AnimationEvent::Stage05_Start, "Stage05_Start" },
//		 { CALUMI::SFBGS::AnimationEvent::Stage05_Stop, "Stage05_Stop" },
//		 { CALUMI::SFBGS::AnimationEvent::Stage1, "Stage1" },
//		 { CALUMI::SFBGS::AnimationEvent::Stage1NoTransition, "Stage1NoTransition" },
//		 { CALUMI::SFBGS::AnimationEvent::Stage2, "Stage2" },
//		 { CALUMI::SFBGS::AnimationEvent::Stage2NoTransition, "Stage2NoTransition" },
//		 { CALUMI::SFBGS::AnimationEvent::Stage3, "Stage3" },
//		 { CALUMI::SFBGS::AnimationEvent::Stage3NoTransition, "Stage3NoTransition" },
//		 { CALUMI::SFBGS::AnimationEvent::Stage4, "Stage4" },
//		 { CALUMI::SFBGS::AnimationEvent::Stage4NoTransition, "Stage4NoTransition" },
//		 { CALUMI::SFBGS::AnimationEvent::StaggerEnd, "StaggerEnd" },
//		 { CALUMI::SFBGS::AnimationEvent::StaggerStart, "StaggerStart" },
//		 { CALUMI::SFBGS::AnimationEvent::StaggerStop, "StaggerStop" },
//		 { CALUMI::SFBGS::AnimationEvent::StandStart, "StandStart" },
//		 { CALUMI::SFBGS::AnimationEvent::startAmbushSeq, "startAmbushSeq" },
//		 { CALUMI::SFBGS::AnimationEvent::StartAnimatedCamera, "StartAnimatedCamera" },
//		 { CALUMI::SFBGS::AnimationEvent::StartAnimatedCameraDelta, "StartAnimatedCameraDelta" },
//		 { CALUMI::SFBGS::AnimationEvent::StartedWalking, "StartedWalking" },
//		 { CALUMI::SFBGS::AnimationEvent::startFloating, "startFloating" },
//		 { CALUMI::SFBGS::AnimationEvent::StartImpactFX, "StartImpactFX" },
//		 { CALUMI::SFBGS::AnimationEvent::StartMeleeAttack, "StartMeleeAttack" },
//		 { CALUMI::SFBGS::AnimationEvent::startRoar, "startRoar" },
//		 { CALUMI::SFBGS::AnimationEvent::StartSighted, "StartSighted" },
//		 { CALUMI::SFBGS::AnimationEvent::StartVoice, "StartVoice" },
//		 { CALUMI::SFBGS::AnimationEvent::StateA_Idle, "StateA_Idle" },
//		 { CALUMI::SFBGS::AnimationEvent::StateA_Play, "StateA_Play" },
//		 { CALUMI::SFBGS::AnimationEvent::StateB_Idle, "StateB_Idle" },
//		 { CALUMI::SFBGS::AnimationEvent::StateB_Play, "StateB_Play" },
//		 { CALUMI::SFBGS::AnimationEvent::StateC_Idle, "StateC_Idle" },
//		 { CALUMI::SFBGS::AnimationEvent::StateC_Play, "StateC_Play" },
//		 { CALUMI::SFBGS::AnimationEvent::StateD_Idle, "StateD_Idle" },
//		 { CALUMI::SFBGS::AnimationEvent::StateD_Play, "StateD_Play" },
//		 { CALUMI::SFBGS::AnimationEvent::Stop, "Stop" },
//		 { CALUMI::SFBGS::AnimationEvent::Stop01, "Stop01" },
//		 { CALUMI::SFBGS::AnimationEvent::StopAttackQueuingWhileReloading, "StopAttackQueuingWhileReloading" },
//		 { CALUMI::SFBGS::AnimationEvent::StopEffect, "StopEffect" },
//		 { CALUMI::SFBGS::AnimationEvent::stopFloating, "stopFloating" },
//		 { CALUMI::SFBGS::AnimationEvent::StopImod, "StopImod" },
//		 { CALUMI::SFBGS::AnimationEvent::StopIMod, "StopIMod" },
//		 { CALUMI::SFBGS::AnimationEvent::StopImpactFX, "StopImpactFX" },
//		 { CALUMI::SFBGS::AnimationEvent::StoppedWalking, "StoppedWalking" },
//		 { CALUMI::SFBGS::AnimationEvent::StopSighted, "StopSighted" },
//		 { CALUMI::SFBGS::AnimationEvent::StunEnd, "StunEnd" },
//		 { CALUMI::SFBGS::AnimationEvent::StunStart, "StunStart" },
//		 { CALUMI::SFBGS::AnimationEvent::StunStop, "StunStop" },
//		 { CALUMI::SFBGS::AnimationEvent::SupportAttackQueuingWhileReloading, "SupportAttackQueuingWhileReloading" },
//		 { CALUMI::SFBGS::AnimationEvent::SwimStart, "SwimStart" },
//		 { CALUMI::SFBGS::AnimationEvent::SwimStart_Gun, "SwimStart_Gun" },
//		 { CALUMI::SFBGS::AnimationEvent::SwimStart_Melee, "SwimStart_Melee" },
//		 { CALUMI::SFBGS::AnimationEvent::SwimStop, "SwimStop" },
//		 { CALUMI::SFBGS::AnimationEvent::SyncDeferDeath, "SyncDeferDeath" },
//		 { CALUMI::SFBGS::AnimationEvent::SyncLeftFoot, "SyncLeftFoot" },
//		 { CALUMI::SFBGS::AnimationEvent::SyncRightFoot, "SyncRightFoot" },
//		 { CALUMI::SFBGS::AnimationEvent::TakeOff, "TakeOff" },
//		 { CALUMI::SFBGS::AnimationEvent::TakeOff_Neon, "TakeOff_Neon" },
//		 { CALUMI::SFBGS::AnimationEvent::TakeOffE3, "TakeOffE3" },
//		 { CALUMI::SFBGS::AnimationEvent::TakeOffFinished, "TakeOffFinished" },
//		 { CALUMI::SFBGS::AnimationEvent::TakeOffNPC, "TakeOffNPC" },
//		 { CALUMI::SFBGS::AnimationEvent::TakeOffSlave, "TakeOffSlave" },
//		 { CALUMI::SFBGS::AnimationEvent::TakeOffStarborn, "TakeOffStarborn" },
//		 { CALUMI::SFBGS::AnimationEvent::TakeOffStarbornNPC, "TakeOffStarbornNPC" },
//		 { CALUMI::SFBGS::AnimationEvent::TakeOffVertical, "TakeOffVertical" },
//		 { CALUMI::SFBGS::AnimationEvent::TauStart, "TauStart" },
//		 { CALUMI::SFBGS::AnimationEvent::TauStop, "TauStop" },
//		 { CALUMI::SFBGS::AnimationEvent::TerminateGraphRagdoll, "TerminateGraphRagdoll" },
//		 { CALUMI::SFBGS::AnimationEvent::TerrormorphRevealAnkylosaurusStart, "TerrormorphRevealAnkylosaurusStart" },
//		 { CALUMI::SFBGS::AnimationEvent::TerrormorphRevealStart, "TerrormorphRevealStart" },
//		 { CALUMI::SFBGS::AnimationEvent::terrormorphSmash, "terrormorphSmash" },
//		 { CALUMI::SFBGS::AnimationEvent::TerrormorphTransformationStart, "TerrormorphTransformationStart" },
//		 { CALUMI::SFBGS::AnimationEvent::test, "test" },
//		 { CALUMI::SFBGS::AnimationEvent::ThetaStart, "ThetaStart" },
//		 { CALUMI::SFBGS::AnimationEvent::ThetaStop, "ThetaStop" },
//		 { CALUMI::SFBGS::AnimationEvent::ThrowGrenadeStart, "ThrowGrenadeStart" },
//		 { CALUMI::SFBGS::AnimationEvent::ThrowInterrupt, "ThrowInterrupt" },
//		 { CALUMI::SFBGS::AnimationEvent::ThrowMineStart, "ThrowMineStart" },
//		 { CALUMI::SFBGS::AnimationEvent::to_PoseA, "to_PoseA" },
//		 { CALUMI::SFBGS::AnimationEvent::to_PoseB, "to_PoseB" },
//		 { CALUMI::SFBGS::AnimationEvent::to_PoseC, "to_PoseC" },
//		 { CALUMI::SFBGS::AnimationEvent::to_ShipDamageState01, "to_ShipDamageState01" },
//		 { CALUMI::SFBGS::AnimationEvent::to_ShipDamageState02, "to_ShipDamageState02" },
//		 { CALUMI::SFBGS::AnimationEvent::to_ShipFullHealth, "to_ShipFullHealth" },
//		 { CALUMI::SFBGS::AnimationEvent::to_ShipFullyDamagedEngine, "to_ShipFullyDamagedEngine" },
//		 { CALUMI::SFBGS::AnimationEvent::to_ShipPartialHealthEngine, "to_ShipPartialHealthEngine" },
//		 { CALUMI::SFBGS::AnimationEvent::Trans01, "Trans01" },
//		 { CALUMI::SFBGS::AnimationEvent::Trans02, "Trans02" },
//		 { CALUMI::SFBGS::AnimationEvent::Trans03, "Trans03" },
//		 { CALUMI::SFBGS::AnimationEvent::Trans04, "Trans04" },
//		 { CALUMI::SFBGS::AnimationEvent::TransitionComplete, "TransitionComplete" },
//		 { CALUMI::SFBGS::AnimationEvent::TransitionStart, "TransitionStart" },
//		 { CALUMI::SFBGS::AnimationEvent::TransitionStop, "TransitionStop" },
//		 { CALUMI::SFBGS::AnimationEvent::TraversalFinished, "TraversalFinished" },
//		 { CALUMI::SFBGS::AnimationEvent::TraversalStart, "TraversalStart" },
//		 { CALUMI::SFBGS::AnimationEvent::TraversalStepDone, "TraversalStepDone" },
//		 { CALUMI::SFBGS::AnimationEvent::TraversalStop, "TraversalStop" },
//		 { CALUMI::SFBGS::AnimationEvent::TunnelingExit, "TunnelingExit" },
//		 { CALUMI::SFBGS::AnimationEvent::TunnelStart, "TunnelStart" },
//		 { CALUMI::SFBGS::AnimationEvent::TunnelStop, "TunnelStop" },
//		 { CALUMI::SFBGS::AnimationEvent::TurnColorBlue, "TurnColorBlue" },
//		 { CALUMI::SFBGS::AnimationEvent::TurnColorDefault, "TurnColorDefault" },
//		 { CALUMI::SFBGS::AnimationEvent::TurnColorGreen, "TurnColorGreen" },
//		 { CALUMI::SFBGS::AnimationEvent::TurnColorOrange, "TurnColorOrange" },
//		 { CALUMI::SFBGS::AnimationEvent::TurnColorRed, "TurnColorRed" },
//		 { CALUMI::SFBGS::AnimationEvent::TurnColorViolet, "TurnColorViolet" },
//		 { CALUMI::SFBGS::AnimationEvent::TurnColorWhite, "TurnColorWhite" },
//		 { CALUMI::SFBGS::AnimationEvent::TurnColorYellow, "TurnColorYellow" },
//		 { CALUMI::SFBGS::AnimationEvent::TurnLeft, "TurnLeft" },
//		 { CALUMI::SFBGS::AnimationEvent::TurnOff, "TurnOff" },
//		 { CALUMI::SFBGS::AnimationEvent::TurnOffFull, "TurnOffFull" },
//		 { CALUMI::SFBGS::AnimationEvent::TurnOn, "TurnOn" },
//		 { CALUMI::SFBGS::AnimationEvent::TurnOnFlicker, "TurnOnFlicker" },
//		 { CALUMI::SFBGS::AnimationEvent::TurnOnFull, "TurnOnFull" },
//		 { CALUMI::SFBGS::AnimationEvent::TurnOnPulse, "TurnOnPulse" },
//		 { CALUMI::SFBGS::AnimationEvent::TurnOnPulseFast, "TurnOnPulseFast" },
//		 { CALUMI::SFBGS::AnimationEvent::TurnOnPulseFlicker, "TurnOnPulseFlicker" },
//		 { CALUMI::SFBGS::AnimationEvent::TurnOnPulseSlow, "TurnOnPulseSlow" },
//		 { CALUMI::SFBGS::AnimationEvent::TurnRight, "TurnRight" },
//		 { CALUMI::SFBGS::AnimationEvent::TurnStateDefault, "TurnStateDefault" },
//		 { CALUMI::SFBGS::AnimationEvent::TurnStop, "TurnStop" },
//		 { CALUMI::SFBGS::AnimationEvent::Unassemble, "Unassemble" },
//		 { CALUMI::SFBGS::AnimationEvent::UnconsciousStart, "UnconsciousStart" },
//		 { CALUMI::SFBGS::AnimationEvent::UnconsciousStop, "UnconsciousStop" },
//		 { CALUMI::SFBGS::AnimationEvent::UncullBone, "UncullBone" },
//		 { CALUMI::SFBGS::AnimationEvent::UnCullFurnitureBone, "UnCullFurnitureBone" },
//		 { CALUMI::SFBGS::AnimationEvent::UncullWeapons, "UncullWeapons" },
//		 { CALUMI::SFBGS::AnimationEvent::UnDock, "UnDock" },
//		 { CALUMI::SFBGS::AnimationEvent::Undock_Bottom, "Undock_Bottom" },
//		 { CALUMI::SFBGS::AnimationEvent::Undock_Nose, "Undock_Nose" },
//		 { CALUMI::SFBGS::AnimationEvent::Undock_Port, "Undock_Port" },
//		 { CALUMI::SFBGS::AnimationEvent::Undock_Starboard, "Undock_Starboard" },
//		 { CALUMI::SFBGS::AnimationEvent::Undock_Top, "Undock_Top" },
//		 { CALUMI::SFBGS::AnimationEvent::UndockingFinished, "UndockingFinished" },
//		 { CALUMI::SFBGS::AnimationEvent::UndockStarborn, "UndockStarborn" },
//		 { CALUMI::SFBGS::AnimationEvent::Unequip, "Unequip" },
//		 { CALUMI::SFBGS::AnimationEvent::UnequipComplete, "UnequipComplete" },
//		 { CALUMI::SFBGS::AnimationEvent::UnequipInterrupted, "UnequipInterrupted" },
//		 { CALUMI::SFBGS::AnimationEvent::UnequipInterrupted_Death, "UnequipInterrupted_Death" },
//		 { CALUMI::SFBGS::AnimationEvent::Unlink, "Unlink" },
//		 { CALUMI::SFBGS::AnimationEvent::Unlock, "Unlock" },
//		 { CALUMI::SFBGS::AnimationEvent::Unpowered, "Unpowered" },
//		 { CALUMI::SFBGS::AnimationEvent::UpsilonStart, "UpsilonStart" },
//		 { CALUMI::SFBGS::AnimationEvent::UpsilonStop, "UpsilonStop" },
//		 { CALUMI::SFBGS::AnimationEvent::UseStimpak, "UseStimpak" },
//		 { CALUMI::SFBGS::AnimationEvent::VehicleDeath, "VehicleDeath" },
//		 { CALUMI::SFBGS::AnimationEvent::VehicleForwardCollisionHard, "VehicleForwardCollisionHard" },
//		 { CALUMI::SFBGS::AnimationEvent::VehicleIdleStart, "VehicleIdleStart" },
//		 { CALUMI::SFBGS::AnimationEvent::VehicleIdleStop, "VehicleIdleStop" },
//		 { CALUMI::SFBGS::AnimationEvent::VehicleJumpLand, "VehicleJumpLand" },
//		 { CALUMI::SFBGS::AnimationEvent::VehicleJumpStart, "VehicleJumpStart" },
//		 { CALUMI::SFBGS::AnimationEvent::VideoOnly_Shot11, "VideoOnly_Shot11" },
//		 { CALUMI::SFBGS::AnimationEvent::VideoOnly_Shot12, "VideoOnly_Shot12" },
//		 { CALUMI::SFBGS::AnimationEvent::VideoOnly_Shot13, "VideoOnly_Shot13" },
//		 { CALUMI::SFBGS::AnimationEvent::VideoOnly_Shot14, "VideoOnly_Shot14" },
//		 { CALUMI::SFBGS::AnimationEvent::VideoOnly_Shot15, "VideoOnly_Shot15" },
//		 { CALUMI::SFBGS::AnimationEvent::VideoOnly_Shot21, "VideoOnly_Shot21" },
//		 { CALUMI::SFBGS::AnimationEvent::VideoOnly_Shot21b, "VideoOnly_Shot21b" },
//		 { CALUMI::SFBGS::AnimationEvent::Violet, "Violet" },
//		 { CALUMI::SFBGS::AnimationEvent::WalkStart, "WalkStart" },
//		 { CALUMI::SFBGS::AnimationEvent::WeaponAttach, "WeaponAttach" },
//		 { CALUMI::SFBGS::AnimationEvent::WeaponDraw, "WeaponDraw" },
//		 { CALUMI::SFBGS::AnimationEvent::WeaponFire, "WeaponFire" },
//		 { CALUMI::SFBGS::AnimationEvent::WeaponFireEnd, "WeaponFireEnd" },
//		 { CALUMI::SFBGS::AnimationEvent::WeaponFireStart, "WeaponFireStart" },
//		 { CALUMI::SFBGS::AnimationEvent::WeaponSheathe, "WeaponSheathe" },
//		 { CALUMI::SFBGS::AnimationEvent::WeaponSheatheInterrupt, "WeaponSheatheInterrupt" },
//		 { CALUMI::SFBGS::AnimationEvent::WeaponSwing, "WeaponSwing" },
//		 { CALUMI::SFBGS::AnimationEvent::White, "White" },
//		 { CALUMI::SFBGS::AnimationEvent::XiStart, "XiStart" },
//		 { CALUMI::SFBGS::AnimationEvent::XiStop, "XiStop" },
//		 { CALUMI::SFBGS::AnimationEvent::Yellow, "Yellow" },
//		 { CALUMI::SFBGS::AnimationEvent::ZeroOutCameraPitch, "ZeroOutCameraPitch" },
//		 { CALUMI::SFBGS::AnimationEvent::ZetaStart, "ZetaStart" },
//		 { CALUMI::SFBGS::AnimationEvent::ZetaStop, "ZetaStop" },
//
//			//END XTBL EVENTS
//		 {CALUMI::SFBGS::AnimationEvent::AdditiveEnd, "AdditiveEnd"}
//
//		};
//
//		const char* SFBGS::AnimationEventToString(AnimationEvent event)
//		{
//			if (EventTable.count(event)) return EventTable[event];
//
//			return "";
//		}
//
//		AnimationEvent AnimationEventFromString(const char* eventStr)
//		{
//			if (std::string(eventStr).length() == 0) return AnimationEvent::NONE;
//
//			for (auto& entry : EventTable)
//			{
//				if (std::strcmp(eventStr, entry.second) == 0) return entry.first;
//			}
//
//			return AnimationEvent::UNKNOWN;
//		}
//
//		EventEntry& SFBGS::EventEntry::operator=(const AnimationEvent& eValue)
//		{
//			assign(AnimationEventToString(eValue));
//			return *this;
//		}
//
//		EventEntry& EventEntry::operator=(unsigned int iValue)
//		{
//			assign(AnimationEventToString(static_cast<AnimationEvent>(iValue)));
//			return *this;
//		}
//
//		AnimationEvent EventEntry::Enum() const
//		{
//			return AnimationEventFromString(c_str());
//		}
//
//#pragma endregion
//
//#pragma region Sync Type Static Map
//		static std::unordered_map<AnimationSyncType, const char*> SyncTypeTable = {
//			{ AnimationSyncType::NONE, "" },
//			{ AnimationSyncType::SyncBoltCharge, "SyncBoltCharge" },
//			{ AnimationSyncType::SyncCastPower, "SyncCastPower" },
//			{ AnimationSyncType::SyncChargeDown, "SyncChargeDown" },
//			{ AnimationSyncType::SyncChargeHold, "SyncChargeHold" },
//			{ AnimationSyncType::SyncChargeUp, "SyncChargeUp" },
//			{ AnimationSyncType::SyncEnterFurniture, "SyncEnterFurniture" },
//			{ AnimationSyncType::SyncEquip, "SyncEquip" },
//			{ AnimationSyncType::SyncFireAuto, "SyncFireAuto" },
//			{ AnimationSyncType::SyncFireSingle, "SyncFireSingle" },
//			{ AnimationSyncType::SyncFootsteps, "SyncFootsteps" },
//			{ AnimationSyncType::SyncGunActions, "SyncGunActions" },
//			{ AnimationSyncType::SyncIdle, "SyncIdle" },
//			{ AnimationSyncType::SyncJumpBoost, "SyncJumpBoost" },
//			{ AnimationSyncType::SyncJumpFall, "SyncJumpFall" },
//			{ AnimationSyncType::SyncJumpFallLoop, "SyncJumpFallLoop" },
//			{ AnimationSyncType::SyncJumpLand, "SyncJumpLand" },
//			{ AnimationSyncType::SyncJumpLandCamera, "SyncJumpLandCamera" },
//			{ AnimationSyncType::SyncJumpStart, "SyncJumpStart" },
//			{ AnimationSyncType::SyncMeleeAttack, "SyncMeleeAttack" },
//			{ AnimationSyncType::SyncMeleeAttackPower, "SyncMeleeAttackPower" },
//			{ AnimationSyncType::SyncReload, "SyncReload" },
//			{ AnimationSyncType::SyncReloadEnd, "SyncReloadEnd" },
//			{ AnimationSyncType::SyncReloadLoop, "SyncReloadLoop" },
//			{ AnimationSyncType::SyncSlide, "SyncSlide" },
//			{ AnimationSyncType::SyncStandToRun, "SyncStandToRun" },
//			{ AnimationSyncType::SyncThrowGrenade, "SyncThrowGrenade" },
//			{ AnimationSyncType::SyncThrowMine, "SyncThrowMine" },
//			{ AnimationSyncType::SyncTurning, "SyncTurning" },
//			{ AnimationSyncType::SyncUnequip, "SyncUnequip" }
//		};
//
//		const char* AnimationSyncTypeToString(AnimationSyncType sType)
//		{
//			if (SyncTypeTable.count(sType)) return SyncTypeTable[sType];
//
//			return "";
//		}
//
//		AnimationSyncType AnimationSyncTypeFromString(const char* sTypeStr)
//		{
//			if (std::string(sTypeStr).length() == 0) return AnimationSyncType::NONE;
//
//			for (auto& entry : SyncTypeTable)
//			{
//				if (std::strcmp(sTypeStr, entry.second) == 0) return entry.first;
//			}
//
//			return AnimationSyncType::UNKNOWN;
//		}
//
//		SyncEntry& SyncEntry::operator=(unsigned int iValue)
//		{
//			assign(AnimationSyncTypeToString(static_cast<AnimationSyncType>(iValue)));
//			return *this;
//		}
//
//		SyncEntry& SyncEntry::operator=(const AnimationSyncType& eValue)
//		{
//			assign(AnimationSyncTypeToString(eValue));
//			return *this;
//		}
//
//		AnimationSyncType SyncEntry::Enum() const
//		{
//			return AnimationSyncTypeFromString(c_str());
//		}
//
//#pragma endregion
//
//#pragma region EventIOComponent
//
//		bool EventIOComponent::operator==(const EventIOComponent& other) const
//		{
//			return Non_Instanced == other.Non_Instanced && Event == other.Event && Payload == other.Payload;
//		}
//
//		bool EventIOComponent::operator!=(const EventIOComponent& other) const
//		{
//			return Non_Instanced != other.Non_Instanced || Event != other.Event || Payload != other.Payload;
//		}
//
//#pragma endregion
//
//#pragma region ImpactTagComponent
//
//		bool ImpactTag::operator==(const ImpactTag& other) const
//		{
//			return Tag_Name == other.Tag_Name && Body_Part == other.Body_Part && Impact == other.Impact;
//		}
//
//		bool ImpactTag::operator!=(const ImpactTag& other) const
//		{
//			return Tag_Name != other.Tag_Name || Body_Part != other.Body_Part || Impact != other.Impact;
//		}
//
//#pragma endregion
//
//		
//#pragma region VariableAssignmentComponent
//
//		const char* VariableAssignmentComponent::GetValue() const
//		{
//			return Value.c_str();
//		}
//
//		AnimationVariableType VariableAssignmentComponent::GetVariableType() const
//		{
//			return _Type;
//		}
//
//		VariableAssignmentComponent::VariableAssignmentComponent(const AnimationVariable& Variable_Name, const Utilities::StringContainer& Value)
//			: Variable_Name(Variable_Name)
//		{
//			Assign(Value.c_str());
//		}
//
//		void VariableAssignmentComponent::Assign(AnimationVariable vName, const char* sVal)
//		{
//			Variable_Name = vName;
//			Assign(sVal);
//		}
//
//		void VariableAssignmentComponent::Assign(const char* sVal)
//		{
//			_Type = GetAnimationVariableType(Variable_Name);
//			switch (_Type)
//			{
//			case CALUMI::SFBGS::AnimationVariableType::Integer:
//			{
//				intEntry iCheck(sVal);
//				auto iResult = iCheck.GetValueAsType();
//				this->Value = std::to_string(iResult).c_str();
//				break;
//			}
//			case CALUMI::SFBGS::AnimationVariableType::Float:
//			{
//				floatEntry fCheck(sVal);
//				auto fResult = fCheck.GetValueAsType();
//				this->Value = std::to_string(fResult).c_str();
//				break;
//			}
//			case CALUMI::SFBGS::AnimationVariableType::String:
//				this->Value = sVal;
//				break;
//			case CALUMI::SFBGS::AnimationVariableType::Boolean:
//			{
//				boolEntry bCheck(sVal);
//				auto bResult = bCheck.GetValueAsType();
//				this->Value = std::to_string(bResult).c_str();
//				break;
//			}
//			case CALUMI::SFBGS::AnimationVariableType::Vector:
//			{
//				vectorEntry vCheck(sVal);
//				Math::Vector3 vResult = vCheck.GetValueAsType();
//				vectorEntry holder;
//				holder = vResult;
//				this->Value = holder.c_str();
//				break;
//			}
//			default:
//				this->Value = "";
//				break;
//			}
//		}
//
//#pragma endregion
//
//}
//}