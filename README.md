# C++ Text Editor

A simple command-line text editor implemented in C++ with support for basic text manipulation and undo/redo operations.

## ✨ Features

- Insert text at a specific position
- Delete text with confirmation
- Copy text to clipboard
- Paste copied text
- Undo previous operations
- Redo undone operations
- Print current text
- Limited operation history

## 🧠 Data Structure

The project uses the **Stack** data structure to manage undo and redo operations.

Each action stores:

- Action type
- Position
- Text content

The editor keeps a maximum of 100 history operations.

## 🛠 Technologies

- C++
- Standard Library
- `std::stack`
- `std::string`
- `std::stringstream`

## 🎮 Available Commands

```text
insert <position> <text>
delete <position> <length>
copy <position> <length>
paste <position>
undo
redo
print
exit
```

## 🚀 Getting Started

### 1. Clone the repository

```bash
git clone
cd cpp-text-editor
## Compile
g++ text_editor.cpp -o text_editor
```