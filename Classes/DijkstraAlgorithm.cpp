#include "DijkstraAlgorithm.h"
#include "cocos2d.h"

const double max_weight = FLT_MAX;


void DijkstraComputePaths(int source,
	const std::vector<std::vector<neighbor> > &adjacency_list,
	std::vector<double> &min_distance,
	std::vector<int> &previous)
{
	int n = adjacency_list.size();

	min_distance.clear();
	min_distance.resize(n, max_weight);
	min_distance[source] = 0;
	previous.clear();
	previous.resize(n, -1);
	std::set<std::pair<double, int> > vertex_queue;
	vertex_queue.insert(std::make_pair(min_distance[source], source));

	while (!vertex_queue.empty())
	{
		double dist = vertex_queue.begin()->first;
		int u = vertex_queue.begin()->second;
		vertex_queue.erase(vertex_queue.begin());

		// Visit each edge exiting u
		const std::vector<neighbor> &neighbors = adjacency_list[u];
		for (std::vector<neighbor>::const_iterator neighbor_iter = neighbors.begin();
			neighbor_iter != neighbors.end();
			neighbor_iter++)
		{
			int v = neighbor_iter->target;
			double weight = neighbor_iter->weight;
			double distance_through_u = dist + weight;
			if (distance_through_u < min_distance[v]) {
				vertex_queue.erase(std::make_pair(min_distance[v], v));

				min_distance[v] = distance_through_u;
				previous[v] = u;
				vertex_queue.insert(std::make_pair(min_distance[v], v));

			}

		}
	}
}


std::list<int> DijkstraGetShortestPathTo(
	int vertex, const std::vector<int> &previous)
{
	std::list<int> path;
	for (; vertex != -1; vertex = previous[vertex])
		path.push_front(vertex);
	return path;
}