#include <netlistDB/netlist.h>

namespace netlistDB {

ComponentMap::ComponentMap(Netlist & parent, std::shared_ptr<Netlist> component) :
		parent(parent), component(component) {
}

void ComponentMap::add(Net * parent_net, Net * component_port) {
	assert(&parent_net->ctx == &parent);
	assert(&component_port->ctx == component.get());
	assert(parent_to_child.find(parent_net) == parent_to_child.end());
	assert(child_to_parent.find(component_port) == child_to_parent.end());

	parent_to_child[parent_net] = component_port;
	child_to_parent[component_port] = parent_net;
}

}
