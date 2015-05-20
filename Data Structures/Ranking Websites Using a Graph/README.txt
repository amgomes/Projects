Adam Gomes
ID: 1105982

HW #7 - PageRank

This program utilizes the following files:
  CommandLineParser.h
  CommandLineParser.cpp
  Edge.h
  Edge.cpp
  Graph.h
  Graph.cpp
  ScriptParser.h
  ScriptParser.cpp
  Vertex.h
  Vertex.cpp
  PageRank - Main Function

This program opens a list of files containing webpages and their contained links. From these files, it creates a graph with the webpages as vertices and the links as edges.

It monitors the indegree and outdegree of each webpage, along with all broken links and sinks (webpages with outdegree = 0).

It then outputs the top five webpages (sorted by indegree first and alphabetically second), as well as all broken links and all sinks.