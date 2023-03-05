#include "pch.h"
#include "CppUnitTest.h"
#include "../runewars/main.h"
#include <unordered_map>
#include <stdio.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(whoWinsTest)
	{
	public:
		std::unordered_map<std::string, Move> moves;
		TEST_METHOD_INITIALIZE(testclass_init) {
			for (auto kv_pair : moves_vector) {
				moves[kv_pair.first] = kv_pair.second;
			}
		}
		TEST_METHOD(mp_rf)
		{
			int x = whoWins(moves["mp"], moves["rf"], 1, 2);
			Assert::AreEqual(1, x);
		}
		TEST_METHOD(rf_mp)
		{
			int x = whoWins(moves["rf"], moves["mp"], 1, 2);
			Assert::AreEqual(2, x);
		}
		TEST_METHOD(rf_lm)
		{
			int x = whoWins(moves["rf"], moves["lm"], 1, 2);
			Assert::AreEqual(0, x);
		}
		TEST_METHOD(mp_p)
		{
			int x = whoWins(moves["mp"], moves["p"], 1, 2);
			Assert::AreEqual(-1, x);
		}
		TEST_METHOD(t_rf)
		{
			int x = whoWins(moves["t"], moves["rf"], 1, 2);
			Assert::AreEqual(2, x);
		}
		TEST_METHOD(rf_t)
		{
			int x = whoWins(moves["rf"], moves["t"], 1, 2);
			Assert::AreEqual(1, x);
		}
	};
	TEST_CLASS(ActionsTest)
	{
		std::vector<std::pair<std::string, Move>> fullMoveName_vector;
		std::unordered_map<std::string, Move> moves;
		std::unordered_map<std::string, Move> fullMoveName;
	public:
		TEST_METHOD_INITIALIZE(actions_init) {
			fullMoveName_vector = createFullMoveNameVector(moves_vector);
			for (auto kv_pair : moves_vector) {
				moves[kv_pair.first] = kv_pair.second;
			}
			for (auto kv_pair : fullMoveName_vector) {
				fullMoveName[kv_pair.first] = kv_pair.second;
			}
		}
		TEST_METHOD(sb_rv) {
			Player p1 = *new Player();
			Player p2 = *new Player();
			std::unordered_map<std::string, Move> m = moves;
			auto play = [&p1, &p2, &m](std::string input, std::string input2) {
				Actions(m, 20, input, input2, p1, p2);
			};
			play("sb", "rv");
			play("rv", "sb");
			Assert::AreEqual(0, p1.scores);
			Assert::AreEqual(0, p2.scores);
		}
	};
}
