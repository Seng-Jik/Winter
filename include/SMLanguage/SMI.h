#pragma once

//#define DEBUG

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


#ifndef DEBUG
#include "../Core/Bundle.h"
#include "../Core/ResFile.h"

#else

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

		std::unordered_set<wchar_t> keywords = { L'<', L'>', L'[', L']', L'!', L'/', L'\\' };

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

		Uint16 now_pos;

	public:
		Interpreter();
		~Interpreter();

		bool RunOneStep();

#ifndef DEBUG
		void Save(Core::Bundle<SAVE_SIZE>&);	//保存解释器状态以存档
		bool Load(Core::Bundle<SAVE_SIZE>&);	//读取解释器状态
#endif
		//New interface
		bool LoadStory(const std::string& filename, bool is_encoding);
		bool PullEvent(SMEvent& out_event);

		//Old interface
										//判断类函数
		bool NextIsText();	//下一个位置是否为对话
		bool CmdOver();	//指令是否读取完成

						//读取类函数
		std::wstring PopCmd();	//读取指令，包括断点和出栈指令
		void PopArg(std::vector<std::wstring>&);	//读取文字参数，如果有双引号则自动去掉
		void PopIntArg(std::vector<int>&);	//读取数字参数

		std::wstring PopText();	//读取文字，到指令开始前，或者到文字结束符之前，如果文字已结束，则返回文字结束符
		std::wstring PopName();	//读取当前对话者名称

							//操作类函数
		bool Goto(const std::wstring&);	//跳转到某个标签
		bool OutStack();	//跳出栈区，自动略过子栈区

		size_t Serialize(BYTE *buffer);
		void Unserialize(BYTE *buffer);

		void InfoOut(std::wostream &out);

	private:
		bool loadText(std::istream& file);

		bool analysis();
		SingleWord getNextChar();
		void backToBeforeChar();

		void handler(const std::wstring& str);

		void cmdAnalysis(const std::wstring& command);
		void cmdOperate();

		void changeState(State state);
		void switchState(State state);
		State returnState();

		bool isKeyword(SingleWord word);
		void addLabel();
		void scanFile();

		void splitArgs(std::wstring& str,std::vector<std::wstring>& arg_array);
		std::wstring trim(std::wstring str);

		void debug(std::ostream& out);
	};
}
