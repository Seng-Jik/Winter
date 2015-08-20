#include <iostream>

#include "./include/SMLanguage/SMI.h"

using namespace std;

using namespace SMI;

string w2s(const wstring& ws)
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

string type_name[] = { "CMD", "NAME", "SAY", "SAYEND", "END" };

int main()
{	
	BYTE buffer[102400] = { 0 };
	cout << "hello world" << endl;
	Interpreter inter;
	inter.LoadStory("D:\\testfile2.txt", true);

	vector<wstring> str_args;
	vector<int> num_args;

	SMEvent game_event;

	while (inter.PullEvent(game_event)) {
		//inter.InfoOut(cout);
		/*cout << "Text: " << w2s(inter.PopText()) << endl;
		cout << "Cmd: " << w2s(inter.PopCmd()) << endl;
		cout << "Args: " << endl;
		inter.PopArg(str_args);
		inter.PopIntArg(num_args);
		for (auto item : str_args) {
			cout << "\t" << w2s(item) << endl;
		}
		for (auto item : num_args) {
			cout << "\t" << item << endl;
		}*/
		cout << "type: " << type_name[game_event.type] << endl;
		cout << "cmd: " << w2s(game_event.cmd) << endl;
		cout << "argv: " << endl;
		for (auto var : game_event.argv) {
			cout << "\t" << w2s(var) << endl;
		}
		cout << endl;
	}
		

	cout << "finished!" << endl;
	system("pause");
	return 0;
}