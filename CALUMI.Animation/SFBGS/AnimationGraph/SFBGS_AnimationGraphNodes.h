#pragma once

//#include "..\..\CALUMI_Math.h"
//#include "SFBGS_AnimationGraphFlags.h"
//#include "SFBGS_AnimationGraphComponents.h"
//
//namespace CALUMI {
//	namespace SFBGS {
//
//		enum class CALUMIANIMATION_API AgxNodeType
//		{
//			NT_ANIMATION_CORRECTED_NODE, //Unused
//			NT_ANIMATION_IMPACT_SELECTOR,
//			NT_ANIMATION_NODE,
//			NT_ASSIGN_ISTATE,
//			NT_ASSIGN_VARIABLE,
//			NT_BLEND_NODE,
//			NT_BLEND_TREE_EMBEDDED,
//			NT_BONE_CONSTRAINT,
//			NT_CLONE_POSE,
//			NT_CONVERT_BONE_DATA_TO_VARIABLES,
//			NT_COPY_BONE_WEIGHTS,
//			NT_CRITICALLY_DAMPEN_VARIABLE, //Unused
//			NT_CUMULATIVE_ANIMATION,
//			NT_CURVED_PATH_BLENDER,
//			NT_DAMPEN_VARIABLE, //Unused
//			NT_DIRECT_AT,
//			NT_DUAL_DIRECT_AT,
//			NT_DYNAMIC_ANIMATION,
//			NT_DYNAMIC_GRAPH_REFERENCE,
//			NT_EFFECT_SEQUENCE,
//			NT_EVALUATE_CONDITION_VARIABLE,
//			NT_EVENT_CONTROLLER,
//			NT_EVENT_FROM_RANGE, //Unused
//			NT_EVERY_N_EVENTS_MODIFIER,
//			NT_FOOT_IK,
//			NT_GAMEBRYO_SEQUENCE,
//			NT_GRAPH_REF,
//			NT_INVALID, //Unused
//			NT_LINEAR_VARIABLE,
//			NT_LOCOMOTION_BLEND,
//			NT_LOOK_AT, //Unused
//			NT_MASS_SPRING_DAMPEN_VARIABLE,
//			NT_MATERIAL_LAYER_SEQUENCE,
//			NT_MERGE_NODE,
//			NT_MIRROR_MODIFIER,
//			NT_MODIFY_GROUP, //Unused
//			NT_MOMENTUM_ANIMATION,
//			NT_MOMENTUM_SWITCHBACK,
//			NT_MULTI_FOOT_IK,
//			NT_NORMALIZE_ROTATION,
//			NT_NUM_ANIMATION_NODES, //Unused
//			NT_PAIRED_ANIMATION,
//			NT_PARTICLE_SEQUENCE,
//			NT_PATHING_ANIMATIONS,
//			NT_PHYSICS_CONTACT_LISTENER,
//			NT_POST_BONE_MODIFIER_CONTROL,
//			NT_RAGDOLL, //Unused
//			NT_RAGDOLL_DRIVE,
//			NT_RAGDOLL_GET_UP,
//			NT_RANDOM_ANIMATION_NODE,
//			NT_RIG_SWITCH,
//			NT_ROLLING_BONE, //Unused
//			NT_ROOT_TWIST,
//			NT_ROTATION_VARIABLE,
//			NT_SET_ORIENT,
//			NT_SET_POS,
//			NT_SINGLE_BONE_IK, //Unused
//			NT_SPEED_SCALE,
//			NT_STAGGER_METER,
//			NT_STATE_MACHINE_EMBEDDED,
//			NT_STATE_VARIABLE_CONTROL,
//			NT_SWAP_GRAPH,
//			NT_SWITCH_NODE,
//			NT_TAG_PROPAGATION,
//			NT_TIMER_EVENT,
//			NT_TRANSLATION_ADJUSTMENT,
//			NT_TWO_BONE_IK,
//			NT_VARIABLE_COMBINER,
//
//
//			UNDEFINED = -1
//		};
//
//		struct AgxNode;
//		struct AgxGraph;
//
//		struct CALUMIANIMATION_API AgxPort
//		{
//			strEntry portName;
//
//			//AgxPort() = default;
//			AgxPort(AgxNode* nodePtr, size_t id);
//			virtual ~AgxPort() = default;
//
//			virtual bool connect(const Utilities::SharedPtrContainer<AgxPort>& other);
//			virtual bool canLink(const AgxPort& other) const;
//			
//		protected:
//			Utilities::WeakPtrContainer<AgxPort> linkedTo;
//			Utilities::WeakPtrContainer<AgxNode> parentNode;
//			size_t id = 0;
//
//		};
//
//		struct CALUMIANIMATION_API AgxPortOutput : AgxPort
//		{
//			~AgxPortOutput() override = default;
//			using AgxPort::AgxPort;
//		};
//
//		struct CALUMIANIMATION_API AgxPortInput : AgxPort
//		{
//			~AgxPortInput() override = default;
//			using AgxPort::AgxPort;
//			bool AGX_INCLUDE_DATA : 1 = true;
//			bool Hidden : 1 = false;
//			bool Allow_Self_Transition : 1 = false;
//			bool Do_Not_Follow_Sync_Group : 1 = false;
//			bool Do_Not_Follow_Sync_Group_Is_Set : 1 = false; //Not in original, debug only
//
//			EventEntry Event;
//			strEntry Comment;
//			strEntry Condition;
//			float Blend_Time = 0.1f;
//			int Priority = 0;
//
//			enum class TransType { Smooth = 0, Linear = 1};
//			TransType Transition_Type = TransType::Smooth;
//		};
//
//		struct CALUMIANIMATION_API AgxNode
//		{
//			bool collapsed : 1 = true;
//			bool nonInstanced : 1 = false;
//			bool useColor : 1 = false;
//			bool defaultState : 1 = true;
//			AnimationFlags flags;
//			strEntry Title; //serialized as <name>
//			int userID = 0;
//			Math::Vector2D pos;
//			Math::Vector2D posExpanded;
//
//			virtual AgxNodeType GetNodeType() = 0;
//
//			AgxNode() = default;
//			virtual ~AgxNode() = default;
//
//			bool IsPortInput(const AgxPort& port) const;
//			bool IsPortOutput(const AgxPort& port) const;
//
//			virtual const AgxPort& AddInput();
//			bool RemoveInput(size_t idx);
//			bool RemoveInput(AgxPort& port);
//			virtual const AgxPort& AddOutput();
//			bool RemoveOutput(size_t idx);
//			bool RemoveOutput(AgxPort& port);
//
//		protected:
//			Utilities::VectorContainer<Utilities::SharedPtrContainer<AgxPortInput>> inputs;
//			Utilities::VectorContainer<Utilities::SharedPtrContainer<AgxPortOutput>> outputs;
//			size_t nextId = 0;
//			strEntry guid;
//		};
//
//#pragma region NodeTypes
//
//		struct CALUMIANIMATION_API AgxNtAnimationImpactSelector : AgxNode
//		{
//			SYNC_ONLY_TRANSITION_OUT;
//			strEntry Name;
//			intEntry Body_Part_Var;
//			intEntry Direction_Var;
//			intEntry Impact_Var;
//
//			Utilities::VectorContainer<EventIOComponent> Enter_Events;
//			Utilities::VectorContainer<EventIOComponent> Exit_Events;
//
//			Utilities::VectorContainer<ImpactTag> Forward_Impact;
//			Utilities::VectorContainer<ImpactTag> Back_Impact;
//			Utilities::VectorContainer<ImpactTag> Left_Impact;
//			Utilities::VectorContainer<ImpactTag> Right_Impact;
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtAnimationImpactSelector() { Title = "Animation Impact Selector"; }
//			~AgxNtAnimationImpactSelector() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtAnimationNode : AgxNode
//		{
//			bool ProcessInactiveEvents : 1 = false;
//			bool Mirror : 1 = false;
//			bool Update_Ref_Count_When_Inactive : 1 = false;
//			bool Process_Inactive_Events : 1 = false;
//			bool Randomize_Start_Point : 1 = false;
//			bool Speed_Multiplier_Is_Duration : 1 = false;
//			SYNC_ONLY_TRANSITION_OUT;
//			bool Sync_Slave : 1 = false;
//			SyncEntry Sync_System;
//			int Crop_Start_Frames = 0;
//			int Crop_End_Frames = 0;
//			strEntry TagName;
//			EventEntry Send_Event_On_End;
//			floatEntry Variable_Control;
//			intEntry State_Variable;
//			intEntry State_Variable_Ref_Count;
//			floatEntry Speed_Multiplier;
//
//			Utilities::VectorContainer<EventIOComponent> Enter_Events;
//			Utilities::VectorContainer<EventIOComponent> Exit_Events;
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			//const AgxPort& AddInput() override;
//			//const AgxPort& AddOutput() override;
//
//			AgxNtAnimationNode() { Title = "Animation"; }
//			~AgxNtAnimationNode() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtAssignIState : AgxNode
//		{
//			strEntry Name = "Assign iStates";
//			intEntry iState_Variable = AnimationVariable::iState;
//
//			Utilities::VectorContainer<iStatePair> iState_Entries;
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtAssignIState() { Title = "Assign iStates"; }
//			~AgxNtAssignIState() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtAssignVariable: AgxNode
//		{
//			bool Only_Assign_Once : 1 = false;
//			SYNC_ONLY_TRANSITION_OUT;
//
//			enum class EOperation {Assign, Decrement, Increment};
//			EOperation Operation = EOperation::Assign;
//
//			strEntry Name = "Assign Variable";
//
//			Utilities::VectorContainer<VariableAssignmentComponent> Variable_Assignments;
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtAssignVariable() { Title = "Assign Variable"; }
//			~AgxNtAssignVariable() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtBlendNode : AgxNode
//		{
//			bool Normalize_Weights : 1 = false;
//			bool Only_Highest_Weight_Active : 1 = false;
//			bool Wrap_To_Range : 1 = false;
//
//		private:
//			int8_t Input_Lock_Time = -1; //Always seems to be negative one. Need to confirm if always present or not... IF not always present, should become a bool with TRUE serializing into -1 on the property sheet.
//
//		public:
//			enum class EVariableType : uint8_t { Standard, Layer, Parametric };
//			EVariableType Variable_Type = EVariableType::Standard;
//			floatEntry Variable_Name;
//			intEntry Sync_Variable;
//			strEntry Name = "Blend Node";
//			float Range_Max = 1.0f;
//			float Range_Min = 0.0f;
//			floatEntry Max_Blend_Time = 0.5f;
//
//
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//
//			AgxNtBlendNode() { Title = "Blend"; }
//			~AgxNtBlendNode() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtBoneConstraint : AgxNode
//		{
//			strEntry Name;
//			intEntry Activate_Variable;
//			strEntry Source_Bone;
//			strEntry Target_Bone;
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtBoneConstraint() = default;
//			~AgxNtBoneConstraint() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtClonePose : AgxNode
//		{
//			strEntry Name = "Clone Pose";
//			EventEntry Event;
//			EventEntry Resume_Event;
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtClonePose() = default;
//			~AgxNtClonePose() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtConvertBoneDataToVariables : AgxNode
//		{
//			strEntry Name = "Convert Bone to Var";
//
//			AgxNtConvertBoneDataToVariables() = default;
//			~AgxNtConvertBoneDataToVariables() override = default;
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//		};
//
//		struct CALUMIANIMATION_API AgxNtCopyBoneWeights : AgxNode
//		{
//			float Blend_Rate = 0.5f;
//			strEntry Name = "Copy Bone Weights";
//			intEntry ControlVariable;
//			Utilities::VectorContainer<EventEntry> Activate_Base_Event_List;
//
//			AgxNtCopyBoneWeights() = default;
//			~AgxNtCopyBoneWeights() override = default;
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//		};
//
//		struct CALUMIANIMATION_API AgxNtCumulativeAnimation: AgxNode
//		{
//			bool Clean_Up_Only : 1 = false;
//			bool Reset_After_Max_Iterations : 1 = false;
//			strEntry Name = "Cumulative Animation";
//			intEntry Max_Cumulative_Animation;
//			EventEntry Event_to_Reset_All_Cumulative_Animations;
//			EventEntry Event_Sent_After_Max_Iterations;
//			intEntry Current_Cumulative_Count;
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtCumulativeAnimation() = default;
//			~AgxNtCumulativeAnimation() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtCurvedPathBlender : AgxNode
//		{
//			strEntry Name = "Curved Path Blender";
//			float Blend_Rate = 0.5f;
//
//			AgxNtCurvedPathBlender() = default;
//			~AgxNtCurvedPathBlender() override = default;
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//		};
//
//		struct CALUMIANIMATION_API AgxNtDirectAt : AgxNode
//		{
//			float Blend_Rate = 0.5f;
//			float Disable_Heading_Max = 90.0f;
//			float Disable_Heading_Min = -90.0f;
//			floatEntry Clamp_Heading_Max;
//			floatEntry Clamp_Heading_Min;
//			floatEntry Clamp_Pitch_Max;
//			floatEntry Clamp_Pitch_Min;
//			vectorEntry DirectAtVector_Variable;
//			intEntry Disable_Variable;
//			floatEntry Heading_Delta_Variable;
//			floatEntry Heading_Lag_Degrees;
//			floatEntry Heading_Offset_Output;
//			boolEntry Is_Active_Var;
//			strEntry Name = "Direct At";
//			floatEntry Pitch_Offset_Output;
//			strEntry Source_Bone;
//			strEntry Source_Location_Bone;
//			
//			AgxNtDirectAt() = default;
//			~AgxNtDirectAt() override = default;
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//		};
//
//		struct CALUMIANIMATION_API AgxNtDualDirectAt : AgxNode
//		{
//			bool Rotate_Head_Before_Chest : 1 = true;
//			float Lag_Degrees = 0.0f;
//			floatEntry Head_Blend_Rate;
//			strEntry Name = "Dual Direct At";
//			floatEntry Aim_Heading_Offset_Output;
//			floatEntry Aim_Pitch_Offset_Output;
//			vectorEntry Aim_Vector_Variable;
//			float Blend_Rate = 0.5f;
//			int Chest_Axis_Index = 1;
//			int Chest_Full_Blend_Max_Angle = 0;
//			strEntry Chest_Location_Bone;
//			float Disable_Heading_Hysteresis = 10.0f;
//			floatEntry Disable_Heading_Max;
//			floatEntry Disable_Heading_Min;
//			floatEntry Head_Lag_Degrees;
//			strEntry Head_Location_Bone;
//			floatEntry Heading_Delta_Variable = AnimationVariable::DirectAt_Heading_Delta;
//			floatEntry Heading_Offset_Output = AnimationVariable::LookAtHeadingVar;
//			boolEntry Is_Aim_Active_Variable = AnimationVariable::bAimActive;
//			boolEntry Is_Head_Tracking_Active_Variable = AnimationVariable::bHeadTrackingActive;
//			vectorEntry Look_Vector_Variable = AnimationVariable::LookAtLocation;
//			intEntry No_Aim_Variable = AnimationVariable::bNoAim;
//			intEntry No_Head_Track_Variable = AnimationVariable::bNoHeadTrack;
//			floatEntry Pitch_Offset_Output = AnimationVariable::LookAtPitchVar;
//			strEntry Source_Bone;
//
//			AgxNtDualDirectAt() = default;
//			~AgxNtDualDirectAt() override = default;
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//		};
//
//		struct CALUMIANIMATION_API AgxNtDynaimcAnimation : AgxNode
//		{
//			bool Is_Action_Animation : 1 = false;
//			bool Mirror : 1 = false;
//			SYNC_ONLY_TRANSITION_OUT;
//			strEntry Name = "Dynamic animation";
//			EventEntry Send_Event_On_End;
//			intEntry State_Variable;
//			SyncEntry Sync_System;
//
//			Utilities::VectorContainer<EventIOComponent> Enter_Events;
//			Utilities::VectorContainer<EventIOComponent> Exit_Events;
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtDynaimcAnimation() = default;
//			~AgxNtDynaimcAnimation() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtDynamicGraphReference : AgxNode
//		{
//			SYNC_ONLY_TRANSITION_OUT;
//			float Blend_Time = 1.0f;
//			strEntry Name = "Dynamic Graph Reference";
//			strEntry Starting_Graph;
//			EventEntry Swap_Graph_Event;
//
//			Utilities::VectorContainer<strEntry> Graph_Names;
//
//			AgxNtDynamicGraphReference() = default;
//			~AgxNtDynamicGraphReference() override = default;
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//		};
//
//		struct CALUMIANIMATION_API AgxNtEffectSequence : AgxNode
//		{
//			bool Allow_No_Effect : 1 = false;
//			bool Has_Looping_Segment : 1 = false;
//			bool Initialize_Sequence_On_Load : 1 = false;
//			SYNC_ONLY_TRANSITION_OUT;
//			BlendModeFunction Blend_Mode_Function = BlendModeFunction::None;
//			int Blend_Out_Frames = 0;
//			float Weight = 0.0f;
//			float Time_Percent_Max = 100.0f;
//			float Time_Percent_Min = 0.0f;
//			strEntry Name = "Effect Sequence";
//			EventEntry Send_Event_On_End;
//			intEntry Sequence_Current_Frame_Index;
//			strEntry Sequence_Name;
//			floatEntry Speed_Multiplier;
//			floatEntry Time_Percent;
//
//			Utilities::VectorContainer<EventIOComponent> Enter_Events;
//			Utilities::VectorContainer<EventIOComponent> Exit_Events;
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtEffectSequence() = default;
//			~AgxNtEffectSequence() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtEvaluateConditionVariable : AgxNode
//		{
//			strEntry Name = "Evaluate Condition Variable";
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtEvaluateConditionVariable() = default;
//			~AgxNtEvaluateConditionVariable() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtEventController : AgxNode
//		{
//			bool Discard_Events : 1 = false;
//			bool Negate_variable : 1 = false;
//			intEntry Logic_Variable = "1";
//			strEntry Name = "Event Controller";
//			EventEntry Release_Event;
//			EventEntry Send_Event_On_Catch;
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtEventController() = default;
//			~AgxNtEventController() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtEveryNEventsModifier : AgxNode
//		{
//			int Max_Times = 1;
//			int Min_Times = 0;
//			strEntry Name = "Every N Events";
//			EventEntry Send_Event = AnimationEvent::IdleFlavor;
//			EventEntry Listen_Event = AnimationEvent::IdleCount;
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtEveryNEventsModifier() = default;
//			~AgxNtEveryNEventsModifier() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtFootIk : AgxNode
//		{
//			bool Align_Orient : 1 = true;
//			int8_t Angle_L_Multiplier = 1;
//			int8_t Angle_R_Multiplier = 1;
//			AgxAxis Axis = AgxAxis::Z;
//			float Blend_On_Off_time = 0.1f;
//			float Cast_Distance_Up = 1.0f;
//			float Cast_Distance_Down = 1.0f;
//			float Down_Error_Bias = 1.0f;
//			float Max_Controller_Seperation = 0.45f;
//			float Max_IK_Up = 0.5f;
//			float Max_Stride_Length = 0.5f;
//			float Root_Feedback_Weight = 0.08f;
//			float Target_Blend_Time = 0.15f;
//			float Max_Bend_Angle = 105.0f;
//			float Min_Bend_Angle = 25.0f;
//			floatEntry Max_IK_Ip_Variable;
//			floatEntry Cast_Distance_Down_Variable;
//			floatEntry Cast_Distance_Up_Variable;
//			intEntry Disable_Variable = AnimationVariable::bNoFootIK;
//			strEntry Ankle_L_Bone_Name;
//			strEntry Ankle_R_Bone_Name;
//			strEntry Hip_L_Bone_Name;
//			strEntry Hip_R_Bone_Name;
//			strEntry Knee_L_Bone_Name;
//			strEntry Knee_R_Bone_Name;
//			strEntry Toe_L_Bone_Name;
//			strEntry Toe_R_Bone_Name;
//			intEntry Left_Foot_Lock_Override;
//			intEntry Right_Foot_Lock_Override;
//			intEntry Left_Foot_Unlock_Override;
//			intEntry Right_Foot_Unlock_Override;
//			EventEntry PositionChange_Event = AnimationEvent::PositionChange;
//			strEntry Name = "Foot IK";
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtFootIk() = default;
//			~AgxNtFootIk() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtGamebryoSequence : AgxNode
//		{
//			SYNC_ONLY_TRANSITION_OUT;
//			BlendModeFunction Blend_Mode_Function = BlendModeFunction::None;
//			uint16_t Blend_Out_Frames = 0;
//			strEntry Name;
//			strEntry Sequence_Name;
//			floatEntry Time_Percent;
//			floatEntry Weight_or_Variable;
//
//			Utilities::VectorContainer<EventIOComponent> Enter_Events;
//			Utilities::VectorContainer<EventIOComponent> Exit_Events;
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtGamebryoSequence() = default;
//			~AgxNtGamebryoSequence() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtGraphRef : AgxNode
//		{
//			SYNC_ONLY_TRANSITION_OUT;
//			bool Wrapped_Graph : 1 = false;
//			strEntry Graph_Ref;
//
//			Utilities::VectorContainer<EventIOComponent> Enter_Events;
//			Utilities::VectorContainer<EventIOComponent> Exit_Events;
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtGraphRef() = default;
//			~AgxNtGraphRef() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtLinearVariable : AgxNode
//		{
//			SYNC_ONLY_TRANSITION_OUT;
//			floatEntry Control_Variable;
//			floatEntry Desired_Value;
//			strEntry Name = "Linear Variable";
//			floatEntry Speed;
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtLinearVariable() = default;
//			~AgxNtLinearVariable() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtLocomotionBlend : AgxNode
//		{
//			floatEntry Max_Blend_Time = "0";
//			strEntry Name;
//			floatEntry Speed_Parameter = AnimationVariable::Speed;
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtLocomotionBlend() = default;
//			~AgxNtLocomotionBlend() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtMassSpringDampenVariable : AgxNode
//		{
//			//All type 1 except for name
//			strEntry Name = "Mass Spring Dampen Variable";
//			float Mass = 1.0f;
//			float Spring_Coefficient = 90.0f;
//			float Dampen = 5.0f;
//			floatEntry Spring_Coefficient_Override;
//			floatEntry Dampen_Override;
//			floatEntry Mass_Override;
//			floatEntry Variable = AnimationVariable::fControllerXRaw;
//			floatEntry Output_Variable = AnimationVariable::fControllerXRawDampen;
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtMassSpringDampenVariable() = default;
//			~AgxNtMassSpringDampenVariable() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtMergeNode : AgxNode
//		{
//			strEntry Name = "Merge Node";
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtMergeNode() = default;
//			~AgxNtMergeNode() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtMirrorModifier : AgxNode
//		{
//			bool Mirror : 1 = true;
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtMirrorModifier() = default;
//			~AgxNtMirrorModifier() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtMomentumAnimation : AgxNode
//		{
//			strEntry Name = "Momentum Animation";
//			floatEntry Current_Direction = AnimationVariable::DirectionSmoothed;
//			floatEntry Current_Speed = AnimationVariable::CurrentSpeed;
//			floatEntry Desired_Direction = AnimationVariable::Direction;
//			floatEntry Desired_Speed = AnimationVariable::Speed;
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtMomentumAnimation() = default;
//			~AgxNtMomentumAnimation() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtMomentimSwitchBack : AgxNode
//		{
//			strEntry Name = "Momentum Switchback";
//			SyncEntry Sync_System;
//			floatEntry Desired_Direction = AnimationVariable::Direction;
//			floatEntry Desired_Speed = AnimationVariable::Speed;
//			float Minimum_Active_Time = 0.5f;
//			float Root_Twist_Bend_Rate = 1.0f;
//			float Walk_Run_Speed = 1.5f;
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtMomentimSwitchBack() = default;
//			~AgxNtMomentimSwitchBack() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtMultiFootIK : AgxNode
//		{
//			AgxAxis Bend_Axis = AgxAxis::Z;
//			float Align_Orient_Percent = 0.5f;
//			float Blend_Of_Off_Time = 0.5f;
//			float Cast_Distance_Down = 0.5f;
//			float Cast_Distance_Up = 0.5f;
//			float Contact_Normal_Blend_Rate = 0.5f;
//			intEntry Disable_Variable = AnimationVariable::bNoFootIK;
//			float Down_Error_Bias = 0.5f;
//			int Final_Bone_Axis_Index = 0;
//			float Foot_Unlock_Blend_Scalar = 2.0f;
//			float Max_Bend_Angle = 90.0f;
//			float Min_Bend_Angle = 5.0f;
//			float Max_Controller_Seperation = 1.0f;
//			EventEntry PositionChangeEvent = AnimationEvent::PositionChange;
//			float Root_Feedback_Weight = 0.5f;
//			intEntry Scale_Set_Variable;
//			float Target_Blend_Time = 0.1f;
//			Math::Vector3 X_Y_Trans_Lock_Tolerance = { 0.2f, 0.2f, 30.0f };
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtMultiFootIK() = default;
//			~AgxNtMultiFootIK() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtNormalizeRotation : AgxNode
//		{
//			strEntry Name = "Normalize Rotation";
//			strEntry Bone_Name;
//			int Normalize_Length = 90;
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtNormalizeRotation() = default;
//			~AgxNtNormalizeRotation() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtPairedAnimation : AgxNode
//		{
//			strEntry Lead_Suffix;
//			strEntry Name;
//			SYNC_ONLY_TRANSITION_OUT;
//			strEntry Victim_Suffix;
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtPairedAnimation() = default;
//			~AgxNtPairedAnimation() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtParticleSequence : AgxNode
//		{
//			float Blend_Out_Time =  0.0f;
//			strEntry Name = "Particle Sequence";
//			int Playback_Speed = 1;
//			strEntry Sequence_Name;
//			SYNC_ONLY_TRANSITION_OUT;
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtParticleSequence() = default;
//			~AgxNtParticleSequence() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtPathingAnimations : AgxNode
//		{
//			strEntry Name = "Pathing Animations";
//			floatEntry Direction_Var = AnimationVariable::Direction;
//			float Move_Start_Correction_Time = 1.5f;
//			floatEntry NPCHeadingOffset_Var = AnimationVariable::NPCHeadingOffset;
//			float Ramp_In_Time = 0.5f;
//			float Range_Max_Multiplier = 1.0f;
//			float Range_Min_Multiplier = 1.0f;
//			float Run_Normalized = 1.25f;
//			float Walk_Normalized = 1.25f;
//			floatEntry Speed_Var = AnimationVariable::Speed;
//			SyncEntry Sync_System;
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtPathingAnimations() = default;
//			~AgxNtPathingAnimations() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtPhysicsContractListener : AgxNode
//		{
//			bool Go_Dynamic_On_Contract = false;
//			Utilities::VectorContainer<strEntry> Ignore_Contact_Bones;
//			strEntry Name = "Physics Contact Listener";
//			EventEntry Send_Event;
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtPhysicsContractListener() = default;
//			~AgxNtPhysicsContractListener() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtPostBoneModifierControl : AgxNode
//		{
//			strEntry Name = "Bone Modifier Control";
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtPostBoneModifierControl() = default;
//			~AgxNtPostBoneModifierControl() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtRagdollDrive : AgxNode
//		{
//			bool Disable_Limits : 1 = false;
//			bool Blend_Forces : 1 = true;
//			strEntry Name = "Ragdoll Drive";
//			float Inertia_Spring = 20.0f;
//			float Angle_Spring = 0.0f;
//			float Dead_Zone = 1.0f;
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtRagdollDrive() = default;
//			~AgxNtRagdollDrive() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtRagdollGetUp : AgxNode
//		{
//			float Blend_From_Ragdoll_Time = 1.0f;
//			strEntry Name = "Ragdoll Get Up";
//			strEntry Core_Body_Bone_Name;
//			strEntry Head_Bone_Name;
//			EventEntry Get_Up_Event = AnimationEvent::GetUp;
//			EventEntry Get_Up_Start_Event = AnimationEvent::GetUpStart;
//			strEntry Left_Hand_Bone_Name;
//			strEntry Left_Leg_Bone_Name;
//			strEntry Right_Hand_Bone_Name;
//			strEntry Right_Leg_Bone_Name;
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtRagdollGetUp() = default;
//			~AgxNtRagdollGetUp() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtRandomAnimationNode : AgxNode
//		{
//			SYNC_ONLY_TRANSITION_OUT;
//			bool Update_Ref_Count_When_Inactive : 1 = true;
//			bool Enumerate_Tag : 1 = false;
//			bool Limit_To_Subgraph : 1 = false;
//			bool Sync_Slave : 1 = false;
//			float Chance_To_Repick = 1.0f;
//			float Cross_blend_time = 0.3f;
//			strEntry Name = "Random Animation";
//			intEntry RepickAtEnd;
//			EventEntry Send_Event_On_End;
//			floatEntry Speed_Multiplier;
//			intEntry State_Variable_Reg_Count;
//			intEntry State_Variable;
//			SyncEntry Sync_System;
//			intEntry Variable_Name;
//
//			Utilities::VectorContainer<EventIOComponent> Enter_Events;
//			Utilities::VectorContainer<EventIOComponent> Exit_Events;
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtRandomAnimationNode() = default;
//			~AgxNtRandomAnimationNode() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtRigSwitch : AgxNode
//		{
//			strEntry Name = "Rig Switch";
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtRigSwitch() = default;
//			~AgxNtRigSwitch() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtRootTwist : AgxNode
//		{
//			bool Direction_Is_Radians = true;
//			float Active_Blend_Rate = 1.0f;
//			float COM_Blend_Amount = 0.5f;
//			floatEntry Direction = AnimationVariable::Direction;
//			float Disable_After = -1.0f;
//			float Disable_Before = -1.0f;
//			float Hips_Blend_Amount = 0.0f;
//			float Inactive_Blend_Rate = 0.0f;
//			float Override_Anim_Direction = -1.0f;
//			float Root_Blend_Amount = 1.0f;
//			float Thighs_Blend_Amount = 0.0f;
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtRootTwist() = default;
//			~AgxNtRootTwist() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtRotationVariable : AgxNode
//		{
//			bool Use_Radians : 1 = true;
//			floatEntry Control_Variable = AnimationVariable::CurrentPosition;
//			floatEntry Desired_Variable;
//			strEntry Name = "Rotation Variable";
//			floatEntry Speed;
//
//			enum class RotationType { Longest_Direction, Looping_Left, Looping_Right, Shortest_Direction, Zero_to_One };
//			RotationType Rotation_Type = RotationType::Longest_Direction;
//			
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtRotationVariable() = default;
//			~AgxNtRotationVariable() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtSetOrient : AgxNode
//		{
//			bool Disable_When_Inactive : 1 = true;
//			float Clamped_Blend_Time = 0.1f;
//			boolEntry Enforce_DirectAt_Limits;
//			intEntry Is_Active_Bool;
//			boolEntry Is_Using_Code_Driven_Rotation = AnimationVariable::IsUsingCodeDrivenRotation;
//			float Max_Correction_Speed = 0.0f;
//			float Max_Post_Correction_Speed = 0.5f;
//			strEntry Name = "Set Orient";
//			floatEntry Orient_Var = AnimationVariable::fDesiredHeading;
//			float Time = -1.0f;
//			floatEntry Turn_Anim_Camera_Var = AnimationVariable::CameraYawSpeed;
//			enum class OrientType { End_Orient, Start_Orient, Turn_Animation_Modify_Translation, Turn_Animation };
//			OrientType Type = OrientType::Start_Orient;
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtSetOrient() = default;
//			~AgxNtSetOrient() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtSetPos : AgxNode
//		{
//		private:
//			float Time = -1.0f;
//			float Alignment_Bone = -1.0f;
//		public:
//			bool Disable_When_Inactive : 1 = true;
//			strEntry Name = "Set Pos";
//			float Max_Correctin_Vel = 500.0f;
//			float Max_Post_Correctin_Vel = 0.0f;
//			Math::Vector3 Oriented_Offset;
//			vectorEntry Position_Var = AnimationVariable::DesiredLocation;
//			enum class PosType {Start_Position, End_Position};
//			PosType Type = PosType::Start_Position;
//			
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtSetPos() = default;
//			~AgxNtSetPos() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtSpeedScale : AgxNode
//		{
//			float Blend_rate_when_active = 0.5f;
//			float Blend_rate_when_inactive = 0.0f;
//			floatEntry Desired_Speed_Var = AnimationVariable::Speed;
//			float Disable_After = -1.0f;
//			float Disable_Before = -1.0f;
//			float Max_Scale = 1.5f;
//			float Min_Scale = 0.5f;
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtSpeedScale() = default;
//			~AgxNtSpeedScale() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtStaggerMeter : AgxNode
//		{
//			float Counter_Attack_Chance = 0.0f;
//			float Counter_Attack_Cooldown = 0.0f;
//			EventEntry Counter_Attack_Event;
//			float Counter_Attack_Threshold = -1.0f;
//			float Immunity_Consume_Rate = 2.0f;
//			float Immunity_Decay_Rate = 1.0f;
//			float Immunity_Delay_Before_Decay = 0.5f;
//			float Immunity_Threshold = 1.0f;
//			EventEntry Listen_Event = AnimationEvent::StaggerStart;
//			strEntry Name = "Stagger Meter";
//			float Stagger_Cooldown = 0.5f;
//			float Stagger_Force_Variable = 1.0f;
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtStaggerMeter() = default;
//			~AgxNtStaggerMeter() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtStateVariableControl : AgxNode
//		{
//			bool Update_Ref_Count_When_Inactive : 1 = false;
//			int End_Value = 1;
//			strEntry Name = "Variable Control";
//			int Start_Value = 0;
//			intEntry State_Variable_Ref_Count;
//			intEntry State_Variable;
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtStateVariableControl() = default;
//			~AgxNtStateVariableControl() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtSwapGraph : AgxNode
//		{
//			bool Disable_Custom_Bones_When_Blending_Out : 1 = false;
//			strEntry Name = "Swap Graph";
//
//			Utilities::VectorContainer<strEntry> Graph_Names;
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtSwapGraph() = default;
//			~AgxNtSwapGraph() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtSwitchNode : AgxNode
//		{
//			bool Randomize_If_Invalid = false;
//			floatEntry Max_Blend_Time = "0";
//			strEntry Name = "Switch";
//			enum class TagAlt {Do_Nothing, Prefix, Suffix};
//			TagAlt Tag_Alteration = TagAlt::Do_Nothing;
//			intEntry Variable_Name;
//			enum class VarType {Variable, Event, Event_and_Variable };
//			VarType Variable_Type = VarType::Variable;
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtSwitchNode() = default;
//			~AgxNtSwitchNode() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtTagPropagation : AgxNode
//		{
//			strEntry Name = "Tag Propagation";
//			strEntry Tag_Sync_System = "TagFurniture";
//			enum class PropType {Propagate_Tag_Upstream, Receive_Tag_from_Downstream};
//			PropType Type_of_Propagation = PropType::Propagate_Tag_Upstream;
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtTagPropagation() = default;
//			~AgxNtTagPropagation() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtTimerEvent : AgxNode
//		{
//			strEntry Name = "Timer Event";
//			EventEntry Event_Name = AnimationEvent::IdleFaceTimer;
//			float Max_time = 10.0f;
//			float Min_time = 0.0f;
//			EventEntry Reset_Event_Name;
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtTimerEvent() = default;
//			~AgxNtTimerEvent() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtTranslationAdjustment : AgxNode
//		{
//			SYNC_ONLY_TRANSITION_OUT;
//			strEntry Name = "Trans Adjustment";
//			strEntry Bone_Name = "Camera Control";
//			Math::Vector3 Value = { 0.0f,0.0f,0.1f };
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtTranslationAdjustment() = default;
//			~AgxNtTranslationAdjustment() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtTwoBoneIK : AgxNode
//		{
//			bool Align_Orient : 1 = true;
//			int Angle_Multiplier = 1;
//			AgxAxis Axis = AgxAxis::Z;
//			strEntry Blend_Bone_Name;
//			float Blend_On_Off_Time = 0.1f;
//			intEntry Disable_Variable;
//			strEntry Name;
//			strEntry Parent_Bone_Name;
//			strEntry Source_Bone_Name;
//			strEntry Target_Bone_Name;
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtTwoBoneIK() = default;
//			~AgxNtTwoBoneIK() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtVariableCombiner : AgxNode
//		{
//			strEntry Name = "Variable Combiner";
//			intEntry Output_Variable;
//			intEntry Variable1;
//			intEntry Variable2;
//			intEntry Variable3;
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtVariableCombiner() = default;
//			~AgxNtVariableCombiner() override = default;
//		};
//
//#pragma region EMBEDDED
//		struct CALUMIANIMATION_API AgxNtBlendTreeEmbedded : AgxNode
//		{
//			SYNC_ONLY_TRANSITION_OUT;
//			strEntry Blend_Tree_Name;
//
//			Utilities::SharedPtrContainer<AgxGraph> graph;
//
//			Utilities::VectorContainer<EventIOComponent> Enter_Events;
//			Utilities::VectorContainer<EventIOComponent> Exit_Events;
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtBlendTreeEmbedded() = default;
//			~AgxNtBlendTreeEmbedded() override = default;
//		};
//
//		struct CALUMIANIMATION_API AgxNtStateMachineEmbedded : AgxNode
//		{
//			SYNC_ONLY_TRANSITION_OUT;
//			strEntry State_Machine_Name = "State Machine";
//
//			Utilities::SharedPtrContainer<AgxGraph> graph;
//
//			Utilities::VectorContainer<EventIOComponent> Enter_Events;
//			Utilities::VectorContainer<EventIOComponent> Exit_Events;
//
//			// Inherited via AgxNode
//			AgxNodeType GetNodeType() override;
//
//			AgxNtStateMachineEmbedded() = default;
//			~AgxNtStateMachineEmbedded() override = default;
//		};
//
//#pragma endregion
//
//
//#pragma endregion
//
//
//}}
