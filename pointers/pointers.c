#include <stdio.h>
#include <stdlib.h>


#define ARRAY_LEN(arr) (sizeof(arr)/sizeof((arr)[0]))


typedef struct {
    unsigned int a;
    unsigned int b;
    int *pC;
    void *pD;
    char *str1;
    char charArr1[8];
    char charArr2[5];
    short array[4];
} Test;


int main(void) {
    int i;
    unsigned char *p;
    Test t;
    Test *pT = &t; // Pointer to the test structure, reference to structure t.

    /*** INIT THE STRUCTURE ***/
    /* Assign a hex value to the variable a of structure t. */
    t.a = 0xDEADBEEF;
    /* Assign a decimal value to the variable b of the structure t. Here we
     * use the pointer pT of the structure t. Notice the difference in the
     * syntax.
     */
    pT->b = 2857740885;
    /* Assign a value of 2048 to the int variable where the pointer pC is
     * pointing to. First allocate memory for the variable.
     */
    t.pC = (int *)malloc(sizeof(int));
    /* After that dereference the pointer with the * operator and subsequently
     * assign a value of 2048.
     */
    *(t.pC) = 2048;
    /* Assign an int value (4 byte) to a memory location where a pointer which
     * has no type (void *) is pointing to. Again allocate memory for the
     * varible.
     */
    t.pD = malloc(sizeof(int));
    /* Cast the void pointer to an int pointer and dereference the int pointer
     * to an int and assign the value of 0x11223344.
     */
    *((int *)(t.pD)) = 0x11223344;
    /* Assign a constant string to the char pointer. */
    t.str1 = "Das ist ein Text.";
    /* Fill the first char array with chars. End the string with zero.
     * Everything after the zero will not be printed by printf.
     */
    t.charArr1[0] = 'H';
    t.charArr1[1] = 'a';
    t.charArr1[2] = 'l';
    t.charArr1[3] = 'l';
    t.charArr1[4] = 'o';
    t.charArr1[5] = '\0';
    t.charArr1[6] = 'X';
    t.charArr1[7] = 'Y';
    /* Fill some numbers to second char array. */
    t.charArr2[0] = 0x57;
    t.charArr2[1] = 0x65;
    t.charArr2[2] = 0x6c;
    t.charArr2[3] = 0x74;
    t.charArr2[4] = 0;
    /* Fill some other numbers to short array */
    t.array[0] = 12;
    t.array[1] = 12 * 16;
    t.array[2] = 12 * 16 * 16;
    t.array[3] = 12 * 16 * 16 * 16;

    /*** PRINT SIZE OF SOME DATATYPES ***/
    printf("Print size of some datatypes:\n");
    printf("sizeof(void *): %lu (Pointer)\n", sizeof(void *));
    printf("sizeof(char): %lu\n", sizeof(char));
    printf("sizeof(short): %lu\n", sizeof(short));
    printf("sizeof(int): %lu\n", sizeof(int));

    /*** MEMORY DUMP OF THE STRUCTURE ***/
    printf("\nMemory dump of the structure: sizeof: %lu\n", sizeof(t));
    p = (unsigned char *)pT;
    for(i = 0; i < sizeof(t); i++)
    {
        printf("%p 0x%02x %c\n", p+i, p[i], p[i]);
    }

    /*** PRINT SOME VARIABLES OF THE STRUCTURE ***/
    printf("\nPrint some variables of the structure:\n");
    printf("%p t.a: %u 0x%x\n", &(t.a), t.a, t.a);
    printf("%p t.b: %u 0x%x\n", &(t.b), t.b, t.b);
    printf("%p t.pC: %p -> %i\n", &(t.pC), t.pC, *(t.pC));
    p = (unsigned char *)(t.pC);
    for(i = 0; i < sizeof(int); i++)
    {
        printf("\t%p 0x%02x\n", p + i, p[i]);
    }
    printf("%p t.pD: %p -> %i\n", &(t.pD), t.pD, *((int *)(t.pD)));
    p = (unsigned char *)(t.pD);
    for(i = 0; i < sizeof(int); i++)
    {
        printf("\t%p 0x%02x\n", p + i, p[i]);
    }
    printf("%p t.str1: %p -> %s\n", &(t.str1), t.str1, t.str1);
    printf("%p t.charArr1: %s\n", t.charArr1, t.charArr1);
    for(i = 0; i < ARRAY_LEN(t.charArr1); i++)
    {
        printf("%p t.charArr1[%i]: %c\n", &(t.charArr1[i]), i, t.charArr1[i]);
    }
    printf("%p t.charArr2: %s\n", t.charArr2, t.charArr2);
    for(i = 0; i < ARRAY_LEN(t.charArr2); i++)
    {
        printf("%p t.charArr2[%i]: %c\n", &(t.charArr2[i]), i, t.charArr2[i]);
    }
    for(i = 0; i < ARRAY_LEN(t.array); i++)
    {
        printf("%p t.array[%i]: 0x%04x %i\n", t.array + i, i,
                (unsigned short)t.array[i], t.array[i]);
    }

    return EXIT_SUCCESS;
}
