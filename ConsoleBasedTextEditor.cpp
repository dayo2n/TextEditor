#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <cstdlib>

using namespace std;
#define LIMIT_LINE 19
// 코드 내에서 전위 표기법을 사용해 구현했으므로 19로 정의
#define LIMIT_BYTE 75

string console = "명령어를 입력하세요";

class TextEditor
{
private:
    vector<string> storeVec;
    static TextEditor *editor_instance;
    int page = 0;

    TextEditor(vector<string> givenStoreVec = {})
    {
        storeVec = givenStoreVec;
    }

public:
    vector<string> getVector()
    {
        return storeVec;
    }
    void set_vector(vector<string> vector)
    {
        storeVec = vector;
    }
    int get_page()
    {
        return page;
    }
    void nextpage()
    {
        page++;
    }
    void prevpage()
    {
        page--;
    }
    static TextEditor *insatnce()
    {
        if (editor_instance == NULL)
        {
            editor_instance = new TextEditor();
        }
        return editor_instance;
    }

    // void loadPage();
    void nextPageOperation();
    void previousPageOperation();
    void insertOperation(char *param);
    void deleteOperation(char *param);
    void searchOperation(char *param);
    void changeOperation(char *param);
    void terminateOperation();
};

TextEditor *TextEditor::editor_instance = 0;

void printDivideSpace()
{

    cout << "-------------------------------------------------------------------------------------------" << endl;
}
void printMenu()
{
    cout << "n: 다음페이지, p: 이전페이지, i: 삽입, d: 삭제, c: 변경, s: 찾기, t: 저장 후 종료" << endl;
}

void printConsoleMsg(string console)
{
    cout << "(Console Msg) " + console << endl;
}

void printUserPrompt()
{
    cout << "입력 : ";
}

void TextEditor::previousPageOperation()
{
    int nowLength = 0;
    int nowLines = LIMIT_BYTE + 1;
    string nowWord;

    vector<string> vec = getVector();
    int presentPage = TextEditor::insatnce()->get_page();
    int prevPage = presentPage - 1;

    bool findPage = false;
    if (presentPage <= 2)
    {
        findPage = true;
    }

    string firstWord = "";

    for (vector<string>::iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (findPage)
        {
            nowWord = *it;

            if ((nowLines + nowWord.length()) > LIMIT_BYTE)
            {
                cout << "" << endl;
                if (nowLength++ > LIMIT_LINE)
                {
                    break;
                }
                if (nowLength < 10)
                {
                    cout << " ";
                }
                cout << nowLength;
                cout << "|  ";
                nowLines = 0;
            }
            if (firstWord != "")
            {
                cout << firstWord + " ";
                firstWord = "";
            }
            cout << nowWord + " ";
            nowLines += nowWord.length() + 1;
        }
        else
        {

            nowWord = *it;

            if ((nowLines + nowWord.length()) > LIMIT_BYTE)
            {
                nowLines = 0;
                if (nowLength > (LIMIT_LINE * (prevPage - 1)))
                {
                    findPage = true;
                    nowLines = LIMIT_BYTE + 1;
                    nowLength = 0;
                    firstWord = nowWord;
                    continue;
                }
                nowLength++;
            }

            nowLines += nowWord.length() + 1;
        }
    }

    cout << "\n";
}
void TextEditor::nextPageOperation()
{
    int nowLength = 0;
    int nowLines = LIMIT_BYTE + 1;
    string nowWord;

    vector<string> vec = getVector();
    int presentPage = TextEditor::insatnce()->get_page();

    bool findPage = false;
    if (presentPage == 0)
    {
        findPage = true;
    }

    string firstWord = "";

    for (vector<string>::iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (findPage)
        {
            nowWord = *it;

            if ((nowLines + nowWord.length()) > LIMIT_BYTE)
            {
                cout << "" << endl;
                if (nowLength++ > LIMIT_LINE)
                {
                    break;
                }
                if (nowLength < 10)
                {
                    cout << " ";
                }
                cout << nowLength;
                cout << "|  ";
                nowLines = 0;
            }
            if (firstWord != "")
            {
                cout << firstWord + " ";
                firstWord = "";
            }
            cout << nowWord + " ";
            nowLines += nowWord.length() + 1;
        }
        else
        {

            nowWord = *it;

            if ((nowLines + nowWord.length()) > LIMIT_BYTE)
            {
                nowLines = 0;
                if (nowLength > (LIMIT_LINE * presentPage))
                {
                    findPage = true;
                    nowLines = LIMIT_BYTE + 1;
                    nowLength = 0;
                    firstWord = nowWord;
                    continue;
                }
                nowLength++;
            }

            nowLines += nowWord.length() + 1;
        }
    }

    cout << "\n";
}

void TextEditor::changeOperation(char *userOrder)
{
    int cnt = 0;
    string changeFrom = "", changeTo = "";
    int flag = 0; // 첫 인자 검사할 때 0, 두번째 때 1
    for (int i = 2; i < strlen(userOrder) - 1; i++)
    {
        if (flag == 0)
        {
            if (userOrder[i] == ',')
            {
                flag = 1;
                continue;
            }
            if (userOrder[i] >= '0' && userOrder[i] <= '9')
            {
                cout << "단어를 입력해야 합니다" << endl;
                return;
            }
            changeFrom += userOrder[i];
        }
        else if (flag == 1)
        {
            if (userOrder[i] >= '0' && userOrder[i] <= '9')
            {
                cout << "단어를 입력해야 합니다" << endl;
                return; // 다시 입력
            }
            changeTo += userOrder[i];
        }
    }
    vector<string> vec = getVector();
    vector<string> newVec;

    for (vector<string>::iterator it = vec.begin(); it != vec.end(); it++)
    {
        if (*it == changeFrom)
        {
            newVec.push_back(changeTo);
        }
        else
        {
            newVec.push_back(*it);
        }
    }
    TextEditor::insatnce()->set_vector(newVec);
}

void TextEditor::searchOperation(char *userOrder)
{
    int cnt = 0;
    string keyword = "";
    for (int i = 2; i < strlen(userOrder) - 1; i++)
    {
        keyword += userOrder[i];
    }

    bool flag = false; // 입력한 첫 단어가 발견되면 true, 그때부터 출력
    bool keywordFlag = false;
    int nowLength = 0;
    int nowLines = 0;
    string nowWord;

    vector<string> vec = getVector();
    for (vector<string>::iterator it = vec.begin(); it != vec.end(); it++)
    {
        nowWord = *it;
        nowLines += (nowWord.length() + 1);

        if (flag)
        {
            if (nowLines > LIMIT_BYTE)
            {
                cout << "" << endl;
                if (nowLength++ > LIMIT_LINE)
                {
                    break;
                }
                if (nowLength < 10)
                {
                    cout << " ";
                }
                cout << nowLength;
                cout << "|  ";
                nowLines = 0;
            }
            if (!keywordFlag)
            {
                cout << keyword + " ";
                keywordFlag = true;
            }
            cout << *it + " ";
        }
        else
        {
            if (*it == keyword)
            {
                flag = true;
                nowLines += (nowWord.length() + 1);
            }
        }
    }
}

void TextEditor::insertOperation(char *userOrder)
{

    int cnt = 0;
    string s_row = "", s_col = "";
    string newInputWord = "";
    int flag = 0; // 첫 인자 검사할 때 0, 두번째 때 1, 세번째 때 2
    for (int i = 2; i < strlen(userOrder) - 1; i++)
    {
        if (flag == 0)
        {
            if (userOrder[i] == ',')
            {
                flag = 1;
                continue;
            }
            if (userOrder[i] < '0' || userOrder[i] > '9')
            {
                cout << "숫자를 입력해야 합니다" << endl;
                return;
            }
            s_row += userOrder[i];
        }
        else if (flag == 1)
        {
            if (userOrder[i] == ',')
            {
                flag = 2;
                continue;
            }
            if (userOrder[i] < '0' || userOrder[i] > '9')
            {
                cout << "숫자를 입력해야 합니다" << endl;
                return;
            }
            s_col += userOrder[i];
        }
        else //flag == 2
        {
            newInputWord += userOrder[i];
        }
    }

    int row = atoi(s_row.c_str());

    if (row > 20)
    {
        cout << "해당 라인은 존재하지 않습니다." << endl;
        return;
    }
    int col = atoi(s_col.c_str());

    vector<string> vec = getVector();
    int presentPage = TextEditor::insatnce()->get_page();
    int prevPage = presentPage - 1;
    string nowWord = "";
    int nowLines = LIMIT_BYTE + 1;
    int nowLength = 0;

    bool findPage = false;
    if (presentPage <= 1)
    {
        findPage = true;
    }

    string firstWord = "";

    for (vector<string>::iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (findPage)
        {
            nowWord = *it;

            if ((nowLines + nowWord.length()) > LIMIT_BYTE)
            {
                // cout << "" << endl;
                if (nowLength++ > LIMIT_LINE)
                {
                    break;
                }
                if (nowLength < 10)
                {
                    // cout << " ";
                }
                // cout << nowLength;
                // cout << "|  ";
                nowLines = 0;
            }
            if (firstWord != "")
            {
                // cout << firstWord + " ";
                firstWord = "";
            }
            if (nowLength == row)
            {
                if (col-- == 0)
                {
                    vec.insert(it, newInputWord);
                    break;
                }
            }
            // cout << nowWord + " ";
            nowLines += nowWord.length() + 1;
        }
        else
        {
            nowWord = *it;
            if ((nowLines + nowWord.length()) > LIMIT_BYTE)
            {
                nowLines = 0;
                if (nowLength > (LIMIT_LINE * prevPage))
                {
                    findPage = true;
                    nowLines = LIMIT_BYTE + 1;
                    nowLength = 0;
                    firstWord = nowWord;
                    continue;
                }
                nowLength++;
            }
            nowLines += nowWord.length() + 1;
        }
    }

    TextEditor::insatnce()->set_vector(vec);
    TextEditor::insatnce()->prevpage();
    TextEditor::insatnce()->nextPageOperation();
    TextEditor::insatnce()->nextpage();
}
void TextEditor::deleteOperation(char *userOrder)
{
    int cnt = 0;
    string s_row = "", s_col = "";
    string newInputWord = "";
    int flag = 0; // 첫 인자 검사할 때 0, 두번째 때 1, 세번째 때 2
    for (int i = 2; i < strlen(userOrder) - 1; i++)
    {
        if (flag == 0)
        {
            if (userOrder[i] == ',')
            {
                flag = 1;
                continue;
            }
            if (userOrder[i] < '0' || userOrder[i] > '9')
            {
                cout << "숫자를 입력해야 합니다" << endl;
                return;
            }
            s_row += userOrder[i];
        }
        else
        {
            if (userOrder[i] < '0' || userOrder[i] > '9')
            {
                cout << "숫자를 입력해야 합니다" << endl;
                return;
            }
            s_col += userOrder[i];
        }
    }

    int row = atoi(s_row.c_str());
    int col = atoi(s_col.c_str());
    if (row > 20)
    {
        cout << "해당 라인은 출력 창에 존재하지 않습니다." << endl;
        return;
    }
    col--;

    vector<string> vec = getVector();
    int presentPage = TextEditor::insatnce()->get_page();
    int prevPage = presentPage - 1;
    string nowWord = "";
    int nowLines = LIMIT_BYTE + 1;
    int nowLength = 0;

    bool findPage = false;
    if (presentPage <= 1)
    {
        findPage = true;
    }

    string firstWord = "";

    for (vector<string>::iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (findPage)
        {
            nowWord = *it;

            if ((nowLines + nowWord.length()) > LIMIT_BYTE)
            {
                // cout << "" << endl;
                if (nowLength++ > LIMIT_LINE)
                {
                    break;
                }
                if (nowLength < 10)
                {
                    // cout << " ";
                }
                // cout << nowLength;
                // cout << "|  ";
                nowLines = 0;
            }
            if (firstWord != "")
            {
                // cout << firstWord + " ";
                firstWord = "";
            }
            if (nowLength == row)
            {
                if (col-- == 0)
                {
                    vec.erase(it);
                    break;
                }
            }
            // cout << nowWord + " ";
            nowLines += nowWord.length() + 1;
        }
        else
        {
            nowWord = *it;
            if ((nowLines + nowWord.length()) > LIMIT_BYTE)
            {
                nowLines = 0;
                if (nowLength > (LIMIT_LINE * prevPage))
                {
                    findPage = true;
                    nowLines = LIMIT_BYTE + 1;
                    nowLength = 0;
                    firstWord = nowWord;
                    continue;
                }
                nowLength++;
            }
            nowLines += nowWord.length() + 1;
        }
    }

    TextEditor::insatnce()->set_vector(vec);
    TextEditor::insatnce()->prevpage();
    TextEditor::insatnce()->nextPageOperation();
    TextEditor::insatnce()->nextpage();
}

void TextEditor::terminateOperation()
{
    ofstream save_stream; //프로그램 종료 전 파일을 저장하는 스트림
    save_stream.open("test.txt", ios::trunc);
    if (save_stream.fail())
    {
        cout << "save file opening failed" << endl;
        exit(1);
    }

    vector<string> vec = insatnce()->getVector();

    for (vector<string>::iterator it = vec.begin(); it != vec.end(); it++)
    {
        save_stream << *it + " ";
    }

    save_stream.close();
}

int main()
{
    vector<string> storeAllWords;
    ifstream in_stream; //test.txt 읽는 스트림
    in_stream.open("test.txt");

    if (in_stream.fail())
    {
        cout << "input file opening failed" << endl;
        exit(1);
    }

    string word;
    while (in_stream >> word)
    {
        storeAllWords.push_back(word);
    }
    in_stream.close();

    TextEditor::insatnce()->set_vector(storeAllWords);

    char *
        userOrder;
    TextEditor::insatnce()->nextPageOperation();
    TextEditor::insatnce()->nextpage();

    do
    {
        printDivideSpace();
        printMenu();
        printDivideSpace();
        printConsoleMsg(console);
        printDivideSpace();
        printUserPrompt();
        int nowPage = TextEditor::insatnce()->get_page();
        cin >> userOrder;

        switch (userOrder[0])
        {
        case 'n':
            if (nowPage > 4)
            {
                console = "This is the last page";
            }
            else
            {
                TextEditor::insatnce()->nextPageOperation();
                TextEditor::insatnce()->nextpage();
                console = "다음 명령어를 입력하세요";
            }
            break;
        case 'p':
            if (nowPage <= 1)
            {
                console = "This is the first page";
            }
            else
            {
                TextEditor::insatnce()->previousPageOperation();
                TextEditor::insatnce()->prevpage();
                console = "다음 명령어를 입력하세요";
            }
            break;
        case 'i':
            TextEditor::insatnce()->insertOperation(userOrder);
            console = "다음 명령어를 입력하세요";
            break;
        case 'd':
            TextEditor::insatnce()->deleteOperation(userOrder);
            console = "다음 명령어를 입력하세요";
            break;
        case 'c':
            TextEditor::insatnce()->changeOperation(userOrder);
            console = "다음 명령어를 입력하세요";
            break;
        case 's':
            TextEditor::insatnce()->searchOperation(userOrder);
            console = "다음 명령어를 입력하세요";
            break;
        case 't':
            TextEditor::insatnce()->terminateOperation();
            return 0;
        default:
            console = "존재하지 않는 명령어 입니다. 명령어를 재입력하세요.";
        }
    } while (!cin.eof());
}