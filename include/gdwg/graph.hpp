#ifndef GDWG_GRAPH_HPP
#define GDWG_GRAPH_HPP

// TODO: Make this graph generic
//       ... this won't just compile
//       straight away
namespace gdwg {
	class graph {
	public:
		struct value_type {
			N from;
			N to;
			E weight;
		};

		// Your member functions go here
	private:
	};
} // namespace gdwg

#endif // GDWG_GRAPH_HPP
