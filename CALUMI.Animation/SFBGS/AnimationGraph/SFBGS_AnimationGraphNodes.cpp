#include "pch.h"
#include "SFBGS_AnimationGraphNodes.h"

//namespace CALUMI {
//	namespace SFBGS {
//
//		AgxNodeType CALUMI::SFBGS::AgxNtAnimationNode::GetNodeType()
//		{
//			return AgxNodeType::NT_ANIMATION_NODE;
//		}
//
//		/*const AgxPort& AgxNtAnimationNode::AddInput()
//		{
//			inputs.push_back(new AgxPort_AnimationNode(this, nextId));
//			nextId++;
//			return *inputs.at(inputs.size() - 1);
//		}
//
//		const AgxPort& AgxNtAnimationNode::AddOutput()
//		{
//			outputs.push_back(new AgxPort_AnimationNode(this, nextId));
//			nextId++;
//			return *outputs.at(outputs.size() - 1);
//		}*/
//
//		bool AgxNode::IsPortInput(const AgxPort& port) const
//		{
//			for (size_t i = 0; i < inputs.size(); i++)
//			{
//				if (inputs.at(i).get() == &port) return true;
//			}
//			return false;
//		}
//
//		bool AgxNode::IsPortOutput(const AgxPort& port) const
//		{
//			for (size_t i = 0; i < outputs.size(); i++)
//			{
//				if (inputs.at(i).get() == &port) return true;
//			}
//			return false;
//		}
//
//		const AgxPort& AgxNode::AddInput()
//		{
//			inputs.push_back(new AgxPortInput(this, nextId));
//			nextId++;
//			return *inputs.at(inputs.size() - 1);
//		}
//
//		bool AgxNode::RemoveInput(size_t idx)
//		{
//			if (inputs.size() > idx)
//			{
//				inputs.erase(idx);
//				return true;
//			}
//			return false;
//		}
//
//		bool AgxNode::RemoveInput(AgxPort& port)
//		{
//			for (size_t i = 0; i < inputs.size(); i++)
//			{
//				if (inputs.at(i).get() == &port)
//				{
//					inputs.erase(i);
//					return true;
//				}
//			}
//			return false;
//		}
//
//		const AgxPort& AgxNode::AddOutput()
//		{
//			outputs.push_back(new AgxPortOutput(this, nextId));
//			nextId++;
//			return *outputs.at(outputs.size() - 1);
//		}
//
//		bool AgxNode::RemoveOutput(size_t idx)
//		{
//			if (outputs.size() > idx)
//			{
//				outputs.erase(idx);
//				return true;
//			}
//			return false;
//		}
//
//		bool AgxNode::RemoveOutput(AgxPort& port)
//		{
//			for (size_t i = 0; i < outputs.size(); i++)
//			{
//				if (outputs.at(i).get() == &port)
//				{
//					outputs.erase(i);
//					return true;
//				}
//			}
//			return false;
//		}
//
//		AgxNodeType CALUMI::SFBGS::AgxNtBlendNode::GetNodeType()
//		{
//			return AgxNodeType::NT_BLEND_NODE;
//		}
//
//		AgxNodeType CALUMI::SFBGS::AgxNtAssignIState::GetNodeType()
//		{
//			return AgxNodeType::NT_ASSIGN_ISTATE;
//		}
//
//		AgxNodeType SFBGS::AgxNtAssignVariable::GetNodeType()
//		{
//			return AgxNodeType::NT_ASSIGN_VARIABLE;
//		}
//
//		AgxNodeType SFBGS::AgxNtBoneConstraint::GetNodeType()
//		{
//			return AgxNodeType::NT_BONE_CONSTRAINT;
//		}
//
//		AgxNodeType AgxNtClonePose::GetNodeType()
//		{
//			return AgxNodeType::NT_CLONE_POSE;
//		}
//
//		AgxNodeType AgxNtBlendTreeEmbedded::GetNodeType()
//		{
//			return AgxNodeType::NT_BLEND_TREE_EMBEDDED;
//		}
//
//		AgxNodeType AgxNtAnimationImpactSelector::GetNodeType()
//		{
//			return AgxNodeType::NT_ANIMATION_IMPACT_SELECTOR;
//		}
//
//		AgxNodeType AgxNtCopyBoneWeights::GetNodeType()
//		{
//			return AgxNodeType::NT_COPY_BONE_WEIGHTS;
//		}
//
//		AgxNodeType AgxNtConvertBoneDataToVariables::GetNodeType()
//		{
//			return AgxNodeType::NT_CONVERT_BONE_DATA_TO_VARIABLES;
//		}
//
//		AgxNodeType AgxNtCumulativeAnimation::GetNodeType()
//		{
//			return AgxNodeType::NT_CUMULATIVE_ANIMATION;
//		}
//
//		AgxNodeType AgxNtCurvedPathBlender::GetNodeType()
//		{
//			return AgxNodeType::NT_CURVED_PATH_BLENDER;
//		}
//
//		AgxNodeType AgxNtDirectAt::GetNodeType()
//		{
//			return AgxNodeType::NT_DIRECT_AT;
//		}
//
//		AgxNodeType AgxNtDualDirectAt::GetNodeType()
//		{
//			return AgxNodeType::NT_DUAL_DIRECT_AT;
//		}
//
//		AgxNodeType AgxNtDynaimcAnimation::GetNodeType()
//		{
//			return AgxNodeType::NT_DYNAMIC_ANIMATION;
//		}
//
//		AgxNodeType AgxNtDynamicGraphReference::GetNodeType()
//		{
//			return AgxNodeType::NT_DYNAMIC_GRAPH_REFERENCE;
//		}
//
//		AgxNodeType AgxNtEffectSequence::GetNodeType()
//		{
//			return AgxNodeType::NT_EFFECT_SEQUENCE;
//		}
//
//		AgxNodeType AgxNtEvaluateConditionVariable::GetNodeType()
//		{
//			return AgxNodeType::NT_EVALUATE_CONDITION_VARIABLE;
//		}
//
//		AgxNodeType AgxNtEventController::GetNodeType()
//		{
//			return AgxNodeType::NT_EVENT_CONTROLLER;
//		}
//
//		AgxNodeType AgxNtEveryNEventsModifier::GetNodeType()
//		{
//			return AgxNodeType::NT_EVERY_N_EVENTS_MODIFIER;
//		}
//
//		AgxNodeType AgxNtFootIk::GetNodeType()
//		{
//			return AgxNodeType::NT_FOOT_IK;
//		}
//
//		AgxNodeType AgxNtGamebryoSequence::GetNodeType()
//		{
//			return AgxNodeType::NT_GAMEBRYO_SEQUENCE;
//		}
//
//		AgxNodeType AgxNtGraphRef::GetNodeType()
//		{
//			return AgxNodeType::NT_GRAPH_REF;
//		}
//
//		AgxNodeType AgxNtLinearVariable::GetNodeType()
//		{
//			return AgxNodeType::NT_LINEAR_VARIABLE;
//		}
//
//		AgxNodeType AgxNtLocomotionBlend::GetNodeType()
//		{
//			return AgxNodeType::NT_LOCOMOTION_BLEND;
//		}
//
//		AgxNodeType AgxNtMassSpringDampenVariable::GetNodeType()
//		{
//			return AgxNodeType::NT_MASS_SPRING_DAMPEN_VARIABLE;
//		}
//
//		AgxNodeType AgxNtMergeNode::GetNodeType()
//		{
//			return AgxNodeType::NT_MERGE_NODE;
//		}
//
//		AgxNodeType AgxNtMirrorModifier::GetNodeType()
//		{
//			return AgxNodeType::NT_MIRROR_MODIFIER;
//		}
//
//		AgxNodeType AgxNtMomentumAnimation::GetNodeType()
//		{
//			return AgxNodeType::NT_MOMENTUM_ANIMATION;
//		}
//
//		AgxNodeType AgxNtMomentimSwitchBack::GetNodeType()
//		{
//			return AgxNodeType::NT_MOMENTUM_SWITCHBACK;
//		}
//
//		AgxNodeType AgxNtMultiFootIK::GetNodeType()
//		{
//			return AgxNodeType::NT_MULTI_FOOT_IK;
//		}
//
//		AgxNodeType AgxNtNormalizeRotation::GetNodeType()
//		{
//			return AgxNodeType::NT_NORMALIZE_ROTATION;
//		}
//
//		AgxNodeType AgxNtPairedAnimation::GetNodeType()
//		{
//			return AgxNodeType::NT_PAIRED_ANIMATION;
//		}
//
//		AgxNodeType AgxNtParticleSequence::GetNodeType()
//		{
//			return AgxNodeType::NT_PARTICLE_SEQUENCE;
//		}
//
//		AgxNodeType AgxNtPathingAnimations::GetNodeType()
//		{
//			return AgxNodeType::NT_PATHING_ANIMATIONS;
//		}
//
//		AgxNodeType AgxNtPhysicsContractListener::GetNodeType()
//		{
//			return AgxNodeType::NT_PHYSICS_CONTACT_LISTENER;
//		}
//
//		AgxNodeType AgxNtPostBoneModifierControl::GetNodeType()
//		{
//			return AgxNodeType::NT_POST_BONE_MODIFIER_CONTROL;
//		}
//
//		AgxNodeType AgxNtRagdollDrive::GetNodeType()
//		{
//			return AgxNodeType::NT_RAGDOLL_DRIVE;
//		}
//
//		AgxNodeType AgxNtRagdollGetUp::GetNodeType()
//		{
//			return AgxNodeType::NT_RAGDOLL_GET_UP;
//		}
//
//		AgxNodeType AgxNtRandomAnimationNode::GetNodeType()
//		{
//			return AgxNodeType::NT_RANDOM_ANIMATION_NODE;
//		}
//
//		AgxNodeType AgxNtRigSwitch::GetNodeType()
//		{
//			return AgxNodeType::NT_RIG_SWITCH;
//		}
//
//		AgxNodeType AgxNtRootTwist::GetNodeType()
//		{
//			return AgxNodeType::NT_ROOT_TWIST;
//		}
//
//		AgxNodeType AgxNtRotationVariable::GetNodeType()
//		{
//			return AgxNodeType::NT_ROTATION_VARIABLE;
//		}
//
//		AgxNodeType AgxNtSetOrient::GetNodeType()
//		{
//			return AgxNodeType::NT_SET_ORIENT;
//		}
//
//		AgxNodeType AgxNtSetPos::GetNodeType()
//		{
//			return AgxNodeType::NT_SET_POS;
//		}
//
//		AgxNodeType AgxNtSpeedScale::GetNodeType()
//		{
//			return AgxNodeType::NT_SPEED_SCALE;
//		}
//
//		AgxNodeType AgxNtStaggerMeter::GetNodeType()
//		{
//			return AgxNodeType::NT_STAGGER_METER;
//		}
//
//		AgxNodeType AgxNtStateMachineEmbedded::GetNodeType()
//		{
//			return AgxNodeType::NT_STATE_MACHINE_EMBEDDED;
//		}
//
//		AgxNodeType AgxNtStateVariableControl::GetNodeType()
//		{
//			return AgxNodeType::NT_STATE_VARIABLE_CONTROL;
//		}
//
//		AgxNodeType AgxNtSwapGraph::GetNodeType()
//		{
//			return AgxNodeType::NT_SWAP_GRAPH;
//		}
//
//		AgxNodeType AgxNtSwitchNode::GetNodeType()
//		{
//			return AgxNodeType::NT_SWITCH_NODE;
//		}
//
//		AgxNodeType AgxNtTagPropagation::GetNodeType()
//		{
//			return AgxNodeType::NT_TAG_PROPAGATION;
//		}
//
//		AgxNodeType AgxNtTimerEvent::GetNodeType()
//		{
//			return AgxNodeType::NT_TIMER_EVENT;
//		}
//
//		AgxNodeType AgxNtTranslationAdjustment::GetNodeType()
//		{
//			return AgxNodeType::NT_TRANSLATION_ADJUSTMENT;
//		}
//
//		AgxNodeType AgxNtTwoBoneIK::GetNodeType()
//		{
//			return AgxNodeType::NT_TWO_BONE_IK;
//		}
//
//		AgxNodeType AgxNtVariableCombiner::GetNodeType()
//		{
//			return AgxNodeType::NT_VARIABLE_COMBINER;
//		}
//
//
//		//AgxPort() = default;
//		AgxPort::AgxPort(AgxNode* nodePtr, size_t id) : id(id) { parentNode = Utilities::WeakPtrContainer<AgxNode>(nodePtr); }
//
//		bool AgxPort::connect(const Utilities::SharedPtrContainer<AgxPort>& other)
//		{
//			if (!canLink(*other)) return false;
//			if(!linkedTo.expired()){
//				auto oldLink = linkedTo.lock();
//				oldLink.reset();
//			}
//			if (!other->linkedTo.expired())
//			{
//				auto otherLinkedTo = other->linkedTo.lock();
//				otherLinkedTo.reset();
//			}
//
//			linkedTo = Utilities::WeakPtrContainer<AgxPort>(other);
//			other->linkedTo = Utilities::WeakPtrContainer<AgxPort>(this);
//			return true;
//		}
//
//		bool AgxPort::canLink(const AgxPort& other) const
//		{
//			/*if( isInput == other.GetIsInput()) return false;*/
//			if (parentNode.expired()||other.parentNode.expired()) return false;
//			auto parent = parentNode.lock();
//			auto otherParent = other.parentNode.lock();
//			bool isInput = parent->IsPortInput(*this);
//			bool isOutput = parent->IsPortOutput(*this);
//			if (isInput && otherParent->IsPortInput(other)) return false;
//			if (isOutput && otherParent->IsPortOutput(other)) return false;
//
//			return true;
//		}
//
//}
//	
//}
//
//
