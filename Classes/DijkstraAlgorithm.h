#ifndef __DIJKSTRA_ALGORITHM_H__
#define __DIJKSTRA_ALGORITHM_H__

#include <iostream>
#include <vector>
#include <string>
#include <list>

#include <limits> // for numeric_limits

#include <set>
#include <utility> // for pair
#include <algorithm>
#include <iterator>


struct neighbor {
	int target;
	double weight;
	neighbor(int arg_target, double arg_weight)
		: target(arg_target), weight(arg_weight) { }
};

extern void DijkstraComputePaths(int source,
	const std::vector<std::vector<neighbor> > &adjacency_list,
	std::vector<double> &min_distance,
	std::vector<int> &previous);

extern std::list<int> DijkstraGetShortestPathTo(
	int vertex, const std::vector<int> &previous);

#endif // __DIJKSTRA_ALGORITHM_H__

/*int main()
{
	// remember to insert edges both ways for an undirected graph
	std::vector<std::vector<neighbor> > adjacency_list(6);
	// 0 = a
	adjacency_list[0].push_back(neighbor(1, 7));
	adjacency_list[0].push_back(neighbor(2, 9));
	adjacency_list[0].push_back(neighbor(5, 14));
	// 1 = b
	adjacency_list[1].push_back(neighbor(0, 7));
	adjacency_list[1].push_back(neighbor(2, 10));
	adjacency_list[1].push_back(neighbor(3, 15));
	// 2 = c
	adjacency_list[2].push_back(neighbor(0, 9));
	adjacency_list[2].push_back(neighbor(1, 10));
	adjacency_list[2].push_back(neighbor(3, 11));
	adjacency_list[2].push_back(neighbor(5, 2));
	// 3 = d
	adjacency_list[3].push_back(neighbor(1, 15));
	adjacency_list[3].push_back(neighbor(2, 11));
	adjacency_list[3].push_back(neighbor(4, 6));
	// 4 = e
	adjacency_list[4].push_back(neighbor(3, 6));
	adjacency_list[4].push_back(neighbor(5, 9));
	// 5 = f
	adjacency_list[5].push_back(neighbor(0, 14));
	adjacency_list[5].push_back(neighbor(2, 2));
	adjacency_list[5].push_back(neighbor(4, 9));

	std::vector<double> min_distance;
	std::vector<int> previous;
	DijkstraComputePaths(0, adjacency_list, min_distance, previous);
	std::cout << "Distance from 0 to 4: " << min_distance[4] << std::endl;
	std::list<int> path = DijkstraGetShortestPathTo(4, previous);
	std::cout << "Path : ";
	std::copy(path.begin(), path.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout << std::endl;



	return 0;
}*/