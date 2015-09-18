#ifndef _HEAD_CORE_CSVEditor_HEAD_
#define _HEAD_CORE_CSVEditor_HEAD_

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

#define EOB	0	// end of block
#define EOF (-1)

typedef std::pair<size_t, size_t> position;

namespace Core{

	class CSVEditor
	{
	private:
		bool in_block;
		const char *now_pos;
		std::string empty;
		std::vector<std::string> lines;

		size_t now_x;
		size_t now_y;

		size_t max_x;
		size_t max_y;

		std::map<position, std::string> table;

	public:
		CSVEditor();

		void LoadCSV(const std::string& filename);
		void SaveCSV(const std::string& filename);
		void Create(size_t w, size_t h);

		const std::string& Str(const size_t x, const size_t y);
		int Int(const size_t x, const size_t y);
		double Float(const size_t x, const size_t y);
		void Set(size_t x, size_t y, const std::string& val);
		void Set(size_t x, size_t y, int val);
		void Set(size_t x, size_t y, double val);

		size_t Width();
		size_t Height();

	private:
		bool open(const std::string & filename);

		char getChar();
		bool getWord(std::string & word, size_t &pos_x, size_t &pos_y);
		void append2Map();

		std::string & formatBlock(std::string & val);
		void rewriteLines();
		std::string & str_offset(std::string & dst, int offset, int start_pos);
	};

}

#endif

