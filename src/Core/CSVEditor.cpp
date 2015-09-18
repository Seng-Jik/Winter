#include "CSVEditor.h"
#include "ResFile.h"
#include "Misc.h"
#include "SDLbase.h"
using namespace std;
using namespace Core;

size_t CSVEditor::Width()
{
	return max_x + 1;
}

size_t CSVEditor::Height()
{
	return max_y + 1;
}

bool CSVEditor::open(const string & filename)
{
	/*ifstream file(filename);
	string str;
	while (!file.eof()) {
		getline(file, str);
		lines.push_back(str);
	} if (!lines.size()) {
		file.close();
		return false;
	}

	now_pos = lines[0].c_str();
	file.close();*/
	ResFile file(filename);
	if(!file.Success()) return false;

	Uint32 pos = 0;
    while(pos<file.Size()){
        lines.push_back(GetLine(file,pos));
    }
    now_pos = lines[0].c_str();
	return true;
}

char CSVEditor::getChar()
{
	// Check the point
	if (!(*now_pos)) {
		in_block = false;
		now_y++;
		if (now_y < lines.size()) {
			now_pos = lines[now_y].c_str();
			now_x = 0;
		} else {
			return EOF;
		}
		return EOB;
	}

	//Deal the "
	if (*now_pos == '\"') {
		if (!in_block) {
			in_block = true;
		} else if (in_block && *(now_pos + 1) != '\"') {
			in_block = false;
		} else if (in_block && *(now_pos + 1) == '\"') {
			now_pos += 2;
			return '\"';
		}
		now_pos++;
	}

	// Check the point
	if (!(*now_pos)) {
		in_block = false;
		now_y++;
		if (now_y < lines.size()) {
			now_pos = lines[now_y].c_str();
			now_x = 0;
		} else {
			return EOF;
		}
		return EOB;
	}

	// Deal with the """" situation
	if (*now_pos == '\"' && *(now_pos + 1) == '\"') {
		now_pos += 2;
		if (in_block) {
			return '\"';
		}
	}

	if (*now_pos == ',') {
		now_pos++;

		if (in_block) {
			return ',';
		} else {
			now_x++;
			return EOB;
		}
	} else {
		return *now_pos++;
	}
}

bool CSVEditor::getWord(string & word, size_t &pos_x, size_t &pos_y)
{
	pos_x = now_x;
	pos_y = now_y;
	word.clear();

	char c = getChar();
	while (c != EOB) {
		if (c == EOF) {
			return false;
		} else {
			word += c;
		}
		c = getChar();
	}

	return true;
}

void CSVEditor::append2Map()
{
	position pos;
	size_t pos_x, pos_y;
	string word;
	while (getWord(word, pos_x, pos_y)) {
		pos.first = pos_x;
		pos.second = pos_y;
		max_x = max_x < pos_x ? pos_x : max_x;
		max_y = max_y < pos_y ? pos_y : max_y;
		table[pos] = word;
		word = string("");
	}
	pos.first = pos_x;
	pos.second = pos_y;
	max_x = max_x < pos_x ? pos_x : max_x;
	max_y = max_y < pos_y ? pos_y : max_y;
	table[pos] = word;
}

string & CSVEditor::formatBlock(string & val)
{
	bool add_block = false;
	for (size_t i = 0; i < val.length(); ++i) {
		if ((val[i] == ',' || val[i] == '\"') && !add_block) {
			add_block = true;
			str_offset(val, 1, 0);
			++i;
			val[0] = '\"';
		} if (val[i] == '\"' && add_block) {
			str_offset(val, 1, i++);
		}
	}

	if (add_block) {
		val += '\"';
	}

	return val;
}

void CSVEditor::rewriteLines()
{
	position pos;
	auto val = table.begin();

	while (lines.size() < max_y + 1) {
		lines.push_back("");
	}
	for (size_t i = 0; i <= max_y; ++i) {
		lines[i].clear();
		for (size_t j = 0; j <= max_x; ++j) {
			pos.first = j;
			pos.second = i;
			val = table.find(pos);
			if (val != table.end()) {
				lines[i] += formatBlock(val->second);
			}
			if (j != max_x) {
				lines[i] += ',';
			}
		}
		cout << lines[i] << endl;
	}
}

string & CSVEditor::str_offset(string & dst, int offset, int start_pos)
{
	if (offset > 0) {
		dst.resize(dst.length() + offset);
		for (int i = dst.length() - 1; i >= start_pos; --i) {
			dst[i + offset] = dst[i];
		}
	}
	return dst;
}

CSVEditor::CSVEditor()
{
	Create(0, 0);
}

void CSVEditor::LoadCSV(const std::string & filename)
{
	open(filename);
	append2Map();
}

void CSVEditor::SaveCSV(const std::string & filename)
{
	rewriteLines();
	if (!lines.size()) {
		return;
	}

	ofstream file(filename);

	for (size_t i = 0; i < lines.size() - 1; ++i) {
		file << lines[i] << endl;
	}
	file << lines[lines.size() - 1];

	file.close();
}

void CSVEditor::Create(size_t w, size_t h)
{
	max_x = w;
	max_y = h;
	lines.clear();
	table.clear();
	now_x = 0;
	now_y = 0;
	in_block = false;
}

const std::string & CSVEditor::Str(const size_t x, const size_t y)
{
	position target;
	target.first = x;
	target.second = y;
	auto result = table.find(target);

	if (result != table.end()) {
		return result->second;
	} else {
		return empty;
	}
}

int CSVEditor::Int(const size_t x, const size_t y)
{
	return atoi(Str(x, y).c_str());
}

double CSVEditor::Float(const size_t x, const size_t y)
{
	return atof(Str(x, y).c_str());
}

void CSVEditor::Set(size_t x, size_t y, const std::string & val)
{
	position pos;
	pos.first = x;
	pos.second = y;

	max_x = max_x < x ? x : max_x;
	max_y = max_y < y ? y : max_y;

	table[pos] = val;
}

void CSVEditor::Set(size_t x, size_t y, int val)
{
	char buf[512] = { 0 };
	sprintf(buf, "%d", val);
	Set(x, y, string(buf));
}

void CSVEditor::Set(size_t x, size_t y, double val)
{
	char buf[512] = { 0 };
	sprintf(buf, "%lf", val);
	Set(x, y, string(buf));
}
