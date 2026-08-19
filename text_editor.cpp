#include <iostream>
#include <stack>
#include <string>
#include <sstream>
#include <limits>

using namespace std;

const int MAX_HISTORY = 100;

enum ActionType { INSERT, DELETE, PASTE };

struct Action {
    ActionType type;
    int position;
    string text;
};

void pushLimited(stack<Action>& stk, const Action& act) {
    stk.push(act);
    if (stk.size() > MAX_HISTORY) {
        stack<Action> temp;
        while (stk.size() > 1) { temp.push(stk.top()); stk.pop(); }
        stk.pop();
        while (!temp.empty()) { stk.push(temp.top()); temp.pop(); }
    }
}

void insertCommand(string &text, stack<Action>& undoStack, stack<Action>& redoStack, const string& line) {
    int pos;
    string insertText;
    stringstream ss(line);
    if (!(ss >> pos)) { cout << "Invalid format.\n please enter like insert <position> <text>\n"; return; }
    getline(ss, insertText);
    if (!insertText.empty() && insertText[0]==' ') insertText = insertText.substr(1);
    if (pos < 0) pos = 0;
    if (pos > text.size()) pos = text.size();
    text.insert(pos, insertText);
    pushLimited(undoStack, {INSERT, pos, insertText});
    while (!redoStack.empty()) redoStack.pop();
}

void deleteCommand(string &text, stack<Action>& undoStack, stack<Action>& redoStack, const string& line) {
    if (text.empty()) { cout << "Text is empty! Nothing to delete.\n"; return; }
    int pos, len;
    stringstream ss(line);
    if (!(ss >> pos >> len) || pos<0 || pos>=text.size() || len<0 || len>text.size()-pos) {
        cout << "Invalid format.\n please enter like delete <position> <length>\n"; return;
    }
    cout << "You are about to delete: \"" << text.substr(pos,len) << "\". Are you sure? (y/n): ";
    char confirm; cin >> confirm; cin.ignore(numeric_limits<streamsize>::max(),'\n');
    if (confirm!='y' && confirm!='Y') { cout << "Delete canceled.\n"; return; }
    string deletedText = text.substr(pos,len);
    text.erase(pos,len);
    pushLimited(undoStack, {DELETE, pos, deletedText});
    while (!redoStack.empty()) redoStack.pop();
}

void copyCommand(const string &text, string &clipboard, const string& line) {
    if (text.empty()) { cout << "Text is empty! Nothing to copy.\n"; return; }
    int pos, len;
    stringstream ss(line);
    if (!(ss >> pos >> len) || pos<0 || pos>=text.size() || len<0 || len>text.size()-pos) {
        cout << "Invalid format.\n please enter like copy <position> <length>\n"; return;
    }
    clipboard = text.substr(pos,len);
    cout << "Copied \"" << clipboard << "\" to clipboard.\n";
}

void pasteCommand(string &text, const string &clipboard, stack<Action>& undoStack, stack<Action>& redoStack, const string& line) {
    if (clipboard.empty()) { cout << "Clipboard is empty!\n"; return; }
    int pos;
    stringstream ss(line);
    if (!(ss >> pos) || pos<0) { cout << "Invalid format.\n please enter like paste <position>\n"; return; }
    if (pos>text.size()) pos = text.size();
    text.insert(pos, clipboard);
    pushLimited(undoStack, {PASTE, pos, clipboard});
    while (!redoStack.empty()) redoStack.pop();
}

void undoCommand(string &text, stack<Action>& undoStack, stack<Action>& redoStack) {
    if (undoStack.empty()) { cout << "Nothing to undo!\n"; return; }
    Action last = undoStack.top(); undoStack.pop();
    if (last.type==INSERT || last.type==PASTE) text.erase(last.position,last.text.size());
    else if (last.type==DELETE) text.insert(last.position,last.text);
    pushLimited(redoStack,last);
}

void redoCommand(string &text, stack<Action>& undoStack, stack<Action>& redoStack) {
    if (redoStack.empty()) { cout << "Nothing to redo!\n"; return; }
    Action last = redoStack.top(); redoStack.pop();
    if (last.type==INSERT || last.type==PASTE) text.insert(last.position,last.text);
    else if (last.type==DELETE) text.erase(last.position,last.text.size());
    pushLimited(undoStack,last);
}

void printCommand(const string &text) { cout << "Text: \"" << text << "\"\n"; }

int main() {
    string text="", clipboard="";
    stack<Action> undoStack, redoStack;

    cout << "Text Editor (CMD)\nCommands: insert, delete, copy, paste, undo, redo, print, exit\n";

    while (true) {
        cout << "\n> ";
        string line; getline(cin >> ws, line);
        if (line.empty()) continue;
        string cmd; stringstream ss(line); ss >> cmd;

        if (cmd=="exit") break;
        else if (cmd=="insert") insertCommand(text, undoStack, redoStack, line.substr(7));
        else if (cmd=="delete") deleteCommand(text, undoStack, redoStack, line.substr(7));
        else if (cmd=="copy") copyCommand(text, clipboard, line.substr(5));
        else if (cmd=="paste") pasteCommand(text, clipboard, undoStack, redoStack, line.substr(6));
        else if (cmd=="undo") undoCommand(text, undoStack, redoStack);
        else if (cmd=="redo") redoCommand(text, undoStack, redoStack);
        else if (cmd=="print") printCommand(text);
        else cout << "Unknown command!";
    }

    cout << "Exiting editor.\n";
    return 0;
}
