//MIT License
//Copyright 2017 Patrick Laughrea
#pragma once

#include <string>
#include <vector>

#include "various/smartIterator.hpp"

namespace ntt
{
	class Parser
	{
	public:
		Parser() = delete;
		
		static std::vector<std::string> parse(various::SmartIterator it)
		{
			return Parser(std::move(it)).parse();
		}
		
	private:
		various::SmartIterator it;
		std::string left, right, leftover;
		
		Parser(various::SmartIterator it);
		~Parser();
		
		std::vector<std::string> parse();
		
		void readStart();
		void readNextTag();
		bool getLine();
	};
}