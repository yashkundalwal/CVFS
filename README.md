# CVFS — Custom Virtual File System

A **user-space Virtual File System implemented in C** that simulates core file-system concepts using custom data structures, inode management, file descriptors, file permissions, dynamic memory allocation, and an interactive command-line interface.

The system manages files in memory using an inode-based architecture and creates a backup of active files when the CVFS is terminated.

---

## Features

- Create regular files
- Read file contents
- Write data to files
- Delete files
- List files
- Display file statistics
- File permission management
- File descriptor management
- Inode-based file management
- User File Descriptor Table (UFDT)
- Dynamic memory allocation
- Custom error handling
- Automatic file backup on exit
- Interactive command-line shell
- Built-in help and manual commands

---

## System Architecture

```text
                     ┌──────────────────────┐
                     │      CVFS Shell      │
                     │   Command Interface  │
                     └──────────┬───────────┘
                                │
                                ▼
                     ┌──────────────────────┐
                     │    Command Parser    │
                     └──────────┬───────────┘
                                │
              ┌─────────────────┼─────────────────┐
              │                 │                 │
              ▼                 ▼                 ▼
           Create              Read              Write
           Delete              Stat               List
              │                 │                 │
              └─────────────────┼─────────────────┘
                                │
                                ▼
                     ┌──────────────────────┐
                     │        UAREA         │
                     │         UFDT         │
                     └──────────┬───────────┘
                                │
                                ▼
                     ┌──────────────────────┐
                     │      File Table      │
                     │                      │
                     │  Read Offset         │
                     │  Write Offset        │
                     │  Mode               │
                     │  Inode Pointer      │
                     └──────────┬───────────┘
                                │
                                ▼
                     ┌──────────────────────┐
                     │        Inode         │
                     │                      │
                     │  File Name           │
                     │  Inode Number        │
                     │  File Size           │
                     │  Actual File Size    │
                     │  File Type           │
                     │  Reference Count     │
                     │  Permissions         │
                     │  Data Buffer         │
                     └──────────┬───────────┘
                                │
                                ▼
                           Data Buffer
```

---

## Core Data Structures

### BootBlock

Stores information associated with the virtual file system.

### SuperBlock

Maintains information about the total and available inodes.

### Inode

The inode stores file metadata and maintains the file's data buffer.

```text
File Name
Inode Number
File Size
Actual File Size
File Type
Reference Count
Permissions
Data Buffer
Next Inode
```

### FileTable

Maintains information associated with an opened file.

```text
Read Offset
Write Offset
Mode
Pointer to Inode
```

### UAREA / UFDT

The User Area contains process-level information and the User File Descriptor Table (UFDT).

```text
UAREA
 │
 ├── Process Name
 │
 └── UFDT
      │
      ├── File Table
      ├── File Table
      ├── File Table
      └── ...
```

---

## System Limits

The current implementation uses the following limits:

| Resource | Limit |
|---|---:|
| Maximum Inodes | 5 |
| Maximum File Size | 50 bytes |
| Maximum Open Files | 10 |
| Maximum File Name Length | 20 characters |

---

## File Permissions

CVFS supports three permission modes:

| Value | Permission |
|---:|---|
| `1` | READ |
| `2` | WRITE |
| `3` | READ + WRITE |

Read and write operations verify the corresponding permissions before accessing the file.

---

## 🖥️ Available Commands

### `help`

Displays the available commands.

```text
Marvellous CVFS : > help
```

---

### `creat`

Creates a new regular file.

```text
creat File_Name Permission
```

Example:

```text
creat Ganesh.txt 3
```

Here, `3` represents **READ + WRITE** permission.

---

### `ls`

Lists files currently present in the virtual file system.

```text
ls
```

---

### `ls -a`

Displays detailed information about files.

```text
ls -a
```

This provides information such as:

- File name
- Inode number
- Actual file size

---

### `stat`

Displays detailed statistics of a specific file.

```text
stat File_Name
```

Example:

```text
stat Ganesh.txt
```

The command displays information including:

- File name
- Inode number
- File size
- Actual file size
- Reference count
- File permissions
- File type

---

### `write`

Writes data into a file using its file descriptor.

```text
write File_Descriptor
```

Example:

```text
write 3
```

The operation checks:

- File descriptor validity
- Write permission
- Available file space

---

### `read`

Reads data from a file using its file descriptor.

```text
read File_Descriptor Size
```

Example:

```text
read 3 10
```

---

### `unlink`

Deletes a file from the virtual file system.

```text
unlink File_Name
```

Example:

```text
unlink Ganesh.txt
```

`rm` can also be used as an alias:

```text
rm Ganesh.txt
```

---

### `man`

Displays the manual page for a command.

```text
man Command
```

Example:

```text
man creat
```

---

### `clear`

Clears the terminal screen.

```text
clear
```

---

### `exit`

Terminates the CVFS application and triggers the backup mechanism.

```text
exit
```

---

## Backup Mechanism

When CVFS is terminated using the `exit` command, the system creates a backup of active files.

```text
                    CVFS
                      │
                    exit
                      │
                      ▼
               create_backup()
                      │
                      ▼
              Traverse Inode List
                      │
                      ▼
                 Active Files
                      │
                      ▼
                  backup/
                 ┌────┼────┐
                 ▼    ▼    ▼
               File  File  File
```

The contents of active files are written to the `backup` directory before the application terminates.

---

## Error Handling

The implementation defines specific error conditions for common file-system operations:

```text
ERR_INVALID_PARAMETER
ERR_NO_INODES
ERR_FILE_ALREADY_EXISTS
ERR_FILE_NOT_EXISTS
ERR_PERMISSION_DENIED
ERR_INSUFFICIENT_SPACE
ERR_INSUFFICIENT_DATA
ERR_MAX_FILES_OPEN
```

This allows the system to identify and report conditions such as:

- Invalid command parameters
- No available inodes
- Duplicate file names
- Non-existent files
- Permission violations
- Insufficient storage
- Invalid read requests
- Maximum open-file limit reached

---

## Project Structure

```text
CVFS/
│
├── cvfs.c
├── cvfsc
├── backup/
│
└── README.md
```

> `cvfsc` is the compiled executable currently present in the repository.

---

## Compilation & Execution

### Prerequisites

- GCC Compiler
- Linux / Unix-like environment recommended

### Compile

```bash
gcc cvfs.c -o cvfs
```

### Run

```bash
./cvfs
```

On Windows using MinGW:

```bash
gcc cvfs.c -o cvfs.exe
cvfs.exe
```

---

## Example Workflow

```text
Marvellous CVFS : > help

Marvellous CVFS : > creat Ganesh.txt 3

File successfully created with FD : 3

Marvellous CVFS : > ls

Ganesh.txt

Marvellous CVFS : > write 3

Enter the data that you want to write into the file:
Hello CVFS

Marvellous CVFS : > stat Ganesh.txt

File Name: Ganesh.txt
Inode Number: 1
File Size: 50
Actual File Size: ...

Marvellous CVFS : > read 3 10

Read Operation is successful

Marvellous CVFS : > unlink Ganesh.txt

File Deleted Successfully

Marvellous CVFS : > exit

Thank you for using Marvellous CVFS
```

---

## Concepts Demonstrated

This project provides practical experience with:

- C Programming
- Operating System Concepts
- File-System Architecture
- Inode Management
- File Descriptors
- File Tables
- User File Descriptor Tables
- Linked Lists
- Structures
- Pointers
- Dynamic Memory Allocation
- Memory Management
- File Permissions
- Command-Line Interfaces
- Error Handling
- File Backup and Data Persistence

---

## Future Improvements

Potential improvements for future versions include:

- Implement dedicated `open()` and `close()` operations
- Support larger numbers of inodes
- Support larger file sizes
- Add directory support
- Add append functionality
- Improve memory cleanup
- Implement metadata restoration
- Add automated unit testing
- Improve command parsing
- Add multiple file types
- Enhance permission management

---

## Author

**Yash Kundalwal**

B.Tech in Artificial Intelligence & Data Science  
Vishwakarma Institute of Information Technology, Pune

[GitHub](https://github.com/yashkundalwal)

[LinkedIn](https://linkedin.com/in/yash-kundalwal)
