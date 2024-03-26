from collections import deque
from queue import PriorityQueue

class Graph:
    def __init__(self):
        self.graph = {}

    def add_edge(self, u, v):
        if u not in self.graph:
            self.graph[u] = []
        self.graph[u].append(v)


    def dfs(self, v, visited):
        print(v)
        visited[v] = True
        if v not in self.graph:
            return
        for vi in self.graph[v]:
            if not visited[vi]:
                self.dfs(vi, visited)

    def bfs(self, v, visited):
        queue = deque([v])
        while queue:
            v = queue.popleft()
            print(v)
            if v not in self.graph:
                continue
            for vi in self.graph[v]:
                if not visited[vi]:
                    visited[vi] = True
                    queue.append(vi)


class WeightedGraph:
    def __init__(self):
        self.graph = {}

    def add_edge(self, u, v, w):
        if u not in self.graph:
            self.graph[u] = []
        self.graph[u].append((v, w))

    def dijkstra(self, start):
        distances = {node: float('infinity') for node in self.graph}
        distances[start] = 0
        queue = PriorityQueue()
        queue.put((0, start))
        while not queue.empty():
            current_distance, current_node = queue.get()
            if current_distance > distances[current_node]:
                continue
            for neighbor, weight in self.graph[current_node]:
                distance = current_distance + weight
                if distance < distances[neighbor]:
                    distances[neighbor] = distance
                    queue.put((distance, neighbor))

def main():
    g = Graph()
    g.add_edge(0, 1)
    g.add_edge(0, 2)
    g.add_edge(1, 3)
    g.add_edge(0, 3)
    print("dfs")
    g.dfs(0, [False] * 4)
    print("bfs")
    g.bfs(0, [False] * 4)


if __name__ == "__main__":
    main()


# [1,4,5,9,10, 11]
