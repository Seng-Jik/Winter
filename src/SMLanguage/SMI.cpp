#include "../../include/SMLanguage/SMI.h"

using namespace SMI;
#ifndef SMI_DEBUG
using namespace ::Snow;
#endif
using namespace std;

string ws2s(const wstring& ws);
wstring s2ws(const string& s);

SMI::Interpreter::Interpreter()
{
}

SMI::Interpreter::~Interpreter()
{
}

bool SMI::Interpreter::LoadStory(const string & filename, bool is_encoding)
{
	bool result = false;
#ifndef SMI_DEBUG
	ResFile res;
	res.Load(string(filename.c_str()));

	string str = "";
	for (unsigned int i = 0; i < res.Size(); ++i) {
		str += ((char*)res)[i];
	}
	istringstream file(str);

	//istringstream file(string(res));
	result = loadText(file);
#else
	ifstream pf(filename, ios::binary);
	pf.seekg(0, ios::end);
	size_t fsize = pf.tellg();
	pf.seekg(0, ios::beg);
	BYTE *buf = new BYTE[fsize];

	pf.read((char*)buf, fsize);

	string str = "";
	for (unsigned int i = 0; i < fsize; ++i) {
		str += ((char*)buf)[i];
	}
	istringstream file(str);
	result = loadText(file);
	delete[] buf;
	pf.close();

#endif
	return result;
}

bool SMI::Interpreter::PullEvent(SMEvent & out_event)
{
	bool result = runOneStep();
	out_event = now_event;
	return result;
}

#ifdef SMI_DEBUG
void SMI::Interpreter::Save(const string save_file)
{
	ofstream file(save_file, ios::binary);

	BYTE buffer[0xffff];
	size_t size = Serialize(buffer);
	file.write(buffer, size);
	file.close();
}

bool SMI::Interpreter::Load(const string save_file)
{
	ifstream file(save_file, ios::binary);

	BYTE buffer[0xffff];
	BYTE *p = buffer;
	memset(buffer, 0, 0xffff);

	while (!file.eof()) {
		*p = file.get();
		++p;
	}

	Unserialize(buffer);
	return true;
}
#else
void SMI::Interpreter::Save(Bundle<SAVE_SIZE> &bundle)
{
	BYTE buffer[SAVE_SIZE];
	size_t size = Serialize(buffer);
	for (size_t i = 0; i < size; ++i) {
		bundle.Write<BYTE>(buffer[i]);
	}
}

bool SMI::Interpreter::Load(Bundle<SAVE_SIZE> &bundle)
{
	BYTE buffer[SAVE_SIZE];
	for (size_t i = 0; i < SAVE_SIZE; ++i) {
		bundle.Read<BYTE>(buffer[i]);
	}
	Unserialize(buffer);
	return true;
}
#endif

size_t SMI::Interpreter::Serialize(BYTE *buffer)
{
	BYTE *p = buffer;

	/**(int*)p = file_name.length();
	p += sizeof(int);
	file_name.copy((char*)p, file_name.length());
	p += sizeof(char) * file_name.length();*/

	*(State*)p = now_state;
	p += sizeof(State);

	*(int*)p = stk_state.size();
	p += sizeof(int);
	stack<State> stk_temp;
	while (stk_state.size()) {
		stk_temp.push(stk_state.top());
		stk_state.pop();
	}
	while (stk_temp.size()) {
		*(State*)p = stk_temp.top();
		stk_state.push(stk_temp.top());
		p += sizeof(State);
		stk_temp.pop();
	}

	/**(int*)p = keywords.size();
	p += sizeof(int);
	for (auto var : keywords) {
		*(wchar_t*)p = var;
		p += sizeof(wchar_t);
	}*/

	/**(int*)p = labels.size();
	p += sizeof(int);
	for (auto var : labels) {
		*(int*)p = var.first.length();
		p += sizeof(int);
		var.first.copy((wchar_t*)p, var.first.length());
		p += sizeof(char) * var.first.length();

		*(int*)p = var.second;
		p += sizeof(int);
	}*/

	*(int*)p = flags.size();
	p += sizeof(int);
	for (auto var : flags) {
		*(int*)p = var.first.length();
		p += sizeof(int);
		var.first.copy((wchar_t*)p, var.first.length());
		p += sizeof(wchar_t) * var.first.length();

		*(int*)p = var.second;
		p += sizeof(int);
	}

	*(bool*)p++ = is_text;
	*(bool*)p++ = is_end;

	/**(int*)p = cmd.length();
	p += sizeof(int);
	cmd.copy((wchar_t*)p, cmd.length());
	p += sizeof(wchar_t) * cmd.length();

	*(int*)p = str_args.size();
	p += sizeof(int);
	for (auto var : str_args) {
		*(int*)p = var.length();
		p += sizeof(int);
		var.copy((wchar_t*)p, var.length());
		p += sizeof(wchar_t) * var.length();
	}

	*(int*)p = num_args.size();
	p += sizeof(int);
	for (auto var : num_args) {
		*(int*)p = var;
		p += sizeof(int);
	}

	*(int*)p = text.length();
	p += sizeof(int);
	text.copy((wchar_t*)p, text.length());
	p += sizeof(wchar_t) * text.length();

	*(int*)p = name.length();
	p += sizeof(int);
	name.copy((wchar_t*)p, name.length());
	p += sizeof(wchar_t) * name.length();
	*/

	/**(int*)p = all_text.size();
	p += sizeof(int);
	for (auto var : all_text) {
		*(int*)p = var.length();
		p += sizeof(int);
		var.copy((wchar_t*)p, var.length());
		p += sizeof(wchar_t) * var.length();
	}*/

	*(size_t*)p = now_line;
	p += sizeof(size_t);

	*(unsigned short*)p = now_pos;

	p += sizeof(unsigned short);

	return p - buffer;
}

void SMI::Interpreter::Unserialize(BYTE *buffer)
{
	BYTE *p = buffer;
	int len = 0;
	int arr_len = 0;
	wstring str_temp = L"";
	int num_temp = 0;

	/*len = *(int*)p;
	p += sizeof(int);
	file_name = "";
	for (int i = 0; i < len; ++i) {
		file_name += (char)*p++;
	}*/

	now_state = *(State*)p;
	p += sizeof(State);

	len = *(int*)p;
	p += sizeof(int);
	for (int i = 0; i < len; ++i) {
		stk_state.push(*(State*)p);
		p += sizeof(State);
	}

	/*len = *(int*)p;
	p += sizeof(int);
	for (int i = 0; i < len; ++i) {
		keywords.insert(*p);
		p += sizeof(wchar_t);
	}*/

	/*arr_len = *(int*)p;
	p += sizeof(int);
	for (int i = 0; i < arr_len; ++i) {
		len = *(int*)p;
		p += sizeof(int);
		str_temp = L"";
		for (int i = 0; i < len; ++i) {
			str_temp += (wchar_t)*p;
			p += sizeof(wchar_t);
		}
		num_temp = *(int*)p;
		p += sizeof(int);

		labels[str_temp] = num_temp;
	}*/

	arr_len = *(int*)p;
	p += sizeof(int);
	for (int i = 0; i < arr_len; ++i) {
		len = *(int*)p;
		p += sizeof(int);
		str_temp = L"";
		for (int i = 0; i < len; ++i) {
			str_temp += (wchar_t)*p;
			p += sizeof(wchar_t);
		}
		num_temp = *(int*)p;
		p += sizeof(int);

		flags[str_temp] = num_temp;
	}

	is_text = *(bool*)p++;
	is_end = *(bool*)p++;

	/*len = *(int*)p;
	p += sizeof(int);
	cmd = L"";
	for (int i = 0; i < len; ++i) {
		cmd += (wchar_t)*p;
		p += sizeof(wchar_t);
	}*/

	/*arr_len = *(int*)p;
	p += sizeof(int);
	for (int i = 0; i < arr_len; ++i) {
		len = *(int*)p;
		p += sizeof(int);
		str_temp = L"";
		for (int i = 0; i < len; ++i) {
			str_temp += (wchar_t)*p;
			p += sizeof(wchar_t);
		}

		str_args.push_back(str_temp);
	}

	arr_len = *(int*)p;
	p += sizeof(int);
	for (int i = 0; i < arr_len; ++i) {
		num_temp = *(int*)p;
		p += sizeof(int);

		num_args.push_back(num_temp);
	}*/

	/*len = *(int*)p;
	p += sizeof(int);
	text = L"";
	for (int i = 0; i < len; ++i) {
		text += (wchar_t)*p;
		p += sizeof(wchar_t);
	}

	len = *(int*)p;
	p += sizeof(int);
	name = L"";
	for (int i = 0; i < len; ++i) {
		name += (wchar_t)*p;
		p += sizeof(wchar_t);
	}*/

	/*arr_len = *(int*)p;
	p += sizeof(int);
	for (int i = 0; i < arr_len; ++i) {
		len = *(int*)p;
		p += sizeof(int);
		str_temp = L"";
		for (int i = 0; i < len; ++i) {
			str_temp += (wchar_t)*p;
			p += sizeof(wchar_t);
		}

		all_text.push_back(str_temp);
	}*/

	now_line = *(size_t*)p;
	p += sizeof(size_t);

	now_pos = *(unsigned short*)p;
}

void SMI::Interpreter::InfoOut(wostream & out)
{
	out << "text: " << text << endl;
	out << "name: " << name << endl;

	out << "cmd: " << cmd << endl;
	out << "args: " << endl;
	for (auto iter : str_args) {
		out << iter << endl;
	}
	for (auto iter : num_args) {
		out << iter << endl;
	}
}

/***************************** private func **********************************/

bool SMI::Interpreter::loadText(istream& file)
{
	string t_str;
	wstring str;

	bool eof = false;

	//read file
	while (!eof) {
		getline(file, t_str);
		str = s2ws(t_str);
		//file.getline(str);
		eof = file.eof();

		//jump the comment
		str = str.substr(0, str.find(L'#'));

		//remove blanks
		str = trim(str);

		all_text.push_back(str);
	}

	//file.close();
	now_state = TEXT;

	//build label list;
	scanFile();
	now_pos = 0;
	return true;
}

bool SMI::Interpreter::runOneStep()
{
	bool result = false;

	if (now_line >= all_text.size()) {
		return false;
	}

	name = L"";
	cmd = L"";
	text = L"";

	//jump commitment or empty line
	while (all_text[now_line].length() == 0 || all_text[now_line].at(0) == '#' || all_text[now_line].at(0) == '\r') {
		++now_line;
		if (now_line >= all_text.size()) {
			return false;
		}
		now_pos = 0;
	}

	if (now_line >= all_text.size()) {
		return false;
	}

	//start analysis
#ifdef SMI_DEBUG
	wcout << all_text[now_line] << endl;
#endif
	do {
		//To do...
		result = analysis();
	} while (now_state == CMD_SINGLE);

	return result;
}


bool SMI::Interpreter::analysis()
{
	SingleWord now_word;

	wstring str = L"";
	bool continue_analysis = true;	//mark if finished a cmd or text

	while (continue_analysis) {
		now_word = getNextChar();
		if (!now_word.word) {
			return false;
		}

		if (isKeyword(now_word)) {
			if (now_state == TEXT && str.length()) {
				backToBeforeChar();
				continue_analysis = false;
				break;
			}
			switch (now_word.word) {
			case L'<':
				changeState(CMD_SINGLE);
				break;
			case L'>':
				continue_analysis = false;
				break;
			case L'[':
				changeState(NAME);
				break;
			case L']':
				continue_analysis = false;
				break;
			case L'!':
				changeState(CMD_MULTI);
				changeState(CMD_SINGLE);
				break;
			case L'/':
				returnState();
				break;
			case L'\\':
				changeState(TEXT_ENDL);
				continue_analysis = false;
				break;
			default:
				break;
			}
		}
		else {
			str += now_word.word;
			if (now_word.is_endl && now_state == CMD_MULTI) {
				continue_analysis = false;
			}
		}
	}

	operate(str);
	return true;
}

SingleWord SMI::Interpreter::getNextChar()
{
	SingleWord result;

	while (all_text[now_line][now_pos] == '\0') {
        ++now_line;
		now_pos = 0;
	}

	if (all_text[now_line][now_pos + 1] == L'\n' || all_text[now_line][now_pos + 1]  == L'\r' || all_text[now_line][now_pos + 1] == L'\0') {
		result.is_endl = true;
	} else {
		result.is_endl = false;
	}

	result.is_transformded = false;
	/*if (*now_pos == '\\') {
		result.is_transformded = true;
		++now_pos;
	} else {
		result.is_transformded = false;
	}*/

	result.word = all_text[now_line][now_pos] ;

	if (result.is_endl) {
		++now_line;
		if (all_text.size() > now_line) {
			now_pos = 0;
		}
	} else {
		++now_pos;
	}
	return result;
}

void SMI::Interpreter::backToBeforeChar()
{
	if (now_pos != 0) {
		--now_pos;
	} else {
		--now_line;
		now_pos = all_text[now_line].length() - 1;
	}
}

bool SMI::Interpreter::isKeyword(SingleWord word)
{
	if (word.is_transformded) {
		return false;
	}

	bool result = keywords.find(word.word) != keywords.end();

	if (word.word == L'!' || word.word == L'/') {
		result &= now_state == CMD_SINGLE || now_state == CMD_MULTI;
	}

	return result;
}

void SMI::Interpreter::operate(const wstring & str)
{
	now_event.type = SMI::SMEType::END;
	now_event.cmd.clear();
	now_event.argv.clear();

	switch (now_state)
	{
	case SMI::Interpreter::TEXT:
		text = str;
		now_event.type = SMI::SMEType::SAY;
		now_event.argv.push_back(str);
		break;
	case SMI::Interpreter::TEXT_ENDL:
		now_event.type = SMI::SMEType::SAYEND;
		returnState();
		break;
	case SMI::Interpreter::NAME:
		name = str;
		now_event.type = SMI::SMEType::NAME;
		now_event.argv.push_back(str);
		returnState();
		break;
	case SMI::Interpreter::CMD_MULTI:
		cmdAnalysis(str);
		break;
	case SMI::Interpreter::CMD_SINGLE:
		cmdAnalysis(str);
		returnState();
		break;
	default:
		break;
	}
}

void SMI::Interpreter::cmdAnalysis(const wstring & command)
{
	vector<wstring> arg_array;
	wstring raw_args = L"";
	if (now_state != CMD_MULTI && now_state != CMD_SINGLE) {
		return;
	}

	if (!command.length()) {
		return;
	}

	//init arg_val
	num_args.clear();
	str_args.clear();

	size_t cut_pos = command.find(L':');
	//int cut_pos2 = string::npos;
	cmd = command.substr(0, cut_pos);
	if (cut_pos != wstring::npos) {
		raw_args = command.substr(cut_pos + 1);

		//remove blank
		raw_args = trim(raw_args);

		splitArgs(raw_args, arg_array);
	} else {
		cout << "empty!!!" << endl;
	}

	//new interface set
	now_event.type = SMI::SMEType::CMD;
	now_event.cmd = cmd;
	now_event.argv.insert(now_event.argv.begin(), arg_array.begin(), arg_array.end());

	//start
	if (cmd == L"flag") {
		str_args.push_back(arg_array[0]);
		if (arg_array.size() == 1) {	// <flag: name>
			num_args.push_back(0);
		}
		else {						// <flag: name, val>
			num_args.push_back(atoi(ws2s(arg_array[1]).c_str()));
		}
	}
	else if (cmd == L"flag_add") {		// <flag_add: name, val>
		num_args.push_back(atoi(ws2s(arg_array[1]).c_str()));
		str_args.push_back(arg_array[0]);

	}
	else if (cmd == L"flag_del") {		// <flag_del: name>
		str_args.push_back(arg_array[0]);
	}
	else if (cmd == L"gotoLable") {		// <gotoLable: name>
		str_args.push_back(arg_array[0]);
	}
	else if (cmd == L"if") {
		str_args.push_back(arg_array[0]);
		for (auto iter = arg_array.begin() + 1; iter != arg_array.end(); ++iter) {
			num_args.push_back(atoi(ws2s(*iter).c_str()));
		}
	}
	else if (cmd == L"select") {		// <select: opt1, opt2, ...>
		for (auto var : arg_array) {
			str_args.push_back(var);
		}
	}
	else {
		str_args.push_back(raw_args);
	}
	cmdOperate();
}

void SMI::Interpreter::cmdOperate()
{
	if (cmd == L"flag") {
		flags[str_args[0]] = num_args[0];
	}
	else if (cmd == L"end") {
		is_end = true;
	}
	else if (cmd == L"flag_add") {		// <flag_add: name, val>
		if (flags.find(str_args[0]) != flags.end()) {
			flags[str_args[0]] += num_args[0];
		}
	}
	else if (cmd == L"flag_del") {		// <flag_del: name>
		flags.erase(str_args[0]);
	}
	else if (cmd == L"gotoLable") {		// <gotoLable: name>
		gotoLable(str_args[0]);
	}
	else if (cmd == L"if") {
		bool test_result = false;
		switch (num_args.size()) {
		case 0:
			test_result = flags.find(str_args[0]) != flags.end();
			break;
		case 1:
			if (flags.find(str_args[0]) != flags.end()) {
				test_result = flags[str_args[0]] == num_args[0];
			}
			break;
		case 2:
			if (flags.find(str_args[0]) != flags.end()) {
				test_result = (flags[str_args[0]] >= num_args[0] && flags[str_args[0]] < num_args[1]);
			}
			break;
		default:
			break;
		}
		//Todo
		changeState(CMD_SINGLE);
		changeState(CMD_MULTI);

		if (!test_result) {
			outStack();
		}
	}
}
bool SMI::Interpreter::gotoLable(const wstring& label)
{
	auto iter = labels.find(label);
	bool result = iter != labels.end();
	if (result) {
		now_line = iter->second;
		now_pos = 0;

		while (stk_state.size()) {
			stk_state.pop();
		}

		switchState(TEXT);
	}
	return result;
}

bool SMI::Interpreter::outStack()
{
	bool result = now_state == CMD_MULTI;
	int floor = 0;

	if (result) {
		size_t pos = now_line;
		while (pos < all_text.size()) {
			if (all_text[pos] == L"<!>" || all_text[pos].find(L"if") != string::npos) {
				++floor;
			}

			if (all_text[pos] == L"</>" || all_text[pos] == L"/") {
				if (floor) {
					--floor;
				}
				else {
					returnState();
					returnState();
					break;
				}
			}
			++pos;
		}
		result = pos != now_line;
		now_line = pos + 1;
		if (now_line >= all_text.size()) {
			now_line = all_text.size() - 1;
		}
		now_pos = 0;
	}

	return result;
}


void SMI::Interpreter::changeState(State state)
{
	stk_state.push(now_state);
	now_state = state;
}

void SMI::Interpreter::switchState(State state)
{
	now_state = state;
}

SMI::Interpreter::State SMI::Interpreter::returnState()
{
	State result = TEXT;
	if (!stk_state.empty()) {
		result = stk_state.top();
		now_state = result;
		stk_state.pop();
	}
	return result;
}

void SMI::Interpreter::addLabel()
{
	wstring label;
	wstring str;
	size_t pos;

	do {
		str = all_text[now_line];
		if (now_state == LABEL_IN) {			//test the state
			label = str.substr(1);
			now_state = LABEL_READING;
			all_text[now_line].insert(all_text[now_line].begin(), '#');	//commit the line

			if (str.at(str.length() - 1) == '-') {
				label = label.substr(0, label.length() - 1);
				labels[label] = now_line;
				now_state = LABEL_EXITING;
			}
			else {
				++now_line;
			}
		}
		else if (now_state == LABEL_READING) {
			pos = str.find('-');		//find the end mark
			if (pos == string::npos) {
				label += str;
				++now_line;
			}
			else {				//found
				label += str.substr(0, pos + 1);
				labels[label] = now_line;
				now_state = LABEL_EXITING;
			}
		}
	} while (now_state != LABEL_EXITING);

	now_state = TEXT;
}

void SMI::Interpreter::scanFile()
{
	now_line = 0;
	wstring str;

	//build labels list
	for (now_line = 0; now_line < all_text.size(); ++now_line) {
		str = all_text[now_line];
		if (str.length() && str.at(0) == '-') {
			now_state = LABEL_IN;
			addLabel();
		}
	}
	now_line = 0;
}

void SMI::Interpreter::splitArgs(wstring & str, vector<wstring>& arg_array)
{
	size_t cut_pos = -1;
	size_t cut_pos2 = str.find(',');

	while (cut_pos2 != string::npos) {
		arg_array.push_back(trim(str.substr(cut_pos + 1, cut_pos2 - cut_pos - 1)));
		cut_pos = cut_pos2;
		cut_pos2 = str.find(',', cut_pos + 1);
	}

	arg_array.push_back(trim(str.substr(cut_pos + 1, cut_pos2 - cut_pos - 1)));
}

wstring SMI::Interpreter::trim(wstring str)
{
	if (str.empty()) {
		return str;
	}

	str.erase(0, str.find_first_not_of(L" "));
	str.erase(str.find_last_not_of(L" ") + 1);
	str.erase(0, str.find_first_not_of('\t'));
	str.erase(str.find_last_not_of('\t') + 1);
	str.erase(0, str.find_first_not_of('\r'));
	str.erase(str.find_last_not_of('\r') + 1);
	return str;
}

void SMI::Interpreter::debug(ostream& out)
{

}

std::string ws2s(const std::wstring& ws)
{
	std::string curLocale = setlocale(LC_ALL, NULL);
	setlocale(LC_ALL, "chs");
	const wchar_t* _Source = ws.c_str();
	size_t _Dsize = 2 * ws.size() + 1;
	char *_Dest = new char[_Dsize];
	memset(_Dest, 0, _Dsize);
	wcstombs(_Dest, _Source, _Dsize);
	std::string result = _Dest;
	delete[]_Dest;
	setlocale(LC_ALL, curLocale.c_str());
	return result;
}

wstring s2ws(const string& s)
{
	setlocale(LC_ALL, "chs");
	const char* _Source = s.c_str();
	size_t _Dsize = s.size() + 1;
	wchar_t *_Dest = new wchar_t[_Dsize];
	wmemset(_Dest, 0, _Dsize);
	mbstowcs(_Dest, _Source, _Dsize);
	std::wstring result = _Dest;
	delete[]_Dest;
	setlocale(LC_ALL, "C");
	return result;
}

void Interpreter::Goto(const std::wstring& label)
{
    gotoLable(label);
}
