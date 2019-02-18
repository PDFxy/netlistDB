#pragma once

#include <netlistDB/netlist.h>

namespace netlistDB {
namespace query {


/*
 * Traverse nodes in circuit
 **/
class QueryTraverse {
public:
	// align to cache line does not bring perf. improvement
	// as there is large number of nodes and the access is more or less random.
	// [TODO] how is this possible?
	using flag_t = bool;
	using atomic_flag_t = std::atomic<flag_t>;

	// return the next nodes which should be probed
	using callback_t = std::function<iNode::iterator(iNode&)>;
	atomic_flag_t * visited;
	bool visited_clean;
	size_t load_balance_limit;
	callback_t callback;
	size_t max_items;

	QueryTraverse(size_t max_items);

	inline bool is_visited(iNode &n) {
		//return visited[n.index].exchange(true);
		return visited[n.index].exchange(true);
	}

	static iNode::iterator dummy_callback(iNode &n);
public:
	/*
	 * Reset visit flags used in traversal
	 * */
	void clean_visit_flags();

	/*
	 * Traverse graph from starts and call callback on each node exactly once
	 * @param starts the nodes where the search should start
	 * @param callback the callback function called on each vistited node (exactly once)
	 * */
	void traverse(std::vector<iNode*> starts, callback_t callback);

protected:
	/*
	 * Traverse using limited DFS, if limit is exceeded the rest children are processed
	 * new task
	 *
	 * @param n node to process
	 * */
	void traverse(iNode & n);
};

}
}