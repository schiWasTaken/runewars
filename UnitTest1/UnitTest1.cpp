#include "pch.h"
#include "CppUnitTest.h"
#include "../runewars/main.h"
#include <unordered_map>
#include <stdio.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
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
			Assert::AreEqual(x, 1);
		}
		TEST_METHOD(rf_mp)
		{
			int x = whoWins(moves["rf"], moves["mp"], 1, 2);
			Assert::AreEqual(x, 2);
		}
		TEST_METHOD(rf_lm)
		{
			int x = whoWins(moves["rf"], moves["lm"], 1, 2);
			Assert::AreEqual(x, 0);
		}
		TEST_METHOD(mp_p)
		{
			int x = whoWins(moves["mp"], moves["p"], 1, 2);
			Assert::AreEqual(x, -1);
		}
		TEST_METHOD(t_rf)
		{
			int x = whoWins(moves["t"], moves["rf"], 1, 2);
			Assert::AreEqual(x, 2);
		}
		TEST_METHOD(rf_t)
		{
			int x = whoWins(moves["rf"], moves["t"], 1, 2);
			Assert::AreEqual(x, 1);
		}
	};
}
