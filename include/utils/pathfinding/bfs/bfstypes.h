#pragma once

namespace puma
{
    enum class NodeType
    {
		Normal, // Normal node that can be traversed.
		Source, // Will be used as a starting point for the search.
		PassableTarget, // A target that can be walked through 
		UnpassableTarget, // Will not go through them, but will be mapped to the pathways. 
		Ignore  // Will not be mapped to the pathways.
	};   
}