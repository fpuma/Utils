#pragma once

#include "bfstypes.h"

#include <utils/numerictypes.h>
#include <utils/geometry/vec2i.h>

#include <assert.h>
#include <functional>
#include <vector>
#include <queue>
#include <set>

namespace puma
{
    /*
    Breadth-First Search (BFS)
    Explores nodes level by level from the source; guarantees shortest path in unweighted graphs.
    */

    struct PathToNode2d
    {
		u32 distance = 0;
		std::vector<Vec2i> path;
    };

	template<class Node>
	class Bfs2d
	{
	public:

		Bfs2d(std::vector<Vec2i>&& directions = 
            { 
                Vec2i(1, 0),  // Right
			    Vec2i(-1, 0), // Left
			    Vec2i(0, 1),  // Down
			    Vec2i(0, -1)  // Up
			})
			: m_directions(std::move(directions))
		{
		}

		using Grid = std::vector<std::vector<Node>>;
		using NodeEvaluator = std::function<NodeType(const Node&)>;
		using Pathways = std::vector<std::vector<PathToNode2d>>;

		void generatePathways(const Grid& grid, NodeEvaluator&& evaluator);

        const PathToNode2d& getPathInfo(const Vec2i& pos) const
        {
            assert(isWithinGrid(pos)); // "Position is out of grid bounds."
            return m_pathways[pos.x][pos.y];
		}

	private:

        const std::vector<Vec2i> m_directions;

        struct InternalNodeInfo
        {
            bool visited = false;
            NodeType type = NodeType::Normal;
			PathToNode2d pathInfo;
        };



		bool isWithinGrid(const Vec2i& pos) const
        {
            return m_pathways.empty() == false &&
                   pos.x >= 0 && pos.x < static_cast<int>(m_pathways.size()) &&
                   pos.y >= 0 && pos.y < static_cast<int>(m_pathways[0].size());
		}

        Pathways m_pathways;
	};

    template<class Node>
    void Bfs2d<Node>::generatePathways(const Bfs2d::Grid& grid, NodeEvaluator&& evaluator)
    {
        if (grid.empty()) return; // Early return because the grid can't be empty.

        size_t width = grid.size();
        size_t height = grid[0].size();
        m_pathways.clear();
        m_pathways.assign(width, std::vector<PathToNode2d>(height));

        std::queue<Vec2i> q;
        std::vector<std::vector<InternalNodeInfo>> generationInfo(width, std::vector<InternalNodeInfo>(height, {}));
        //std::set<Vec2i> targets;

        for (size_t x = 0; x < width; ++x)
        {
            if (grid[x].size() != height)
            {
                // Not all columns in the grid have the same size
                m_pathways.clear();
                return;
            }

            for (size_t y = 0; y < height; ++y)
            {
                const Node& node = grid[x][y];
                InternalNodeInfo& nodeInfo = generationInfo[x][y];
                nodeInfo.type = evaluator(node);
                
                switch (nodeInfo.type)
                {
                case NodeType::Normal:
                /*{
                    break;
                }*/
                case NodeType::PassableTarget:
                case NodeType::UnpassableTarget:
                {
                    //targets.insert({ x,y });
                    break;
                }
                case NodeType::Source:
                {
					s32 sx = static_cast<s32>(x);
					s32 sy = static_cast<s32>(y);
                    q.push({ sx, sy });
                    nodeInfo.visited = true;
                    PathToNode2d& pathToNode = m_pathways[x][y];
                    pathToNode.path.push_back({ sx, sy });
                    break;
                }
                case NodeType::Ignore:
                {
					nodeInfo.visited = true;
                    break;
                }
                default:
                    assert(false); // Unhandled node type
                }
            }
        }

        while (!q.empty())
        {
            Vec2i current = q.front();
            q.pop();

            for (const Vec2i& dir : m_directions)
            {
                Vec2i next = current + dir;

                if(!isWithinGrid(next)) continue; // Skip if next position is out of bounds
                
                PathToNode2d& currentNodePath = m_pathways[current.x][current.y];
                PathToNode2d& nextNodePath = m_pathways[next.x][next.y];

                InternalNodeInfo& nodeInfo = generationInfo[next.x][next.y];

				if (nodeInfo.visited) continue; // Skip if next position has already been visited

				assert(nodeInfo.type != NodeType::Ignore); // "Node type should not be Ignore here."

                nodeInfo.visited = true;
                
                nextNodePath.path.insert(nextNodePath.path.end(), currentNodePath.path.begin(), currentNodePath.path.end());
                nextNodePath.path.push_back(next);
                nextNodePath.distance = currentNodePath.distance != kMaxU32 ? (m_pathways[current.x][current.y].distance + 1) : 1;
                
                if (nodeInfo.type != NodeType::UnpassableTarget)
                {
                    q.push(next);
                }
                
            }
        }
    }    
}