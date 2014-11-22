#include "Utilities.h"
#include "Random.h"
#include "Timer.h"
#include "BoardPoint.h"
#include "BoardMask.h"
#include "Move.h"
#include "Board.h"
#include "DepthEstimator.h"
#include "HeatMap.h"
#include "BoardNode.h"
#include "TreeNode.h"
#include "GameInputOutput.h"
#include "Dijkstra.h"

/// TODO: Can we combine MCTS with the cellular automata supperoptimization?
/// Like hashlife—I think we can

int main(int argc, char* argv[]) funk;

uint64 hashBoard(const Board& board);

void benchmarkRollout()
{
	// Benchmark
	const Board startingBoard;
	TreeNode root;
	uint simulations = 0;
	Timer::instance.update();
	float start = Timer::instance.remaining();
	while(Timer::instance.remaining() > 20.0) {
		root.rollOut(startingBoard);
		++simulations;
		Timer::instance.update();
	}
	float stop = Timer::instance.remaining();
	cerr << "Time: " << (start - stop) << endl;
	cerr << "Simulations: " << simulations << endl;
	cerr << "S/sec: " << (float(simulations)/(start - stop)) << endl;
}

void benchmarkSelect()
{
	// Benchmark
	const Board startingBoard;
	TreeNode root;
	uint simulations = 0;
	Timer::instance.update();
	float start = Timer::instance.remaining();
	while(Timer::instance.remaining() > 20.0) {
		root.selectAction(startingBoard);
		++simulations;
		Timer::instance.update();
	}
	float stop = Timer::instance.remaining();
	cerr << "Time: " << (start - stop) << endl;
	cerr << "Simulations: " << simulations << endl;
	cerr << "S/sec: " << (float(simulations)/(start - stop)) << endl;
}

void recurse(const BoardNode::OrientedBoardNode& node)
{
	if(node.second->height() > 0)
		for(uint i = 0; i < 4; ++i)
			recurse(node.second->piece(i));
	if(node.second->height() > 1)
		for(uint i = 4; i < 9; ++i)
			recurse(node.second->piece(i));
}

void recurse(TreeNode* tree)
{
	cerr << BoardNode::fragmentCount() << endl;
	recurse(BoardNode::get(tree->board()));
	for(TreeNode* child = tree->firstChild(); child; child = child->sibling())
		recurse(child);
}

int main(int argc, char* argv[])
{
	Timer::instance.start();
	cerr << "R " << argv[0]  << endl;
	cerr << "RAND_MAX = " << RAND_MAX << endl;
	cerr << "sizeof(bool) = " << sizeof(bool) << endl;
	cerr << "sizeof(uint) = " << sizeof(uint) << endl;
	cerr << "sizeof(uint64) = " << sizeof(uint64) << endl;
	cerr << "sizeof(uint128) = " << sizeof(uint128) << endl;
	cerr << "sizeof(void*) = " << sizeof(void*) << endl;
	cerr << "sizeof(std::size_t) = " << sizeof(std::size_t) << endl;
	cerr << "sizeof(Rotation) = " << sizeof(Rotation) << endl;
	cerr << "sizeof(BoardPoint) = " << sizeof(BoardPoint) << endl;
	cerr << "sizeof(Move) = " << sizeof(Move) << endl;
	cerr << "sizeof(BoardMask) = " << sizeof(BoardMask) << endl;
	cerr << "sizeof(Board) = " << sizeof(Board) << endl;
	cerr << "sizeof(BoardNode) = " << sizeof(BoardNode) << endl;
	cerr << "sizeof(TreeNode) = " << sizeof(TreeNode) << endl;
	srand(time(0));
	BoardNode::initialize();
	BoardMask::initialize();
	assert(BoardNode::fragmentCount() == 37);
	cerr << "Initialized" << endl;
	
	TreeNode gameTree;
	gameTree.loadGames("games.csv");
	cerr << "Got " << BoardNode::fragmentCount() << " fragments" << endl;
	
	return 0;
	
	GameInputOutput gio;
	gio.run();
	cerr << "Exit" << endl;
	Timer::instance.stop();
	return 0;
}
