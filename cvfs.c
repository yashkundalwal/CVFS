////////////////////////////////////////////////////////////////////////////////////////
//
//     Header File Inclusion
//
////////////////////////////////////////////////////////////////////////////////////////

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<stdbool.h>

////////////////////////////////////////////////////////////////////////////////////////
//
//     User Defined Macros
//
////////////////////////////////////////////////////////////////////////////////////////

#define MAXINODE 5
#define MAXFILESIZE 50
#define MAXOPENFILES 10

#define READ 1
#define WRITE 2
#define EXECUTE 4

#define START 0
#define CURRENT 1
#define END 2

#define EXECUTE_SUCCESS 0

#define REGULARFILE 1
#define SPECIALFILE 2

////////////////////////////////////////////////////////////////////////////////////////
//
//     User Defined Macros for Error Handling
//
////////////////////////////////////////////////////////////////////////////////////////

#define ERR_INVALID_PARAMETER -1

#define ERR_NO_INODES -2

#define ERR_FILE_ALREADY_EXISTS -3
#define ERR_FILE_NOT_EXISTS -4

#define ERR_PERMISSION_DENIED -5

#define ERR_INSUFFICIENT_SPACE -6
#define ERR_INSUFFICIENT_DATA -7

#define ERR_MAX_FILES_OPEN -8

////////////////////////////////////////////////////////////////////////////////////////
//
//     Structure Name  :  BootBlock  
//     Description     :  It holds the information to
//                        Boot the Operating System
//
////////////////////////////////////////////////////////////////////////////////////////

struct BootBlock
{
    char Information[100];
};

////////////////////////////////////////////////////////////////////////////////////////
//
//     Structure Name  :  SuperBlock
//     Description     :  It holds the information of
//                        complete File System
//
////////////////////////////////////////////////////////////////////////////////////////

struct SuperBlock
{
    int TotalInodes;
    int FreeInodes;
};

////////////////////////////////////////////////////////////////////////////////////////
//
//     Structure Name  :  Inode
//     Description     :  It holds information of file
//
////////////////////////////////////////////////////////////////////////////////////////

#pragma pack(1)
struct Inode
{
    char FileName[20];
    int InodeNumber;
    int FileSize;
    int ActualFileSize;
    int FileType;
    int ReferenceCount;
    int Permission;
    char * Buffer;
    struct Inode * next;
};

typedef struct Inode INODE;
typedef struct Inode * PINODE;
typedef struct Inode * * PPINODE;

////////////////////////////////////////////////////////////////////////////////////////
//
//     Structure Name  :  FileTable
//     Description     :  It holds information of Opened
//                        Files
//
////////////////////////////////////////////////////////////////////////////////////////

#pragma pack(1)
struct FileTable
{
    int ReadOffset;
    int WriteOffset;
    int Mode;
    PINODE ptrinode;  
};

typedef struct FileTable FILETABLE;
typedef struct FileTable * PFILETABLE;

////////////////////////////////////////////////////////////////////////////////////////
//
//     Structure Name  :  UAREA
//     Description     :  It holds information of process
//
////////////////////////////////////////////////////////////////////////////////////////

struct UAREA
{
    char ProcessName[20];
    PFILETABLE UFDT[MAXOPENFILES];
};

////////////////////////////////////////////////////////////////////////////////////////
//
//     Global Variables used in the Project
//
////////////////////////////////////////////////////////////////////////////////////////

struct BootBlock bootobj;
struct SuperBlock superobj;
struct UAREA uareaobj;

PINODE head = NULL;

////////////////////////////////////////////////////////////////////////////////////////
//
//     Function Name    :    InitialiseUAREA()
//     Description      :    It is used to initialise UAREA
//     Author           :    Yash Yogesh Kundalwal
//     Date             :    31/07/2026
//
////////////////////////////////////////////////////////////////////////////////////////

void InitialiseUAREA()
{
    int i = 0;

    strcpy(uareaobj.ProcessName, "Myexe");

    for(i = 0; i < MAXOPENFILES ; i++)
    {
        uareaobj.UFDT[i] = NULL;
    }

    printf("Marvellous CVFS : UREA gets initialised successfully \n");
}

////////////////////////////////////////////////////////////////////////////////////////
//
//     Function Name    :    InitialiseSuperBlock()
//     Description      :    It is used to initialise Super Block
//     Author           :    Yash Yogesh Kundalwal
//     Date             :    31/07/2026
//
////////////////////////////////////////////////////////////////////////////////////////

void InitialiseSuperBlock()
{
    superobj.TotalInodes = MAXINODE;
    superobj.FreeInodes = MAXINODE;

    printf("Marvellous CVFS : Super Block gets initialised successfully \n");
}

////////////////////////////////////////////////////////////////////////////////////////
//
//     Function Name    :    CreateDILB()
//     Description      :    It is used to create Linked List of Inodes
//     Author           :    Yash Yogesh Kundalwal
//     Date             :    31/07/2026
//
////////////////////////////////////////////////////////////////////////////////////////

void CreateDILB()
{
    PINODE newn = NULL;
    PINODE temp = NULL;
    
    int i = 0;

    temp = head;

    for(i = 1 ; i <= MAXINODE ; i++)
    {
        newn = (PINODE)malloc(sizeof(INODE));
        
        strcpy(newn->FileName, "\0");
        newn->InodeNumber = i;
        newn->FileSize = 0;
        newn->ActualFileSize = 0;
        newn->FileType = 0;
        newn->ReferenceCount = 0;
        newn->Permission = 0;
        newn->Buffer = NULL;
        newn->next = NULL;

        if(temp == NULL)
        {
            head = newn;
            temp = head;
        }
        else
        {
            temp->next = newn;
            temp = temp->next;
        }
    }

    printf("Marvellous CVFS : DILB gets created Successfully\n");
    
}

////////////////////////////////////////////////////////////////////////////////////////
//
//     Function Name    :    StartAuxillaryDataInitialisation()
//     Description      :    It is used to call all such functions 
//                           which are used to initialise auxillary data
//     Author           :    Yash Yogesh Kundalwal
//     Date             :    31/07/2026
//
////////////////////////////////////////////////////////////////////////////////////////

void StartAuxillaryDataInitialisation()
{
    strcpy(bootobj.Information,"Booting Process of Marvellous CVFS is completed");

    printf("%s\n",bootobj.Information);

    InitialiseUAREA();

    InitialiseSuperBlock();

    CreateDILB();
}

////////////////////////////////////////////////////////////////////////////////////////
//
//     Function Name    :    DisplayHelp()
//     Description      :    It is used to display help to 
//                           the user of project
//     Author           :    Yash Yogesh Kundalwal
//     Date             :    1/08/2026
//
////////////////////////////////////////////////////////////////////////////////////////

void DisplayHelp()
{
    printf("---------------------------------------------------------------------------\n");
    printf("-------------------------Marvellous CVFS Help Page-------------------------\n");
    printf("---------------------------------------------------------------------------\n");

    printf("man    : It is used to display the manual page\n");
    printf("clear  : It is used to clear the terminal screen\n");
    printf("creat  : It is used to create regular file\n");
    printf("open   : It is open regular file\n");
    printf("close  : It is used to close a regular file\n");
    printf("write  : It is used to write the data into the file\n");
    printf("read   : It is used to read the data from the file\n");
    printf("stat   : It is used to Display statistical information of the file\n");
    printf("unlink : It is used to delete the file\n");
    printf("exit   : It is used to terminate Marvellous CVFS\n");
}

////////////////////////////////////////////////////////////////////////////////////////
//
//     Function Name    :    ManPageDisplay()
//     Description      :    It is used to display man page of 
//                           specific command
//     Input            :    Name of command
//     Author           :    Yash Yogesh Kundalwal
//     Date             :    1/08/2026
//
////////////////////////////////////////////////////////////////////////////////////////

void ManPageDisplay(char Name[])
{
    if(strcmp(Name,"exit") == 0)
    {
        printf("About : It is used to terminate the project\n");
        printf("Usage : exit\n");
    }
    else if(strcmp(Name,"ls") == 0)
    {
        printf("About : It is used to list all the files from current directory\n");
        printf("Usage : ls\n");
    }
    else if(strcmp(Name,"clear") == 0)
    {
        printf("About : It is used to clear the terminal\n");
        printf("Usage : clear\n");
    }
    else if(strcmp(Name,"creat") == 0)
    {
        printf("About : It is used to creat new File\n");
        printf("Usage : creat File_Name Permission\n");

        printf("File_Name : Name of File that we want to create\n");

        printf("Permission : Permission of the new File");
        printf("Permission : READ -> 1\n");
        printf("Permission : WRITE -> 2\n");
        printf("Permission : READ + WRITE -> 3");
    }
    else if(strcmp(Name,"unlink") == 0)
    {
        printf("About : It is used to delete an existing File\n");
        printf("Usage : unlink File_Name\n");

        printf("File_Name : Name of File that we want to Display\n");
    }
    else if(strcmp(Name,"stat") == 0)
    {
        printf("About : It is used to get information of a File\n");
        printf("Usage : stat File_Name\n");

        printf("File_Name : Name of File whose information we want to Display\n");
    }
    else
    {
        printf("No manual entry found for %s\n",Name);
    }
}

////////////////////////////////////////////////////////////////////////////////////////
//
//     Function Name    :    IsFileExist()
//     Description      :    It is used to check whether the file is present or not
//     Input            :    Name of file
//     Output           :    True if present
//                           False if not present
//     Author           :    Yash Yogesh Kundalwal
//     Date             :    1/08/2026
//
////////////////////////////////////////////////////////////////////////////////////////

bool IsFileExist(
                    char name[]
                )
{
    PINODE temp = head;
    bool bFlag = false;

    while(temp != NULL)
    {
        if(strcmp(temp->FileName, name) == 0)
        {
            bFlag = true;
            break;
        }
        temp = temp->next;
    }

    return bFlag;
}

////////////////////////////////////////////////////////////////////////////////////////
//
//     Function Name    :    CreateFile()
//     Description      :    It is used to create new file
//     Input            :    Name of file and Permissions
//     Output           :    File Descriptor
//     Author           :    Yash Yogesh Kundalwal
//     Date             :    1/08/2026
//
////////////////////////////////////////////////////////////////////////////////////////

int CreateFile  (
                    char name[],        // Name of File
                    int permission      // File Permission 
                )
{
    int i = 0;
    int fd = 0;

    PINODE temp = head;

    if(superobj.FreeInodes == 0)
    {
        return ERR_NO_INODES;
    }

    // if Permission value is wrong
    // Permission = 1 -> READ
    // Permission = 2 ->WRITE
    // Permission = 3 -> READ + WRITE

    if(permission < 1 || permission > 3)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(IsFileExist(name) == true)
    {
        return ERR_FILE_ALREADY_EXISTS;
    }

    // Search for empty inode
    while(temp != NULL)
    {
        if(temp->FileType == 0)
        {
            break;
        }

        temp = temp->next;
    }

    // Rare case
    if(temp == NULL)
    {
        return ERR_NO_INODES;
    }
    
    // Search entry UFDT entry
    // Reserve First 3 FD's
    for(i = 3; i < MAXINODE ; i++)
    {
        if(uareaobj.UFDT[i] == NULL)
        {
            break;
        }
    }

    if(i == MAXOPENFILES)
    {
        return ERR_MAX_FILES_OPEN;
    }

    //Allocate Memory for file table

    uareaobj.UFDT[i] = (PFILETABLE)malloc(sizeof(FILETABLE));

    // Initialize File Table

    uareaobj.UFDT[i]->ReadOffset = 0;
    uareaobj.UFDT[i]->WriteOffset = 0;
    uareaobj.UFDT[i]->Mode = permission;

    // Connect File Table with inode

    uareaobj.UFDT[i]->ptrinode = temp;

    // Initialise all members of inode

    strcpy(uareaobj.UFDT[i]->ptrinode->FileName, name);

    uareaobj.UFDT[i]->ptrinode->FileSize = MAXFILESIZE;

    uareaobj.UFDT[i]->ptrinode->ActualFileSize = 0;

    uareaobj.UFDT[i]->ptrinode->FileType = REGULARFILE;

    uareaobj.UFDT[i]->ptrinode->ReferenceCount = 1;

    uareaobj.UFDT[i]->ptrinode->Permission = permission;

    // Allocate memory for Files Data ( It is our Data block)

    uareaobj.UFDT[i]->ptrinode->Buffer = (char*)malloc(MAXFILESIZE);

    superobj.FreeInodes--;

    // we want fd and it will be same as i as it is the inode number and index of UFDT after loop traversal
    fd = i;

    return fd;
}

////////////////////////////////////////////////////////////////////////////////////////
//
//     Function Name    :    LsFile()
//     Description      :    It is used to display names
//                           of all file
//     Input            :    None
//     Output           :    None
//     Author           :    Yash Yogesh Kundalwal
//     Date             :    1/08/2026
//
////////////////////////////////////////////////////////////////////////////////////////

void LsFile()
{
    PINODE temp = head;

    printf("---------------------------------------------------------------------------\n");
    printf("---------------------Marvellous CVFS Files Information---------------------\n");
    printf("---------------------------------------------------------------------------\n");

    while(temp != NULL)
    {
        if(temp->FileType != 0)
        {
            printf("%s\n",temp->FileName);
        }

        temp = temp->next;
    }
}

////////////////////////////////////////////////////////////////////////////////////////
//
//     Function Name    :    LsFile_All()
//     Description      :    It is used to display all details
//                           of all files
//     Input            :    None
//     Output           :    None
//     Author           :    Yash Yogesh Kundalwal
//     Date             :    2/08/2026
//
////////////////////////////////////////////////////////////////////////////////////////

void LsFile_All()
{
    PINODE temp = head;

    printf("---------------------------------------------------------------------------\n");
    printf("---------------------Marvellous CVFS Files Information---------------------\n");
    printf("---------------------------------------------------------------------------\n");

    while(temp != NULL)
    {
        if(temp->FileType != 0)
        {
            printf("%s %d %d\n",temp->FileName, temp->InodeNumber, temp->ActualFileSize);
        }

        temp = temp->next;
    }
}

////////////////////////////////////////////////////////////////////////////////////////
//
//     Function Name    :    stat_file()
//     Description      :    It is used to display all details
//                           of specific files
//     Input            :    File Name
//     Output           :    Exit Status of Function
//     Author           :    Yash Yogesh Kundalwal
//     Date             :    2/08/2026
//
////////////////////////////////////////////////////////////////////////////////////////

int stat_file(
                char name[]
             )
{
    PINODE temp = NULL;
    int Permission = 0;
    int Type = 0;

    if(IsFileExist(name) == false)
    {
        return ERR_FILE_NOT_EXISTS;
    }

    temp = head;

    while(temp != NULL)
    {
        if(strcmp(temp->FileName,name) == 0)
        {
            printf("---------------------------------------------------------------------------\n");
            printf("----------------------Statistical Information of File----------------------\n");
            printf("---------------------------------------------------------------------------\n");

            printf("File Name: %s\n", temp->FileName);

            printf("Inode Number: %d \n", temp->InodeNumber);

            printf("File Size: %d\n", temp->FileSize);

            printf("Actual File Size: %d\n", temp->ActualFileSize);

            printf("Reference Count: %d\n", temp->ReferenceCount);

            Permission = temp->Permission;

            if(Permission == READ)
            {
                printf("File Permission: Read Only\n");
            }
            else if(Permission == WRITE)
            {
                printf("File Permission: Write\n");
            }
            else if(Permission == READ + WRITE)
            {
                printf("File Permission: Read + Write\n");
            }

            Type = temp->FileType;

            if(Type == REGULARFILE)
            {
                printf("File Type: Regular Type\n");
            }
            else if(Type == SPECIALFILE)
            {
                printf("File Type: Special Type\n");
            }

            printf("---------------------------------------------------------------------------\n");
            
            break;
        }

        temp = temp->next;
    }

    return EXECUTE_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////////////
//
//     Function Name    :    unlink_file()
//     Description      :    It is used to delete the
//                           specific file
//     Input            :    File Name
//     Output           :    Exit Status of Function
//     Author           :    Yash Yogesh Kundalwal
//     Date             :    2/08/2026
//
////////////////////////////////////////////////////////////////////////////////////////

int unlink_file(
                    char name[]
               )
{
    int i = 0;

    if(IsFileExist(name) == false)
    {
        return ERR_FILE_NOT_EXISTS;
    }

    for(i = 0 ; i < MAXOPENFILES ; i++)
    {
        if(uareaobj.UFDT[i] != NULL)
        {
            if(strcmp(uareaobj.UFDT[i]->ptrinode->FileName, name) == 0)
            {
                // Deallocate the memory of Buffer
                free(uareaobj.UFDT[i]->ptrinode->Buffer);
                uareaobj.UFDT[i]->ptrinode->Buffer = NULL;

                strcpy(uareaobj.UFDT[i]->ptrinode->FileName,"\0");

                uareaobj.UFDT[i]->ptrinode->FileSize = 0;

                uareaobj.UFDT[i]->ptrinode->ActualFileSize = 0;

                uareaobj.UFDT[i]->ptrinode->ReferenceCount = 0;

                uareaobj.UFDT[i]->ptrinode->Permission = 0;

                uareaobj.UFDT[i]->ptrinode->FileType = 0;
                
                // Deallocate memory of File Table
                free(uareaobj.UFDT[i]);

                uareaobj.UFDT[i] = NULL;

                superobj.FreeInodes++;

                break; // imp
            } // end of if
        }// end of if
    }// end of for
    
    return EXECUTE_SUCCESS;
} // end of unlink function

////////////////////////////////////////////////////////////////////////////////////////
//
//     Function Name    :    write_file()
//     Description      :    It is used to write the
//                           data into specific file
//     Input            :    File Descriptor, 
//                           Data that we want to write
//                           Size of Data  
//     Output           :    Number of Bytes Successfully returned
//     Author           :    Yash Yogesh Kundalwal
//     Date             :    2/08/2026
//
////////////////////////////////////////////////////////////////////////////////////////

int write_file(
                    int fd,
                    char * data,
                    int size
              )
{
    printf("File Descriptor: %d\n", fd);
    printf("Data that we want to write: %s\n",data);
    printf("Size of Data: %d\n",size);

    if((fd < 0) || (fd > MAXOPENFILES))
    {
        return ERR_INVALID_PARAMETER;
    }

    // if writing permission is not there
    if(uareaobj.UFDT[fd]->ptrinode->Permission < WRITE)
    {
        return ERR_PERMISSION_DENIED;
    }

    // check the space is there or not
    if((MAXFILESIZE - uareaobj.UFDT[fd]->WriteOffset) < size)
    {
        return ERR_INSUFFICIENT_SPACE;
    }

    // Actual Data Writing
    strncpy(uareaobj.UFDT[fd]->ptrinode->Buffer + uareaobj.UFDT[fd]->WriteOffset,data,size);

    // Update the write offset
    uareaobj.UFDT[fd]->WriteOffset = uareaobj.UFDT[fd]->WriteOffset + size;

    // Update Actual File Size
    uareaobj.UFDT[fd]->ptrinode->ActualFileSize = uareaobj.UFDT[fd]->ptrinode->ActualFileSize + size;

    return size;
}

////////////////////////////////////////////////////////////////////////////////////////
//
//     Function Name    :    read_file()
//     Description      :    It is used to read the
//                           data into specific file
//     Input            :    File Descriptor 
//                           Address of empty Buffer
//                           Size of Data  
//     Output           :    Number of Bytes Successfully read
//     Author           :    Yash Yogesh Kundalwal
//     Date             :    2/08/2026
//
////////////////////////////////////////////////////////////////////////////////////////

int read_file(
                int fd,
                char * data,
                int size
             )
{
    if(fd < 0 || fd > MAXOPENFILES)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(size < 0)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(uareaobj.UFDT[fd] == NULL)
    {
        return ERR_FILE_NOT_EXISTS;
    }

    // Filter for Permission
    if(uareaobj.UFDT[fd]->ptrinode->Permission < READ)
    {
        return ERR_PERMISSION_DENIED;
    }

    // Insufficient data
    if((MAXFILESIZE - uareaobj.UFDT[fd]->ReadOffset) < size)
    {
        return ERR_INSUFFICIENT_DATA;
    }

    // Read the Data

    strncpy(data,uareaobj.UFDT[fd]->ptrinode->Buffer + uareaobj.UFDT[fd]->ReadOffset, size);

    uareaobj.UFDT[fd]->ReadOffset = uareaobj.UFDT[fd]->ReadOffset + size;

    return size;
}

////////////////////////////////////////////////////////////////////////////////////////
//
//     Function Name    :    create_backup()
//     Description      :    It creates the backup of
//                           files when applicatio is closed
//     Input            :    none
//     Output           :    none
//     Author           :    Yash Yogesh Kundalwal
//     Date             :    6/08/2026
//
////////////////////////////////////////////////////////////////////////////////////////

void create_backup()
{
    PINODE temp = NULL;
    int fd = 0;\
    char path[100] = {'\0'};
    
    temp = head;

    while(temp != NULL)
    {
        if(temp->FileType != 0)
        {
            sprintf(path,"./backup/%s",temp->FileName);

            fd = creat(path,0777);

            if(fd != -1)
            {
                write(fd,temp->Buffer,temp->ActualFileSize);
            }

            close(fd);
        }
        temp = temp->next;
    }

    printf("All of your Data is in backup Directory\n");
}

////////////////////////////////////////////////////////////////////////////////////////
//
//     Entry Point Function of the CVFS Project
//
////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    // Input Command
    char str[80] = {'\0'};

    // Tokenised Command
    char Command[5][20] = {{'\0'}};

    // Data for write System Call
    char InputBuffer[MAXFILESIZE] = {'\0'};

    int iRet = 0;
    int iCount = 0;
    int size = 0;

    char * EmptyBuffer = NULL;

    StartAuxillaryDataInitialisation();

    printf("--------------------------------------------------------------------------\n");
    printf("------------------ Marvellous CVFS started successfully ------------------\n");
    printf("--------------------------------------------------------------------------\n");

    // Infinite Listening Shell

    while(1)
    {
        fflush(stdin);

        strcpy(str,"");

        printf("\nMarvellous CVFS : > ");
        fgets(str,sizeof(str),stdin);

        iCount = sscanf(str,"%s %s %s %s %s",Command[0],Command[1],Command[2],Command[3],Command[4]);

        fflush(stdin);

        if(iCount == 1)
        {
            // Marvellous CVFS : > exit
            if(strcmp(Command[0],"exit") == 0)
            {
                printf("Thank you for using Marvellous CVFS\n");
                printf("Deallocation all resources of Marvellous CVFS \n");
                create_backup();

                break;
            }

            // Marvellous CVFS : > help
            else if(strcmp(Command[0],"help") == 0)
            {
                DisplayHelp();
            }

            // Marvellous CVFS : > clear

            else if(strcmp(Command[0],"clear") == 0)
            {
                #ifdef _WIN32
                    system("cls");
                #else
                    system("clear");
                #endif
            }
            else if(strcmp(Command[0],"ls") == 0)
            {
                LsFile();
            }
            else
            {
                printf("Command not found\n");
                printf("Please refer help option to get more information\n");
                printf("Please refer manual page of command using man\n");
            }
        }
        else if(iCount == 2)
        {
            // Marvellous CVFS : > man open
            if(strcmp(Command[0],"man") == 0)
            {
                ManPageDisplay(Command[1]);
            }
            // Marvellous CVFS : > ls -a
            else if((strcmp(Command[0],"ls") == 0) && (strcmp(Command[1],"-a") == 0))
            {
                LsFile_All();
            }
            // Marvellous CVFS : > stat Ganesh.txt
            else if(strcmp(Command[0],"stat") == 0)
            {
                iRet = stat_file(Command[1]);

                if(iRet == ERR_FILE_NOT_EXISTS)
                {
                    printf("Error : File not Exist\n");
                }
            }
            // Marvellous CVFS : > unlink Ganesh.txt
            else if((strcmp(Command[0],"unlink") == 0) || (strcmp(Command[0],"rm") == 0))
            {
                iRet = unlink_file(Command[1]);

                if(iRet == ERR_FILE_NOT_EXISTS)
                {
                    printf("Error : File not Exist\n");
                }
                else if(iRet == EXECUTE_SUCCESS)
                {
                    printf("Marvellous CVFS : File Deleted Successfully");
                }
            }
            // Marvellous CVFS : > write 1
            else if((strcmp(Command[0],"write") == 0))
            {
                printf("Enter the data that you want to write into the file: \n");

                fgets(InputBuffer,MAXFILESIZE,stdin);

                size = strlen(InputBuffer);

                iRet = write_file(atoi(Command[1]), InputBuffer, size);

                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid Parameter\n");

                    printf("Please refer man page for more information\n");
                }
                else if(iRet == ERR_PERMISSION_DENIED)
                {
                    printf("Error : There is no permission to write the data\n");
                }
                else if(iRet == ERR_INSUFFICIENT_SPACE)
                {
                    printf("Error : There is no space to write the data\n");
                }
                else
                {
                    printf("%d bytes gets successfully written into the file \n", iRet);
                }
            }
            else
            {
                printf("Command not found\n");
                printf("Please refer help option to get more information\n");
                printf("Please refer manual page of command using man\n");
            }
        }
        else if(iCount == 3)
        {
            // Marvellous CVFS : > creat Ganesh.txt 3
            if(strcmp(Command[0],"creat") == 0)
            {
                iRet = CreateFile(Command[1],atoi(Command[2]));

                if(iRet == ERR_NO_INODES)
                {
                    printf("Error : Unable to create new file\n");
                    printf("Because there is no free inode\n");
                }
                else if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Unable to create new file\n");
                    printf("Because parameters of command are invalid\n");
                    printf("Please use man page to get actual parameters\n");
                }
                else if(iRet == ERR_FILE_ALREADY_EXISTS)
                {
                    printf("Error : Unable to create new file\n");
                    printf("Because Filename is already present\n");
                    printf("Please use ls command to check names of all file\n");
                }
                else if(iRet == ERR_MAX_FILES_OPEN)
                {
                    printf("Error : Unable to create new file\n");
                    printf("Because UFDT is FULL\n");
                    printf("Please close some opened file\n");
                }
                else
                {
                    printf("File successfully created with FD : %d\n", iRet);
                }
            }

            // Marvellous CVFS : > read Ganesh.txt 3
            else if(strcmp(Command[0],"read") == 0)
            {
                EmptyBuffer = (char *)malloc(atoi(Command[2]));

                iRet = read_file(atoi(Command[1]),EmptyBuffer,atoi(Command[2]));

                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid Parameter\n");
                }
                else if(iRet == ERR_FILE_NOT_EXISTS)
                {
                    printf("Error : File not exist\n");
                }
                else if(iRet == ERR_INSUFFICIENT_DATA)
                {
                    printf("Error : Insufficient Data\n");
                }
                else if(iRet == ERR_PERMISSION_DENIED)
                {
                    printf("Error : Permission Denied\n");
                }
                else
                {
                    printf("Read Operation is successfull\n");

                    printf("Data succesfully read is: %s\n", EmptyBuffer);

                }
            }
            else
            {
                printf("Command not found\n");
                printf("Please refer help option to get more information\n");
                printf("Please refer manual page of command using man\n");
            }
        }
        else if(iCount == 4)
        {
            
        }
        else
        {
            printf("Command not found\n");
            printf("Please refer help option to get more information\n");
            printf("Please refer manual page of command using man\n");
        }
    } // End of while

    return 0;
} // End of main