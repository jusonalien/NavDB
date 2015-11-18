#include <stdio.h>
#include <stdlib.h>

typedef struct R1 {
    int ID ;
    int age;
} R1;

typedef struct R2 {
    int ID;
    int salary;
} R2;

typedef struct R3 {
    int ID;
    int age;

    int salary;
} R3;

#define BLOCK_UNIT_R1  20
#define BLOCK_UNIT_R2  10
#define R1_TURPLE_NUM  2000
#define R2_TURPLE_NUM  1000
#define R3_TURPLE_NUM  6666

R1 readbuffer_r1[BLOCK_UNIT_R1 + 1];
R2 readbuffer_r2[BLOCK_UNIT_R2 + 1];


R1 relation1[R1_TURPLE_NUM];
R2 relation2[R2_TURPLE_NUM];
R3 relation3[R3_TURPLE_NUM];

void
read_a_block_of_r1(int block_index)
{
    int i = block_index * BLOCK_UNIT_R1;
    int j = 0;
    int end = block_index * BLOCK_UNIT_R1 + BLOCK_UNIT_R1 - 1;

    for (;i <= end;++i,++j) {
        readbuffer_r1[j].age = relation1[i].age;
        readbuffer_r1[j].ID  = relation1[i].ID;
    }
}

void
read_a_block_of_r2(int block_index)
{
    int i = block_index * BLOCK_UNIT_R2;
    int j = 0;
    int end = block_index * BLOCK_UNIT_R2 + BLOCK_UNIT_R2 - 1;
    for (;i <= end;++i,++j) {
        readbuffer_r2[j].ID     = relation2[i].ID;
        readbuffer_r2[j].salary = relation2[i].salary;
    }

}

int R3_POS = 0;

void
nested_loop_link()
{
    int i, j;
    for (i = 0;i < BLOCK_UNIT_R1;++i)
    for (j = 0;j < BLOCK_UNIT_R2;++j)
        if (readbuffer_r1[i].ID == readbuffer_r2[j].ID) {
                relation3[R3_POS].age    = readbuffer_r1[i].age;
                relation3[R3_POS].ID     = readbuffer_r1[i].ID;
                relation3[R3_POS].salary = readbuffer_r2[j].salary;
                R3_POS++;
        }
}


void
read_r1()
{
    int i;
    for (i = 0;i < R1_TURPLE_NUM;++i) {
        scanf("%d %d",&(relation1[i].ID),&(relation1[i].age));
    }
    printf("R1 finish reading\n");
}

void
read_r2()
{
    int i;
    for (i = 0; i< R2_TURPLE_NUM;++i) {
        scanf("%d",&(relation2[i].ID));
        scanf("%d",&(relation2[i].salary));
    }
    printf("R2 finish reading\n");
}

void
print_rela1()
{
    int i;
    for (i = 0; i < R1_TURPLE_NUM;++i)
        printf("%d %d\n",relation1[i].age,relation1[i].ID);
}
void
print_rela2()
{
    int i;
    for (i = 0;i < R2_TURPLE_NUM;++i)
        printf("%d %d\n",relation2[i].ID,relation2[i].salary);
}

void
print_r3()
{

    int i;
    for (i = 0;i < R3_POS;++i) {
        printf("%d %d %d \n",relation3[i].ID,relation3[i].salary,relation3[i].age);
    }
}
int main()
{

    int block_num_r1,block_num_r2;
    block_num_r1 = R1_TURPLE_NUM / BLOCK_UNIT_R1;
    block_num_r2 = R2_TURPLE_NUM / BLOCK_UNIT_R2;

    int i,j;
    read_r1();
    read_r2();
    printf("%d %d\n",block_num_r1, block_num_r2);
    for (i = 0;i < block_num_r1;++i) {
        for (j = 0;j < block_num_r2;++j) {
            read_a_block_of_r1(i);
            read_a_block_of_r2(j);
            nested_loop_link();
        }
    }
    printf("finish nested loop\n");
    printf("%d\n",R3_POS);
    print_r3();

    return 0;
}

