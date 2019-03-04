#include <netlistDB/statement_if.h>
#include <netlistDB/netlist.h>

using namespace netlistDB;

Netlist::Netlist(const std::string & name) :
		name(name) {
}

void Netlist::register_node(iNode & n) {
	n.index = nodes.size();
	nodes.push_back(&n);
}

void Netlist::register_node(Net & n) {
	n.net_index = nets.size();
	nets.push_back(&n);
	register_node(static_cast<iNode&>(n));
}

void Netlist::unregister_node(iNode & n) {
	nodes[n.index] = nullptr;
}

void Netlist::unregister_node(Net & n) {
	nets[n.net_index] = nullptr;
	unregister_node(static_cast<iNode&>(n));
}

Net & Netlist::sig_in(const std::string & name, hw_type::iHwType & t) {
	auto s = new Net(*this, t, name, DIR_IN);
	s->id.hidden = false;
	return *s;
}

Net & Netlist::sig_out(const std::string & name, hw_type::iHwType & t) {
	auto s = new Net(*this, t, name, DIR_OUT);
	s->id.hidden = false;
	return *s;
}

Net & Netlist::sig(hw_type::iHwType & t) {
	return sig("sig_", t);
}

Net & Netlist::sig(const std::string & name, hw_type::iHwType & t) {
	auto s = new Net(*this, t, name, DIR_UNKNOWN);
	return *s;
}

void Netlist::integrty_assert() {
	size_t i = 0;
	for (auto n : nodes) {
		assert(i == n->index);
		for (auto f : n->forward) {
			assert(nodes[f->index] == f);
		}
		for (auto b : n->forward) {
			assert(nodes[b->index] == b);
		}
		i++;
	}
	i = 0;
	for (auto n : nets) {
		assert(nodes[n->index] == n);
		assert(i == n->net_index);
		i++;
	}
}


Netlist::~Netlist() {
	for (auto o : nodes) {
		if (o != nullptr) {
			delete o;
		}
	}
	nodes.clear();
}
