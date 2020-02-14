#include "validation.h"

#define LAB_VALIDATED(level,num) \
	printf("Lab %d.%d validated (%lu bytes)\n", level, num, labSize##level##_##num); \
        submit(SERVER_IP, PORT, YOURNAME, level, num, labSize##level##_##num);

#define ASM __asm__ __volatile__
void failLab(const char *fmt, ...)
{
    char szData[4096] = "";
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(szData, sizeof(szData), fmt, ap);
    printf("%s\nHit a key to exit.\n",szData);
    va_end(ap);
    fgetc(stdin);
    exit(-1);
}

void validate_lab1_1()
{
    unsigned long a = 0, b = 0;

    ASM("pushad;\n\t"
        "mov eax, 1;\n\t"
        "mov ebx, 2;\n\t"
        "call lab1_1;\n\t"
        "mov %[a], eax;\n\t"
        "mov %[b], ebx;\n\t"
        "popad;\n\t"
        : [a]"=m"(a), [b]"=m"(b)
        ::);
    if( a!=2 || b!=1 )
    {
        failLab("Lab 1.1 failed! \n"
                "Should have: EAX:0x%08X EBX:0x%08X\n"
                "Did have:    EAX:0x%08X EBX:0x%08X\n",
                2, 1, a, b );
    }
    LAB_VALIDATED(1,1);
}

#define EAX 0x123
#define EBX 0x234
#define ECX 0x345
#define EDX 0x456

void validate_lab1_2()
{
    unsigned long a = 0, b = 0, c = 0, d = 0;
    ASM("pushad;\n\t"
        "mov eax, %[eax];\n\t"
        "mov ebx, %[ebx];\n\t"
        "mov ecx, %[ecx];\n\t"
        "mov edx, %[edx];\n\t"
        "call lab1_2;\n\t"
        "mov %[a], eax;\n\t"
        "mov %[b], ebx;\n\t"
        "mov %[c], ecx;\n\t"
        "mov %[d], edx;\n\t"
        "popad;\n\t"
        : [a]"=m"(a), [b]"=m"(b), [c]"=m"(c), [d]"=m"(d)
        : [eax]"i"(EAX), [ebx]"i"(EBX), [ecx]"i"(ECX), [edx]"i"(EDX)
        :);
    if (a != (EBX + ECX + 0x42) || b != EBX || c != ECX || d != EDX)
    {
        failLab("Lab 1.2 failed!\n"
                "Should have: EAX:0x%08X EBX:0x%08X ECX:0x%08X EDX:0x%08X\n"
                "Did have:    EAX:0x%08X EBX:0x%08X ECX:0x%08X EDX:0x%08X\n",
                EBX + ECX + 0x42, EBX, ECX, EDX, a, b, c, d);
    }
    LAB_VALIDATED(1,2);
}

void validate_lab1_3()
{
    unsigned long a = 0, b = 0, c = 0, d = 0;
    ASM("pushad;\n\t"
        "mov eax, %[eax];\n\t"
        "mov ebx, %[ebx];\n\t"
        "mov ecx, %[ecx];\n\t"
        "mov edx, %[edx];\n\t"
        "call lab1_3;\n\t"
        "mov %[a], eax;\n\t"
        "mov %[b], ebx;\n\t"
        "mov %[c], ecx;\n\t"
        "mov %[d], edx;\n\t"
        "popad;\n\t"
        : [a]"=m"(a), [b]"=m"(b), [c]"=m"(c), [d]"=m"(d)
        : [eax]"i"(EAX), [ebx]"i"(EBX), [ecx]"i"(ECX), [edx]"i"(EDX)
        :);
	if(a != EBX || b != EAX || c != ECX|| d != EDX)
	{
            failLab("Lab 1.3 failed! \n"
                    "Should have: EAX:0x%08X EBX:0x%08X ECX:0x%08X EDX:0x%08X\n"
                    "Did have:    EAX:0x%08X EBX:0x%08X ECX:0x%08X EDX:0x%08X\n",
                    EBX, EAX, ECX, EDX, a, b, c, d);
	}
	LAB_VALIDATED(1,3);
}


void validate_lab2_1()
{
    unsigned char *bp = (unsigned char*)lab2_1;

    // Check for a leading int3, skip it if present
    if( 0xCC == *(unsigned char*)lab2_1)
    {
        bp = bp + 1;
    }

    // Checks for long jump (0xE9 op code) or short jump (0xEB op
    // code) with value of -(sizeof(jump)). These would translate to a
    // single-jmp infinite loop.
    unsigned char longJmp[] = { 0xE9, 0xFB, 0XFF, 0XFF, 0XFF };
    unsigned char shortJmp[] = { 0xEB, 0xFE };
    if(0!= memcmp(longJmp, bp, sizeof(longJmp)) &&
       0!= memcmp(shortJmp, bp, sizeof(shortJmp)))
    {
        failLab("Lab 2.1 failed! \n"
                "You do not appear to have a single jmp infinite loop.\n"
                " Perhaps you have extra instructions (e.g.: int3) in front of the jmp?\n");
    }
    LAB_VALIDATED(2,1);
}

#undef EAX
#define EAX 42

void validate_lab2_2()
{
    unsigned long a = 0, b = 0, c = 0, d = 0;
    ASM("pushad;\n\t"
        "mov eax, %[eax];\n\t"
        "mov ebx, %[ebx];\n\t"
        "mov ecx, %[ecx];\n\t"
        "mov edx, %[edx];\n\t"
        "call lab2_2;\n\t"
        "mov %[a], eax;\n\t"
        "mov %[b], ebx;\n\t"
        "mov %[c], ecx;\n\t"
        "mov %[d], edx;\n\t"
        "popad;\n\t"
        : [a]"=m"(a), [b]"=m"(b), [c]"=m"(c), [d]"=m"(d)
        : [eax]"i"(EAX), [ebx]"i"(EBX), [ecx]"i"(ECX), [edx]"i"(EDX)
        :
        );
    if(a != EAX || b != 1 || c != ECX || d != 2)
    {
        failLab("Lab 2.2 failed! \n"
                "Should have: EAX:0x%08X EBX:0x%08X ECX:0x%08X EDX:0x%08X\n"
                "Did have:    EAX:0x%08X EBX:0x%08X ECX:0x%08X EDX:0x%08X\n",
                EAX, 1, ECX, 2, a, b, c, d);
    }
    LAB_VALIDATED(2,2);
}

#undef EAX
#define EAX 1005

void validate_lab2_3()
{
    unsigned long a = 0, b = 0, c = 0, d = 0;
    ASM("pushad;\n\t"
        "mov eax, %[eax];\n\t"
        "mov ebx, %[ebx];\n\t"
        "mov ecx, %[ecx];\n\t"
        "mov edx, %[edx];\n\t"
        "call lab2_3;\n\t"
        "mov %[a], eax;\n\t"
        "mov %[b], ebx;\n\t"
        "mov %[c], ecx;\n\t"
        "mov %[d], edx;\n\t"
        "popad;\n\t"
        : [a]"=m"(a), [b]"=m"(b), [c]"=m"(c), [d]"=m"(d)
        : [eax]"i"(EAX), [ebx]"i"(EBX), [ecx]"i"(ECX), [edx]"i"(EDX)
        :
    );
    if(a != EAX || b != EBX || c != EAX || d != EDX)
    {
        failLab("Lab 2.3 failed! \n"
                "Should have: EAX:0x%08X EBX:0x%08X ECX:0x%08X EDX:0x%08X\n"
                "Did have:    EAX:0x%08X EBX:0x%08X ECX:0x%08X EDX:0x%08X\n",
                EAX, EBX, ECX, EDX, a, b, c, d);
    }
    LAB_VALIDATED(2,3);
}


void validate_lab2_4()
{
    unsigned long a = 0, b = 0, c = 0, d = 0;
    ASM("pushad;\n\t"
        "mov eax, %[eax];\n\t"
        "mov ebx, %[ebx];\n\t"
        "mov ecx, %[ecx];\n\t"
        "mov edx, %[edx];\n\t"
        "call lab2_4;\n\t"
        "mov %[a], eax;\n\t"
        "mov %[b], ebx;\n\t"
        "mov %[c], ecx;\n\t"
        "mov %[d], edx;\n\t"
        "popad;\n\t"
        : [a]"=m"(a), [b]"=m"(b), [c]"=m"(c), [d]"=m"(d)
        : [eax]"i"(EAX), [ebx]"i"(EBX), [ecx]"i"(ECX), [edx]"i"(EDX)
        :
    );
    if(a != 16983 || b != EBX || d != EDX)
    {
        failLab("Lab 2.4 failed! \n"
                "Should have: EAX:0x%08X EBX:0x%08X EDX:0x%08X\n"
                "Did have:    EAX:0x%08X EBX:0x%08X EDX:0x%08X\n",
                16983, EBX, EDX, a, b, d);
    }
    LAB_VALIDATED(2,4);
}


#undef EAX
#define EAX 0x87654321


void validate_lab3_1()
{
    unsigned long a = 0, b = 0, c = 0, d = 0;
    ASM("pushad;\n\t"
        "mov eax, %[eax];\n\t"
        "mov ebx, %[ebx];\n\t"
        "mov ecx, %[ecx];\n\t"
        "mov edx, %[edx];\n\t"
        "call lab3_1;\n\t"
        "mov %[a], eax;\n\t"
        "mov %[b], ebx;\n\t"
        "mov %[c], ecx;\n\t"
        "mov %[d], edx;\n\t"
        "popad;\n\t"
        : [a]"=m"(a), [b]"=m"(b), [c]"=m"(c), [d]"=m"(d)
        : [eax]"i"(EAX), [ebx]"i"(EBX), [ecx]"i"(ECX), [edx]"i"(EDX)
        :
    );
    if( a!=0x87652143 || b!=EBX || c!=ECX || d!=EDX )
    {
            failLab("Lab 3.1 failed! \n"
                    "Should have: EAX:0x%08X EBX:0x%08X ECX:0x%08X EDX:0x%08X\n"
                    "Did have:    EAX:0x%08X EBX:0x%08X ECX:0x%08X EDX:0x%08X\n",
                    0x87652143, EBX, ECX, EDX,
                    a, b, c, d);
    }
    LAB_VALIDATED(3,1);
}


void validate_lab3_2()
{
    unsigned long a = 0, b = 0, c = 0, d = 0;
    ASM("pushad;\n\t"
        "mov eax, %[eax];\n\t"
        "mov ebx, %[ebx];\n\t"
        "mov ecx, %[ecx];\n\t"
        "mov edx, %[edx];\n\t"
        "call lab3_2;\n\t"
        "mov %[a], eax;\n\t"
        "mov %[b], ebx;\n\t"
        "mov %[c], ecx;\n\t"
        "mov %[d], edx;\n\t"
        "popad;\n\t"
        : [a]"=m"(a), [b]"=m"(b), [c]"=m"(c), [d]"=m"(d)
        : [eax]"i"(EAX), [ebx]"i"(EBX), [ecx]"i"(ECX), [edx]"i"(EDX)
        :
    );
    if( a!=EAX || b!=EBX || c!=ECX || (d&0xFF)!=0x87)
    {
            failLab("Lab 3.2 failed! \n"
                    "Should have: EAX:0x%08X EBX:0x%08X ECX:0x%08X EDX:0x??????87\n"
                    "Did have:    EAX:0x%08X EBX:0x%08X ECX:0x%08X EDX:0x%08X\n",
                    EAX, EBX, ECX, a, b, c, d);
    }
    LAB_VALIDATED(3,2);
}


void validate_lab3_3()
{
    unsigned long a = 0, b = 0, c = 0, d = 0;
    ASM("pushad;\n\t"
        "mov eax, %[eax];\n\t"
        "mov ebx, %[ebx];\n\t"
        "mov ecx, %[ecx];\n\t"
        "mov edx, %[edx];\n\t"
        "call lab3_3;\n\t"
        "mov %[a], eax;\n\t"
        "mov %[b], ebx;\n\t"
        "mov %[c], ecx;\n\t"
        "mov %[d], edx;\n\t"
        "popad;\n\t"
        : [a]"=m"(a), [b]"=m"(b), [c]"=m"(c), [d]"=m"(d)
        : [eax]"i"(EAX), [ebx]"i"(EBX), [ecx]"i"(ECX), [edx]"i"(EDX)
        :
    );
    if( a!=0x43 || b!=EBX || c!=ECX || d!=EDX )
    {
        failLab("Lab 3.3 failed! \n"
                "Should have: EAX:0x%08X EBX:0x%08X ECX:0x%08X EDX:0x%08X\n"
                "Did have:    EAX:0x%08X EBX:0x%08X ECX:0x%08X EDX:0x%08X\n",
                0x43, EBX, ECX, EDX, a, b, c, d);
    }
    LAB_VALIDATED(3,3);
}


void validate_lab3_4()
{
    unsigned long a = 0, b = 0, c = 0, d = 0;
    ASM("pushad;\n\t"
        "mov eax, %[eax];\n\t"
        "mov ebx, %[ebx];\n\t"
        "mov ecx, %[ecx];\n\t"
        "mov edx, %[edx];\n\t"
        "call lab3_4;\n\t"
        "mov %[a], eax;\n\t"
        "mov %[b], ebx;\n\t"
        "mov %[c], ecx;\n\t"
        "mov %[d], edx;\n\t"
        "popad;\n\t"
        : [a]"=m"(a), [b]"=m"(b), [c]"=m"(c), [d]"=m"(d)
        : [eax]"i"(EAX), [ebx]"i"(EBX), [ecx]"i"(ECX), [edx]"i"(EDX)
        :
    );
    if( a!=(EAX/64) || b!=EBX || c!=ECX || d!=EDX )
    {
        failLab("Lab 3.4 failed! \n"
                "Should have: EAX:0x%08X EBX:0x%08X ECX:0x%08X EDX:0x%08X\n"
                "Did have:    EAX:0x%08X EBX:0x%08X ECX:0x%08X EDX:0x%08X\n",
                (EAX/64), EBX, ECX, EDX, a, b, c, d);
    }
    LAB_VALIDATED(3,4);
}


void validate_lab3_5()
{
    unsigned long a = 0, b = 0, c = 0, d = 0;
    ASM("pushad;\n\t"
        "mov eax, %[eax];\n\t"
        "mov ebx, %[ebx];\n\t"
        "mov ecx, %[ecx];\n\t"
        "mov edx, %[edx];\n\t"
        "call lab3_5;\n\t"
        "mov %[a], eax;\n\t"
        "mov %[b], ebx;\n\t"
        "mov %[c], ecx;\n\t"
        "mov %[d], edx;\n\t"
        "popad;\n\t"
        : [a]"=m"(a), [b]"=m"(b), [c]"=m"(c), [d]"=m"(d)
        : [eax]"i"(EAX), [ebx]"i"(EBX), [ecx]"i"(ECX), [edx]"i"(EDX)
        :
    );
    if(a != 0x21436587 || c != ECX || d != EDX)
    {
        failLab("Lab 3.5 failed! \n"
                "Should have: EAX:0x%08X EBX:0x%08X ECX:0x%08X EDX:0x%08X\n"
                "Did have:    EAX:0x%08X EBX:0x%08X ECX:0x%08X EDX:0x%08X\n",
                0x21436587, EBX, ECX, EDX, a, b, c, d);
    }
    LAB_VALIDATED(3,5);
}


unsigned long tmp;
unsigned long *pTmp = &tmp;

void validate_lab4_1()
{
    unsigned long a = 0, b = 0, c = 0, d = 0;

    ASM("pushad;\n\t"
        "mov eax, %[eax];\n\t"
        "mov ebx, %[ebx];\n\t"
        "mov ecx, %[p];\n\t"
        "mov edx, %[edx];\n\t"
        "call lab4_1;\n\t"
        "mov %[a], eax;\n\t"
        "mov %[b], ebx;\n\t"
        "mov %[c], ecx;\n\t"
        "mov %[d], edx;\n\t"
        "popad;\n\t"
        : [a]"=m"(a), [b]"=m"(b), [c]"=m"(c), [d]"=m"(d)
        : [eax]"i"(EAX), [ebx]"i"(EBX), [p]"m"(pTmp), [edx]"i"(EDX)
        :
    );

    if( 0x93 == *(unsigned char*)lab4_1 )
        failLab("Lab 4.1 failed! You are not supposed to use XCHG!\n");

    if( a != EBX || b != EAX || c != (unsigned long)pTmp || d != EDX )
    {
        failLab("Lab 4.1 failed! \n"
                "Should have: EAX:0x%08X EBX:0x%08X ECX:0x%08X EDX:0x%08X\n"
                "Did have:    EAX:0x%08X EBX:0x%08X ECX:0x%08X EDX:0x%08X\n",
                EBX, EAX, pTmp, EDX, a, b, c, d);
    }
    LAB_VALIDATED(4,1);
}


void validate_lab4_2()
{
    unsigned long ma = 0, mb = 0x12345678;
    unsigned long *pma = &ma;
    unsigned long *pmb = &mb;
    unsigned long a = 0, b = 0, c = 0, d = 0;
    ASM("pushad;\n\t"
        "mov eax, %[pma];\n\t"
        "mov ebx, %[pmb];\n\t"
        "mov ecx, %[ecx];\n\t"
        "mov edx, %[edx];\n\t"
        "call lab4_2;\n\t"
        "mov %[a], eax;\n\t"
        "mov %[b], ebx;\n\t"
        "mov %[c], ecx;\n\t"
        "mov %[d], edx;\n\t"
        "popad;\n\t"
        : [a]"=m"(a), [b]"=m"(b), [c]"=m"(c), [d]"=m"(d)
        : [pma]"m"(pma), [pmb]"m"(pmb), [ecx]"i"(ECX), [edx]"i"(EDX)
        :
    );

    if( mb!=0x12345678 )
            failLab("Lab 4.2 failed! You modified memory pointed to be EBX!\n");
    if( 0x78 != ma )
            failLab("Lab 4.2 failed! DWORD pointed to by EAX is 0x%08X, should be 0x00000078!\n", ma);
    if( a != (unsigned long)pma || c != ECX || d != EDX )
    {
        failLab("Lab 4.2 failed! \n"
                "Should have: EAX:0x%08X ECX:0x%08X EDX:0x%08X\n"
                "Did have:    EAX:0x%08X ECX:0x%08X EDX:0x%08X\n",
                pma, ECX, EDX, a, c, d);
    }
    LAB_VALIDATED(4,2);
}

void validate_lab4_3()
{
    unsigned long a = 0, b = 0, c = 0, d = 0;
    ASM("pushad;\n\t"
        "mov eax, %[eax];\n\t"
        "mov ebx, %[ebx];\n\t"
        "mov ecx, %[ecx];\n\t"
        "mov edx, %[edx];\n\t"
        "call lab4_3;\n\t"
        "mov %[a], eax;\n\t"
        "mov %[b], ebx;\n\t"
        "mov %[c], ecx;\n\t"
        "mov %[d], edx;\n\t"
        "popad;\n\t"
        : [a]"=m"(a), [b]"=m"(b), [c]"=m"(c), [d]"=m"(d)
        : [eax]"i"(EAX), [ebx]"i"(EBX), [ecx]"i"(ECX), [edx]"i"(EDX)
        :
    );

    if( a!=(EBX+ECX) || b!=EBX || c!=ECX || d!=EDX )
    {
            failLab("Lab 4.3 failed! \n"
                    "Should have: EAX:0x%08X EBX:0x%08X ECX:0x%08X EDX:0x%08X\n"
                    "Did have:    EAX:0x%08X EBX:0x%08X ECX:0x%08X EDX:0x%08X\n",
                    (EBX+ECX), EBX, ECX, EDX,
                    a, b, c, d);
    }
    if( labSize4_3 >3 )
            failLab("Lab 4.3 failed, your code shouldn't be more than 3 bytes\n"
            "   (If its only one instruction and >3 bytes, tell the instructor!)\n");

    if( labSize4_3 <3 )
            failLab("Lab 4.3: WHOAH! Tell the instructor!\n");

    LAB_VALIDATED(4,3);
}


void validate_lab4_4()
{
    unsigned char buffer[1024];
    unsigned char *pb = buffer;

    memset(&buffer, 0xCC, sizeof(buffer));

    unsigned long a = 0, b = 0, c = 0, d = 0;
    ASM("pushad;\n\t"
        "mov eax, %[pb];\n\t"
        "mov ebx, %[ebx];\n\t"
        "mov ecx, %[ecx];\n\t"
        "mov edx, %[edx];\n\t"
        "call lab4_4;\n\t"
        "mov %[a], eax;\n\t"
        "mov %[b], ebx;\n\t"
        "mov %[c], ecx;\n\t"
        "mov %[d], edx;\n\t"
        "popad;\n\t"
        : [a]"=m"(a), [b]"=m"(b), [c]"=m"(c), [d]"=m"(d)
        : [pb]"m"(pb), [ebx]"i"(EBX), [ecx]"i"(ECX), [edx]"i"(EDX)
        :
    );
    if(a != (unsigned long)buffer || b != EBX || d != EDX)
    {
        failLab("Lab 4.4 failed! \n"
                "Should have: EAX:0x%08X EBX:0x%08X EDX:0x%08X\n"
                "Did have:    EAX:0x%08X EBX:0x%08X EDX:0x%08X\n",
                buffer, EBX, EDX, a, b, d);
    }

    unsigned int i;
    for(i = 0; i < sizeof(buffer); i++)
    {
        if(i <= 64)
        {
            if(buffer[i] != i)
            {
                failLab("Lab 4.4 failed!\n"
                        "Buffer pointed to by EAX+%d should be %d (0x%02X) but is %d (0x%02X)%s\n",
                        i, i, i, buffer[i], buffer[i],
                        buffer[i] == 0xCC ? " (probably uninitialized)" : "");
            }
        }
        else if(0xCC != buffer[i])
        {
            failLab("Lab 4.4 failed! You wrote past the buffer!\n");
        }
    }
    LAB_VALIDATED(4,4);
}


void validate_lab4_5_string(const char *sz)
{
    char orig[1024];
    char buffer[1024];
    char *pb = buffer;
    memset(buffer, 0xCC, sizeof(buffer));
    strncpy(orig, sz, sizeof(buffer));

    unsigned long a = 0, b = 0;
    ASM("pushad;\n\t"
        "mov eax, %[sz];\n\t"
        "mov ebx, %[pb];\n\t"
        "call lab4_5;\n\t"
        "mov %[a], eax;\n\t"
        "mov %[b], ebx;\n\t"
        "popad;\n\t"
        : [a]"=m"(a), [b]"=m"(b)
        : [pb]"m"(pb), [sz]"m"(sz)
        :
    );

    int actualLen = strlen(sz);
    int givenLen = (int)(*(unsigned short*)buffer);


    if(a != (unsigned long)sz || b != (unsigned long)pb)
    {
        failLab("Lab 4.5 failed! \n"
                "Should have: EAX:0x%08X EBX:0x%08X\n"
                "Did have:    EAX:0x%08X EBX:0x%08X\n",
                sz, pb, a, b);
    }

    if( *(unsigned short *)buffer == 0xCCCC)
        failLab("Lab 4.5 failed! You did not store length!\n");

    if(givenLen != actualLen)
    {
        failLab("Lab 4.5 failed! You reported string length as %d(0x%02X), it's actually %d(0x%02X)\n",
                    givenLen, givenLen, actualLen, actualLen);
    }

    if(0 != memcmp(buffer+2, sz, actualLen))
    {
        failLab("Lab 4.5 failed! Strings do not match!\n");
    }

    if(!buffer[2 + actualLen])
    {
        failLab("Lab 4.5 failed! You copied the NULL!\n");
    }

    if(0 != strcmp(orig, sz))
    {
        failLab("Lab 4.5 failed! You modified the original string!\n");
    }
}

void validate_lab4_5()
{
    validate_lab4_5_string("Hello");
    validate_lab4_5_string("");
    validate_lab4_5_string("G");
    validate_lab4_5_string("Little man quit poking me");
    LAB_VALIDATED(4,5);
}


void validate_lab5_1()
{
    unsigned long a = 0, b = 0, c = 0, d = 0;
    ASM("pushad;\n\t"
        "mov eax, %[eax];\n\t"
        "mov ebx, %[ebx];\n\t"
        "mov ecx, %[ecx];\n\t"
        "mov edx, %[edx];\n\t"
        "call lab5_1;\n\t"
        "mov %[a], eax;\n\t"
        "mov %[b], ebx;\n\t"
        "mov %[c], ecx;\n\t"
        "mov %[d], edx;\n\t"
        "popad;\n\t"
        : [a]"=m"(a), [b]"=m"(b), [c]"=m"(c), [d]"=m"(d)
        : [eax]"i"(EAX), [ebx]"i"(EBX), [ecx]"i"(ECX), [edx]"i"(EDX)
        :
    );

    if(a != EBX || b != EAX || c != ECX || d != EDX )
    {
        failLab("Lab 5.1 failed! \n"
                "Should have: EAX:0x%08X EBX:0x%08X ECX:0x%08X EDX:0x%08X\n"
                "Did have:    EAX:0x%08X EBX:0x%08X ECX:0x%08X EDX:0x%08X\n",
                EBX, EAX, ECX, EDX, a, b, c, d);
    }
    if(4 != labSize5_1)
    {
        failLab("Lab 5.1 failed! \n"
         "... er... I'm not sure what you did, because this lab should have been 4 bytes...\n");
    }
    LAB_VALIDATED(5,1);
}


void validate_lab5_2()
{
    unsigned long a = 0, b = 0, c = 0, d = 0;
    ASM("pushad;\n\t"
        "mov eax, %[eax];\n\t"
        "mov ebx, %[ebx];\n\t"
        "mov ecx, %[ecx];\n\t"
        "mov edx, %[edx];\n\t"
        "call lab5_1;\n\t"
        "mov %[a], eax;\n\t"
        "mov %[b], ebx;\n\t"
        "mov %[c], ecx;\n\t"
        "mov %[d], edx;\n\t"
        "popad;\n\t"
        : [a]"=m"(a), [b]"=m"(b), [c]"=m"(c), [d]"=m"(d)
        : [eax]"i"(EAX), [ebx]"i"(EBX), [ecx]"i"(ECX), [edx]"i"(EDX)
        :
    );
    if(a != EBX || b != EAX || c != ECX || d != EDX )
    {
        failLab("Lab 5.2 failed! \n"
                "Should have: EAX:0x%08X EBX:0x%08X ECX:0x%08X EDX:0x%08X\n"
                "Did have:    EAX:0x%08X EBX:0x%08X ECX:0x%08X EDX:0x%08X\n",
                EBX, EAX, ECX, EDX, a, b, c, d);
    }
    if(9 != labSize5_2)
    {
        failLab("Lab 5.2 failed! \n"
        "... er... I'm not sure what you did, because this lab should have been 9 bytes...\n");
    }
    LAB_VALIDATED(5,2);
}

void validate_lab5_3()
{
    unsigned long a = 0, b = 0, c = 0, d = 0;
    ASM("pushad;\n\t"
        "mov eax, %[eax];\n\t"
        "mov ebx, %[ebx];\n\t"
        "mov ecx, %[ecx];\n\t"
        "mov edx, %[edx];\n\t"
        "jmp Here5_3;\n\t"
"There5_3:;\n\t"
        "sub esp, 0x800;\n\t"
        "jmp lab5_3;\n\t"
"Here5_3:;\n\t"
        "call There5_3;\n\t"
        "mov %[a], eax;\n\t"
        "mov %[b], ebx;\n\t"
        "mov %[c], ecx;\n\t"
        "mov %[d], edx;\n\t"
        "popad;\n\t"
        : [a]"=m"(a), [b]"=m"(b), [c]"=m"(c), [d]"=m"(d)
        : [eax]"i"(EAX), [ebx]"i"(EBX), [ecx]"i"(ECX), [edx]"i"(EDX)
        :
    );
    if(a != EAX || b != EBX || c != ECX || d != EDX)
    {
        failLab("Lab 5.3 failed! \n"
                "Should have: EAX:0x%08X EBX:0x%08X ECX:0x%08X EDX:0x%08X\n"
                "Did have:    EAX:0x%08X EBX:0x%08X ECX:0x%08X EDX:0x%08X\n",
                EAX, EBX, ECX, EDX, a, b, c, d);
    }
    LAB_VALIDATED(5,3);
}


void validate_lab5_4_string(const char *sz)
{
    unsigned char buffer[1024];
    memset(buffer, 0xCC, sizeof(buffer));
    unsigned long pb = (unsigned long)buffer;

    unsigned long a = 0, b = 0, c = 0, d = 0;
    ASM("pushad;\n\t"
        "mov eax, %[sz];\n\t"
        "mov ebx, %[pb];\n\t"
        "mov ecx, %[ecx];\n\t"
        "mov edx, %[edx];\n\t"
        "call lab5_4;\n\t"
        "mov %[a], eax;\n\t"
        "mov %[b], ebx;\n\t"
        "mov %[c], ecx;\n\t"
        "mov %[d], edx;\n\t"
        "popad;\n\t"
        : [a]"=m"(a), [b]"=m"(b), [c]"=m"(c), [d]"=m"(d)
        : [pb]"m"(pb), [sz]"m"(sz), [ecx]"i"(ECX), [edx]"i"(EDX)
        :
    );
    int actualLen = strlen(sz);

    if(a != (unsigned long)actualLen || b != pb|| c != ECX || d != EDX)
    {

        failLab("Lab 5.4 failed! \n"
                "Should have: EAX:0x%08X EBX:0x%08X ECX:0x%08X EDX:0x%08X\n"
                "Did have:    EAX:0x%08X EBX:0x%08X ECX:0x%08X EDX:0x%08X\n",
                actualLen, pb, ECX, EDX, a, b, c, d);
    }
    if(actualLen && 0 != memcmp(sz, buffer, actualLen+1))
    {
        failLab("Lab 5.4 failed! Didn't copy string right!\n");
    }

    if(*((unsigned char*)buffer + actualLen+2) != 0xcc)
    {
        failLab("Lab 5.4 failed! You wrote past the end of the string\n");
    }
}

void validate_lab5_4()
{
    validate_lab5_4_string("Hello");
    validate_lab5_4_string("");
    validate_lab5_4_string("G");
    validate_lab5_4_string("Little man quit poking");
    LAB_VALIDATED(5,4);
}

struct PUSHAD_DATA
{
    unsigned long edi;
    unsigned long esi;
    unsigned long ebp;
    unsigned long esp;
    unsigned long ebx;
    unsigned long edx;
    unsigned long ecx;
    unsigned long eax;

    operator unsigned long*() const { return (unsigned long*)this; }
};

enum GenReg //pushad ordering
{
    GR_EDI,
    GR_ESI,
    GR_EBP,
    GR_ESP,
    GR_EBX,
    GR_EDX,
    GR_ECX,
    GR_EAX,
    COUNT_GenReg
};


const char *GenRegNames[COUNT_GenReg] =
{
    "EDI",
    "ESI",
    "EBP",
    "ESP",
    "EBX",
    "EDX",
    "ECX",
    "EAX",
};

#define GRF_EDI (1<<GR_EDI)
#define GRF_ESI (1<<GR_ESI)
#define GRF_EBP (1<<GR_EBP)
#define GRF_ESP (1<<GR_ESP)
#define GRF_EBX (1<<GR_EBX)
#define GRF_EDX (1<<GR_EDX)
#define GRF_ECX (1<<GR_ECX)
#define GRF_EAX (1<<GR_EAX)

PUSHAD_DATA grDefault = { 0x900, 0x800, 0x555, 0, 0x200, 0x400, 0x300, 0x100 };

unsigned long GR_FLAGS_ALL = 0xFF;
unsigned long GR_FLAGS_NONE = 0;
unsigned long GR_FLAGS_4 =   GRF_EAX | GRF_EBX | GRF_ECX | GRF_EDX ;
unsigned long GR_FLAGS_PRES =   GRF_EBX | GRF_ESI | GRF_EDI | GRF_EBP ;

//_declspec(naked)
__attribute((naked))
void __attribute__((stdcall)) getGenReg(void *pushad_data)
{
    ASM("pushad;\n\t"
        "mov edi, [esp+0x24];\n\t"
        "mov esi, esp;\n\t"
        "mov ecx, 0x20;\n\t"
        "rep movsb;\n\t"
        "mov edi, [esp+0x24];\n\t"
        "add dword ptr [edi+4*%[GR_ESP]], 8;\n\t"
        "popad;\n\t"
        "ret 4;\n\t"
        :
        :[GR_ESP]"i"(GR_ESP)
        :
    );
}

//_declspec(naked)
__attribute__((naked))
void __attribute__((stdcall)) setGenReg(void *pushad_data, unsigned long grInFlags=GR_FLAGS_ALL)
{
    ASM("pushad;\n\t"
        "mov esi, [esp+0x24];\n\t"
        "mov eax, [esp+0x28];\n\t"
        "xor ecx, ecx;\n\t"
"SetRegLoop:\n\t"
        "test eax, 1;\n\t"
        "jz SetRegSkip;\n\t"
        "mov edx, [esi + ecx*4];\n\t"
        "mov [esp + ecx*4], edx;\n\t"

"SetRegSkip:\n\t"
        "shr eax, 1;\n\t"
        "inc ecx;\n\t"
        "cmp ecx, 8;\n\t"
        "jl SetRegLoop;\n\t"

        "popad;\n\t"
        "ret 8;\n\t"
        :
        :
        :
    );
}

/*

[stackbytes]
[ret]
[stackbytes]
[labWrap return]
[lab_func]
*/


PUSHAD_DATA g_grOut;
const PUSHAD_DATA *g_pgrOut = &g_grOut;
PUSHAD_DATA grSafe;
const PUSHAD_DATA *pgrSafe = &grSafe;
void *wrapReturn;
void *g_func;


//Returns 0 on success, or number of non-matching registers
__attribute__((naked))
void __attribute__((stdcall))
labWrap(const void *func, const PUSHAD_DATA *grIn=0, const unsigned long grInFlags=0)
{
    ASM("pop wrapReturn;\n\t" //save and remove wrap reurn

        //Backup verifier registers
        "push pgrSafe;\n\t"
        "call %P0;\n\t"

        //Save func
        "pop %P2;\n\t"

        //Call setGenReg... this removes last args
        "call %P1;\n\t"

        //Now stack is as it was before call to this function:
        // FIXME: For 8_1 if you do an add esp, 0x4 it gets the args right but the the stack
        // is off by 8 bytes...
        "call [%P2];\n\t"

        "pushfd;\n\t"
        "test dword ptr [esp], 0x400;\n\t"
        "jz DfClear;\n\t"

        // If you hit this, you left the DF flag set!!!
        "int 3;\n\t"
        "int 3;\n\t"
        "int 3;\n\t"
        "int 3;\n\t"
"DfClear:\n\t"
        "add esp, 4;\n\t"
        //Get registers
        "push g_pgrOut;\n\t"
        "call %P0;\n\t"

        //Restore our wrap registers
        "push GR_FLAGS_PRES;\n\t"
        "push pgrSafe;\n\t"
        "call %P1;\n\t"

        //Return to our wrap caller
        "push wrapReturn;\n\t"
        "ret;\n\t"
        :
        :[getGenReg]"i"(getGenReg), [setGenReg]"i"(setGenReg), [g_func]"m"(g_func)
        :);
}


void checkReg(int level, int lab, unsigned long *grIdealOut, unsigned long outFlags=0, unsigned long stackDiff=0)
{
    outFlags &= ~GRF_ESP;

    unsigned long espAfterLab = 0;
    ASM("mov %[espAfterLab], ebp;\n\t"
        :[espAfterLab]"=m"(espAfterLab)
        :
        :);

    espAfterLab += 5*4 +8; //args

    unsigned long espShouldBe = pgrSafe->esp + 0xC + stackDiff;
    //unsigned long espShouldBe = pgrSafe->esp + stackDiff;
    if(espShouldBe != espAfterLab)
    {
        failLab("Lab %d.%d failed!\n"
                "   ESP was incorrect after lab: WAS:0x%08X SHOULD BE:0x%08X\n"
                "   Off by %d bytes.\n",
                level, lab, espAfterLab , espShouldBe ,
                (long)espAfterLab - (long)espShouldBe
                );
    }

    int iWrong = 0;
    for(int i = 0; i < COUNT_GenReg; i++)
    {
        if(!((outFlags >> i) & 1))
        {
            continue;
        }

        if(g_grOut[i] != grIdealOut[i])
        {
            iWrong++;
        }
    }

    if(!iWrong)
    {
        return;
    }

    printf("Lab %d.%d failed!\n", level, lab);

    printf("Should have:");
    for(int i = 0; i < COUNT_GenReg; i++)
    {
        if(!((outFlags >> i) & 1) || g_grOut[i] == grIdealOut[i])
        {
            continue;
        }
        printf(" %s:%08lX", GenRegNames[i], grIdealOut[i]);
    }

    printf("\nDid have   :");
    for(int i = 0; i < COUNT_GenReg; i++)
    {
        if(!((outFlags >> i) & 1) || g_grOut[i] == grIdealOut[i])
        {
            continue;
        }
        printf(" %s:%08lX", GenRegNames[i], g_grOut[i]);
    }

    failLab("");
}

void validate_lab6_1_string(const char *sz)
{
    ASM("push %[sz];\n\t"
        :
        :[sz]"r"(sz)
        :
    );
    labWrap((void*)lab6_1, &grDefault, GR_FLAGS_ALL);
    checkReg(6,1, grDefault, GR_FLAGS_PRES, 4);

    if( g_grOut.eax != strlen(sz) )
    {
        failLab("Lab 6.1 failed! \n"
                " String length of '%s' reported as %d, but should be %d!\n",
                sz, g_grOut.eax, strlen(sz) );
    }
}

void validate_lab6_1()
{
    //ASM("int 3;");
    validate_lab6_1_string("Hello");
    validate_lab6_1_string("");
    validate_lab6_1_string("G");
    validate_lab6_1_string("Hello");
    LAB_VALIDATED(6,1);
}


void validate_lab6_2_inner(unsigned long a, unsigned long b, unsigned long c)
{
    PUSHAD_DATA grInReg = grDefault;
    grInReg.ecx = a;
    grInReg.edx = b;
    ASM("push %[c]":
        :[c]"m"(c)
    :);
    labWrap((void*)lab6_2, &grInReg, GR_FLAGS_ALL);
    checkReg(6,2, grInReg, GR_FLAGS_PRES, 4);

    if(g_grOut.eax != (a + b + c))
    {
        failLab("Lab 6.2 failed! \n"
                " 0x%X + 0x%X + 0x%X  == 0x%X, but you returned 0x%X!\n",
                a, b, c, a+b+c, g_grOut.eax );
    }
}

void validate_lab6_2()
{
    validate_lab6_2_inner(0x300, 0x40, 0x7);
    LAB_VALIDATED(6,2);
}


extern "C"
int __attribute__((stdcall)) my_strlen(char *sz)
{
    int len = strlen(sz);
    sz = (char*)0xBBBBBBBB; //Make sure they don't assume args are re-usable
    return len;
}

void *g_my_malloc_ret =NULL;

extern "C"
void * __attribute__((cdecl)) my_malloc(int size) //allocates heap buffer of specified size
{
    if( size <=0 )
            failLab("Lab 6.3 failed! You tried to allocate %d bytes from _my_malloc!\n", size);

    g_my_malloc_ret = malloc(size);
    memset(g_my_malloc_ret, 0xCC, size);
    size = (int)0xBBBBBBBB;  //Make sure they don't assume args are re-usable
    return g_my_malloc_ret;
}

extern "C"
void __attribute__((fastcall)) my_memcpy(void *dst,  void *src, int size) //copies memory
{
    memcpy(dst, src, size);

    //Make sure they don't assume args are re-usable
    dst = (void*)0xBBBBBBBB;
    src = (void*)0xBBBBBBBB;
    size = (int)0xBBBBBBBB;

}

void validate_lab6_3_string(const char *sz)
{
    ASM("push %[sz]":
        :[sz]"m"(sz)
        :);
    labWrap((void *)lab6_3, &grDefault, GR_FLAGS_ALL);
    checkReg(6,3, grDefault, GR_FLAGS_PRES, 4);

    int actualLen = strlen(sz);
    char *szCopy = (char *)g_grOut.eax;

    if( g_my_malloc_ret != szCopy )
            failLab("Lab 6.3 failed! You did not return the newly allocated string!\n");

    if( szCopy[actualLen]!=0)
            failLab("Lab 6.3 failed! Did you forget to copy the NULL terminator?\n");

    if( 0!=strcmp(szCopy, sz) )
            failLab("Lab 6.3 failed! \n"
                        "  Strings do not match!\n");

    free(szCopy); //free returned string
}

void validate_lab6_3()
{
    validate_lab6_3_string("Hello");
    validate_lab6_3_string("");
    validate_lab6_3_string("G");
    validate_lab6_3_string("Hello");
    LAB_VALIDATED(6,3);
}

void validate_lab6_4_inner(int count)
{
    //STUDENTS: If you are looking at this, please DO NOT *EVER* use
    // inline assembly like this!
    ASM("mov eax, 1;\n\t"
        "mov ecx, %[count];\n\t"
"Loop6_4:\n\t"
        "test ecx, ecx;\n\t"
        "jz Done6_4;\n\t"
        "dec ecx;\n\t"

        "push eax;\n\t"
        "inc eax;\n\t"

        "jmp Loop6_4;\n\t"
"Done6_4:\n\t"
        "push %[count];\n\t"
        :
        :[count]"m"(count)
    :);

    labWrap((void*)lab6_4, &grDefault, GR_FLAGS_ALL);

    unsigned long sum=0;
    for(int i = 0; i < count; i++)
    {
        sum+= (i+1);
    }

    PUSHAD_DATA grOut = grDefault;
    grOut.eax = sum;

    checkReg(6, 4, grOut, GR_FLAGS_PRES | GRF_EAX, 0);

    ASM("mov eax, %[count];\n\t"
        "lea esp, [4+ esp+4*eax];\n\t"
        :
        :[count]"m"(count)
    :);
}

void validate_lab6_4()
{
    validate_lab6_4_inner(0);
    validate_lab6_4_inner(1);
    validate_lab6_4_inner(2);
    validate_lab6_4_inner(30);
    LAB_VALIDATED(6,4);
}

void validate_lab6_5()
{
    lab6_5();
    LAB_VALIDATED(6, 5);
}

void validate_lab7_1()
{
    ASM("push 0x42");
    ASM("push 0x33");
    labWrap((void*)lab7_1, &grDefault, GR_FLAGS_ALL);
    checkReg(7, 1, grDefault, GR_FLAGS_PRES, 8);

    if(g_grOut.eax != 0x42 + 0x33)
    {
        failLab("Lab 7.1 failed! \n"
                "  EAX was 0x%08X, should have been 0x%08X!\n",
                g_grOut.eax, 0x42+0x33 );
    }

    LAB_VALIDATED(7, 1);
}


void validate_lab7_2_string(const char *szA, const char *szB, int maxLength, unsigned int res )
{
    ASM("push %[maxLength];\n\t"
        "push %[szB];\n\t"
        "push %[szA];\n\t"
        :
        :[maxLength]"m"(maxLength), [szB]"m"(szB), [szA]"m"(szA)
    :);

    labWrap((void*)lab7_2, &grDefault, GR_FLAGS_ALL);
    checkReg(7, 2, grDefault, GR_FLAGS_PRES, 0xC);

    if( g_grOut.eax != res )
    {
        failLab("Lab 7.2 failed!\n"
                "   szA: '%s'\n"
                "   szB: '%s'\n"
                "   maxLength: %d\n"
                " Expected Result: %d\n"
                " Your Result:     %d\n",
                szA, szB, maxLength, res, g_grOut.eax );
    }
}

void validate_lab7_2()
{
    validate_lab7_2_string("","",0,0);
    validate_lab7_2_string("","",1,0);
    validate_lab7_2_string("X","",1,1);
    validate_lab7_2_string("X","Y",1,1);
    validate_lab7_2_string("X","Y",0,0);
    validate_lab7_2_string("XX","XY",1,0);
    validate_lab7_2_string("XX","XY",2,1);
    validate_lab7_2_string("XX","XXX",2,0);
    validate_lab7_2_string("XX","XXX",3,1);
    validate_lab7_2_string("XX","XXX",2000,1);
    validate_lab7_2_string("XXX\0Y","XXX\0X",2000,0);
    validate_lab7_2_string("Hello", "Hello, World!", 5, 0);
    validate_lab7_2_string("Hello", "Hello, World!", 6, 1);
    validate_lab7_2_string("Hello, World!", "Hello", 5, 0);
    validate_lab7_2_string("Hello, World!", "Hello", 6, 1);
    LAB_VALIDATED(7, 2);
}

void validate_lab7_3_string(const char *sz)
{
    ASM("push %[sz]"
        :
        :[sz]"m"(sz)
    :);
    labWrap((void*)lab7_3, &grDefault, GR_FLAGS_ALL);
    checkReg(7,3, grDefault, GR_FLAGS_PRES, 4);

    int actualLen = strlen(sz);
    char *szCopy = (char *)g_grOut.eax;

    if(g_my_malloc_ret != szCopy)
    {
        failLab("Lab 7.3 failed! You did not return the newly allocated string!\n");
    }

    if(szCopy[actualLen] != 0)
    {
        failLab("Lab 7.3 failed! Did you forget to copy the NULL terminator?\n");
    }

    if(0 != strcmp(szCopy, sz))
    {
        failLab("Lab 7.3 failed! \n"
                "  Strings do not match!\n");
    }

    free(szCopy); //free returned string
}

void validate_lab7_3()
{
    validate_lab7_3_string("Hello");
    validate_lab7_3_string("");
    validate_lab7_3_string("G");
    validate_lab7_3_string("Hello");
    LAB_VALIDATED(7, 3);
}

void validate_lab8_1_inner(char *sz)
{
    unsigned char buffer[500];
    unsigned char *pbuf = buffer;
    memset(buffer, 0xCC, sizeof(buffer));

    int len = strlen(sz);

    ASM("push %[len];\n\t"
        "push %[sz];\n\t"
        "push %[pbuf];\n\t"
        :
        :[len]"m"(len), [sz]"m"(sz), [pbuf]"m"(pbuf)
    :);

    labWrap((void*)lab8_1, &grDefault, GR_FLAGS_ALL);
    checkReg(8, 1, grDefault, GR_FLAGS_PRES, 0);
    ASM("add esp, 0xC");

    if(0 != memcmp(buffer, sz, len))
    {
        failLab("Lab 8.1 failed! \n"
                "  Memory not copied correctly!\n");
    }
    if(buffer[len] != 0xCC)
    {
        failLab("Lab 8.1 failed! \n"
                "  You copied too much! Overflow!\n");
    }
}

void validate_lab8_1()
{
    char one[256] = "Hello";
    char two[256] = "";
    char three[256] = "X";
    char four[256] = "Hello There";
    validate_lab8_1_inner(one);
    validate_lab8_1_inner(two);
    validate_lab8_1_inner(three);
    validate_lab8_1_inner(four);
    LAB_VALIDATED(8,1);
}

void validate_lab8_2_inner(const char *sz)
{
	unsigned char buffer[500];
	unsigned char *pbuf=buffer;
	memset(buffer, 0xCC, sizeof(buffer));

	int len = strlen(sz);

    ASM("push %[len];\n\t"
        "push %[sz];\n\t"
        "push %[pbuf];\n\t"
        :
        :[len]"m"(len), [sz]"m"(sz), [pbuf]"m"(pbuf)
    :);
	labWrap((void*)lab8_2, &grDefault, GR_FLAGS_ALL);
	checkReg(8, 2, grDefault, GR_FLAGS_PRES, 0);
	ASM("add esp, 0xC;\n\t");

	if(0 != memcmp(buffer, sz, len))
		failLab("Lab 8.2 failed! \n"
			    "  Memory not copied correctly!\n");
	if(buffer[len] != 0xCC)
		failLab("Lab 8.2 failed! \n"
			    "  You copied too much! Overflow!\n");
}

void validate_lab8_2()
{
	validate_lab8_2_inner("Hello");
	validate_lab8_2_inner("");
	validate_lab8_2_inner("X");
	validate_lab8_2_inner("Hello There");
	LAB_VALIDATED(8,2);
}

void validate_lab8_3_inner(int size, int ch)
{
	unsigned char buffer[5000];
	unsigned char *pbuf = buffer;
	memset(buffer, ch+1, sizeof(buffer));
	int wtf = size;
	int wtf2 = ch;

	ASM("push %[wtf];\n\t"
	    "push %[wtf2];\n\t"
	    "push %[pbuf];\n\t"
        :
        : [wtf]"m"(wtf), [wtf2]"m"(wtf2), [pbuf]"m"(pbuf)
        :);
	labWrap((void*)lab8_3, &grDefault, GR_FLAGS_ALL);
	checkReg(8, 3, grDefault, GR_FLAGS_PRES, 0xC);

	int i;
	for(i=0; i < size && buffer[i] == ch; i++) {}

	if(i != size)
		failLab("Lab 8.3 failed!\n"
				"  Buffer not filled with %d bytes of 0x%02X! (starting at byte %d)\n",
				size, ch, i );
	if(buffer[size] != (ch + 1))
		failLab("Lab 8.3 failed!\n You overwrote the buffer's end!\n");
}

void validate_lab8_3()
{
	validate_lab8_3_inner(0, 0x42);
	validate_lab8_3_inner(1, 0x42);
	validate_lab8_3_inner(2, 0x42);
	validate_lab8_3_inner(600, 0x42);
	LAB_VALIDATED(8,3);
}

void validate_lab8_4_inner(const char *sz, int offset)
{
	unsigned char buffer[5000];
	unsigned char *sbuf=buffer;
	memset(buffer, 0xCC, sizeof(buffer));
	int len = strlen(sz) + 1;
	memcpy(buffer, sz, len);
	unsigned char *dbuf = sbuf + offset;

	PUSHAD_DATA grIn = grDefault;
	grIn.ecx = (unsigned long)dbuf;
	grIn.edx = (unsigned long)sbuf;

	ASM("push %[len];\n\t"::[len]"m"(len):);
	labWrap((void*)lab8_4, &grIn, GR_FLAGS_ALL);
 	checkReg(8, 4, grIn, GR_FLAGS_PRES, 4);

	if(0 != memcmp(dbuf, sz, len))
		failLab("Lab 8.4 failed! \n"
			    "  Memory not copied correctly!\n");
}

void validate_lab8_4()
{
	validate_lab8_4_inner("", 0);
	validate_lab8_4_inner("X", 0);
	const char *sz = "Hello There";
	validate_lab8_4_inner(sz, 1);
	validate_lab8_4_inner(sz, 2);
	validate_lab8_4_inner(sz, strlen(sz));
	validate_lab8_4_inner(sz, strlen(sz)+1);
	validate_lab8_4_inner(sz, strlen(sz)+2);
	LAB_VALIDATED(8,4);
}

void validate_lab8_5_string(const char *szA, const char *szB, int maxLength, unsigned int res )
{
	ASM("push %[maxLength];\n\t"
	    "push %[szB];\n\t"
	    "push %[szA];\n\t"
        :
        : [maxLength]"m"(maxLength), [szB]"m"(szB), [szA]"m"(szA)
        :);
	labWrap((void*)lab8_5, &grDefault, GR_FLAGS_ALL);
	checkReg(8, 5, grDefault, GR_FLAGS_PRES, 0xC);

	if(g_grOut.eax != res)
		failLab("Lab 8.5 failed!\n"
				"   szA: '%s'\n"
				"   szB: '%s'\n"
				"   maxLength: %d\n"
				" Expected Result: %d\n"
				" Your Result:     %d\n",
				szA, szB, maxLength, res, g_grOut.eax );
}

void validate_lab8_5()
{
	validate_lab8_5_string("","",0,0);
	validate_lab8_5_string("","",1,0);
	validate_lab8_5_string("X","",1,1);
	validate_lab8_5_string("X","Y",1,1);
	validate_lab8_5_string("X","Y",0,0);
	validate_lab8_5_string("XX","XY",1,0);
	validate_lab8_5_string("XX","XY",2,1);
	validate_lab8_5_string("XX","XXX",2,0);
	validate_lab8_5_string("XX","XXX",3,1);
	validate_lab8_5_string("XX","XXX",2000,1);
	validate_lab8_5_string("XXX\0Y","XXX\0X",2000,0);
	LAB_VALIDATED(8, 5);
}

void validate_lab8_6_string(const char *sz, int ch)
{
	int len = strlen(sz);
	unsigned long wtf = ch;

	ASM("push %[len];\n\t"
	    "push %[wtf];\n\t"
	    "push %[sz];\n\t"
        :
        :[len]"m"(len), [wtf]"m"(wtf), [sz]"m"(sz)
        :);
	labWrap((void*)lab8_6, &grDefault, GR_FLAGS_ALL);
	checkReg(8, 6, grDefault, GR_FLAGS_PRES, 0xC);

	unsigned long res = (unsigned long)memchr(sz, ch, len);

	if(g_grOut.eax != res)
		failLab("Lab 8.6 failed!\n"
				"   sz:   '%s'\n"
				"   ch:   '%c'\n"
				"   count: %d\n"
				" Expected Result: %p\n"
				" Your Result:     %p\n",
				sz, ch, len, res,  g_grOut.eax );

}

void validate_lab8_6()
{
	validate_lab8_6_string("",'X');
	validate_lab8_6_string("XY",'X');
	validate_lab8_6_string("YX",'X');
	validate_lab8_6_string("YX",'Z');
	validate_lab8_6_string("Hello",'l');
	LAB_VALIDATED(8, 6);
}

void validate_lab9_1()
{
    unsigned long a = 0;
    ASM("pushad;\n\t"
        "mov eax, 1;\n\t"
        "test eax, eax;\n\t"
        "call lab9_1;\n\t"
        "setz al;\n\t"
        "and eax, 0x01;\n\t"
        "mov %[a], eax;\n\t"
        "popad;\n\t"
        :[a]"=m"(a)::);
    if(a != 1)
    {
        failLab("Lab 9.1 failed! ZF not set!\n");
    }
    LAB_VALIDATED(9,1);
}

void validate_lab9_2()
{
    unsigned long a = 0, b = 0;
    ASM("pushad;\n\t"
        "mov eax, 1;\n\t"
        "test eax, eax;\n\t"
        "call lab9_2;\n\t"
        "seto al;\n\t"
        "setc bl;\n\t"
        "and eax, 0x01;\n\t"
        "and ebx, 0x01;\n\t"
        "mov %[a], eax;\n\t"
        "mov %[b], ebx;\n\t"
        "popad;\n\t"
    :[a]"=m"(a), [b]"=m"(b)::);
    if(a != 1 || b != 1)
    {
        failLab("Lab 9.2 failed! Both CF and OF are not set!\n");
    }
    LAB_VALIDATED(9,2);
}

void validate_lab10_1()
{
    unsigned long sedi, sesi, sebx, sebp = 0;
    unsigned long nedi, nesi, nebx, nebp = 0;
    unsigned long a = 0;
    ASM("pushad;\n\t"
        "mov %[sedi], edi;\n\t"
        "mov %[sesi], esi;\n\t"
        "mov %[sebp], ebp;\n\t"
        "mov %[sebx], ebx;\n\t"
        "mov ecx, 100;\n\t"
        "mov edx, 3;\n\t"
        "call lab10_1;\n\t"
        "mov %[a], eax;\n\t"
        "mov %[nedi], edi;\n\t"
        "mov %[nesi], esi;\n\t"
        "mov %[nebp], ebp;\n\t"
        "mov %[nebx], ebx;\n\t"
        "popad;\n\t"
        :[sedi]"=m"(sedi), [sesi]"=m"(sesi), [sebp]"=m"(sebp),
         [sebx]"=m"(sebx), [nedi]"=m"(nedi), [nesi]"=m"(nesi),
         [nebp]"=m"(nebp), [nebx]"=m"(nebx), [a]"=m"(a)
        :
        :);
    if(a != 33)
    {
        failLab("Lab 10.1 failed! Wrong answer!\n");
    }
    if((sedi!=nedi) || (sesi!=nesi) || (sebx!=nebx) || (sebp!=nebp))
    {
        failLab("Lab 10.1 failed! Windows registers not preserved!\n");
    }
    LAB_VALIDATED(10,1);
}

void validate_lab10_2()
{
    unsigned long a = 0;
    unsigned long c = 0;
    unsigned long d = 0;
    unsigned long aa = 0;
    unsigned long cc = 0;
    unsigned long dd = 0;
    unsigned long aba = 0;
    unsigned long ccc = 0;
    unsigned long ddd = 0;
    ASM("pushad;\n\t"
        "mov ecx, 0xff;\n\t"
        "call lab10_2;\n\t"
        "mov %[a], eax;\n\t"
        "mov %[c], ecx;\n\t"
        "mov %[d], edx;\n\t"
        "mov ecx, 0x00;\n\t"
        "call lab10_2;\n\t"
        "mov %[aa], eax;\n\t"
        "mov %[cc], ecx;\n\t"
        "mov %[dd], edx;\n\t"
        "mov ecx, 0x65;\n\t"
        "call lab10_2;\n\t"
        "mov %[aba], eax;\n\t"
        "mov %[ccc], ecx;\n\t"
        "mov %[ddd], edx;\n\t"
        "popad;\n\t"
        :[a]"=m"(a), [c]"=m"(c), [d]"=m"(d),
         [aa]"=m"(aa), [cc]"=m"(cc), [dd]"=m"(dd),
         [aba]"=m"(aba), [ccc]"=m"(ccc), [ddd]"=m"(ddd)
        :
        :);
    if( a!=3 || d!=2 || c!= 0xf0) failLab("Lab 10.2 failed! Wrong answer!\n");
    if( aa!=3 || dd!=0 || cc!= 0x03) failLab("Lab 10.2 failed! Wrong answer!\n");
    if( aba!=6 || ddd!=2 || ccc!= 0x62) failLab("Lab 10.2 failed! Wrong answer!\n");

    LAB_VALIDATED(10,2);
}

void validate_lab11_1()
{
	unsigned long a  = -1;
	unsigned long a2 = -1;
	unsigned long a3 = -1;
	unsigned long a4 = -1;
	ASM("pushad;\n\t"
		"push 0x01;\n\t"
		"call lab11_1;\n\t"
		"mov %[a], eax;\n\t"
		"push 0x2;\n\t"
		"call lab11_1;\n\t"
		"mov %[a2], eax;\n\t"
		"push 0xa;\n\t"
		"call lab11_1;\n\t"
		"mov %[a3], eax;\n\t"
		"push 0x1e;\n\t"
		"call lab11_1;\n\t"
		"mov %[a4], eax;\n\t"
		"popad;\n\t"
        : [a]"=m"(a), [a2]"=m"(a2), [a3]"=m"(a3), [a4]"=m"(a4)
        ::
    );

	if( a != 0 || a2 != 1 || a3 != 34 || a4 != 514229) failLab("Challenge 1 failed! Wrong answer!\n");

	LAB_VALIDATED(11,1);
}

void validate_lab11_2()
{
	unsigned long a  = -1;
	unsigned long a2 = -1;
	unsigned long a3 = -1;
	unsigned long a4 = -1;
	ASM(
		"pushad;\n\t"
		"push 0x00;\n\t"
		"call lab11_2;\n\t"
		"mov %[a], eax;\n\t"
		"push 100;\n\t"
		"call lab11_2;\n\t"
		"mov %[a2], eax;\n\t"
		"push 200;\n\t"
		"call lab11_2;\n\t"
		"mov %[a3], eax;\n\t"
		"push 0xffff;\n\t"
		"call lab11_2;\n\t"
		"mov %[a4], eax;\n\t"
		"popad;\n\t"
        : [a]"=m"(a), [a2]"=m"(a2), [a3]"=m"(a3), [a4]"=m"(a4)
        ::
    );

	//validation for your return values goes here
	if( a != 0 || a2 != 315 || a3 != 1365 || a4 != 143193975) failLab("Challenge 2 failed! Wrong answer!\n");

	LAB_VALIDATED(11,2);
}

void validate_lab11_3()
{
	static const int data0[] = {1, 2, 3, 4, 5};
	static const int data1[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	static const int data2[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
	static const int data3[] = {40, 13, 50, 73, 86, 54, 83, 28, 40, 8};
	static const int data4[] = {33, 44, 17, 49, 49, 7, 1, 27, 67, 35, 33, 0, 41, 37, 37, 46, 27, 31, 7, 66, 19, 11, 30, 72, 25, 23, 58, 1, 36, 60, 45, 63, 1, 61, 28, 67, 67, 67, 38, 41, 73, 33, 25, 8, 50, 29, 41, 25, 67, 58, 41, 0, 58, 8, 53, 47, 69, 56, 14, 29, 25, 70, 67, 12, 51, 70, 49, 16, 46, 16, 11, 15, 24, 40, 66, 40, 41, 26, 52, 69, 2, 34, 15, 31, 70, 35, 10, 31, 71, 37, 1, 65, 73, 16, 65, 0, 48, 44, 16, 10, 61, 33, 72, 73, 41, 13, 61, 71, 3, 70, 59, 30, 32, 49, 8, 10, 57, 5, 21, 3, 65, 70, 12, 60, 59, 50, 11, 46, 54, 61, 22, 72, 48, 12, 70, 57, 66, 68, 45, 64, 6, 72, 41, 70, 39, 32, 13, 55, 48, 52, 62, 31, 46, 70, 3, 48, 47, 54, 26, 45, 54, 51, 40, 18, 51, 53, 71, 5, 56, 16, 29, 25, 23, 48, 57, 32, 1, 0, 50, 4, 25, 66, 6, 18, 53, 67, 61, 45, 49, 5, 28, 25, 14, 47, 53, 30, 16, 13, 40, 7, 63, 51, 33, 14, 59, 50, 71, 44, 59, 42, 63, 49, 35, 43, 32, 54, 48, 57, 52, 66, 21, 17, 64, 11, 1, 31, 42, 66, 23, 69, 49, 23, 72, 70, 11, 29, 46, 42, 3, 12, 5, 56, 11, 13, 71, 50, 53, 17, 51, 21, 19, 55, 50, 50, 27, 7, 33, 40, 4, 68, 35, 31, 50, 8, 49, 70, 57, 21, 3, 64, 33, 35, 22, 41, 26, 58, 20, 26, 22, 12, 42, 25, 70, 23, 42, 24, 41, 22, 39, 12, 56, 5, 10, 22, 56, 38, 2, 67, 67, 16, 6, 21, 19, 1, 46, 48, 51, 56, 21, 44, 56, 37, 62, 18, 55, 53, 25, 51, 65, 2, 33, 43, 27, 35, 55, 27, 72, 66, 5, 44, 44, 33, 31, 27, 60, 53, 39, 9, 13, 20, 72, 32, 11, 45, 53, 22, 72, 39, 39, 4, 1, 31, 45, 72, 50, 67, 72, 39, 26, 49, 36, 60, 35, 9, 11, 52, 3, 2, 53, 54, 73, 65, 2, 45, 41, 33, 4, 36, 32, 20, 0, 50, 26, 0, 56, 28, 16, 58, 12, 72, 60, 21, 19, 53, 18, 13, 46, 52, 3, 72, 8, 35, 35, 0, 32, 32, 63, 3, 25, 65, 60, 56, 66, 64, 38, 50, 25, 38, 9, 64, 11, 3, 61, 20, 1, 8, 8, 0, 46, 13, 19, 35, 67, 18, 27, 43, 39, 57, 60, 53, 52, 20, 57, 35, 38, 65, 38, 54, 10, 48, 6, 28, 8, 47, 19, 35, 60, 61, 59, 21, 11, 4, 7, 14, 66, 50, 22, 46, 4, 54, 52, 49, 46, 68, 65, 16, 67, 71, 17, 4, 58, 16, 0, 42, 61, 2, 24, 62, 57, 57, 32, 12, 70, 61, 53, 50, 32, 50, 23, 12, 2, 1, 70, 30, 38, 52, 23, 20, 13, 55, 19, 2, 72, 45, 14, 10, 47, 27, 5, 26, 58, 9, 19, 52, 7, 69, 49, 66, 41, 42, 4, 39, 63, 73, 34, 67, 53, 8, 35, 16, 67, 31, 5, 53, 63, 40, 48, 59, 11, 33, 27, 71, 70, 68, 16, 24, 63, 39, 51, 3, 33, 37, 0, 5, 63, 72, 63, 34, 73, 29, 60, 68, 71, 29, 8, 20, 37, 38, 41, 39, 38, 55, 53, 40, 3, 22, 62, 14, 70, 54, 37, 58, 52, 39, 22, 43, 24, 72, 71, 36, 40, 14, 67, 44, 14, 7, 13, 44, 50, 22, 14, 55, 26, 3, 8, 9, 36, 8, 3, 41, 41, 71, 61, 18, 47, 18, 38, 22, 22, 32, 19, 35, 11, 3, 60, 50, 50, 2, 67, 29, 72, 73, 47, 72, 65, 55, 58, 50, 13, 3, 71, 18, 7, 33, 56, 38, 63, 30, 73, 34, 58, 61, 39, 19, 2, 25, 48, 21, 51, 60, 73, 50, 72, 27, 27, 20, 39, 10, 50, 22, 72, 16, 25, 4, 55, 61, 70, 40, 56, 44, 42, 51, 19, 9, 28, 5, 11, 55, 55, 33, 15, 16, 20, 8, 16, 38, 10, 24, 68, 55, 54, 69, 51, 55, 48, 61, 26, 25, 28, 46, 27, 30, 12, 2, 43, 29, 11, 5, 35, 4, 12, 56, 57, 2, 27, 29, 18, 20, 1, 32, 16, 54, 25, 51, 54, 23, 12, 15, 58, 54, 40, 59, 6, 3, 32, 19, 16, 32, 22, 64, 24, 70, 4, 72, 14, 27, 32, 31, 39, 69, 44, 24, 9, 1, 32, 27, 5, 48, 2, 34, 57, 48, 20, 35, 40, 17, 68, 8, 53, 4, 7, 16, 50, 56, 61, 48, 42, 72, 59, 14, 32, 28, 26, 1, 29, 18, 41, 19, 8, 70, 48, 28, 9, 53, 60, 44, 53, 1, 4, 0, 53, 73, 59, 65, 35, 52, 23, 46, 52, 17, 0, 20, 14, 49, 45, 65, 31, 66, 66, 40, 65, 27, 20, 56, 14, 45, 11, 59, 12, 46, 58, 65, 71, 21, 45, 49, 22, 73, 37, 56, 69, 4, 28, 46, 47, 50, 34, 64, 67, 6, 59, 9, 4, 46, 68, 38, 43, 27, 59, 17, 53, 68, 3, 34, 29, 15, 10, 33, 57, 17, 29, 71, 20, 68, 45, 41, 6, 5, 19, 52, 54, 38, 8, 32, 33, 1, 40, 39, 10, 13, 11, 51, 68, 1, 39, 57, 42, 36, 37, 42, 45, 18, 50, 66, 64, 32, 40, 35, 13, 34, 15, 40, 67, 29, 45, 46, 39, 13, 21, 55, 20, 26, 64, 70, 18, 34, 25, 27, 21, 17, 59, 57, 71, 4, 18, 1, 36, 27, 35, 49, 8, 3, 2, 62, 18, 70, 30, 38, 40, 25, 42, 41, 21, 6, 55, 15, 31, 28, 70, 66, 58, 0, 34, 50, 8, 19, 73, 4, 30, 49, 70, 50, 44, 8, 68, 21, 53, 49, 6, 52};
	static const int data5[] = {37, -39, 32, 33, -34, -40, -69, 15, 15, -19, 11, -72, -68, -30, 21, -32, -34, -19, -67, 49, 22, 16, -73, -13, 3, 9, -24, -43, -10, 47, 73, 7, 1, 8, 48, 39, 16, -27, 10, 16, 52, 44, 29, 8, 68, -5, 35, -61, -31, -9, -9, 63, -33, 25, 8, -71, 37, 1, 24, 59, 14, 52, -5, 22, -40, -39, -68, -12, -69, -67, 21, -16, 2, 54, 17, 28, -54, -50, 25, 20, -6, 65, 0, -22, -6, 19, -12, 42, 20, 42, 68, 0, -23, -5, -45, 12, -33, -38, -34, 58, -30, 19, -21, -24, 6, 31, -8, 32, -66, 53, -3, -9, 49, 20, -53, -12, 7, -49, 16, -20, 71, 6, -3, -29, -6, -49, -60, 14, -19, 52, -31, -64, 28, -11, -16, 19, 39, 6, 40, -58, 3, 54, -7, 38, -38, 58, 21, -64, 0, -4, -46, -21, -56, -33, -42, 15, 40, 34, 53, -46, 65, -63, -18, 33, -47, -9, -21, 36, 19, -19, -9, -9, 33, -36, 29, 28, -36, -55, 36, -30, -11, 42, -53, -22, 60, 20, -16, 1, -28, -32, -67, 19, -55, 53, 39, -16, 30, -42, 6, 63, -10, -52, -45, 42, -10, -15, 12, -4, -41, 32, 67, 29, -72, 28, -17, -38, -24, 14, -24, -68, -58, 54, 18, -37, 18, -8, -22, 26, -2, 24, -24, 21, 17, -45, 17, -8, 0, 35, -50, 18, 51, -64, -36, 54, -61, -16, -9, -13, -17, -1, 21, -33, -59, 21, 23, 47, 12, -60, 61, -37, -13, -35, -41, -9, -59, 37, -39, 51, -23, -58, 71, -4, -57, -71, -29, -57, 71, 30, -13, -12, 55, 48, 12, 51, -32, 10, -68, -12, -69, 15, -71, -51, -41, -49, -36, -63, -26, 14, -35, -33, -26, 53, -1, 39, 2, -47, -8, 27, -71, 33, 8, -35, 38, 62, -8, -48, -66, 72, 35, -23, -37, -36, -57, 38, 60, 44, 66, -69, -46, -45, 57, -10, 50, 43, 24, -41, -44, 8, -43, -56, 13, -52, 64, 62, -35, 8, 61, -28, -64, 52, 11, 9, 71, -56, 24, -1, 33, 39, 11, -23, 6, -23, 5, 44, -40, -73, -54, -72, -22, -47, 63, 17, -52, 4, 49, -15, -33, 22, -25, 37, -28, 21, 62, -44, -26, 67, -8, -18, 60, -21, 10, -40, -37, 31, -24, 40, 38, 53, 67, -9, 23, -25, -60, 13, -30, -1, 32, -67, -18, -21, -3, -66, -14, -43, 11, 54, 63, 58, 12, 52, 6, 5, -59, 64, -66, 68, -43, 61, -59, -5, 40, 50, -2, 36, 22, 57, -4, 10, -28, -16, -11, 58, 25, 6, -10, 35, 33, -10, -10, -34, -65, 20, 32, 38, -27, 59, 31, 51, 59, 15, 30, -39, 42, -36, 0, -13, -47, -55, 32, -13, -38, -8, 70, -12, -8, -27, -31, 36, -27, -51, 57, 18, -4, -16, -30, 15, -5, -1, -39, 52, 59, -26, 53, 27, 50, 40, -40, 39, 9, -29};
	const int *data0p = data0;
	const int *data1p = data1;
	const int *data2p = data2;
	const int *data3p = data3;
	const int *data4p = data4;
	const int *data5p = data5;
	unsigned long e0 = 0xcccccccc;
	unsigned long e1 = 0xcccccccc;
	unsigned long a0 = -1;
	unsigned long a1 = -1;
	unsigned long a2 = -1;
	unsigned long a3 = -1;
	unsigned long a4 = -1;
	unsigned long a5 = -1;
	ASM("pushad;\n\t"

		"push %[data0p];\n\t"
		"push 0;\n\t"
		"call lab11_3;\n\t"
		"mov %[e0], eax;\n\t"

		"push %[data0p];\n\t"
		"push 3;\n\t"
		"call lab11_3;\n\t"
		"mov %[e1], eax;\n\t"

		"push %[data0p];\n\t"
		"push 5;\n\t"
		"call lab11_3;\n\t"
		"mov %[a0], eax;\n\t"

		"push %[data1p];\n\t"
		"push 10;\n\t"
		"call lab11_3;\n\t"
		"mov %[a1], eax;\n\t"

		"push %[data2p];\n\t"
		"push 10;\n\t"
		"call lab11_3;\n\t"
		"mov %[a2], eax;\n\t"

		"push %[data3p];\n\t"
		"push 10;\n\t"
		"call lab11_3;\n\t"
		"mov %[a3], eax;\n\t"

		"push %[data4p];\n\t"
		"push 1000;\n\t"
		"call lab11_3;\n\t"
		"mov %[a4], eax;\n\t"

		"push %[data5p];\n\t"
		"push 500;\n\t"
		"call lab11_3;\n\t"
		"mov %[a5], eax;\n\t"

		"popad;\n\t"
        : [e0]"=m"(e0), [e1]"=m"(e1), [a0]"=m"(a0),
          [a1]"=m"(a1), [a2]"=m"(a2), [a3]"=m"(a3),
          [a4]"=m"(a4), [a5]"=m"(a5)
        : [data0p]"m"(data0p), [data1p]"m"(data1p),
          [data2p]"m"(data2p), [data3p]"m"(data3p),
          [data4p]"m"(data4p), [data5p]"m"(data5p)
        :);

	if ( e0 != -1UL ) failLab("Challenge 3 failed! Didn't return -1 on error! (n=0)\n");
	if ( e1 != -1UL ) failLab("Challenge 3 failed! Didn't return -1 on error! (n=3)\n");
	if ( a0 != 120 ) failLab("Challenge 3 failed! Minimal case, n=5. Expected: 120  Got: %ld\n", a0);
	if ( a1 != 15120 ) failLab("Challenge 3 failed! Sequence case, n=10. Expected: 15120  Got: %ld\n", a1);
	if ( a2 != 15120 ) failLab("Challenge 3 failed! Reverse sequence case, n=10. Expected: 15120  Got: %ld\n", a2);
	if ( a3 != 1406899800 ) failLab("Challenge 3 failed! Arbitrary array case, only positive numbers, n=10. Expected: 1406899800  Got: %ld\n", a3);
	if ( a4 != 1156109760 ) failLab("Challenge 3 failed! Arbitrary array case, only positive numbers, n=1000. Expected: 1156109760  Got: %ld\n", a4);
	if ( a5 != 753409536 ) failLab("Challenge 3 failed! Arbitrary array case, n=500. Expected: 753409536  Got: %ld\n", a5);
	LAB_VALIDATED(11,3);
}

void validate_lab11_4_string(const char *sz, int key, const char *result)
{
	char buffer[1024];
	char *pb = buffer;
	unsigned long len;
	len = strlen(sz);
	memset(buffer, 0xCC, sizeof(buffer));
	strncpy(buffer, sz, sizeof(buffer));

	ASM("pushad;\n\t"
		"push %[len];\n\t"
		"push %[key];\n\t"
		"push %[pb];\n\t"
		"call lab11_4;\n\t"
		"popad;\n\t"
        :
        : [len]"m"(len), [key]"m"(key), [pb]"m"(pb)
        :
    );

	if( strncmp(buffer, result, len) != 0 )
	{
		failLab("Lab 11.4 failed! Output string is not decoded correctly!\n"
			"Should have: %s\n"
			"Did have:    %s\n",
			result, buffer);
	}
}

void validate_lab11_4()
{
	validate_lab11_4_string("", 21, "");
	validate_lab11_4_string("asdf", 21, "tfqs");
	validate_lab11_4_string("i'm a little teapot", 20, "}3y4u4x}``xq4`qud{`");
	LAB_VALIDATED(11,4);
}

void validate_lab11_5_string(const char *input, const char *output)
{
	char buffer[1024];
	char *pb = buffer;
	char *ob = NULL;
	unsigned long len;
	len = strlen(input);
	memset(buffer, 0xCC, sizeof(buffer));
	strncpy(buffer, input, sizeof(buffer));
	ASM("pushad;\n\t"
		"push %[len];\n\t"
		"push %[pb];\n\t"
		"call lab11_5;\n\t"
		"mov %[ob], eax;\n\t"
		"popad;\n\t"
        : [ob]"=m"(ob)
        : [len]"m"(len),  [pb]"m"(pb)
        :
    );

	if (ob == NULL)
		failLab("Lab 11.5 failed. You returned a null pointer!\n");
	if (strncmp(ob, output, strlen(output)) != 0)
	{
		failLab("Lab 11.5 failed. Output string is not correct.\n"
			"Should have: %s\n"
			"Did have:    %s\n",
			output, ob);
	}

	free(ob); //free returned string
}

void validate_lab11_5()
{
	validate_lab11_5_string("1234", "1,234");
	validate_lab11_5_string("1234567", "1,234,567");
	validate_lab11_5_string("123", "123");
	validate_lab11_5_string("123456789", "123,456,789"); //make sure they aren't prepending at the 3-boundary and manually checking <4
	LAB_VALIDATED(11,5);
}

// skipping 11_6 for now
//TODO write a validator for this
void validate_lab11_6()
{
	LAB_VALIDATED(11,6);
}

void validate_lab11_7_string(const char *sz, unsigned long result)
{
	char buffer[1024];
	char *pb = buffer;
	unsigned long len, res;
	len = strlen(sz);
	memset(buffer, 0xCC, sizeof(buffer));
	strncpy(buffer, sz, sizeof(buffer));

	ASM("pushad;\n\t"
		"push %[len];\n\t"
		"push %[pb];\n\t"
		"call lab11_7;\n\t"
		"mov %[res], eax;\n\t"
		"popad;\n\t"
        : [res]"=m"(res)
        : [len]"m"(len), [pb]"m"(pb)
        :
    );

	if( res != result )
	{
		failLab("Lab 11.7 failed! Returned incorrect result!\n"
			"Should have: %d\n"
			"Did have:    %d\n",
			result, res);
	}
}

void validate_lab11_7()
{
	validate_lab11_7_string("a1A@", 1);      //pass
	validate_lab11_7_string("asdf1234!", 0); //no capital
	validate_lab11_7_string("1df3@F", 0);    //starts with a number
	validate_lab11_7_string("Adf3@f", 1);    //pass
	validate_lab11_7_string("ADF3%F", 0);    //no lowercase
	LAB_VALIDATED(11,7);
}
