#include <stdio.h>
#include <process.h>
#include <string.h>
#include <stdlib.h>

#define NUMBER_OF_STRING 30
#define MAX_STRING_SIZE 50
static int NUMBER_OF_COURSES=0;
static int  i=0;
static int  xp=0;
typedef struct Course
{
    char courseName[40];
    int courseHours;
    char courseCode[10 ];
    char courseDep[40];
    char courseTopics[NUMBER_OF_STRING][MAX_STRING_SIZE];
    int sizeofTopics;
    struct Course* Left;
    struct Course* Right;
    int Height;
}Course;

typedef struct File File;

void PrintSameDep(struct Course* , char []);
struct Course* ReadCorse(Course *);
struct Course *leftRotate(struct Course *);
struct Course *rightRotate(struct Course *);
struct Course* newNode(char [], int ,char [],char [], char [][MAX_STRING_SIZE],int);
int height(struct Course *);
int max(int, int);
int getBalance(struct Course *);
struct Course* insert(struct Course* ,char [], int ,char [],char [], char [][MAX_STRING_SIZE],int);
void preOrder(struct Course *);
struct Course* AddingCourse(Course*);
Course *Find( char [], Course * ,Course*);
struct Course* deleteNode(struct Course* , char[]);
struct Course * minValueNode(struct Course* );
void PrintInOrder( Course*);
struct Course* PrintTopics(struct Course* ,char[] );
void findfirstLetter(struct Course* , char ,char [][10]);
void deleteSameDep(struct Course* ,char [],char codes[30][10]) ;
void export(Course * );
void export_recursive(File* , Course * );
void printInOrder_Name(struct Course* ,char [][90]);
void PrintInOrder2( Course* ,char [][90]);
int main() {

    Course *course;

    int choice;
    char fin[10];
    char code[10];
    char dep[40];
    char dep2[40];
    char del[10];
    char fl;
    char  h;
    char codes[NUMBER_OF_COURSES][10];
    char names[NUMBER_OF_COURSES][90];
    Course *founded=NULL;
    do {
        printf("\n 1. Read the file courses.txt file and create the tree.\n"
               " 2. Insert a new course from the user with all its associated data.\n"
               " 3. Find a course and support updating of its information. \n"
               " 4. List courses in lexicographic order with their associated information\n"
               " 5. List all topics associated with a given course. \n"
               " 6. List all courses in lexicographic order that belong to the same department.\n"
               " 7. Delete a course.\n"
               " 8. Delete all courses that start with a specific letter.\n"
               " 9. Delete all courses that belong to a given department.\n"
               " 10.Save all words in file offered_courses.txt\n"
               "Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                course=ReadCorse(course);


                break;
            case 2:
                course=AddingCourse(course);

                break;
            case 3:

                printf("Enter Course Code to find : ");
                scanf("%s",&fin);
                course=Find(fin,course,course);
                break;
            case 4:

                printf("Sorting according to \n1:Course Code.\n2:Course Name:\n");
                int x;
                scanf("%d",&x);
                switch (x) {

                    case 1:
                        PrintInOrder(course);
                        break;
                    case 2:
                        printInOrder_Name(course,names);//copy all courses names in the array
                        char temp[90];
                        //sorting the array according to the name
                        for(int k=0;k<xp;k++)
                            for(int j=k+1;j<xp;j++)
                                if(strcmp(names[k],names[j])>0)
                                {
                                    strcpy(temp,names[k]);
                                    strcpy(names[k],names[j]);
                                    strcpy(names[j],temp);
                                }

                        for(int i=0;i<xp;i++)//printing the tree after sorting
                            PrintInOrder2(course,names[i]);
                        for(int s=0;s<xp;s++)
                            for (int j = 0; j <50 ; j++) {
                                names[s][j]="";
                            }


                        xp=0;

                        break;
                    default:
                        break;

                }

                break;
            case 5:

                printf("Enter Course Code to print the topics :\n");
                scanf("%s",&code);
                founded= PrintTopics(course,code);//find the course
                for (int d = 0; d < founded->sizeofTopics; d++) {//print the topics that have the course code
                    printf("%s\n",founded->courseTopics[d]);

                }
                break;
            case 6:
                printf("Enter Department to print the courses :\n");
                scanf("%c",&h);
                gets(dep);
                PrintSameDep(course,dep);//print the info at the same department
                break;
            case 7:
                printf("Enter Course Code to Delete :\n");
                scanf("%s",&del);
                course=deleteNode(course,del);//delete a specific course
                break;
            case 8:
                printf("Enter First Letter that you want to Delete :\n");
                scanf("%c",&h);
                scanf("%c",&fl);
                findfirstLetter(course, fl,codes);//find the courses starts with a letter
                for (int j = 0; j <i; j++) {//delete the courses
                    course= deleteNode(course,codes[j]);
                }
                for(int s=0;s<i;s++)
                    for (int j = 0; j <10 ; j++) {
                        codes[s][j]="";
                    }

                i=0;
                break;
            case 9:
                printf("Enter Department to Delete the courses :\n");
                scanf("%c",&h);
                gets(dep2);
                deleteSameDep(course,dep2,codes);//find the courses at the same department
                for (int l = 0; l<i; l++) {//delete the courses founded
                    course= deleteNode(course,codes[l]);
                }
                for(int s=0;s<i;s++)
                    for (int j = 0; j <10 ; j++) {
                        codes[s][j]="";
                    }

                i=0;
                break;
            case 10:
                export(course);
                break;

            default:
                printf("Please try again");
                break;
        }

    } while (choice != 11);




    return 0;
}
struct Course* ReadCorse(Course *course){//read courses file and lode it to an AVL tree.

    FILE * fin;
    fin = fopen("courses", "r");
    if (fin == NULL)
    {
        printf( "Error! File cannot be read");
        exit (1);
    }
    char line[300];
    char * sr;
    char* ret;
    printf("      Course Name \t             C_Hours      C_Code          C_Department\t          #Topics\n");
    while (fgets(line, 300, fin))
    {
        char courseName[40];
        int courseHours;
        char courseCode[10];
        char courseDep[40];
        char courseTopics[NUMBER_OF_STRING][MAX_STRING_SIZE];
        // Remove trailing newline
        line[strcspn(line, "\n")] = 0;

        sr = strtok(line, ":");
        strcpy(courseName,sr);


        sr = strtok(NULL, "#");
        courseHours= atoi(sr);

        sr = strtok(NULL, "#");
        strcpy(courseCode, sr);
        if(courseCode==' '){//if there is any specie at the first of the string we remove it
            for (int e = 0; e < strlen(courseCode); e++) {
                courseCode[e]=courseCode[e+1];
            }
        }

        sr = strtok(NULL, "/");
        strcpy(courseDep, sr);
        if(courseDep[0]==' '){
            for (int e = 0; e < strlen(courseDep); e++) {
                courseDep[e]=courseDep[e+1];
            }
        }


        sr = strtok(NULL, "\n");
        ret = strtok(sr, ",");

        int size =0;
        for (int w = 0; w < NUMBER_OF_STRING; w++) {
            if (ret!=NULL) {
                strcpy(courseTopics[w], ret);
                if (courseTopics[w][0] == ' ') {
                    for (int e = 0; e < strlen(courseTopics[w]); e++) {
                        courseTopics[w][e] = courseTopics[w][e + 1];
                    }
                }
                ret = strtok(NULL, ",");
                size=w+1;

            }
        }
        printf("*------------------------------------------------------------------------------------------------------*\n");
        printf("  %-27s     |   %-2d   |   %-10s |   %-19s   |       %-15d \n",courseName,courseHours,courseCode,courseDep,size);


        course=insert(course,courseName,courseHours,courseCode,courseDep,courseTopics,size);//call inserting function
        NUMBER_OF_COURSES++;//increment the number of courses
    }
    fclose(fin);
    return course;
}

// Recursive function to insert a key in the subtree rooted
// with node and returns the new root of the subtree.
struct Course* insert(struct Course* course,char courseName[40], int courseHours,char courseCode[10],char courseDep[40], char courseTopics[NUMBER_OF_STRING][MAX_STRING_SIZE],int size)
{
    /* 1.  Perform the normal BST insertion */
    if (course == NULL) {
        course= (newNode(courseName, courseHours, courseCode, courseDep, courseTopics,size));
        return course;
    }
    if(strcmp(courseCode,course->courseCode)==-1)
        course->Left  = insert(course->Left, courseName,courseHours,courseCode,courseDep,courseTopics,size);
    else if (strcmp(courseCode,course->courseCode)==1)
        course->Right = insert(course->Right, courseName,courseHours,courseCode,courseDep,courseTopics,size);
    else // Equal keys are not allowed in BST
        return course;

    /* 2. Update height of this ancestor node */
    course->Height = height(course);

    /* 3. Get the balance factor of this ancestor
          node to check whether this node became
          unbalanced */
    int balance = getBalance(course);

    // If this node becomes unbalanced, then
    // there are 4 cases

    // Left Left Case
    if (balance > 1 &&strcmp(courseCode,course->Left->courseCode)==-1)
        return rightRotate(course);

    // Right Right Case
    if (balance < -1 && strcmp(courseCode,course->Right->courseCode)==1)
        return leftRotate(course);

    // Left Right Case
    if (balance > 1 &&strcmp(courseCode,course->Left->courseCode)==1)
    {
        course->Left =  leftRotate(course->Left);
        return rightRotate(course);
    }

    // Right Left Case
    if (balance < -1 && strcmp(courseCode,course->Right->courseCode)==-1)
    {
        course->Right = rightRotate(course->Right);
        return leftRotate(course);
    }

    /* return the (unchanged) node pointer */
    return course;
}
// A utility function to left rotate subtree rooted with x
// See the diagram given above.
struct Course *leftRotate(struct Course *x)
{
    struct Course *y = x->Right;
    struct Course *T2 = y->Left;

    // Perform rotation
    y->Left = x;
    x->Right = T2;

    //  Update heights
    x->Height = height(x);
    y->Height = height(y);

    // Return new root
    return y;
}
// A utility function to right rotate subtree rooted with y
// See the diagram given above.
struct Course *rightRotate(struct Course *y)
{
    struct Course *x = y->Left;
    struct Course *T2 = x->Right;

    // Perform rotation
    x->Right = y;
    y->Left = T2;

    // Update heights
    y->Height = height(y);
    x->Height = height(x);

    // Return new root
    return x;
}
/* Helper function that allocates a new node with the given key and
    NULL left and right pointers. */
struct Course* newNode(char courseName[40], int courseHours,char courseCode[10],char courseDep[40], char courseTopics[NUMBER_OF_STRING][MAX_STRING_SIZE],int size)
{
    struct Course* course = (struct Course*)
            malloc(sizeof(struct Course));//create a new node and copy the data
    strcpy(course->courseName,courseName);
    course->courseHours=courseHours;
    strcpy(course->courseCode,courseCode);
    strcpy(course->courseDep,courseDep);
    course->sizeofTopics=size;
    for (int i = 0; i < size; i++) {
        if(courseTopics[i]!=NULL)
            strcpy(course->courseTopics[i],courseTopics[i]);
    }

    course->Left   = NULL;
    course->Right  = NULL;
    course->Height = 0;  // new node is initially added at leaf
    return(course);
}
// A utility function to get the height of the tree
int height(struct Course *N)
{
    if (N == NULL)
        return 0;
    return 1+max(height(N->Left), height(N->Right));
}

// A utility function to get maximum of two integers
int max(int a, int b)
{
    return (a > b)? a : b;
}
// Get Balance factor of node N
int getBalance(struct Course *N)
{
    if (N == NULL)
        return 0;
    return height(N->Left) - height(N->Right);
}
// A utility function to print preorder traversal
// of the tree.
// The function also prints height of every node
void preOrder(struct Course *root)
{
    if(root != NULL)
    {
        printf("%s\n", root->courseCode);
        preOrder(root->Left);
        preOrder(root->Right);
    }
}
struct Course* AddingCourse(Course* course){//adding a new course
    char courseName[40];
    int courseHours;
    char courseCode[10];
    char courseDep[40];
    char courseTopics[NUMBER_OF_STRING][MAX_STRING_SIZE];
    char h;
    char c[50];


    printf("Please Enter Course Name : \n");
    scanf("%c",&h);
    gets(c);
    strcpy(courseName,c);
    printf("Please Enter Course Hours : \n");
    scanf("%d",&courseHours);
    printf("Please Enter course Code .exp(Comp133): \n");
    scanf("%c",&h);
    gets(c);
    strcpy(courseCode,c);
    printf("Please Enter Course Department: \n");
    gets(c);
    strcpy(courseDep,c);
    printf("Please Enter Course Topics and then write Stop: \n");
    scanf("%c",&h);
    gets(c);
    int s=0;
    for (int i = 0; i <NUMBER_OF_STRING; i++) {//enter the topics
        if (strcmp(c, "Stop") != 0) {
            s++;
            strcpy(courseTopics[i], c);
            gets(c);

        }
    }

    course=insert(course,courseName,courseHours,courseCode,courseDep,courseTopics,s);

    printf("   %s     |    %d    |   %s    |    %s    |    %d \n",courseName,courseHours,courseCode,courseDep,s);
    return course;
}
Course *Find( char X[], Course *T ,Course* ridi)//find a course with the courseCode and allow the user to update its data
{int a;
    Course* temp;
    Course* temp2;
    int choice;
    char c[50];
    char h;
    //here we search in the tree to find the needed course according to the course code
    if( T == NULL )
        return NULL;
    if(strcmp(X,T->courseCode)==-1 )
        return Find( X, T->Left,ridi );
    else if(strcmp(X,T->courseCode)==1)
        return Find( X, T->Right ,ridi);
    else {

        printf("%s   |  %d  |  %s  |  %s  | %d\n",T->courseName,T->courseHours,T->courseCode,T->courseDep,T->sizeofTopics);
        printf("Would You Update Information\n1: Yes\n2: NO\n");
        scanf("%d", &a);
        if (a == 1) {
            printf("Choose what do you want to update:\n1:Course Name\n2:Course Hours\n3:Course Code\n4:Course Department\n5:Course Topics\n");
            scanf("%d",&choice);
            if(choice==1){
                printf("Enter new Course Name : \n");
                scanf("%c",&h);
                gets(c);
                strcpy(T->courseName,c);
                printf(" %s   |    %d    |   %s    |   %s   |   %d \n",T->courseName,T->courseHours,T->courseCode,T->courseDep, T->sizeofTopics);

            } else if(choice==2){
                printf("Enter new Course Hours : \n");
                scanf("%c",&h);
                gets(c);
                T->courseHours= atoi(c);
                printf(" %s   |    %d    |   %s    |    %s    |    %d \n",T->courseName,T->courseHours,T->courseCode,T->courseDep,T->sizeofTopics);
            }else if(choice==3){
                printf("Enter new Course Code : \n");
                scanf("%c",&h);
                gets(c);
                temp2=T;
                //here we make rotate by deleting the node and inserting it again
                temp= newNode(T->courseName,T->courseHours,c,T->courseDep,T->courseTopics,T->sizeofTopics);
                ridi= deleteNode(ridi,T->courseCode);
                ridi=insert(ridi, temp->courseName, temp->courseHours, temp->courseCode, temp->courseDep,
                            temp->courseTopics,temp->sizeofTopics);
                T=temp2;
                printf(" %s   |    %d    |   %s    |    %s    |    %d \n",temp->courseName,temp->courseHours,temp->courseCode,temp->courseDep,T->sizeofTopics);
            }else if(choice==4){
                printf("Enter new Course Department : \n");
                scanf("%c",&h);
                gets(c);
                strcpy(T->courseDep,c);
                printf(" %s   |    %d    |   %s    |    %s    |    %d \n",T->courseName,T->courseHours,T->courseCode,T->courseDep,T->sizeofTopics);
            }
            else if(choice==5){
                for (int l = 0; l < T->sizeofTopics; l++) {//clear all the topics
                    strcpy(T->courseTopics[l],"");
                }
                printf("Enter Course Topics and then write Stop: \n");
                scanf("%c",&h);
                gets(c);
                for (int i = 0; i < T->sizeofTopics; i++) {//insert new topics
                    if (strcmp(c, "Stop") != 0) {
                        strcpy(T->courseTopics[i], c);
                        gets(c);

                    }


                }
                printf("   %s     |    %d    |   %s    |    %s    |    %d \n",T->courseName,T->courseHours,T->courseCode,T->courseDep,T->sizeofTopics);
            }

        } else{

            return ridi;
        }

        return ridi;
    }
}
// Recursive function to delete a node with given code
// from subtree with given root. It returns root of
// the modified subtree.
struct Course* deleteNode(struct Course* root, char CourseCode[10])
{
    if (root == NULL)
        return root;
    // If the course to be deleted is smaller than the root's course, then it lies in left subtree
    if ( strcmp(CourseCode,root->courseCode)==-1)
        root->Left = deleteNode(root->Left, CourseCode);// If the course to be deleted is greater than the root's course, then it lies in right subtree
    else if( strcmp(CourseCode,root->courseCode)==1)
        root->Right = deleteNode(root->Right, CourseCode);

        // if course is same as root's code, then This is the node to be deleted
    else
    {
        // node with only one child or no child
        if( (root->Left == NULL) || (root->Right == NULL) )
        {
            struct Course *temp = root->Left ? root->Left :root->Right;
            // No child case
            if (temp == NULL)
            {
                temp = root;
                root = NULL;

            }
            else // One child case
                *root = *temp; // Copy the contents of
            // the non-empty child

            free(temp);
        }
        else
        {
            // node with two children: Get the inorder successor (smallest in the right subtree)
            struct Course* temp = minValueNode(root->Right);

            // Copy the inorder successor's data to this node
            strcpy(root->courseCode,temp->courseCode);
            strcpy(root->courseName,temp->courseName);
            strcpy(root->courseDep,temp->courseDep);
            strcpy(root->courseTopics,temp->courseTopics);
            root->sizeofTopics=temp->sizeofTopics;
            root->courseHours=temp->courseHours;
            // Delete the inorder successor
            root->Right = deleteNode(root->Right, temp->courseCode);
        }
    }
    // If the tree had only one node then return
    if (root == NULL)
        return root;

    //  UPDATE HEIGHT OF THE CURRENT NODE
    root->Height = 1 + max(height(root->Left),height(root->Right));

    //  check whether this node became unbalanced)
    int balance = getBalance(root);

    // If this node becomes unbalanced, then there are 4 cases

    // Left Left Case
    if (balance > 1 && getBalance(root->Left) >= 0)
        return rightRotate(root);

    // Left Right Case
    if (balance > 1 && getBalance(root->Left) < 0)
    {
        root->Left =  leftRotate(root->Left);
        return rightRotate(root);
    }

    // Right Right Case
    if (balance < -1 && getBalance(root->Right) <= 0)
        return leftRotate(root);

    // Right Left Case
    if (balance < -1 && getBalance(root->Right) > 0)
    {
        root->Right = rightRotate(root->Right);
        return leftRotate(root);
    }
    NUMBER_OF_COURSES--;
    return root;
}

/* Given a non-empty binary search tree, return the
   node with minimum key value found in that tree.
   Note that the entire tree does not need to be
   searched. */
struct Course * minValueNode(struct Course* course)
{
    struct Course* current = course;

    /* loop down to find the leftmost leaf */
    while (current->Left != NULL)
        current = current->Left;

    return current;
}

void PrintInOrder( Course* T)//printing data in order
{
    if( T != NULL) {
        PrintInOrder(T->Left);
        printf("%s     |    %d    |   %s    |    %s     |    %d\n", T->courseName, T->courseHours, T->courseCode,
               T->courseDep,T->sizeofTopics);
        printf("************************List Of Topics************************\n");
        for (int i = 0; i < T->sizeofTopics; i++) {
            printf("%s", T->courseTopics[i]);
            printf("\n");
        }
        printf("\n");
        PrintInOrder( T->Right );
    }
}
struct Course* PrintTopics(struct Course* course,char code[]){//finding a specific course according its course code to print it topics
    if(course==NULL)
        return NULL;
    if(strcmp(code,course->courseCode)==-1){
        return PrintTopics(course->Left,code);
    } else if(strcmp(code,course->courseCode)==1){
        return PrintTopics(course->Right,code);
    } else
        return course;
}
void PrintSameDep(struct Course* course,char dep[40]) {//find courses from the same department and printing there data InOrder

    if (course != NULL) {
        PrintSameDep(course->Left, dep);
        if (strcmp(dep, course->courseDep) == 0) {

            printf("%s     |    %d    |   %s    |    %s     |    %d\n", course->courseName, course->courseHours,
                   course->courseCode,
                   course->courseDep, course->sizeofTopics);
            printf("************************List Of Topics************************\n");
            for (int i = 0; i < course->sizeofTopics; i++) {//printing the topics
                printf("%s", course->courseTopics[i]);
                printf("\n");
            }
            printf("\n");

        }
        PrintSameDep(course->Right, dep);



    }
}

void findfirstLetter(struct Course* course,char code,char codes[30][10]){

//store cods in array and check first letter then send them to delete;
    if( course != NULL) {
        findfirstLetter(course->Left, code,codes);
        findfirstLetter(course->Right, code,codes);
        if(code==course->courseName[0]){
            strcpy( codes[i],course->courseCode);
            i++;
        }
    }

}
void deleteSameDep(struct Course* course,char dep[40],char codes[][10]) {
//store cods in array and check same department then send them to delete;
    if (course != NULL) {
        deleteSameDep(course->Left, dep,codes);

        if (strcmp(dep, course->courseDep) == 0) {
            strcpy( codes[i],course->courseCode);
            i++;
        }
        deleteSameDep(course->Right, dep,codes);
    }
}

void export(Course * root)//printing on the file
{
    FILE *out;
    out=fopen("offered_courses","w");
    export_recursive(out, root);//print data recursively
    fclose(out);
}

void export_recursive(File* out, Course * T)//printing each course in a file
{
    if(T!=NULL)
    {
        export_recursive(out,T->Left);//go to every root in the tree and print it at the file
        fprintf(out,"%s:%d#%s#%s/", T->courseName, T->courseHours, T->courseCode,
                T->courseDep);

        for (int i = 0; i < T->sizeofTopics; i++) {//printing topics
            fprintf(out,"%s", T->courseTopics[i]);
            if(i<T->sizeofTopics-1){
                fprintf(out,", ");
            }

        }
        fprintf(out,"\n");
        export_recursive(out,T->Right);
    }
}
void printInOrder_Name(struct Course* T,char names[][90]){//store all courses name in an array

    if( T != NULL) {

        strcpy(names[xp],T->courseName);
        xp++;
        printInOrder_Name(T->Left,names);
        printInOrder_Name( T->Right,names );
    }


}
void PrintInOrder2( Course* T,char name[][90])//printing the courses' info after sorting according to the course name
{
    if( T != NULL) {
        PrintInOrder2(T->Left,name);

        if(strcmp(T->courseName,name)==0){//if the  name in the tree same as  the one in the array then print it
            printf("%s     |    %d    |   %s    |    %s     |    %d\n", T->courseName, T->courseHours, T->courseCode,
                   T->courseDep,T->sizeofTopics);
            printf("************************List Of Topics************************\n");
            for (int i = 0; i < T->sizeofTopics; i++) {
                printf("%s", T->courseTopics[i]);
                printf("\n");
            }
            printf("\n");
        }

        PrintInOrder2( T->Right,name );
    }
}