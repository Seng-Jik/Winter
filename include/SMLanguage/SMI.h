#pragma once

//#define SMI_DEBUG

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <stack>
#include <algorithm>
#include <sstream>
#include <memory.h>

#define SAVE_SIZE 65536


#ifndef SMI_DEBUG
#include "../Snow/Bundle.h"
#include "../Snow/ResFile.h"

#else
typedef char BYTE;
#endif
namespace SMI
{
	enum SMEType
	{
		CMD,
		NAME,
		SAY,
		SAYEND,
		END,
	};

	struct SMEvent
	{
		SMEType type;
		std::wstring cmd;
		std::vector<std::wstring> argv;
	};

	struct SingleWord
	{
		wchar_t word;
		bool is_transformded;
		bool is_endl;
	};

	class Interpreter
	{
	private:
		enum State
		{
			TEXT,
			TEXT_ENDL,
			NAME,
			LABEL_IN,
			LABEL_READING,
			LABEL_EXITING,
			CMD_MULTI,
			CMD_SINGLE,

		};

		SMEvent now_event;

		std::string file_name;

		State now_state;
		std::stack<State> stk_state;

		const std::unordered_set<wchar_t> keywords = { L'<', L'>', L'[', L']', L'!', L'/', L'\\' };

		std::unordered_map<std::wstring, int> labels;
		std::unordered_map<std::wstring, int> flags;
		bool is_text;
		bool is_end;

		std::wstring cmd;
		std::vector<std::wstring> str_args;
		std::vector<int> num_args;

		std::wstring text;
		std::wstring name;

		std::vector<std::wstring> all_text;
		size_t now_line;

		unsigned short now_pos;

	public:
		Interpreter();
		~Interpreter();

		bool LoadStory(const std::string& filename, bool is_encoding);
		bool PullEvent(SMEvent& out_event);

#ifdef SMI_DEBUG
		void Save(const std::string save_file);
		bool Load(const std::string save_file);
#else
		void Save(::Snow::Bundle<SAVE_SIZE>&);	//保存解释器状态以存档
		bool Load(::Snow::Bundle<SAVE_SIZE>&);	//读取解释器状态
#endif

		size_t Serialize(BYTE *buffer);
		void Unserialize(BYTE *buffer);

		void InfoOut(std::wostream &out);

		//I need Goto Interface
		void Goto(const std::wstring& label);

	private:
		bool loadText(std::istream& file);

		//syntax analysis
		bool runOneStep();	//get next cmd or text to analysis
		bool analysis();	//analysis a sentence and prepare handle it
		SingleWord getNextChar();
		void backToBeforeChar();
		bool isKeyword(SingleWord word);

		//semantic analysis
		void operate(const std::wstring& str);
		void cmdAnalysis(const std::wstring& command);
		void cmdOperate();

		bool gotoLable(const std::wstring&);	//跳转到某个标签
		bool outStack();	//跳出栈区，自动略过子栈区

		//state help func
		void changeState(State state);
		void switchState(State state);
		State returnState();

		//init help func
		void addLabel();
		void scanFile();

		//general help func
		void splitArgs(std::wstring& str,std::vector<std::wstring>& arg_array);
		std::wstring trim(std::wstring str);

		void debug(std::ostream& out);
	};
}
