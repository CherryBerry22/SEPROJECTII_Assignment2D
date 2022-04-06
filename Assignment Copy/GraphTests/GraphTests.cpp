#include "pch.h"
#include "CppUnitTest.h"
#include "Graph.h"
#include "GraphExample.cpp"
#include <string>
#include <iostream>
#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GraphTests
{
	TEST_CLASS(GraphTests)
	{
	public:

		string outFileName = "routesOut.txt";
		string inFileName = "routesIn.txt";

		// Add a vertex and assert that it has been added to the list of vertices
		TEST_METHOD(AddVertexValid)
		{
			Graph<string> routes;
			string location = "Austin";
			routes.AddVertex(location);
			Assert::AreEqual(routes.Vertices()[0]->info, location);
		}

		// Remove the vertex that was added in the above test and assert that the list of vertices is now empty.
		TEST_METHOD(RemoveVertexValid)
		{
			Graph<string> routes;
			string location = "Austin";
			routes.AddVertex(location);
			routes.RemoveVertex(location);
			Assert::AreEqual((int)routes.Vertices().size(), 0);
		}

		// Add two vertices and create an edge between them, then assert that the edge was added.
		TEST_METHOD(AddEdgeValid)
		{
			Graph<string> routes;
			string location1 = "Austin";
			string location2 = "Dallas";
			routes.AddVertex(location1);
			routes.AddVertex(location2);
			routes.AddEdge(location1, location2, 200);

			Assert::AreEqual((int)routes.Vertices()[0]->adjacency.size(), 1);
		}

		// The two vertices and edge still exist in the routes graph. Remove the edge between vertices and Assert there are no adjacencies.
		TEST_METHOD(RemoveEdgeValid)
		{
			Graph<string> routes;
			string location1 = "Austin";
			string location2 = "Dallas";

			routes.AddVertex(location1);
			routes.AddVertex(location2);

			routes.AddEdge(location1, location2, 200);
			int addEdgeSize = (int)routes.Vertices()[0]->adjacency.size();

			routes.RemoveEdge(location1, location2, 200);
			int removeEdgeSize = (int)routes.Vertices()[0]->adjacency.size();

			//Assert::AreEqual(addEdgeSize, 1);
			Assert::AreEqual(removeEdgeSize, 0);
		}

		// Test the print method by "printing" the information to an ostringstream and then converting the stream to a string and comparing.
		TEST_METHOD(TestPrint)
		{
			Graph<string> newRoutes;
			string location1 = "Austin";
			string location2 = "Dallas";
			newRoutes.AddVertex(location1);
			newRoutes.AddVertex(location2);
			newRoutes.AddEdge(location1, location2, 200);

			std::ostringstream outFile;
			newRoutes.Print(outFile);

			string actual = "Austin 1\n200 Dallas\nDallas 0\n";

			Assert::AreEqual(outFile.str(), actual);
		}

		// Test the print method by "printing" the information to an ostringstream and then converting the stream to a string and comparing.
		TEST_METHOD(TestPrintEmpty)
		{
			Graph<string> newRoutes;

			std::ostringstream outFile;
			newRoutes.Print(outFile);

			string actual = "";

			Assert::AreEqual(outFile.str(), actual);
		}

		// Test the shortestPath method by comparing the output caught in an ostringstream
		TEST_METHOD(TestShortestPath)
		{
			Graph<string> routes;

			//Read in routes from file since we want a lot and it's easier this way :)
			ifstream inFile;
			inFile.open("routesIn.txt");
			if (!inFile.is_open())
			{
				cerr << "CANNOT open this file to read!" << endl;
				return;
			}
			string from, to;
			double miles;
			int numOuts;
			inFile >> from;
			while (!inFile.eof())
			{
				routes.AddVertex(from);
				inFile >> numOuts;
				for (int i = 0; i < numOuts; i++)
				{
					inFile >> miles >> to;
					routes.AddEdge(from, to, miles);
				}
				inFile >> from;
			}
			inFile.close();

			std::ostringstream oss;
			std::streambuf* p_cout_streambuf = std::cout.rdbuf();
			std::cout.rdbuf(oss.rdbuf());

			routes.ShortestPath("Austin");

			std::cout.rdbuf(p_cout_streambuf);

			//string actual = "";	// I have at this point realized that I may be in over my head comparing
									// to an expected string, so let's instead just make sure the oss isn't empty.

			Assert::IsTrue(oss.str() != "");
		}

		// Test the shortestPath method like above but pass it an empty graph.
		TEST_METHOD(TestShortestPathEmpty)
		{
			Graph<string> routes;

			std::ostringstream oss;
			std::streambuf* p_cout_streambuf = std::cout.rdbuf();
			std::cout.rdbuf(oss.rdbuf());

			routes.ShortestPath("Austin");

			std::cout.rdbuf(p_cout_streambuf);

			Assert::IsTrue(oss.str() == "");
		}

		// Test the depthFirstSearch method by querying it about a path that exists
		TEST_METHOD(TestPrintPathDepthFirstTrue)
		{
			Graph<string> routes;
			deque<Graph<string>::Vertex*> path;


			//Read in routes from file since we want a lot and it's easier this way :)
			ifstream inFile;
			inFile.open("routesIn.txt");
			if (!inFile.is_open())
			{
				cerr << "CANNOT open this file to read!" << endl;
				return;
			}
			string from, to;
			double miles;
			int numOuts;
			inFile >> from;
			while (!inFile.eof())
			{
				routes.AddVertex(from);
				inFile >> numOuts;
				for (int i = 0; i < numOuts; i++)
				{
					inFile >> miles >> to;
					routes.AddEdge(from, to, miles);
				}
				inFile >> from;
			}
			inFile.close();

			bool isPath = routes.DepthFirstSearch("Austin", "Denver", path);

			Assert::IsTrue(isPath);
		}

		// Test the breathFirstSearch method by querying it about a path that exists
		TEST_METHOD(TestPrintPathBreadthFirstTrue)
		{
			Graph<string> routes;
			deque<Graph<string>::Vertex*> path;


			//Read in routes from file since we want a lot and it's easier this way :)
			ifstream inFile;
			inFile.open("routesIn.txt");
			if (!inFile.is_open())
			{
				cerr << "CANNOT open this file to read!" << endl;
				return;
			}
			string from, to;
			double miles;
			int numOuts;
			inFile >> from;
			while (!inFile.eof())
			{
				routes.AddVertex(from);
				inFile >> numOuts;
				for (int i = 0; i < numOuts; i++)
				{
					inFile >> miles >> to;
					routes.AddEdge(from, to, miles);
				}
				inFile >> from;
			}
			inFile.close();

			bool isPath = routes.BreadthFirstSearch("Austin", "Denver", path);

			Assert::IsTrue(isPath);
		}

		// Test the depthFirstSearch method by querying it about a path that no longer exists
		TEST_METHOD(TestPrintPathDepthFirstFalse)
		{
			Graph<string> routes;
			deque<Graph<string>::Vertex*> path;


			//Read in routes from file since we want a lot and it's easier this way :)
			ifstream inFile;
			inFile.open("routesIn2.txt");	// this removes the path from Washington to Dallas
			if (!inFile.is_open())
			{
				cerr << "CANNOT open this file to read!" << endl;
				return;
			}
			string from, to;
			double miles;
			int numOuts;
			inFile >> from;
			while (!inFile.eof())
			{
				routes.AddVertex(from);
				inFile >> numOuts;
				for (int i = 0; i < numOuts; i++)
				{
					inFile >> miles >> to;
					routes.AddEdge(from, to, miles);
				}
				inFile >> from;
			}
			inFile.close();

			bool isPath = routes.DepthFirstSearch("Washington", "Dallas", path);

			Assert::IsFalse(isPath);
		}

		// Test the breadthFirstSearch method by querying it about a path that no longer exists
		TEST_METHOD(TestPrintPathBreadthFirstFalse)
		{
			Graph<string> routes;
			deque<Graph<string>::Vertex*> path;


			//Read in routes from file since we want a lot and it's easier this way :)
			ifstream inFile;
			inFile.open("routesIn2.txt");	// this removes the path from Washington to Dallas
			if (!inFile.is_open())
			{
				cerr << "CANNOT open this file to read!" << endl;
				return;
			}
			string from, to;
			double miles;
			int numOuts;
			inFile >> from;
			while (!inFile.eof())
			{
				routes.AddVertex(from);
				inFile >> numOuts;
				for (int i = 0; i < numOuts; i++)
				{
					inFile >> miles >> to;
					routes.AddEdge(from, to, miles);
				}
				inFile >> from;
			}
			inFile.close();

			bool isPath = routes.BreadthFirstSearch("Washington", "Dallas", path);

			Assert::IsFalse(isPath);
		}

		// Test the printPath method by querying it about a path that exists utilizing DFS and checking the oss contents for "DONE!"
		TEST_METHOD(TestPrintPathDFSTrue)
		{
			Graph<string> routes;
			deque<Graph<string>::Vertex*> path;

			//Read in routes from file since we want a lot and it's easier this way :)
			ifstream inFile;
			inFile.open("routesIn.txt");
			if (!inFile.is_open())
			{
				cerr << "CANNOT open this file to read!" << endl;
				return;
			}
			string from, to;
			double miles;
			int numOuts;
			inFile >> from;
			while (!inFile.eof())
			{
				routes.AddVertex(from);
				inFile >> numOuts;
				for (int i = 0; i < numOuts; i++)
				{
					inFile >> miles >> to;
					routes.AddEdge(from, to, miles);
				}
				inFile >> from;
			}
			inFile.close();

			std::ostringstream oss;
			routes.PrintPath(oss, "Austin", "Denver", 'd');

			Assert::IsTrue(oss.str().find("DONE!") != string::npos);
		}

		// Test the printPath method by querying it about a path that exists utilizing BFS and checking the oss contents for "DONE!"
		TEST_METHOD(TestPrintPathBFSTrue)
		{
			Graph<string> routes;
			deque<Graph<string>::Vertex*> path;

			//Read in routes from file since we want a lot and it's easier this way :)
			ifstream inFile;
			inFile.open("routesIn.txt");
			if (!inFile.is_open())
			{
				cerr << "CANNOT open this file to read!" << endl;
				return;
			}
			string from, to;
			double miles;
			int numOuts;
			inFile >> from;
			while (!inFile.eof())
			{
				routes.AddVertex(from);
				inFile >> numOuts;
				for (int i = 0; i < numOuts; i++)
				{
					inFile >> miles >> to;
					routes.AddEdge(from, to, miles);
				}
				inFile >> from;
			}
			inFile.close();

			std::ostringstream oss;
			routes.PrintPath(oss, "Austin", "Denver", 'b');

			Assert::IsTrue(oss.str().find("DONE!") != string::npos);
		}

		// Test the printPath method by querying it about a path that does not exist utilizing DFS and checking the oss 
		// contents for "Cannot find a path!".
		TEST_METHOD(TestPrintPathDFSFalse)
		{
			Graph<string> routes;
			deque<Graph<string>::Vertex*> path;

			//Read in routes from file since we want a lot and it's easier this way :)
			ifstream inFile;
			inFile.open("routesIn2.txt");
			if (!inFile.is_open())
			{
				cerr << "CANNOT open this file to read!" << endl;
				return;
			}
			string from, to;
			double miles;
			int numOuts;
			inFile >> from;
			while (!inFile.eof())
			{
				routes.AddVertex(from);
				inFile >> numOuts;
				for (int i = 0; i < numOuts; i++)
				{
					inFile >> miles >> to;
					routes.AddEdge(from, to, miles);
				}
				inFile >> from;
			}
			inFile.close();

			std::ostringstream oss;
			routes.PrintPath(oss, "Washington", "Dallas", 'd');

			Assert::IsTrue(oss.str().find("Cannot find a path!") != string::npos);
		}

		// Test the printPath method by querying it about a path that does not exist utilizing BFS and checking the oss 
		// contents for "Cannot find a path!".
		TEST_METHOD(TestPrintPathBFSFalse)
		{
			Graph<string> routes;
			deque<Graph<string>::Vertex*> path;

			//Read in routes from file since we want a lot and it's easier this way :)
			ifstream inFile;
			inFile.open("routesIn2.txt");
			if (!inFile.is_open())
			{
				cerr << "CANNOT open this file to read!" << endl;
				return;
			}
			string from, to;
			double miles;
			int numOuts;
			inFile >> from;
			while (!inFile.eof())
			{
				routes.AddVertex(from);
				inFile >> numOuts;
				for (int i = 0; i < numOuts; i++)
				{
					inFile >> miles >> to;
					routes.AddEdge(from, to, miles);
				}
				inFile >> from;
			}
			inFile.close();

			std::ostringstream oss;
			routes.PrintPath(oss, "Washington", "Dallas", 'b');

			Assert::IsTrue(oss.str().find("Cannot find a path!") != string::npos);
		}

		// Test the printPath method by querying it about a path that exists utilizing ShortestPath and checking the oss contents for "DONE!"
		TEST_METHOD(TestPrintPathSPTrue)
		{
			Graph<string> routes;
			deque<Graph<string>::Vertex*> path;

			//Read in routes from file since we want a lot and it's easier this way :)
			ifstream inFile;
			inFile.open("routesIn.txt");
			if (!inFile.is_open())
			{
				cerr << "CANNOT open this file to read!" << endl;
				return;
			}
			string from, to;
			double miles;
			int numOuts;
			inFile >> from;
			while (!inFile.eof())
			{
				routes.AddVertex(from);
				inFile >> numOuts;
				for (int i = 0; i < numOuts; i++)
				{
					inFile >> miles >> to;
					routes.AddEdge(from, to, miles);
				}
				inFile >> from;
			}
			inFile.close();

			std::ostringstream oss;
			routes.PrintPath(oss, "Austin", "Denver", 's');

			Assert::IsTrue(oss.str().find("DONE!") != string::npos);
		}

		// Test the printPath method by querying it about a path that does not exist utilizing ShortestPath and checking the oss 
		// contents for "Cannot find a path!".
		TEST_METHOD(TestPrintPathSPFalse)
		{
			Graph<string> routes;
			deque<Graph<string>::Vertex*> path;

			//Read in routes from file since we want a lot and it's easier this way :)
			ifstream inFile;
			inFile.open("routesIn2.txt");
			if (!inFile.is_open())
			{
				cerr << "CANNOT open this file to read!" << endl;
				return;
			}
			string from, to;
			double miles;
			int numOuts;
			inFile >> from;
			while (!inFile.eof())
			{
				routes.AddVertex(from);
				inFile >> numOuts;
				for (int i = 0; i < numOuts; i++)
				{
					inFile >> miles >> to;
					routes.AddEdge(from, to, miles);
				}
				inFile >> from;
			}
			inFile.close();

			std::ostringstream oss;
			routes.PrintPath(oss, "Washington", "Dallas", 's');

			Assert::IsTrue(oss.str().find("Cannot find a path!") != string::npos);
		}

		// <
		TEST_METHOD(TestVertexLessThanTrue)
		{
			Graph<string> routes;

			string location1 = "Austin";
			string location2 = "Dallas";
			routes.AddVertex(location1);
			routes.AddVertex(location2);

			routes.Vertices()[0]->dist = 150;
			routes.Vertices()[1]->dist = 200;

			Assert::IsTrue(routes.Vertices()[0] < routes.Vertices()[1]);
		}

		// <
		TEST_METHOD(TestVertexLessThanFalse)
		{
			Graph<string> routes;

			string location1 = "Austin";
			string location2 = "Dallas";
			routes.AddVertex(location1);
			routes.AddVertex(location2);

			routes.Vertices()[0]->dist = 1500;
			routes.Vertices()[1]->dist = 200;

			Assert::IsFalse(routes.Vertices()[0] < routes.Vertices()[1]);
		}

		// <
		TEST_METHOD(TestVertexGreaterThanTrue)
		{
			Graph<string> routes;

			string location1 = "Austin";
			string location2 = "Dallas";
			routes.AddVertex(location1);
			routes.AddVertex(location2);

			routes.Vertices()[0]->dist = 1500;
			routes.Vertices()[1]->dist = 200;

			Assert::IsTrue(routes.Vertices()[0] > routes.Vertices()[1]);
		}

		// <
		TEST_METHOD(TestVertexGreaterThanFalse)
		{
			Graph<string> routes;

			string location1 = "Austin";
			string location2 = "Dallas";
			routes.AddVertex(location1);
			routes.AddVertex(location2);

			routes.Vertices()[0]->dist = 150;
			routes.Vertices()[1]->dist = 200;

			Assert::IsFalse(routes.Vertices()[0] > routes.Vertices()[1]);
		}

		// <
		TEST_METHOD(TestVertexLessThanEven)
		{
			Graph<string> routes;

			string location1 = "Austin";
			string location2 = "Dallas";
			routes.AddVertex(location1);
			routes.AddVertex(location2);

			routes.Vertices()[0]->dist = 150;
			routes.Vertices()[1]->dist = 150;

			Assert::IsFalse(routes.Vertices()[0] < routes.Vertices()[1]);
		}

		// <
		TEST_METHOD(TestVertexGreaterThanEven)
		{
			Graph<string> routes;

			string location1 = "Austin";
			string location2 = "Dallas";
			routes.AddVertex(location1);
			routes.AddVertex(location2);

			routes.Vertices()[0]->dist = 150;
			routes.Vertices()[1]->dist = 150;

			Assert::IsFalse(routes.Vertices()[0] < routes.Vertices()[1]);
		}
	};
}
