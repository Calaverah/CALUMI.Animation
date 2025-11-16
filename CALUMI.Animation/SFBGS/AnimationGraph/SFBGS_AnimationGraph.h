#pragma once

//#include "SFBGS_AnimationGraphNodes.h"
//
//namespace CALUMI { namespace SFBGS {
//
//	struct CALUMIANIMATION_API AgxComment
//	{
//		Utilities::StringContainer Text;
//		Math::Vector2D TargetPos;
//		Math::Vector2D Pos;
//	};
//
//	struct CALUMIANIMATION_API AgxGraph
//	{
//		uint8_t Link_Style = 0;
//		Utilities::VectorContainer<Utilities::SharedPtrContainer<AgxNode>> nodes;
//		AgxComment comment;
//		
//		AgxGraph() = default;
//	};
//
//	struct CALUMIANIMATION_API AgxRoot : AgxGraph
//	{
//		Utilities::StringContainer Catergory;
//		Utilities::StringContainer Path;
//
//	};
//
//
//}}
//
//#pragma warning(disable: 4661)
//template struct CALUMIANIMATION_API CALUMI::Utilities::UniquePtrContainer<CALUMI::SFBGS::AgxPort>;
//template struct CALUMIANIMATION_API CALUMI::Utilities::UniquePtrContainer<CALUMI::SFBGS::AgxPortOutput>;
//template struct CALUMIANIMATION_API CALUMI::Utilities::UniquePtrContainer<CALUMI::SFBGS::AgxPortInput>;
//template struct CALUMIANIMATION_API CALUMI::Utilities::SharedPtrContainer<CALUMI::SFBGS::AgxPort>;
//template struct CALUMIANIMATION_API CALUMI::Utilities::SharedPtrContainer<CALUMI::SFBGS::AgxPortOutput>;
//template struct CALUMIANIMATION_API CALUMI::Utilities::SharedPtrContainer<CALUMI::SFBGS::AgxPortInput>;
//template struct CALUMIANIMATION_API CALUMI::Utilities::WeakPtrContainer<CALUMI::SFBGS::AgxPort>;
//template struct CALUMIANIMATION_API CALUMI::Utilities::WeakPtrContainer<CALUMI::SFBGS::AgxPortOutput>;
//template struct CALUMIANIMATION_API CALUMI::Utilities::WeakPtrContainer<CALUMI::SFBGS::AgxPortInput>;
//template struct CALUMIANIMATION_API CALUMI::Utilities::UniquePtrContainer<CALUMI::SFBGS::AgxGraph>;
//template struct CALUMIANIMATION_API CALUMI::Utilities::SharedPtrContainer<CALUMI::SFBGS::AgxGraph>;
//template struct CALUMIANIMATION_API CALUMI::Utilities::WeakPtrContainer<CALUMI::SFBGS::AgxGraph>;
//template struct CALUMIANIMATION_API CALUMI::Utilities::UniquePtrContainer<CALUMI::SFBGS::AgxNode>;
//template struct CALUMIANIMATION_API CALUMI::Utilities::SharedPtrContainer<CALUMI::SFBGS::AgxNode>;
//template struct CALUMIANIMATION_API CALUMI::Utilities::WeakPtrContainer<CALUMI::SFBGS::AgxNode>;
//template struct CALUMIANIMATION_API CALUMI::Utilities::VectorContainer<CALUMI::Utilities::UniquePtrContainer<CALUMI::SFBGS::AgxPort>>;
//template struct CALUMIANIMATION_API CALUMI::Utilities::VectorContainer<CALUMI::Utilities::UniquePtrContainer<CALUMI::SFBGS::AgxPortOutput>>;
//template struct CALUMIANIMATION_API CALUMI::Utilities::VectorContainer<CALUMI::Utilities::UniquePtrContainer<CALUMI::SFBGS::AgxPortInput>>;
//template struct CALUMIANIMATION_API CALUMI::Utilities::VectorContainer<CALUMI::Utilities::SharedPtrContainer<CALUMI::SFBGS::AgxPort>>;
//template struct CALUMIANIMATION_API CALUMI::Utilities::VectorContainer<CALUMI::Utilities::SharedPtrContainer<CALUMI::SFBGS::AgxPortOutput>>;
//template struct CALUMIANIMATION_API CALUMI::Utilities::VectorContainer<CALUMI::Utilities::SharedPtrContainer<CALUMI::SFBGS::AgxPortInput>>;
//template struct CALUMIANIMATION_API CALUMI::Utilities::VectorContainer<CALUMI::Utilities::WeakPtrContainer<CALUMI::SFBGS::AgxPort>>;
//template struct CALUMIANIMATION_API CALUMI::Utilities::VectorContainer<CALUMI::Utilities::WeakPtrContainer<CALUMI::SFBGS::AgxPortOutput>>;
//template struct CALUMIANIMATION_API CALUMI::Utilities::VectorContainer<CALUMI::Utilities::WeakPtrContainer<CALUMI::SFBGS::AgxPortInput>>;
//template struct CALUMIANIMATION_API CALUMI::Utilities::VectorContainer<CALUMI::Utilities::UniquePtrContainer<CALUMI::SFBGS::AgxNode>>;
//template struct CALUMIANIMATION_API CALUMI::Utilities::VectorContainer<CALUMI::Utilities::SharedPtrContainer<CALUMI::SFBGS::AgxNode>>;
//template struct CALUMIANIMATION_API CALUMI::Utilities::VectorContainer<CALUMI::Utilities::WeakPtrContainer<CALUMI::SFBGS::AgxNode>>;
//#pragma warning(default: 4661)