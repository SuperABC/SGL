all:
	g++ -c -o Debug/frame.o SGL/frame.cpp -I pack/include/
	g++ -c -o Debug/geometry.o SGL/geometry.cpp -I pack/include/
	#g++ -c -o Debug/graph.o SGL/graph.cpp -I pack/include/
	g++ -c -o Debug/imfile.o SGL/imfile.cpp -I pack/include/
	g++ -c -o Debug/data.o SGL/data.cpp -I pack/include/
	g++ -c -o Debug/encryption.o SGL/encryption.cpp -I pack/include/
	