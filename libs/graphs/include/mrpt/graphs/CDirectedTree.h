/* +---------------------------------------------------------------------------+
   |                 The Mobile Robot Programming Toolkit (MRPT)               |
   |                                                                           |
   |                          http://www.mrpt.org/                             |
   |                                                                           |
   | Copyright (c) 2005-2012, Individual contributors, see AUTHORS file        |
   | Copyright (c) 2005-2012, MAPIR group, University of Malaga                |
   | Copyright (c) 2012, University of Almeria                                 |
   | All rights reserved.                                                      |
   |                                                                           |
   | Redistribution and use in source and binary forms, with or without        |
   | modification, are permitted provided that the following conditions are    |
   | met:                                                                      |
   |    * Redistributions of source code must retain the above copyright       |
   |      notice, this list of conditions and the following disclaimer.        |
   |    * Redistributions in binary form must reproduce the above copyright    |
   |      notice, this list of conditions and the following disclaimer in the  |
   |      documentation and/or other materials provided with the distribution. |
   |    * Neither the name of the copyright holders nor the                    |
   |      names of its contributors may be used to endorse or promote products |
   |      derived from this software without specific prior written permission.|
   |                                                                           |
   | THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       |
   | 'AS IS' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED |
   | TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR|
   | PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE |
   | FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL|
   | DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR|
   |  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)       |
   | HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,       |
   | STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN  |
   | ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE           |
   | POSSIBILITY OF SUCH DAMAGE.                                               |
   +---------------------------------------------------------------------------+ */
#ifndef  MRPT_DIRECTED_TREE_H
#define  MRPT_DIRECTED_TREE_H

//#include <mrpt/math/utils.h>
#include <mrpt/utils/utils_defs.h>

/*---------------------------------------------------------------
	Class
  ---------------------------------------------------------------*/
namespace mrpt
{
	namespace graphs
	{
		using mrpt::utils::TNodeID;

		/** A special kind of graph in the form of a tree with directed edges and optional edge annotations of templatized type "TYPE_EDGES".
		  *  The tree is represented by means of:
		  *		- \a root: The ID of the root node.
		  *		- \a edges_to_children: A map from node ID to all the edges to its children.
		  *
		  *  This class is less general than CDirectedGraph but more efficient to traverse (see \a visitDepthFirst and \a visitBreadthFirst).
		  *
		  *  If annotations in edges are not required, you can leave TYPE_EDGES to its default type "uint8_t".
		  *  \sa CDirectedGraph, CDijkstra, mrpt::graphs::CNetworkOfPoses,
		 * \ingroup mrpt_graphs_grp
		  */
		template <class TYPE_EDGES = uint8_t>
		class CDirectedTree
		{
		public:
			struct TEdgeInfo
			{
				TNodeID    id;      //!< The ID of the child node.
				bool       reverse; //!< True if edge direction is child->parent, false if it's parent->child.
				TYPE_EDGES data;    //!< User data for this edge.
			};
			typedef std::list<TEdgeInfo>          TListEdges;
			typedef std::map<TNodeID,TListEdges>  TMapNode2ListEdges;

			/** @name Data
			    @{ */
			TNodeID            root;               //!< The root of the tree
			TMapNode2ListEdges edges_to_children;  //!< The edges of each node
			/** @} */

			/** @name Utilities
			    @{ */

			/** Empty all edge data and set "root" to INVALID_NODEID */
			void clear() { edges_to_children.clear(); root = INVALID_NODEID; }

			/** Virtual base class for user-defined visitors */
			struct Visitor
			{
				typedef CDirectedTree<TYPE_EDGES> tree_t;

				/** Virtual method to be implemented by the user and which will be called during the visit to a graph with visitDepthFirst or visitBreadthFirst
				  *  Specifically, the method will be called once for each <b>edge</b> in the tree.
				  * \param parent [IN] The ID of the parent node.
				  * \param edge_to_child [IN] The edge information from the parent to "edge_to_child.id"
				  * \param depth_level [IN] The "depth level" of the child node "edge_to_child.id" (root node is at 0, its children are at 1, etc.).
				  */
				virtual void OnVisitNode( const TNodeID parent, const typename tree_t::TEdgeInfo &edge_to_child, const size_t depth_level ) = 0;
			};

			/** Depth-first visit of all children nodes of a given root (itself excluded from the visit), invoking a user-provided function for each node/edge. \sa visitBreadthFirst */
			void visitDepthFirst( const TNodeID root, Visitor & user_visitor, const size_t root_depth_level =0 ) const
			{
				const size_t next_depth_level = root_depth_level+1;
				typename TMapNode2ListEdges::const_iterator itChildren = edges_to_children.find(root);
				if (itChildren==edges_to_children.end()) return; // No children
				const TListEdges &children = itChildren->second;
				for (typename TListEdges::const_iterator itEdge=children.begin();itEdge!=children.end();++itEdge)
				{
					user_visitor.OnVisitNode(root,*itEdge,next_depth_level);
					visitDepthFirst(itEdge->id,user_visitor, next_depth_level); // Recursive depth-first call.
				}
			}

			/** Breadth-first visit of all children nodes of a given root (itself excluded from the visit), invoking a user-provided function for each node/edge. \sa visitDepthFirst */
			void visitBreadthFirst( const TNodeID root, Visitor & user_visitor, const size_t root_depth_level =0  ) const
			{
				const size_t next_depth_level = root_depth_level+1;
				typename TMapNode2ListEdges::const_iterator itChildren = edges_to_children.find(root);
				if (itChildren==edges_to_children.end()) return; // No children
				const TListEdges &children = itChildren->second;
				for (typename TListEdges::const_iterator itEdge=children.begin();itEdge!=children.end();++itEdge)
					user_visitor.OnVisitNode(root,*itEdge,next_depth_level);
				for (typename TListEdges::const_iterator itEdge=children.begin();itEdge!=children.end();++itEdge)
					visitDepthFirst(itEdge->id,user_visitor,next_depth_level); // Recursive breath-first call.
			}

			/** Return a text representation of the tree spanned in a depth-first view, as in this example:
			  *  \code
			  *    0
			  *     -> 1
			  *     -> 2
			  *         -> 4
			  *         -> 5
			  *     -> 3
			  *  \endcode
			  */
			std::string getAsTextDescription() const
			{
				std::ostringstream s;
				struct CMyVisitor : public mrpt::graphs::CDirectedTree<TYPE_EDGES>::Visitor
				{
					std::ostringstream  &m_s;
					CMyVisitor(std::ostringstream &s) : m_s(s) { }
					virtual void OnVisitNode( const TNodeID parent, const typename mrpt::graphs::CDirectedTree<TYPE_EDGES>::Visitor::tree_t::TEdgeInfo &edge_to_child, const size_t depth_level ) {
						m_s << std::string(depth_level*5, ' ') << (edge_to_child.reverse ? "<-" : "->" ) //;
							<< std::setw(3) << edge_to_child.id << std::endl;
					}
				};
				CMyVisitor myVisitor(s);
				s << std::setw(3) << root << std::endl;
				visitDepthFirst( root, myVisitor );
				return s.str();
			}

		};

		/** @} */
	} // End of namespace
} // End of namespace
#endif
